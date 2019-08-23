#include "pch.h"

#include <initializer_list>

/*
template<class _Elem>	class initializer_list{
public:
	typedef _Elem value_type;	typedef const _Elem& reference;
	typedef const _Elem& const_reference;	typedef size_t size_type;
	typedef const _Elem* iterator;	typedef const _Elem* const_iterator;

	constexpr initializer_list() noexcept
		: _First(nullptr), _Last(nullptr){	}

	constexpr initializer_list(const _Elem *_First_arg,
		const _Elem *_Last_arg) noexcept
		: _First(_First_arg), _Last(_Last_arg)
		{	// construct with pointers
		}
	 constexpr const _Elem * begin() const noexcept
		{		return (_First);	}

	 constexpr const _Elem * end() const noexcept
		{	return (_Last);	}

	 constexpr size_t size() const noexcept
		{	return (static_cast<size_t>(_Last - _First));}
private:
	const _Elem *_First;
	const _Elem *_Last;
	};

Require compile to convert {1,2,3,4,5} ->  initializer_list object -> some thing like a range

*/


using namespace std;

