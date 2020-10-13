#include "pch.h"

// chain of responsibility
namespace chainofresponsibility {

	class Handler {
	public:
		virtual Handler* SetNext(Handler* handler) = 0;
		virtual std::string Handle(std::string request) = 0;
	};

    class AbstractHandler : public Handler {
        /**
         * @var Handler
         */
    private:
        Handler* next_handler_;

    public:
        AbstractHandler() : next_handler_(nullptr) {
        }
        Handler* SetNext(Handler* handler) override {
            this->next_handler_ = handler;
            // Returning a handler from here will let us link handlers in a convenient
            // way like this:
            // $monkey->setNext($squirrel)->setNext($dog);
            return handler;
        }
        std::string Handle(std::string request) override {
            if (this->next_handler_) {
                return this->next_handler_->Handle(request);
            }

            return {};
        }
    };

    class MonkeyHandler : public AbstractHandler {
    public:
        std::string Handle(std::string request) override {
            if (request == "Banana") {
                return "Monkey: I'll eat the " + request + ".\n";
            }
            else {
                return AbstractHandler::Handle(request);
            }
        }
    };

    class SquirrelHandler : public AbstractHandler {
    public:
        std::string Handle(std::string request) override {
            if (request == "Nut") {
                return "Squirrel: I'll eat the " + request + ".\n";
            }
            else {
                return AbstractHandler::Handle(request);
            }
        }
    };

    class DogHandler : public AbstractHandler {
    public:
        std::string Handle(std::string request) override {
            if (request == "MeatBall") {
                return "Dog: I'll eat the " + request + ".\n";
            }
            else {
                return AbstractHandler::Handle(request);
            }
        }
    };

    void ClientCode(Handler& handler) {
        std::vector<std::string> food = { "Nut", "Banana", "Cup of coffee" };
        for (const std::string& f : food) {
            std::cout << "Client: Who wants a " << f << "?\n";
            const std::string result = handler.Handle(f);
            if (!result.empty()) {
                std::cout << "  " << result;
            }
            else {
                std::cout << "  " << f << " was left untouched.\n";
            }
        }
    }

}

TEST(dp23, chainofresponsibility) {
    using namespace chainofresponsibility;

    MonkeyHandler* monkey = new MonkeyHandler;
    SquirrelHandler* squirrel = new SquirrelHandler;
    DogHandler* dog = new DogHandler;
    monkey->SetNext(squirrel)->SetNext(dog);

    /**
     * The client should be able to send a request to any handler, not just the
     * first one in the chain.
     */
    std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
    ClientCode(*monkey);
    std::cout << "\n";
    std::cout << "Subchain: Squirrel > Dog\n\n";
    ClientCode(*squirrel);

    delete monkey;
    delete squirrel;
    delete dog;
}

namespace command {
    class Command {
    public:
        virtual ~Command() {
        }
        virtual void Execute() const = 0;
    };

    class SimpleCommand : public Command {
    private:
        std::string pay_load_;

    public:
        explicit SimpleCommand(std::string pay_load) : pay_load_(pay_load) {
        }
        void Execute() const override {
            std::cout << "SimpleCommand: See, I can do simple things like printing (" << this->pay_load_ << ")\n";
        }
    };

    class Receiver {
    public:
        void DoSomething(const std::string& a) {
            std::cout << "Receiver: Working on (" << a << ".)\n";
        }
        void DoSomethingElse(const std::string& b) {
            std::cout << "Receiver: Also working on (" << b << ".)\n";
        }
    };

    class ComplexCommand : public Command {
        /**
         * @var Receiver
         */
    private:
        Receiver* receiver_;
        /**
         * Context data, required for launching the receiver's methods.
         */
        std::string a_;
        std::string b_;
        /**
         * Complex commands can accept one or several receiver objects along with any
         * context data via the constructor.
         */
    public:
        ComplexCommand(Receiver* receiver, std::string a, std::string b) : receiver_(receiver), a_(a), b_(b) {
        }
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override {
            std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
            this->receiver_->DoSomething(this->a_);
            this->receiver_->DoSomethingElse(this->b_);
        }
    };

