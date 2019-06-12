#include "pch.h"


/*
		// CLASS TEMPLATE allocator
template<class _Ty>
	class allocator
	{
public:
	using value_type = _Ty;
	 typedef _Ty * pointer;
	 typedef const _Ty * const_pointer;
	 typedef _Ty& reference;
	 typedef const _Ty& const_reference;
	 typedef size_t size_type;
	 typedef ptrdiff_t difference_type;

	constexpr allocator() noexcept	{		}

	constexpr allocator(const allocator&) noexcept = default;
	template<class _Other>constexpr allocator(const allocator<_Other>&) noexcept	{		}

	template<class _Other> struct  rebind
		{
		using other = allocator<_Other>;
		};
	_Ty * address(_Ty& _Val) const noexcept
		{	// return address of mutable _Val
		return (_STD addressof(_Val));
		}
	const _Ty * address(const _Ty& _Val) const noexcept
		{	// return address of nonmutable _Val
		return (_STD addressof(_Val));
		}

	void deallocate(_Ty * const _Ptr, const size_t _Count)
		{	// deallocate object at _Ptr
		// no overflow check on the following multiply; we assume _Allocate did that check
		_Deallocate<_New_alignof<_Ty>>(_Ptr, sizeof(_Ty) * _Count);
		}

	  _Ty * allocate( const size_t _Count)
		{	// allocate array of _Count elements
		return (static_cast<_Ty *>(_Allocate<_New_alignof<_Ty>>(_Get_size_of_n<sizeof(_Ty)>(_Count))));
		}

	   _Ty * allocate( const size_t _Count, const void *)
		{	// allocate array of _Count elements, ignore hint
		return (allocate(_Count));
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
		{	// destroy object at _Ptr
		_Ptr->~_Uty();
		}

	  size_t max_size() const noexcept
		{	// estimate maximum array size
		return (static_cast<size_t>(-1) / sizeof(_Ty));
		}
	};
*/


TEST(Allocator, Basic) {
	std::allocator<int> a;
	cout << "a.max_size()" << a.max_size() << endl;
		
	int localInt = 12;;
	
	printf("localInt=%d, address=0x%p\n", localInt, &localInt);

	cout << "a.address(localInt)" << a.address(localInt) << endl;

	int* p12 = a.allocate(12);
	a.deallocate(p12, 12);

	
	
}