#include "pch.h"


namespace factory {
	class Product {
	public:
		virtual ~Product() {}
		virtual std::string Operation() const = 0;
	};
	class ConcreteProduct1 : public Product {
	public:
		std::string Operation() const override {
			return "{Result of the ConcreteProduct1}";
		}
	};
	class ConcreteProduct2 : public Product {
	public:
		std::string Operation() const override {
			return "{Result of the ConcreteProduct2}";
		}
	};

	class Creator {
		/**
		 * Note that the Creator may also provide some default implementation of the
		 * factory method.
		 */
	public:
		virtual ~Creator() {};
		virtual Product* FactoryMethod() const = 0;
		/**
		 * Also note that, despite its name, the Creator's primary responsibility is
		 * not creating products. Usually, it contains some core business logic that
		 * relies on Product objects, returned by the factory method. Subclasses can
		 * indirectly change that business logic by overriding the factory method and
		 * returning a different type of product from it.
		 */

		std::string SomeOperation() const {
			// Call the factory method to create a Product object.
			Product* product = this->FactoryMethod();
			// Now, use the product.
			std::string result = "Creator: The same creator's code has just worked with " + product->Operation();
			delete product;
			return result;
		}
	};
	class ConcreteCreator1 : public Creator {
		/**
		 * Note that the signature of the method still uses the abstract product type,
		 * even though the concrete product is actually returned from the method. This
		 * way the Creator can stay independent of concrete product classes.
		 */
	public:
		Product* FactoryMethod() const override {
			return new ConcreteProduct1();
		}
	};

	class ConcreteCreator2 : public Creator {
	public:
		Product* FactoryMethod() const override {
			return new ConcreteProduct2();
		}
	};

	void ClientCode(const Creator& creator) {
		// ...
		std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
			<< creator.SomeOperation() << std::endl;
		// ...
	}
}

TEST(dp23, factory) {
	using namespace factory;
	std::cout << "App: Launched with the ConcreteCreator1.\n";
	Creator* creator = new ConcreteCreator1();
	ClientCode(*creator);
	std::cout << std::endl;
	std::cout << "App: Launched with the ConcreteCreator2.\n";
	Creator* creator2 = new ConcreteCreator2();
	ClientCode(*creator2);

	delete creator;
	delete creator2;

}

namespace abstractfactory {
	/**
 * Each distinct product of a product family should have a base interface. All
 * variants of the product must implement this interface.
 */
	class AbstractProductA {
	public:
		virtual ~AbstractProductA() {};
		virtual std::string UsefulFunctionA() const = 0;
	};
	/**
 * Concrete Products are created by corresponding Concrete Factories.
 */
	class ConcreteProductA1 : public AbstractProductA {
	public:
		std::string UsefulFunctionA() const override {
			return "The result of the product A1.";
		}
	};

	class ConcreteProductA2 : public AbstractProductA {
		std::string UsefulFunctionA() const override {
			return "The result of the product A2.";
		}
	};

	/**
 * Here's the the base interface of another product. All products can interact
 * with each other, but proper interaction is possible only between products of
 * the same concrete variant.
 */
	class AbstractProductB {
		/**
		 * Product B is able to do its own thing...
		 */
	public:
		virtual ~AbstractProductB() {};
		virtual std::string UsefulFunctionB() const = 0;
		/**
		 * ...but it also can collaborate with the ProductA.
		 *
		 * The Abstract Factory makes sure that all products it creates are of the
		 * same variant and thus, compatible.
		 */
		virtual std::string AnotherUsefulFunctionB(const AbstractProductA& collaborator) const = 0;
	};

	/**
 * Concrete Products are created by corresponding Concrete Factories.
 */
	class ConcreteProductB1 : public AbstractProductB {
	public:
		std::string UsefulFunctionB() const override {
			return "The result of the product B1.";
		}
		/**
		 * The variant, Product B1, is only able to work correctly with the variant,
		 * Product A1. Nevertheless, it accepts any instance of AbstractProductA as an
		 * argument.
		 */
		std::string AnotherUsefulFunctionB(const AbstractProductA& collaborator) const override {
			const std::string result = collaborator.UsefulFunctionA();
			return "The result of the B1 collaborating with ( " + result + " )";
		}
	};