    class Invoker {
        /**
         * @var Command
         */
    private:
        Command* on_start_;
        /**
         * @var Command
         */
        Command* on_finish_;
        /**
         * Initialize commands.
         */
    public:
        ~Invoker() {
            delete on_start_;
            delete on_finish_;
        }

        void SetOnStart(Command* command) {
            this->on_start_ = command;
        }
        void SetOnFinish(Command* command) {
            this->on_finish_ = command;
        }
        /**
         * The Invoker does not depend on concrete command or receiver classes. The
         * Invoker passes a request to a receiver indirectly, by executing a command.
         */
        void DoSomethingImportant() {
            std::cout << "Invoker: Does anybody want something done before I begin?\n";
            if (this->on_start_) {
                this->on_start_->Execute();
            }
            std::cout << "Invoker: ...doing something really important...\n";
            std::cout << "Invoker: Does anybody want something done after I finish?\n";
            if (this->on_finish_) {
                this->on_finish_->Execute();
            }
        }
    };




}

TEST(dp23, command) {
    using namespace command;

    Invoker* invoker = new Invoker;
    invoker->SetOnStart(new SimpleCommand("Say Hi!"));
    Receiver* receiver = new Receiver;
    invoker->SetOnFinish(new ComplexCommand(receiver, "Send email", "Save report"));
    invoker->DoSomethingImportant();

    delete invoker;
    delete receiver;
}

namespace iterator {

    template <typename T, typename U>
    class Iterator {
    public:
        typedef typename std::vector<T>::iterator iter_type;
        Iterator(U* p_data, bool reverse = false) : m_p_data_(p_data) {
            m_it_ = m_p_data_->m_data_.begin();
        }

        void First() {
            m_it_ = m_p_data_->m_data_.begin();
        }

        void Next() {
            m_it_++;
        }

        bool IsDone() {
            return (m_it_ == m_p_data_->m_data_.end());
        }

        iter_type Current() {
            return m_it_;
        }

    private:
        U* m_p_data_;
        iter_type m_it_;
    };

    template <class T>
    class Container {
        friend class Iterator<T, Container>;

    public:
        void Add(T a) {
            m_data_.push_back(a);
        }

        Iterator<T, Container>* CreateIterator() {
            return new Iterator<T, Container>(this);
        }

    private:
        std::vector<T> m_data_;
    };

    class Data {
    public:
        Data(int a = 0) : m_data_(a) {}

        void set_data(int a) {
            m_data_ = a;
        }

        int data() {
            return m_data_;
        }

    private:
        int m_data_;
    };

    void ClientCode() {
        std::cout << "________________Iterator with int______________________________________" << std::endl;
        Container<int> cont;

        for (int i = 0; i < 10; i++) {
            cont.Add(i);
        }

        Iterator<int, Container<int>>* it = cont.CreateIterator();
        for (it->First(); !it->IsDone(); it->Next()) {
            std::cout << *it->Current() << std::endl;
        }

        Container<Data> cont2;
        Data a(100), b(1000), c(10000);
        cont2.Add(a);
        cont2.Add(b);
        cont2.Add(c);

        std::cout << "________________Iterator with custom Class______________________________" << std::endl;
        Iterator<Data, Container<Data>>* it2 = cont2.CreateIterator();
        for (it2->First(); !it2->IsDone(); it2->Next()) {
            std::cout << it2->Current()->data() << std::endl;
        }
        delete it;
        delete it2;
    }

}

TEST(dp23, iterator) {
    using namespace iterator;
    ClientCode();
}

namespace mediator {
    class BaseComponent;
    class Mediator {
    public:
        virtual void Notify(BaseComponent* sender, std::string event) const = 0;
    };

