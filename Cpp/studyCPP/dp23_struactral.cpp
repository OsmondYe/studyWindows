#include "pch.h"


using namespace std;

namespace adapter{
	class Target {
	public:
		virtual ~Target() = default;

		virtual std::string Request() const {
			return "Target: The default target's behavior.";
		}
	};

	class Adaptee {
	public:
		std::string SpecificRequest() const {
			return ".eetpadA eht fo roivaheb laicepS";
		}
	};

	class Adapter : public Target {
	private:
		Adaptee* adaptee_;

	public:
		Adapter(Adaptee* adaptee) : adaptee_(adaptee) {}
		std::string Request() const override {
			std::string to_reverse = this->adaptee_->SpecificRequest();
			std::reverse(to_reverse.begin(), to_reverse.end());
			return "Adapter: (TRANSLATED) " + to_reverse;
		}
	};

	void ClientCode(const Target* target) {
		std::cout << target->Request();
	}
}

TEST(dp23, adapter) {
	using namespace adapter;

	std::cout << "Client: I can work just fine with the Target objects:\n";
	Target* target = new Target;
	ClientCode(target);
	std::cout << "\n\n";
	Adaptee* adaptee = new Adaptee;
	std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
	std::cout << "Adaptee: " << adaptee->SpecificRequest();
	std::cout << "\n\n";
	std::cout << "Client: But I can work with it via the Adapter:\n";
	Adapter* adapter = new Adapter(adaptee);
	ClientCode(adapter);
	std::cout << "\n";

	delete target;
	delete adaptee;
	delete adapter;
}

namespace bridge {
	class Implementation {
	public:
		virtual ~Implementation() {}
		virtual std::string OperationImplementation() const = 0;
	};

	class ConcreteImplementationA : public Implementation {
	public:
		std::string OperationImplementation() const override {
			return "ConcreteImplementationA: Here's the result on the platform A.\n";
		}
	};
	class ConcreteImplementationB : public Implementation {
	public:
		std::string OperationImplementation() const override {
			return "ConcreteImplementationB: Here's the result on the platform B.\n";
		}
	};

	class Abstraction {
		/**
		 * @var Implementation
		 */
	protected:
		Implementation* implementation_;

	public:
		Abstraction(Implementation* implementation) : implementation_(implementation) {
		}

		virtual ~Abstraction() {
		}

		virtual std::string Operation() const {
			return "Abstraction: Base operation with:\n" +
				this->implementation_->OperationImplementation();
		}
	};

	class ExtendedAbstraction : public Abstraction {
	public:
		ExtendedAbstraction(Implementation* implementation) : Abstraction(implementation) {
		}
		std::string Operation() const override {
			return "ExtendedAbstraction: Extended operation with:\n" +
				this->implementation_->OperationImplementation();
		}
	};

	void ClientCode(const Abstraction& abstraction) {
		// ...
		std::cout << abstraction.Operation();
		// ...
	}
}
TEST(dp23, bridge) {
	using namespace bridge;
	Implementation* implementation = new ConcreteImplementationA;
	Abstraction* abstraction = new Abstraction(implementation);
	ClientCode(*abstraction);
	std::cout << std::endl;
	delete implementation;
	delete abstraction;

	implementation = new ConcreteImplementationB;
	abstraction = new ExtendedAbstraction(implementation);
	ClientCode(*abstraction);

	delete implementation;
	delete abstraction;
}

namespace composite {
	class Component {
		
	protected:
		Component* parent_;
		/**
		 * Optionally, the base Component can declare an interface for setting and
		 * accessing a parent of the component in a tree structure. It can also
		 * provide some default implementation for these methods.
		 */
	public:
		virtual ~Component() {}
		void SetParent(Component* parent) {
			this->parent_ = parent;
		}
		Component* GetParent() const {
			return this->parent_;
		}
		/**
		 * In some cases, it would be beneficial to define the child-management
		 * operations right in the base Component class. This way, you won't need to
		 * expose any concrete component classes to the client code, even during the
		 * object tree assembly. The downside is that these methods will be empty for
		 * the leaf-level components.
		 */
		virtual void Add(Component* component) {}
		virtual void Remove(Component* component) {}
		/**
		 * You can provide a method that lets the client code figure out whether a
		 * component can bear children.
		 */
		virtual bool IsComposite() const {
			return false;
		}
		/**
		 * The base Component may implement some default behavior or leave it to
		 * concrete classes (by declaring the method containing the behavior as
		 * "abstract").
		 */
		virtual std::string Operation() const = 0;
	};

