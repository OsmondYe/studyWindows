#include "pch.h"
/*
	更加泛化的new delete,
*/
template<class _Ty>
class oye_allocator
{
public:
	using value_type = _Ty;
	typedef _Ty * pointer;
	typedef const _Ty * const_pointer;
	typedef _Ty& reference;
	typedef const _Ty& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	constexpr oye_allocator() noexcept {}
	constexpr oye_allocator(const oye_allocator&) noexcept = default;
	template<class _Other>constexpr oye_allocator(const oye_allocator<_Other>&) noexcept {		}

	template<class _Other> struct  rebind{
		using other = oye_allocator<_Other>;
	};

	_Ty * address(_Ty& _Val) const noexcept{	
		// return address of mutable _Val
		return (_STD addressof(_Val));
	}
	const _Ty * address(const _Ty& _Val) const noexcept{
		// return address of nonmutable _Val
		return (_STD addressof(_Val));
	}

	_Ty * allocate(const size_t _Count){
		// allocate array of _Count elements
		return (static_cast<_Ty *>(_Allocate<_New_alignof<_Ty>>(_Get_size_of_n<sizeof(_Ty)>(_Count))));
	}

	_Ty * allocate(const size_t _Count, const void *){
		// allocate array of _Count elements, ignore hint
		return (allocate(_Count));
	}
	void deallocate(_Ty * const _Ptr, const size_t _Count){
		// deallocate object at _Ptr
		// no overflow check on the following multiply; we assume _Allocate did that check
		_Deallocate<_New_alignof<_Ty>>(_Ptr, sizeof(_Ty) * _Count);
	}

	template<class _Objty,
		class... _Types>
		void construct(_Objty * const _Ptr, _Types&&... _Args)
	{	// construct _Objty(_Types...) at _Ptr
		::new (const_cast<void *>(static_cast<const volatile void *>(_Ptr)))
			_Objty(_STD forward<_Types>(_Args)...);
	}

	template<class _Uty>
	void destroy(_Uty * const _Ptr)
	{	
		_Ptr->~_Uty();// destroy object at _Ptr
	}

	size_t max_size() const noexcept
	{	
		return (static_cast<size_t>(-1) / sizeof(_Ty));// estimate maximum array size
	}
};


// allocator只提供接口,让实现者自己去定义malloc,这样吧new 和 constuct分开
TEST(Allocator, Basic) {
	std::allocator<int> a;
	cout << "a.max_size()" <<std::hex<< a.max_size() << endl;
		
	int localInt = 12;	
	printf("localInt=%d, address=0x%p\n", localInt, &localInt);

	cout << "a.address(localInt)" << a.address(localInt) << endl;

	// alloc 
	int* p12 = a.allocate(12);
	// dealloc
	a.deallocate(p12, 12);

	std::allocator<std::string> a2;

	decltype(a)::rebind<std::string>::other a2_1;

	std::string* pstr = a2_1.allocate(2);
	a2_1.construct(pstr, "this is good");
	a2_1.construct(pstr + 1, "this is bad");

	std::cout << pstr[0] << pstr[1] << endl;

	a2_1.destroy(pstr);
	a2_1.destroy(pstr + 1);

	a2_1.deallocate(pstr, 2);
	
}