    class BaseComponent {
    protected:
        Mediator* mediator_;

    public:
        BaseComponent(Mediator* mediator = nullptr) : mediator_(mediator) {
        }
        void set_mediator(Mediator* mediator) {
            this->mediator_ = mediator;
        }
    };

    class Component1 : public BaseComponent {
    public:
        void DoA() {
            std::cout << "Component 1 does A.\n";
            this->mediator_->Notify(this, "A");
        }
        void DoB() {
            std::cout << "Component 1 does B.\n";
            this->mediator_->Notify(this, "B");
        }
    };

    class Component2 : public BaseComponent {
    public:
        void DoC() {
            std::cout << "Component 2 does C.\n";
            this->mediator_->Notify(this, "C");
        }
        void DoD() {
            std::cout << "Component 2 does D.\n";
            this->mediator_->Notify(this, "D");
        }
    };

    class ConcreteMediator : public Mediator {
    private:
        Component1* component1_;
        Component2* component2_;

    public:
        ConcreteMediator(Component1* c1, Component2* c2) : component1_(c1), component2_(c2) {
            this->component1_->set_mediator(this);
            this->component2_->set_mediator(this);
        }
        void Notify(BaseComponent* sender, std::string event) const override {
            if (event == "A") {
                std::cout << "Mediator reacts on A and triggers following operations:\n";
                this->component2_->DoC();
            }
            if (event == "D") {
                std::cout << "Mediator reacts on D and triggers following operations:\n";
                this->component1_->DoB();
                this->component2_->DoC();
            }
        }
    };

    void ClientCode() {
        Component1* c1 = new Component1;
        Component2* c2 = new Component2;
        ConcreteMediator* mediator = new ConcreteMediator(c1, c2);
        std::cout << "Client triggers operation A.\n";
        c1->DoA();
        std::cout << "\n";
        std::cout << "Client triggers operation D.\n";
        c2->DoD();

        delete c1;
        delete c2;
        delete mediator;
    }

}

TEST(dp23, mediator) {
    using namespace mediator;
    ClientCode();
}

namespace memento {
    class Memento {
    public:
        virtual std::string GetName() const = 0;
        virtual std::string date() const = 0;
        virtual std::string state() const = 0;
    };

    class ConcreteMemento : public Memento {
    private:
        std::string state_;
        std::string date_;

    public:
        ConcreteMemento(std::string state) : state_(state) {
            this->state_ = state;
            std::time_t now = std::time(0);
            this->date_ = std::ctime(&now);
        }
        /**
         * The Originator uses this method when restoring its state.
         */
        std::string state() const override {
            return this->state_;
        }
        /**
         * The rest of the methods are used by the Caretaker to display metadata.
         */
        std::string GetName() const override {
            return this->date_ + " / (" + this->state_.substr(0, 9) + "...)";
        }
        std::string date() const override {
            return this->date_;
        }
    };

    /**
 * The Originator holds some important state that may change over time. It also
 * defines a method for saving the state inside a memento and another method for
 * restoring the state from it.
 */
    class Originator {
    private:
        std::string state_;

        std::string GenerateRandomString(int length = 10) {
            const char alphanum[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
            int stringLength = sizeof(alphanum) - 1;

            std::string random_string;
            for (int i = 0; i < length; i++) {
                random_string += alphanum[std::rand() % stringLength];
            }
            return random_string;
        }

    public:
        Originator(std::string state) : state_(state) {
            std::cout << "Originator: My initial state is: " << this->state_ << "\n";
        }
        /**
         * The Originator's business logic may affect its internal state. Therefore,
         * the client should backup the state before launching methods of the business
         * logic via the save() method.
         */
        void DoSomething() {
            std::cout << "Originator: I'm doing something important.\n";
            this->state_ = this->GenerateRandomString(30);
            std::cout << "Originator: and my state has changed to: " << this->state_ << "\n";
        }

        /**
         * Saves the current state inside a memento.
         */
        Memento* Save() {
            return new ConcreteMemento(this->state_);
        }
        /**
         * Restores the Originator's state from a memento object.
         */
        void Restore(Memento* memento) {
            this->state_ = memento->state();
            std::cout << "Originator: My state has changed to: " << this->state_ << "\n";
        }
    };