	class Leaf : public Component {
	public:
		std::string Operation() const override {
			return "Leaf";
		}
	};

	class Composite : public Component {
		
	protected:
		std::list<Component*> children_;

	public:
		/**
		 * A composite object can add or remove other components (both simple or
		 * complex) to or from its child list.
		 */
		void Add(Component* component) override {
			this->children_.push_back(component);
			component->SetParent(this);
		}
		/**
		 * Have in mind that this method removes the pointer to the list but doesn't
		 * frees the
		 *     memory, you should do it manually or better use smart pointers.
		 */
		void Remove(Component* component) override {
			children_.remove(component);
			component->SetParent(nullptr);
		}
		bool IsComposite() const override {
			return true;
		}
		/**
		 * The Composite executes its primary logic in a particular way. It traverses
		 * recursively through all its children, collecting and summing their results.
		 * Since the composite's children pass these calls to their children and so
		 * forth, the whole object tree is traversed as a result.
		 */
		std::string Operation() const override {
			std::string result;
			for (const Component* c : children_) {
				if (c == children_.back()) {
					result += c->Operation();
				}
				else {
					result += c->Operation() + "+";
				}
			}
			return "Branch(" + result + ")";
		}
	};

	void ClientCode(Component* component) {
		// ...
		std::cout << "RESULT: " << component->Operation();
		// ...
	}

	void ClientCode2(Component* component1, Component* component2) {
		// ...
		if (component1->IsComposite()) {
			component1->Add(component2);
		}
		std::cout << "RESULT: " << component1->Operation();
		// ...
	}


}

TEST(dp23, composite) {
	using namespace composite;
	Component* simple = new Leaf;
	std::cout << "Client: I've got a simple component:\n";
	ClientCode(simple);
	std::cout << "\n\n";
	
	Component* tree = new Composite;
	Component* branch1 = new Composite;

	Component* leaf_1 = new Leaf;
	Component* leaf_2 = new Leaf;
	Component* leaf_3 = new Leaf;
	branch1->Add(leaf_1);
	branch1->Add(leaf_2);
	Component* branch2 = new Composite;
	branch2->Add(leaf_3);
	tree->Add(branch1);
	tree->Add(branch2);
	std::cout << "Client: Now I've got a composite tree:\n";
	ClientCode(tree);
	std::cout << "\n\n";

	std::cout << "Client: I don't need to check the components classes even when managing the tree:\n";
	ClientCode2(tree, simple);
	std::cout << "\n";

	delete simple;
	delete tree;
	delete branch1;
	delete branch2;
	delete leaf_1;
	delete leaf_2;
	delete leaf_3;

}

namespace decorator {

	class Component {
	public:
		virtual ~Component() {}
		virtual std::string Operation() const = 0;
	};

	class ConcreteComponent : public Component {
	public:
		std::string Operation() const override {
			return "ConcreteComponent";
		}
	};

	class Decorator : public Component {

	protected:
		Component* component_;

	public:
		Decorator(Component* component) : component_(component) {
		}
		
		std::string Operation() const override {
			return this->component_->Operation();
		}
	};

	class ConcreteDecoratorA : public Decorator {
		/**
		 * Decorators may call parent implementation of the operation, instead of
		 * calling the wrapped object directly. This approach simplifies extension of
		 * decorator classes.
		 */
	public:
		ConcreteDecoratorA(Component* component) : Decorator(component) {
		}
		std::string Operation() const override {
			return "ConcreteDecoratorA(" + Decorator::Operation() + ")";
		}
	};
	/**
	 * Decorators can execute their behavior either before or after the call to a
	 * wrapped object.
	 */
	class ConcreteDecoratorB : public Decorator {
	public:
		ConcreteDecoratorB(Component* component) : Decorator(component) {
		}

		std::string Operation() const override {
			return "ConcreteDecoratorB(" + Decorator::Operation() + ")";
		}
	};