	class ConcreteProductB2 : public AbstractProductB {
	public:
		std::string UsefulFunctionB() const override {
			return "The result of the product B2.";
		}
		/**
		 * The variant, Product B2, is only able to work correctly with the variant,
		 * Product A2. Nevertheless, it accepts any instance of AbstractProductA as an
		 * argument.
		 */
		std::string AnotherUsefulFunctionB(const AbstractProductA& collaborator) const override {
			const std::string result = collaborator.UsefulFunctionA();
			return "The result of the B2 collaborating with ( " + result + " )";
		}
	};

	/**
 * The Abstract Factory interface declares a set of methods that return
 * different abstract products. These products are called a family and are
 * related by a high-level theme or concept. Products of one family are usually
 * able to collaborate among themselves. A family of products may have several
 * variants, but the products of one variant are incompatible with products of
 * another.
 */
	class AbstractFactory {
	public:
		virtual AbstractProductA* CreateProductA() const = 0;
		virtual AbstractProductB* CreateProductB() const = 0;
	};

	/**
 * Concrete Factories produce a family of products that belong to a single
 * variant. The factory guarantees that resulting products are compatible. Note
 * that signatures of the Concrete Factory's methods return an abstract product,
 * while inside the method a concrete product is instantiated.
 */
	class ConcreteFactory1 : public AbstractFactory {
	public:
		AbstractProductA* CreateProductA() const override {
			return new ConcreteProductA1();
		}
		AbstractProductB* CreateProductB() const override {
			return new ConcreteProductB1();
		}
	};

	/**
 * Each Concrete Factory has a corresponding product variant.
 */
	class ConcreteFactory2 : public AbstractFactory {
	public:
		AbstractProductA* CreateProductA() const override {
			return new ConcreteProductA2();
		}
		AbstractProductB* CreateProductB() const override {
			return new ConcreteProductB2();
		}
	};

	void ClientCode(const AbstractFactory& factory) {
		const AbstractProductA* product_a = factory.CreateProductA();
		const AbstractProductB* product_b = factory.CreateProductB();
		std::cout << product_b->UsefulFunctionB() << "\n";
		std::cout << product_b->AnotherUsefulFunctionB(*product_a) << "\n";
		delete product_a;
		delete product_b;
	}

}

TEST(dp23, abstractfactory) {
	using namespace abstractfactory;

	std::cout << "Client: Testing client code with the first factory type:\n";
	ConcreteFactory1* f1 = new ConcreteFactory1();
	ClientCode(*f1);
	delete f1;
	std::cout << std::endl;
	std::cout << "Client: Testing the same client code with the second factory type:\n";
	ConcreteFactory2* f2 = new ConcreteFactory2();
	ClientCode(*f2);
	delete f2;

}


namespace builder {
	/**
 * It makes sense to use the Builder pattern only when your products are quite
 * complex and require extensive configuration.
 *
 * Unlike in other creational patterns, different concrete builders can produce
 * unrelated products. In other words, results of various builders may not
 * always follow the same interface.
 */

	class Product1 {
	public:
		std::vector<std::string> parts_;
		void ListParts()const {
			std::cout << "Product parts: ";
			for (size_t i = 0; i < parts_.size(); i++) {
				if (parts_[i] == parts_.back()) {
					std::cout << parts_[i];
				}
				else {
					std::cout << parts_[i] << ", ";
				}
			}
			std::cout << "\n\n";
		}
	};

	/**
 * The Builder interface specifies methods for creating the different parts of
 * the Product objects.
 */
	class Builder {
	public:
		virtual ~Builder() {}
		virtual void ProducePartA() const = 0;
		virtual void ProducePartB() const = 0;
		virtual void ProducePartC() const = 0;
	};

	/**
 * The Concrete Builder classes follow the Builder interface and provide
 * specific implementations of the building steps. Your program may have several
 * variations of Builders, implemented differently.
 */
	class ConcreteBuilder1 : public Builder {
	private:

		Product1* product;

		/**
		 * A fresh builder instance should contain a blank product object, which is
		 * used in further assembly.
		 */
	public:

		ConcreteBuilder1() {
			this->Reset();
		}

		~ConcreteBuilder1() {
			delete product;
		}

		void Reset() {
			this->product = new Product1();
		}
		/**
		 * All production steps work with the same product instance.
		 */