    class Caretaker {
    private:
        std::vector<Memento*> mementos_;

        Originator* originator_;

    public:
        Caretaker(Originator* originator) : originator_(originator) {
            this->originator_ = originator;
        }

        void Backup() {
            std::cout << "\nCaretaker: Saving Originator's state...\n";
            this->mementos_.push_back(this->originator_->Save());
        }
        void Undo() {
            if (!this->mementos_.size()) {
                return;
            }
            Memento* memento = this->mementos_.back();
            this->mementos_.pop_back();
            std::cout << "Caretaker: Restoring state to: " << memento->GetName() << "\n";
            try {
                this->originator_->Restore(memento);
            }
            catch (...) {
                this->Undo();
            }
        }
        void ShowHistory() const {
            std::cout << "Caretaker: Here's the list of mementos:\n";
            for (Memento* memento : this->mementos_) {
                std::cout << memento->GetName() << "\n";
            }
        }
    };

    void ClientCode() {
        Originator* originator = new Originator("Super-duper-super-puper-super.");
        Caretaker* caretaker = new Caretaker(originator);
        caretaker->Backup();
        originator->DoSomething();
        caretaker->Backup();
        originator->DoSomething();
        caretaker->Backup();
        originator->DoSomething();
        std::cout << "\n";
        caretaker->ShowHistory();
        std::cout << "\nClient: Now, let's rollback!\n\n";
        caretaker->Undo();
        std::cout << "\nClient: Once more!\n\n";
        caretaker->Undo();

        delete originator;
        delete caretaker;
    }

}

TEST(dp23, memento) {
    using namespace memento;
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    ClientCode();

}

namespace observer {
    class IObserver {
    public:
        virtual ~IObserver() {};
        virtual void Update(const std::string& message_from_subject) = 0;
    };

    class ISubject {
    public:
        virtual ~ISubject() {};
        virtual void Attach(IObserver* observer) = 0;
        virtual void Detach(IObserver* observer) = 0;
        virtual void Notify() = 0;
    };

    class Subject : public ISubject {
    public:
        virtual ~Subject() {
            std::cout << "Goodbye, I was the Subject.\n";
        }

        /**
         * The subscription management methods.
         */
        void Attach(IObserver* observer) override {
            list_observer_.push_back(observer);
        }
        void Detach(IObserver* observer) override {
            list_observer_.remove(observer);
        }
        void Notify() override {
            std::list<IObserver*>::iterator iterator = list_observer_.begin();
            HowManyObserver();
            while (iterator != list_observer_.end()) {
                (*iterator)->Update(message_);
                ++iterator;
            }
        }

        void CreateMessage(std::string message = "Empty") {
            this->message_ = message;
            Notify();
        }
        void HowManyObserver() {
            std::cout << "There are " << list_observer_.size() << " observers in the list.\n";
        }
        /**
   * Usually, the subscription logic is only a fraction of what a Subject can
   * really do. Subjects commonly hold some important business logic, that
   * triggers a notification method whenever something important is about to
   * happen (or after it).
   */
        void SomeBusinessLogic() {
            this->message_ = "change message message";
            Notify();
            std::cout << "I'm about to do some thing important\n";
        }

    private:
        std::list<IObserver*> list_observer_;
        std::string message_;
    };

    class Observer : public IObserver {
    public:
        Observer(Subject& subject) : subject_(subject) {
            this->subject_.Attach(this);
            std::cout << "Hi, I'm the Observer \"" << ++Observer::static_number_ << "\".\n";
            this->number_ = Observer::static_number_;
        }
        virtual ~Observer() {
            std::cout << "Goodbye, I was the Observer \"" << this->number_ << "\".\n";
        }