	void ClientCode(Component* component) {
		// ...
		std::cout << "RESULT: " << component->Operation();
		// ...
	}
}

TEST(dp23, decorator) {
	using namespace decorator;
	/**
   * This way the client code can support both simple components...
   */
	Component* simple = new ConcreteComponent;
	std::cout << "Client: I've got a simple component:\n";
	ClientCode(simple);
	std::cout << "\n\n";
	/**
	 * ...as well as decorated ones.
	 *
	 * Note how decorators can wrap not only simple components but the other
	 * decorators as well.
	 */
	Component* decorator1 = new ConcreteDecoratorA(simple);
	Component* decorator2 = new ConcreteDecoratorB(decorator1);
	std::cout << "Client: Now I've got a decorated component:\n";
	ClientCode(decorator2);
	std::cout << "\n";

	delete simple;
	delete decorator1;
	delete decorator2;
}

namespace facade {
	class Subsystem1 {
	public:
		std::string Operation1() const {
			return "Subsystem1: Ready!\n";
		}
		// ...
		std::string OperationN() const {
			return "Subsystem1: Go!\n";
		}
	};
	class Subsystem2 {
	public:
		std::string Operation1() const {
			return "Subsystem2: Get ready!\n";
		}
		// ...
		std::string OperationZ() const {
			return "Subsystem2: Fire!\n";
		}
	};

	class Facade {
	protected:
		Subsystem1* subsystem1_;
		Subsystem2* subsystem2_;
		
	public:
		
		Facade(
			Subsystem1* subsystem1 = nullptr,
			Subsystem2* subsystem2 = nullptr) 
		{
			this->subsystem1_ = subsystem1 ? subsystem1 : new Subsystem1;
			this->subsystem2_ = subsystem2 ? subsystem2 : new Subsystem2;
		}
		~Facade() {
			delete subsystem1_;
			delete subsystem2_;
		}
		
		std::string Operation() {
			std::string result = "Facade initializes subsystems:\n";
			result += this->subsystem1_->Operation1();
			result += this->subsystem2_->Operation1();
			result += "Facade orders subsystems to perform the action:\n";
			result += this->subsystem1_->OperationN();
			result += this->subsystem2_->OperationZ();
			return result;
		}
	};

	void ClientCode(Facade* facade) {
		// ...
		std::cout << facade->Operation();
		// ...
	}


}

TEST(dp23, facade) {
	using namespace facade;

	Subsystem1* subsystem1 = new Subsystem1;
	Subsystem2* subsystem2 = new Subsystem2;
	Facade* facade = new Facade(subsystem1, subsystem2);
	ClientCode(facade);

	delete facade;
}

namespace flyweight {
	struct SharedState {
		string brand_;
		string model_;
		string color_;

		SharedState(const string& brand,const string& model,const string& color):
				brand_(brand),model_(model), color_(color)
		{}

		friend std::ostream& operator << (std::ostream& os, const SharedState& ss) {
			return os << "[ " << ss.brand_ << ", " << ss.model_ << ", " << ss.color_ <<"]";
		}

	};

	struct UniqueState {
		string owner_;
		string plates_;
		UniqueState(const std::string& owner, const std::string& plates)
			: owner_(owner), plates_(plates)
		{
		}

		friend std::ostream& operator<<(std::ostream& os, const UniqueState& us)
		{
			return os << "[ " << us.owner_ << " , " << us.plates_ << " ]";
		}
	};

	class Flyweight {
	private:
		SharedState* shared_state_;
	public:
		Flyweight(const SharedState* shared_state) : shared_state_(new SharedState(*shared_state))
		{}
		Flyweight(const Flyweight& other) : shared_state_(new SharedState(*other.shared_state_)) {

		}

		~Flyweight() {
			delete shared_state_;
		}

		SharedState* shared_state() const {
			return shared_state_;
		}

		void Operation(const UniqueState& unique_state) const {
			std::cout << "Flyweight: Displaying shared (" << *shared_state_ << ") and unique (" << unique_state << ") state.\n";
		}

	};

	class FlyweightFactory {
	private:
		unordered_map<string, Flyweight> flyweights_;

