#include "pch.h"

// -c11, introduce a new keyword  override

// -c11 new concept,  reference_qualifier, see Widget

namespace kw_override {

	class Base {
	public:
		virtual void doWork()& =0;
		virtual void doWork() && = 0;;
	};
	
	class Widget :public Base {

	public:
		// give doWord two version
		void doWork()& override
		{
			cout << "*this is left-value\n";
		}

		void doWork()&& override 
		{
			cout << "*this is right-value\n";
		}
	};

	Widget makeWidget() {
		return Widget();
	}
}

TEST(Syntax, ReferenceQualifier) {
	using namespace kw_override;

	Widget w;

	w.doWork();				// letf-val
	Widget().doWork();		// right-val
	makeWidget().doWork();	// right-val




}