        void Update(const std::string& message_from_subject) override {
            message_from_subject_ = message_from_subject;
            PrintInfo();
        }
        void RemoveMeFromTheList() {
            subject_.Detach(this);
            std::cout << "Observer \"" << number_ << "\" removed from the list.\n";
        }
        void PrintInfo() {
            std::cout << "Observer \"" << this->number_ << "\": a new message is available --> " << this->message_from_subject_ << "\n";
        }

    private:
        std::string message_from_subject_;
        Subject& subject_;
        static int static_number_;
        int number_;
    };

    int Observer::static_number_ = 0;

    void ClientCode() {
        Subject* subject = new Subject;
        Observer* observer1 = new Observer(*subject);
        Observer* observer2 = new Observer(*subject);
        Observer* observer3 = new Observer(*subject);
        Observer* observer4;
        Observer* observer5;

        subject->CreateMessage("Hello World! :D");
        observer3->RemoveMeFromTheList();

        subject->CreateMessage("The weather is hot today! :p");
        observer4 = new Observer(*subject);

        observer2->RemoveMeFromTheList();
        observer5 = new Observer(*subject);

        subject->CreateMessage("My new car is great! ;)");
        observer5->RemoveMeFromTheList();

        observer4->RemoveMeFromTheList();
        observer1->RemoveMeFromTheList();

        delete observer5;
        delete observer4;
        delete observer3;
        delete observer2;
        delete observer1;
        delete subject;
    }

}

TEST(dp23, observer) {
    using namespace observer;
    ClientCode();
}


namespace state {
    class Context;
    class State {
        /**
         * @var Context
         */
    protected:
        Context* context_;

    public:
        virtual ~State() {
        }

        void set_context(Context* context) {
            this->context_ = context;
        }

        virtual void Handle1() = 0;
        virtual void Handle2() = 0;
    };

    class Context {
        /**
         * @var State A reference to the current state of the Context.
         */
    private:
        State* state_;

    public:
        Context(State* state) : state_(nullptr) {
            this->TransitionTo(state);
        }
        ~Context() {
            delete state_;
        }
        /**
         * The Context allows changing the State object at runtime.
         */
        void TransitionTo(State* state) {
            std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";
            if (this->state_ != nullptr)
                delete this->state_;
            this->state_ = state;
            this->state_->set_context(this);
        }
        /**
         * The Context delegates part of its behavior to the current State object.
         */
        void Request1() {
            this->state_->Handle1();
        }
        void Request2() {
            this->state_->Handle2();
        }
    };

    class ConcreteStateA : public State {
    public:
        void Handle1() override;

        void Handle2() override {
            std::cout << "ConcreteStateA handles request2.\n";
        }
    };

    class ConcreteStateB : public State {
    public:
        void Handle1() override {
            std::cout << "ConcreteStateB handles request1.\n";
        }
        void Handle2() override {
            std::cout << "ConcreteStateB handles request2.\n";
            std::cout << "ConcreteStateB wants to change the state of the context.\n";
            this->context_->TransitionTo(new ConcreteStateA);
        }
    };

    void ConcreteStateA::Handle1() {
        {
            std::cout << "ConcreteStateA handles request1.\n";
            std::cout << "ConcreteStateA wants to change the state of the context.\n";

            this->context_->TransitionTo(new ConcreteStateB);
        }
    }

    void ClientCode() {
        Context* context = new Context(new ConcreteStateA);
        context->Request1();
        context->Request2();
        delete context;
    }
}

TEST(dp23, state) {
    using namespace state;
    ClientCode();
}

namespace strategy {
    class Strategy
    {
    public:
        virtual ~Strategy() {}
        virtual std::string DoAlgorithm(const std::vector<std::string>& data) const = 0;
    };