		std::string GetKey(const SharedState& ss) const
		{
			return ss.brand_ + "_" + ss.model_ + "_" + ss.color_;
		}

	public:

		FlyweightFactory(std::initializer_list<SharedState> share_states)
		{
			for (const SharedState& ss : share_states)
			{
				this->flyweights_.insert(std::make_pair<std::string, Flyweight>(this->GetKey(ss), Flyweight(&ss)));
			}
		}


		void ListFlyweights() const
		{
			size_t count = this->flyweights_.size();
			std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
			for (std::pair<std::string, Flyweight> pair : this->flyweights_)
			{
				std::cout << pair.first << "\n";
			}
		}

		Flyweight GetFlyweight(const SharedState& shared_state)
		{
			std::string key = this->GetKey(shared_state);
			if (this->flyweights_.find(key) == this->flyweights_.end())
			{
				std::cout << "FlyweightFactory: Can't find a flyweight, creating new one.\n";
				this->flyweights_.insert(std::make_pair(key, Flyweight(&shared_state)));
			}
			else
			{
				std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
			}
			return this->flyweights_.at(key);
		}
	};


	void AddCarToPoliceDatabase(
		FlyweightFactory& ff, const std::string& plates, const std::string& owner,
		const std::string& brand, const std::string& model, const std::string& color)
	{
		std::cout << "\nClient: Adding a car to database.\n";
		const Flyweight& flyweight = ff.GetFlyweight({ brand, model, color });
		// The client code either stores or calculates extrinsic state and passes it
		// to the flyweight's methods.
		flyweight.Operation({ plates, owner });
	}

	void test_flyweight() {
		FlyweightFactory* factory = new FlyweightFactory(
			{ 
				{"Chevrolet", "Camaro2018", "pink"}, 
				{"Mercedes Benz", "C300", "black"}, 
				{"Mercedes Benz", "C500", "red"}, 
				{"BMW", "M5", "red"}, 
				{"BMW", "X6", "white"} 
			}
		);
		factory->ListFlyweights();

		AddCarToPoliceDatabase(*factory,
			"CL234IR", "James Doe","BMW","M5","red");

		AddCarToPoliceDatabase(*factory,
			"CL234IR","James Doe","BMW","X1","red");
		factory->ListFlyweights();
		delete factory;
	}

}// end ns

TEST(dp23, flyweight) {

	using namespace flyweight;
	
	test_flyweight();
}

namespace proxy {
	class Subject {
	public:
		virtual void Request() const = 0;
	};

	class RealSubject : public Subject {
	public:
		void Request() const override {
			std::cout << "RealSubject: Handling request.\n";
		}
	};

	class Proxy : public Subject {

	private:
		RealSubject* real_subject_;

		bool CheckAccess() const {
			// Some real checks should go here.
			std::cout << "Proxy: Checking access prior to firing a real request.\n";
			return true;
		}
		void LogAccess() const {
			std::cout << "Proxy: Logging the time of request.\n";
		}

		/**
		 * The Proxy maintains a reference to an object of the RealSubject class. It
		 * can be either lazy-loaded or passed to the Proxy by the client.
		 */
	public:
		Proxy(RealSubject* real_subject) : real_subject_(new RealSubject(*real_subject)) {
		}

		~Proxy() {
			delete real_subject_;
		}
		/**
		 * The most common applications of the Proxy pattern are lazy loading,
		 * caching, controlling the access, logging, etc. A Proxy can perform one of
		 * these things and then, depending on the result, pass the execution to the
		 * same method in a linked RealSubject object.
		 */
		void Request() const override {
			if (this->CheckAccess()) {
				this->real_subject_->Request();
				this->LogAccess();
			}
		}
	};

	void ClientCode(const Subject& subject) {
		// ...
		subject.Request();
		// ...
	}
}

TEST(dp23, proxy) {
	using namespace proxy;
	std::cout << "Client: Executing the client code with a real subject:\n";
	RealSubject* real_subject = new RealSubject;
	ClientCode(*real_subject);
	std::cout << "\n";
	std::cout << "Client: Executing the same client code with a proxy:\n";
	Proxy* proxy = new Proxy(real_subject);
	ClientCode(*proxy);

	delete real_subject;
	delete proxy;
}