		void ProducePartA()const override {
			this->product->parts_.push_back("PartA1");
		}

		void ProducePartB()const override {
			this->product->parts_.push_back("PartB1");
		}

		void ProducePartC()const override {
			this->product->parts_.push_back("PartC1");
		}

		/**
		 * Concrete Builders are supposed to provide their own methods for
		 * retrieving results. That's because various types of builders may create
		 * entirely different products that don't follow the same interface.
		 * Therefore, such methods cannot be declared in the base Builder interface
		 * (at least in a statically typed programming language). Note that PHP is a
		 * dynamically typed language and this method CAN be in the base interface.
		 * However, we won't declare it there for the sake of clarity.
		 *
		 * Usually, after returning the end result to the client, a builder instance
		 * is expected to be ready to start producing another product. That's why
		 * it's a usual practice to call the reset method at the end of the
		 * `getProduct` method body. However, this behavior is not mandatory, and
		 * you can make your builders wait for an explicit reset call from the
		 * client code before disposing of the previous result.
		 */

		 /**
		  * Please be careful here with the memory ownership. Once you call
		  * GetProduct the user of this function is responsable to release this
		  * memory. Here could be a better option to use smart pointers to avoid
		  * memory leaks
		  */

		Product1* GetProduct() {
			Product1* result = this->product;
			this->Reset();
			return result;
		}
	};

	/**
 * The Director is only responsible for executing the building steps in a
 * particular sequence. It is helpful when producing products according to a
 * specific order or configuration. Strictly speaking, the Director class is
 * optional, since the client can control builders directly.
 */
	class Director {
		/**
		 * @var Builder
		 */
	private:
		Builder* builder;
		/**
		 * The Director works with any builder instance that the client code passes
		 * to it. This way, the client code may alter the final type of the newly
		 * assembled product.
		 */

	public:

		void set_builder(Builder* builder) {
			this->builder = builder;
		}

		/**
		 * The Director can construct several product variations using the same
		 * building steps.
		 */

		void BuildMinimalViableProduct() {
			this->builder->ProducePartA();
		}

		void BuildFullFeaturedProduct() {
			this->builder->ProducePartA();
			this->builder->ProducePartB();
			this->builder->ProducePartC();
		}
	};

	/**
 * The client code creates a builder object, passes it to the director and then
 * initiates the construction process. The end result is retrieved from the
 * builder object.
 */
 /**
  * I used raw pointers for simplicity however you may prefer to use smart
  * pointers here
  */
	void ClientCode(Director& director)
	{
		ConcreteBuilder1* builder = new ConcreteBuilder1();
		director.set_builder(builder);
		std::cout << "Standard basic product:\n";
		director.BuildMinimalViableProduct();

		Product1* p = builder->GetProduct();
		p->ListParts();
		delete p;

		std::cout << "Standard full featured product:\n";
		director.BuildFullFeaturedProduct();

		p = builder->GetProduct();
		p->ListParts();
		delete p;

		// Remember, the Builder pattern can be used without a Director class.
		std::cout << "Custom product:\n";
		builder->ProducePartA();
		builder->ProducePartC();
		p = builder->GetProduct();
		p->ListParts();
		delete p;

		delete builder;
	}
}

TEST(dp23, builder) {
	using namespace builder;

	Director* director = new Director();
	ClientCode(*director);
	delete director;

}


namespace prototype {
	enum Type {
		PROTOTYPE_1 = 0,
		PROTOTYPE_2
	};
	class Prototype {
	protected:
		string prototype_name_;
		float prototype_field_;

	public:
		Prototype() {}
		Prototype(string prototype_name)
			: prototype_name_(prototype_name) {
		}
		virtual ~Prototype() {}
		virtual Prototype* Clone() const = 0;
		virtual void Method(float prototype_field) {
			this->prototype_field_ = prototype_field;
			std::cout << "Call Method from " << prototype_name_ << " with field : " << prototype_field << std::endl;
		}
	};

	class ConcretePrototype1 : public Prototype {
	private:
		float concrete_prototype_field1_;

	public:
		ConcretePrototype1(string prototype_name, float concrete_prototype_field)
			: Prototype(prototype_name), concrete_prototype_field1_(concrete_prototype_field) {
		}

