 #include "pch.h"

// smart pointer
TEST(Memory, SmartPtr_Basic) {
	int* p = NULL;
	delete p;	// delete NULL has no side-effect

	cout << "delete NULL is safe\n";

	cout << "smart ptr is a class, will call delete in it's destructor\n";
}


namespace example_uptr {

	/*
		NO Copy semantics
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;
	*/

	class Investment;
	// will triger memleak, since Investment is not a whole class
	auto i_del(Investment* p) {
		delete p;
	}

	class Investment
	{
	public:
		virtual std::string name() noexcept = 0;
		virtual ~Investment() {			
			cout << __FUNCDNAME__ << endl;
			str = "hello world";
			cout << str;
		};
		string str{"base class Investment"};
	};

	class Stock : public Investment {
	public:
		Stock() {
			p.reset(new string("StockStockStockStockStockStockStock"));
		}

		// Inherited via Investment
		virtual std::string name() noexcept override
		{
			return "Stock"+*p;
		}
		~Stock() { std::cout << __FUNCTION__ <<str<<s_str<< endl; }
		string s_str{ "stock" };
		unique_ptr<string> p;
	};

	class Bond : public Investment {
	public:
		Bond() {
			p.reset(new string("Bond"));
		}
		// Inherited via Investment
		virtual std::string name() noexcept override
		{
			return "Bond"+*p;
		}
		~Bond() { 
			std::cout << __FUNCTION__ <<str<<b_str<< endl; 
		}
		string b_str{"bont"};
		unique_ptr<string> p;
	};

	class RealEstate : public Investment {
	public:
		RealEstate() {
			p.reset(new string("RealEstate"));
		}
		// Inherited via Investment
		virtual std::string name() noexcept override
		{
			return "RealEstate"+*p;
		}
		~RealEstate() { 
			std::cout << __FUNCTION__ <<str<<r_str<<*p<< endl; 
		}
		string r_str{"realestate"};
		unique_ptr<string> p;
	};



	auto delInvest = [](Investment* p) {
		cout << "del Investment:" << typeid(p).name() << endl;
		delete p;
	};
	// !!mem leak
	std::unique_ptr<Investment, decltype(delInvest)> build(int type) {

		std::unique_ptr<Investment, decltype(delInvest)>spRt(nullptr,delInvest);

		switch (type)
		{
		case 1:
			/*
			template <class _Dx2 = _Dx, _Unique_ptr_enable_default_t<_Dx2> = 0>
			explicit unique_ptr(pointer _Ptr) noexcept : _Mypair(_Zero_then_variadic_args_t(), _Ptr) {}
			*/
			spRt.reset(new Stock());
			break;
		case 2:	
			spRt.reset(new Bond());
			break;
		case 3:
			spRt.reset(new RealEstate);
			break;
		default:
			break;
		}
		return spRt;
	}

	std::unique_ptr<Investment> build2(int type)
	{
		std::unique_ptr<Investment>spRt(nullptr);

		switch (type)
		{
		case 1:
			/*
			template <class _Dx2 = _Dx, _Unique_ptr_enable_default_t<_Dx2> = 0>
			explicit unique_ptr(pointer _Ptr) noexcept : _Mypair(_Zero_then_variadic_args_t(), _Ptr) {}
			*/
			spRt.reset(new Stock());
			break;
		case 2:
			spRt.reset(new Bond());
			break;
		case 3:
			spRt.reset(new RealEstate);
			break;
		default:
			break;
		}
		return spRt;
	}



	template<typename ... Ts>
	std::unique_ptr<Investment,decltype(delInvest)>
	makeInvest(Ts&& ... params) {
		std::unique_ptr<Investment, decltype(delInvest)> pInv(nullptr, delInvest);

		pInv.reset(new Stock(std::forward<Ts>(params)));

		return pInv;
	}

}

TEST(Memory, SmartPtr_Unique) {	
	cout << "non-copyable pointer to an object\n";
	cout << "unique means: unique object ownership semantics\ncan be used in stl\n";
	cout << "share the same semantics with naked pointer\n";
	cout << "can be moved, can't copy, move will transfer the owenership\n";

	using namespace example_uptr;
	//for(int j=0;j<10000000;j++)
	// !!mem leak
	for (int i = 1; i <= 3; i++) {
		auto  sp = build(i);  // sp a scope obje, will call ~unique_prt at end of current loop nth;
		cout << sp->name() << endl;
		
	}
	for (int i = 1; i <= 3; ++i) {
		auto sp2 = build2(i);
		cout << sp2->name() << endl;
	}


	unique_ptr<Investment> b(new Bond());
	cout << b->name() << endl;


	Investment* p = new RealEstate();
	cout << "!!mem leak for p:" << p->name() << endl;
	i_del(p);

	cout << "endl;;;" << endl;

	// convert unique_ptr to shared_ptr

}

TEST(Memory, SmartPtr_Weak) {
}

TEST(Memory, SmartPtr_Shared) {
	// shared_ptr
	std::shared_ptr<int> spInt(new int(12));
}

TEST(Memory, SmartPtr_Array) {
	// unique_prt has buildin

	std::unique_ptr<int[]> spArray(new int[10]); // must init the value

	// give init value
	std::fill(spArray.get(), spArray.get() + 10, 0);



	for (int i = 0; i < 10; ++i) {
		spArray[i] = i;
	}
	

}