    class Context
    {
        /**
         * @var Strategy The Context maintains a reference to one of the Strategy
         * objects. The Context does not know the concrete class of a strategy. It
         * should work with all strategies via the Strategy interface.
         */
    private:
        Strategy* strategy_;
        /**
         * Usually, the Context accepts a strategy through the constructor, but also
         * provides a setter to change it at runtime.
         */
    public:
        Context(Strategy* strategy = nullptr) : strategy_(strategy)
        {
        }
        ~Context()
        {
            delete this->strategy_;
        }
        /**
         * Usually, the Context allows replacing a Strategy object at runtime.
         */
        void set_strategy(Strategy* strategy)
        {
            delete this->strategy_;
            this->strategy_ = strategy;
        }
        /**
         * The Context delegates some work to the Strategy object instead of
         * implementing +multiple versions of the algorithm on its own.
         */
        void DoSomeBusinessLogic() const
        {
            // ...
            std::cout << "Context: Sorting data using the strategy (not sure how it'll do it)\n";
            std::string result = this->strategy_->DoAlgorithm(std::vector<std::string>{"a", "e", "c", "b", "d"});
            std::cout << result << "\n";
            // ...
        }
    };

    class ConcreteStrategyA : public Strategy
    {
    public:
        std::string DoAlgorithm(const std::vector<std::string>& data) const override
        {
            std::string result;
            std::for_each(std::begin(data), std::end(data), [&result](const std::string& letter) {
                result += letter;
                });
            std::sort(std::begin(result), std::end(result));

            return result;
        }
    };

    class ConcreteStrategyB : public Strategy
    {
        std::string DoAlgorithm(const std::vector<std::string>& data) const override
        {
            std::string result;
            std::for_each(std::begin(data), std::end(data), [&result](const std::string& letter) {
                result += letter;
                });
            std::sort(std::begin(result), std::end(result));
            for (int i = 0; i < result.size() / 2; i++)
            {
                std::swap(result[i], result[result.size() - i - 1]);
            }

            return result;
        }
    };

    void ClientCode()
    {
        Context* context = new Context(new ConcreteStrategyA);
        std::cout << "Client: Strategy is set to normal sorting.\n";
        context->DoSomeBusinessLogic();
        std::cout << "\n";
        std::cout << "Client: Strategy is set to reverse sorting.\n";
        context->set_strategy(new ConcreteStrategyB);
        context->DoSomeBusinessLogic();
        delete context;
    }

}

TEST(dp23, strategy) {
    using namespace strategy;
    ClientCode();
}

namespace templateM {
    class AbstractClass {
        /**
         * The template method defines the skeleton of an algorithm.
         */
    public:
        void TemplateMethod() const {
            this->BaseOperation1();
            this->RequiredOperations1();
            this->BaseOperation2();
            this->Hook1();
            this->RequiredOperation2();
            this->BaseOperation3();
            this->Hook2();
        }
        /**
         * These operations already have implementations.
         */
    protected:
        void BaseOperation1() const {
            std::cout << "AbstractClass says: I am doing the bulk of the work\n";
        }
        void BaseOperation2() const {
            std::cout << "AbstractClass says: But I let subclasses override some operations\n";
        }
        void BaseOperation3() const {
            std::cout << "AbstractClass says: But I am doing the bulk of the work anyway\n";
        }
        /**
         * These operations have to be implemented in subclasses.
         */
        virtual void RequiredOperations1() const = 0;
        virtual void RequiredOperation2() const = 0;
        /**
         * These are "hooks." Subclasses may override them, but it's not mandatory
         * since the hooks already have default (but empty) implementation. Hooks
         * provide additional extension points in some crucial places of the
         * algorithm.
         */
        virtual void Hook1() const {}
        virtual void Hook2() const {}
    };

    class ConcreteClass1 : public AbstractClass {
    protected:
        void RequiredOperations1() const override {
            std::cout << "ConcreteClass1 says: Implemented Operation1\n";
        }
        void RequiredOperation2() const override {
            std::cout << "ConcreteClass1 says: Implemented Operation2\n";
        }
    };