		/**
		 * Notice that Clone method return a Pointer to a new ConcretePrototype1
		 * replica. so, the client (who call the clone method) has the responsability
		 * to free that memory. I you have smart pointer knowledge you may prefer to
		 * use unique_pointer here.
		 */
		Prototype* Clone() const override {
			return new ConcretePrototype1(*this);
		}
	};

	class ConcretePrototype2 : public Prototype {
	private:
		float concrete_prototype_field2_;

	public:
		ConcretePrototype2(string prototype_name, float concrete_prototype_field)
			: Prototype(prototype_name), concrete_prototype_field2_(concrete_prototype_field) {
		}
		Prototype* Clone() const override {
			return new ConcretePrototype2(*this);
		}
	};

	class PrototypeFactory {
	private:
		std::unordered_map<Type, Prototype*, std::hash<int>> prototypes_;

	public:
		PrototypeFactory() {
			prototypes_[Type::PROTOTYPE_1] = new ConcretePrototype1("PROTOTYPE_1 ", 50.f);
			prototypes_[Type::PROTOTYPE_2] = new ConcretePrototype2("PROTOTYPE_2 ", 60.f);
		}

		/**
		 * Be carefull of free all memory allocated. Again, if you have smart pointers
		 * knowelege will be better to use it here.
		 */

		~PrototypeFactory() {
			delete prototypes_[Type::PROTOTYPE_1];
			delete prototypes_[Type::PROTOTYPE_2];
		}

		/**
		 * Notice here that you just need to specify the type of the prototype you
		 * want and the method will create from the object with this type.
		 */
		Prototype* CreatePrototype(Type type) {
			return prototypes_[type]->Clone();
		}
	};

	void Client(PrototypeFactory& prototype_factory) {
		std::cout << "Let's create a Prototype 1\n";

		Prototype* prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_1);
		prototype->Method(90);
		delete prototype;

		std::cout << "\n";

		std::cout << "Let's create a Prototype 2 \n";

		prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_2);
		prototype->Method(10);

		delete prototype;
	}

}

TEST(dp23, prototype) {
	using namespace prototype;

	PrototypeFactory* prototype_factory = new PrototypeFactory();
	Client(*prototype_factory);
	delete prototype_factory;

}


namespace singleton {

	/**
 * The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
	class Singleton
	{

		/**
		 * The Singleton's constructor/destructor should always be private to
		 * prevent direct construction/desctruction calls with the `new`/`delete`
		 * operator.
		 */
	private:
		static Singleton* pinstance_;
		static std::mutex mutex_;

	protected:
		Singleton(const std::string value) : value_(value)
		{
		}
		~Singleton() {}
		std::string value_;

	public:
		/**
		 * Singletons should not be cloneable.
		 */
		Singleton(Singleton& other) = delete;
		/**
		 * Singletons should not be assignable.
		 */
		void operator=(const Singleton&) = delete;
		/**
		 * This is the static method that controls the access to the singleton
		 * instance. On the first run, it creates a singleton object and places it
		 * into the static field. On subsequent runs, it returns the client existing
		 * object stored in the static field.
		 */

		static Singleton* GetInstance(const std::string& value);
		/**
		 * Finally, any singleton should define some business logic, which can be
		 * executed on its instance.
		 */
		void SomeBusinessLogic()
		{
			// ...
		}

		std::string value() const {
			return value_;
		}
	};

	Singleton* Singleton::pinstance_{ nullptr };
	std::mutex Singleton::mutex_;

	Singleton* Singleton::GetInstance(const std::string& value)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (pinstance_ == nullptr)
		{
			pinstance_ = new Singleton(value);
		}
		return pinstance_;
	}

	void ThreadFoo() {
		// Following code emulates slow initialization.
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Singleton* singleton = Singleton::GetInstance("FOO");
		std::cout << singleton->value() << "\n";
	}

	void ThreadBar() {
		// Following code emulates slow initialization.
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Singleton* singleton = Singleton::GetInstance("BAR");
		std::cout << singleton->value() << "\n";
	}

}

TEST(dp23, singleton) {
	using namespace singleton;
	std::cout << "If you see the same value, then singleton was reused (yay!\n" <<
		"If you see different values, then 2 singletons were created (booo!!)\n\n" <<
		"RESULT:\n";
	std::thread t1(ThreadFoo);
	std::thread t2(ThreadBar);
	t1.join();
	t2.join();
}