    class ConcreteClass2 : public AbstractClass {
    protected:
        void RequiredOperations1() const override {
            std::cout << "ConcreteClass2 says: Implemented Operation1\n";
        }
        void RequiredOperation2() const override {
            std::cout << "ConcreteClass2 says: Implemented Operation2\n";
        }
        void Hook1() const override {
            std::cout << "ConcreteClass2 says: Overridden Hook1\n";
        }
    };

    void ClientCode(AbstractClass* class_) {
        // ...
        class_->TemplateMethod();
        // ...
    }

}

TEST(dp23, templateM) {
    using namespace templateM;
    std::cout << "Same client code can work with different subclasses:\n";
    ConcreteClass1* concreteClass1 = new ConcreteClass1;
    ClientCode(concreteClass1);
    std::cout << "\n";
    std::cout << "Same client code can work with different subclasses:\n";
    ConcreteClass2* concreteClass2 = new ConcreteClass2;
    ClientCode(concreteClass2);
    delete concreteClass1;
    delete concreteClass2;
}

namespace visitor {

    class ConcreteComponentA;
    class ConcreteComponentB;

    class Visitor {
    public:
        virtual void VisitConcreteComponentA(const ConcreteComponentA* element) const = 0;
        virtual void VisitConcreteComponentB(const ConcreteComponentB* element) const = 0;
    };

    class Component {
    public:
        virtual ~Component() {}
        virtual void Accept(Visitor* visitor) const = 0;
    };

    class ConcreteComponentA : public Component {
        /**
         * Note that we're calling `visitConcreteComponentA`, which matches the
         * current class name. This way we let the visitor know the class of the
         * component it works with.
         */
    public:
        void Accept(Visitor* visitor) const override {
            visitor->VisitConcreteComponentA(this);
        }
        /**
         * Concrete Components may have special methods that don't exist in their base
         * class or interface. The Visitor is still able to use these methods since
         * it's aware of the component's concrete class.
         */
        std::string ExclusiveMethodOfConcreteComponentA() const {
            return "A";
        }
    };

    class ConcreteComponentB : public Component {
        /**
         * Same here: visitConcreteComponentB => ConcreteComponentB
         */
    public:
        void Accept(Visitor* visitor) const override {
            visitor->VisitConcreteComponentB(this);
        }
        std::string SpecialMethodOfConcreteComponentB() const {
            return "B";
        }
    };

    class ConcreteVisitor1 : public Visitor {
    public:
        void VisitConcreteComponentA(const ConcreteComponentA* element) const override {
            std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor1\n";
        }

        void VisitConcreteComponentB(const ConcreteComponentB* element) const override {
            std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor1\n";
        }
    };

    class ConcreteVisitor2 : public Visitor {
    public:
        void VisitConcreteComponentA(const ConcreteComponentA* element) const override {
            std::cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor2\n";
        }
        void VisitConcreteComponentB(const ConcreteComponentB* element) const override {
            std::cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor2\n";
        }
    };

    void ClientCode(std::array<const Component*, 2> components, Visitor* visitor) {
        // ...
        for (const Component* comp : components) {
            comp->Accept(visitor);
        }
        // ...
    }
}

TEST(dp23, visitor) {
    using namespace visitor;

    std::array<const Component*, 2> components = { new ConcreteComponentA, new ConcreteComponentB };
    std::cout << "The client code works with all visitors via the base Visitor interface:\n";
    ConcreteVisitor1* visitor1 = new ConcreteVisitor1;
    ClientCode(components, visitor1);
    std::cout << "\n";
    std::cout << "It allows the same client code to work with different types of visitors:\n";
    ConcreteVisitor2* visitor2 = new ConcreteVisitor2;
    ClientCode(components, visitor2);

    for (const Component* comp : components) {
        delete comp;
    }
    delete visitor1;
    delete visitor2;

}