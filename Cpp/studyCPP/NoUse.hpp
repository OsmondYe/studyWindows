



// CLASS TEMPLATE vector
template<class _Ty,	class _Alloc = allocator<_Ty>>
	class vector: public _Vector_alloc<_Vec_base_types<_Ty, _Alloc>>
{	// varying size array of values
private:
	using _Mybase = _Vector_alloc<_Vec_base_types<_Ty, _Alloc>>;
	using _Alty = typename _Mybase::_Alty;
	using _Alty_traits = typename _Mybase::_Alty_traits;
public:
	using value_type = _Ty;
	using allocator_type = _Alloc;
	using pointer = typename _Mybase::pointer;
	using const_pointer = typename _Mybase::const_pointer;
	using reference = _Ty & ;
	using const_reference = const _Ty&;
	using size_type = typename _Mybase::size_type;
	using difference_type = typename _Mybase::difference_type;
	using iterator = typename _Mybase::iterator;
	using const_iterator = typename _Mybase::const_iterator;
	using reverse_iterator = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;



	explicit vector(_CRT_GUARDOVERFLOW const size_type _Count, const _Alloc& _Al = _Alloc())
		: _Mybase(_Al)
	{	// construct from _Count * _Ty(), optional allocator
		if (_Buy(_Count))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast() = _Udefault(this->_Myfirst(), _Count);
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

	vector(_CRT_GUARDOVERFLOW const size_type _Count, const _Ty& _Val, const _Alloc& _Al = _Alloc())
		: _Mybase(_Al)
	{	// construct from _Count * _Val, optional allocator
		if (_Buy(_Count))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast() = _Ufill(this->_Myfirst(), _Count, _Val);
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

private:
	template<class _Iter>
	void _Range_construct_or_tidy(_Iter _First, _Iter _Last, input_iterator_tag)
	{	// initialize with [_First, _Last), avoid leaking, input iterators
		_TRY_BEGIN
			for (; _First != _Last; ++_First)
			{
				emplace_back(*_First);	// performance note: emplace_back()'s strong guarantee is unnecessary here
			}
		_CATCH_ALL
			_Tidy();
		_RERAISE;
		_CATCH_END
	}

	template<class _Iter>
	void _Range_construct_or_tidy(_Iter _First, _Iter _Last, forward_iterator_tag)
	{	// initialize with [_First, _Last), avoid leaking, forward iterators
		if (_Buy(_Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)))))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast() = _Ucopy(_First, _Last, this->_Myfirst());
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

public:
	template<class _Iter,
		class = enable_if_t<_Is_iterator_v<_Iter>>>
		vector(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc())
		: _Mybase(_Al)
	{	// construct from [_First, _Last) with optional allocator
		_Adl_verify_range(_First, _Last);
		_Range_construct_or_tidy(_Get_unwrapped(_First), _Get_unwrapped(_Last), _Iter_cat_t<_Iter>{});
	}

	vector(initializer_list<_Ty> _Ilist, const _Alloc& _Al = _Alloc())
		: _Mybase(_Al)
	{	// construct from initializer_list, optional allocator
		_Range_construct_or_tidy(_Ilist.begin(), _Ilist.end(), random_access_iterator_tag{});
	}

	vector(const vector& _Right)
		: _Mybase(_Alty_traits::select_on_container_copy_construction(_Right._Getal()))
	{	// construct by copying _Right
		if (_Buy(_Right.size()))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast() = _Ucopy(_Right._Myfirst(), _Right._Mylast(), this->_Myfirst());
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

	vector(const vector& _Right, const _Alloc& _Al)
		: _Mybase(_Al)
	{	// construct by copying _Right, allocator
		if (_Buy(_Right.size()))
		{	// nonzero, fill it
			_TRY_BEGIN
				this->_Mylast() = _Ucopy(_Right._Myfirst(), _Right._Mylast(), this->_Myfirst());
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
		}
	}

private:
	void _Move_from(vector&& _Right, true_type) noexcept
	{	// move from _Right, stealing its contents
		this->_Swap_all(_Right);

		this->_Myfirst() = _Right._Myfirst();
		this->_Mylast() = _Right._Mylast();
		this->_Myend() = _Right._Myend();

		_Right._Myfirst() = pointer();
		_Right._Mylast() = pointer();
		_Right._Myend() = pointer();
	}

	void _Move_from(vector&& _Right, false_type)
	{	// move from _Right, possibly moving its contents
		if (this->_Getal() == _Right._Getal())
		{
			_Move_from(_STD move(_Right), true_type{});
		}
		else if (_Buy(_Right.size()))
		{	// nonzero, fill it
			this->_Mylast() = _Umove(_Right._Myfirst(), _Right._Mylast(), this->_Myfirst());
		}
	}

public:
	vector(vector&& _Right) noexcept
		: _Mybase(_STD move(_Right._Getal()))
	{	// construct by moving _Right
		_Move_from(_STD move(_Right), true_type{});
	}

	vector(vector&& _Right, const _Alloc& _Al)
		_NOEXCEPT_COND(_Alty_traits::is_always_equal::value) // strengthened
		: _Mybase(_Al)
	{	// construct by moving _Right, allocator
		if /* constexpr */ (_Alty_traits::is_always_equal::value)
		{
			_Move_from(_STD move(_Right), true_type{});
		}
		else
		{
#pragma warning(push) // TRANSITION, if constexpr
#pragma warning(disable: 4297) // function assumed not to throw an exception but does
			_TRY_BEGIN
				_Move_from(_STD move(_Right), false_type{});
			_CATCH_ALL
				_Tidy();
			_RERAISE;
			_CATCH_END
#pragma warning(pop)
		}
	}

private:
	void _Move_assign_from(vector&& _Right, true_type) noexcept
	{	// move from _Right, stealing its contents
		_Move_from(_STD move(_Right), true_type{});
	}

	void _Move_assign_from(vector&& _Right, false_type)
	{	// move from _Right, possibly moving its contents
		if (this->_Getal() == _Right._Getal())
		{
			_Move_from(_STD move(_Right), true_type{});
			return;
		}

		const pointer _First = _Right._Myfirst();
		const pointer _Last = _Right._Mylast();
		const size_type _Newsize = _Right.size();

		this->_Orphan_all();

		const size_type _Oldsize = size();
		const size_type _Oldcapacity = capacity();

		if (_Newsize > _Oldcapacity)
		{	// reallocate
			if (_Newsize > max_size())
			{
				_Xlength();
			}

			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			if (this->_Myfirst() != pointer())
			{	// destroy and deallocate old array
				_Destroy(this->_Myfirst(), this->_Mylast());
				this->_Getal().deallocate(this->_Myfirst(), _Oldcapacity);
			}

			_Buy(_Newcapacity);

			this->_Mylast() = _Umove(_First, _Last, this->_Myfirst());
		}
		else if (_Newsize > _Oldsize)
		{
			const pointer _Mid = _First + _Oldsize;
			_Move_unchecked(_First, _Mid, this->_Myfirst());
			this->_Mylast() = _Umove(_Mid, _Last, this->_Mylast());
		}
		else
		{
			const pointer _Newlast = this->_Myfirst() + _Newsize;
			_Move_unchecked(_First, _Last, this->_Myfirst());
			_Destroy(_Newlast, this->_Mylast());
			this->_Mylast() = _Newlast;
		}
	}

public:
	vector& operator=(vector&& _Right)
		_NOEXCEPT_COND(_Always_equal_after_move<_Alty>)
	{	// assign by moving _Right
		if (this != _STD addressof(_Right))
		{	// different, assign it
#pragma warning(push)
#pragma warning(disable: 4127)	// conditional expression is constant
			if (_Always_equal_after_move<_Alty>
				|| this->_Getal() == _Right._Getal())
			{	// will steal _Right's contents (also, POCMA non-equal must reload array)
				_Tidy();
			}
#pragma warning(pop)
			this->_Move_alloc(_Right._Getal());
			_Move_assign_from(_STD move(_Right), bool_constant<_Always_equal_after_move<_Alty>>{});
		}
		return (*this);
	}

	~vector() noexcept
	{	// destroy the object
		_Tidy();
	}

private:
	template<class... _Valty>
	decltype(auto) _Emplace_back_with_unused_capacity(_Valty&&... _Val)
	{	// insert by perfectly forwarding into element at end, provide strong guarantee
		// pre: _Has_unused_capacity()
		_Alty_traits::construct(this->_Getal(), _Unfancy(this->_Mylast()), _STD forward<_Valty>(_Val)...);
		_Orphan_range(this->_Mylast(), this->_Mylast());
		_Ty& _Result = *this->_Mylast();
		++this->_Mylast();
#if _HAS_CXX17
		return (_Result);
#else /* ^^^ _HAS_CXX17 ^^^ // vvv !_HAS_CXX17 vvv */
		(void)_Result;
#endif /* _HAS_CXX17 */
	}

public:
	template<class... _Valty>
	decltype(auto) emplace_back(_Valty&&... _Val)
	{	// insert by perfectly forwarding into element at end, provide strong guarantee
		if (_Has_unused_capacity())
		{
			return (_Emplace_back_with_unused_capacity(_STD forward<_Valty>(_Val)...));
		}

		_Ty& _Result = *_Emplace_reallocate(this->_Mylast(), _STD forward<_Valty>(_Val)...);
#if _HAS_CXX17
		return (_Result);
#else /* ^^^ _HAS_CXX17 ^^^ // vvv !_HAS_CXX17 vvv */
		(void)_Result;
#endif /* _HAS_CXX17 */
	}

	void push_back(const _Ty& _Val)
	{	// insert element at end, provide strong guarantee
		emplace_back(_Val);
	}

	void push_back(_Ty&& _Val)
	{	// insert by moving into element at end, provide strong guarantee
		emplace_back(_STD move(_Val));
	}

	template<class... _Valty>
	pointer _Emplace_reallocate(const pointer _Whereptr, _Valty&&... _Val)
	{	// reallocate and insert by perfectly forwarding _Val at _Whereptr
		// pre: !_Has_unused_capacity()
		const size_type _Whereoff = static_cast<size_type>(_Whereptr - this->_Myfirst());
		_Alty& _Al = this->_Getal();
		const size_type _Oldsize = size();

		if (_Oldsize == max_size())
		{
			_Xlength();
		}

		const size_type _Newsize = _Oldsize + 1;
		const size_type _Newcapacity = _Calculate_growth(_Newsize);

		const pointer _Newvec = _Al.allocate(_Newcapacity);
		const pointer _Constructed_last = _Newvec + _Whereoff + 1;
		pointer _Constructed_first = _Constructed_last;

		_TRY_BEGIN
			_Alty_traits::construct(_Al, _Unfancy(_Newvec + _Whereoff), _STD forward<_Valty>(_Val)...);
		_Constructed_first = _Newvec + _Whereoff;

		if (_Whereptr == this->_Mylast())
		{	// at back, provide strong guarantee
			_Umove_if_noexcept(this->_Myfirst(), this->_Mylast(), _Newvec);
		}
		else
		{	// provide basic guarantee
			_Umove(this->_Myfirst(), _Whereptr, _Newvec);
			_Constructed_first = _Newvec;
			_Umove(_Whereptr, this->_Mylast(), _Newvec + _Whereoff + 1);
		}
		_CATCH_ALL
			_Destroy(_Constructed_first, _Constructed_last);
		_Al.deallocate(_Newvec, _Newcapacity);
		_RERAISE;
		_CATCH_END

			_Change_array(_Newvec, _Newsize, _Newcapacity);
		return (this->_Myfirst() + _Whereoff);
	}

	template<class... _Valty>
	iterator emplace(const_iterator _Where, _Valty&&... _Val)
	{	// insert by perfectly forwarding _Val at _Where
		const pointer _Whereptr = _Where._Ptr;
		const pointer _Oldlast = this->_Mylast();
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Where._Getcont() == _STD addressof(this->_Get_data())
			&& _Whereptr >= this->_Myfirst()
			&& _Oldlast >= _Whereptr, "vector emplace iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		if (_Has_unused_capacity())
		{
			if (_Whereptr == _Oldlast)
			{	// at back, provide strong guarantee
				_Emplace_back_with_unused_capacity(_STD forward<_Valty>(_Val)...);
			}
			else
			{
				_Ty _Obj(_STD forward<_Valty>(_Val)...);	// handle aliasing
				// after constructing _Obj, provide basic guarantee
				_Orphan_range(_Whereptr, _Oldlast);
				_Alty_traits::construct(this->_Getal(), _Unfancy(_Oldlast), _STD move(_Oldlast[-1]));
				++this->_Mylast();
				_Move_backward_unchecked(_Whereptr, _Oldlast - 1, _Oldlast);
				*_Whereptr = _STD move(_Obj);
			}

			return (this->_Make_iterator(_Whereptr));
		}

		return (this->_Make_iterator(_Emplace_reallocate(_Whereptr, _STD forward<_Valty>(_Val)...)));
	}

	iterator insert(const_iterator _Where, const _Ty& _Val)
	{	// insert _Val at _Where
		return (emplace(_Where, _Val));
	}

	iterator insert(const_iterator _Where, _Ty&& _Val)
	{	// insert by moving _Val at _Where
		return (emplace(_Where, _STD move(_Val)));
	}

	iterator insert(const_iterator _Where, _CRT_GUARDOVERFLOW const size_type _Count, const _Ty& _Val)
	{	// insert _Count * _Val at _Where
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Where._Getcont() == _STD addressof(this->_Get_data())
			&& _Where._Ptr >= this->_Myfirst()
			&& this->_Mylast() >= _Where._Ptr, "vector insert iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		const size_type _Whereoff = static_cast<size_type>(_Where._Ptr - this->_Myfirst());
		const bool _One_at_back = _Count == 1 && _Where._Ptr == this->_Mylast();

		if (_Count == 0)
		{	// nothing to do, avoid invalidating iterators
		}
		else if (_Count > _Unused_capacity())
		{	// reallocate
			const size_type _Oldsize = size();

			if (_Count > max_size() - _Oldsize)
			{
				_Xlength();
			}

			const size_type _Newsize = _Oldsize + _Count;
			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			const pointer _Newvec = this->_Getal().allocate(_Newcapacity);
			const pointer _Constructed_last = _Newvec + _Whereoff + _Count;
			pointer _Constructed_first = _Constructed_last;

			_TRY_BEGIN
				_Ufill(_Newvec + _Whereoff, _Count, _Val);
			_Constructed_first = _Newvec + _Whereoff;

			if (_One_at_back)
			{	// provide strong guarantee
				_Umove_if_noexcept(this->_Myfirst(), this->_Mylast(), _Newvec);
			}
			else
			{	// provide basic guarantee
				_Umove(this->_Myfirst(), _Where._Ptr, _Newvec);
				_Constructed_first = _Newvec;
				_Umove(_Where._Ptr, this->_Mylast(), _Newvec + _Whereoff + _Count);
			}
			_CATCH_ALL
				_Destroy(_Constructed_first, _Constructed_last);
			this->_Getal().deallocate(_Newvec, _Newcapacity);
			_RERAISE;
			_CATCH_END

				_Change_array(_Newvec, _Newsize, _Newcapacity);
		}
		else if (_One_at_back)
		{	// provide strong guarantee
			_Emplace_back_with_unused_capacity(_Val);
		}
		else
		{	// provide basic guarantee
			const _Ty _Tmp = _Val;	// handle aliasing
			const pointer _Oldlast = this->_Mylast();
			const size_type _Affected_elements = static_cast<size_type>(_Oldlast - _Where._Ptr);
			_Orphan_range(_Where._Ptr, _Oldlast);

			if (_Count > _Affected_elements)
			{	// new stuff spills off end
				this->_Mylast() = _Ufill(_Oldlast, _Count - _Affected_elements, _Tmp);
				this->_Mylast() = _Umove(_Where._Ptr, _Oldlast, this->_Mylast());
				_Fill_unchecked(_Where._Ptr, _Oldlast, _Tmp);
			}
			else
			{	// new stuff can all be assigned
				this->_Mylast() = _Umove(_Oldlast - _Count, _Oldlast, _Oldlast);
				_Move_backward_unchecked(_Where._Ptr, _Oldlast - _Count, _Oldlast);
				_Fill_unchecked(_Where._Ptr, _Where._Ptr + _Count, _Tmp);
			}
		}

		return (this->_Make_iterator_offset(_Whereoff));
	}

private:
	template<class _Iter>
	void _Insert_range(const_iterator _Where, _Iter _First, _Iter _Last, input_iterator_tag)
	{	// insert [_First, _Last) at _Where, input iterators
		if (_First == _Last)
		{
			return;	// nothing to do, avoid invalidating iterators
		}

		const size_type _Whereoff = static_cast<size_type>(_Where._Ptr - this->_Myfirst());
		const size_type _Oldsize = size();

		// For one-at-back, provide strong guarantee.
		// Otherwise, provide basic guarantee (despite N4659 26.3.11.5 [vector.modifiers]/1).
		// Performance note: except for one-at-back, emplace_back()'s strong guarantee is unnecessary here.

		for (; _First != _Last; ++_First)
		{
			emplace_back(*_First);
		}

		_Orphan_range(this->_Myfirst() + _Whereoff, this->_Myfirst() + _Oldsize);

		_Rotate_unchecked(this->_Myfirst() + _Whereoff, this->_Myfirst() + _Oldsize, this->_Mylast());
	}

	template<class _Iter>
	void _Insert_range(const_iterator _Where, _Iter _First, _Iter _Last, forward_iterator_tag)
	{	// insert [_First, _Last) at _Where, forward iterators
		const size_type _Count = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)));
		const size_type _Whereoff = static_cast<size_type>(_Where._Ptr - this->_Myfirst());
		const bool _One_at_back = _Count == 1 && _Where._Ptr == this->_Mylast();

		if (_Count == 0)
		{	// nothing to do, avoid invalidating iterators
		}
		else if (_Count > _Unused_capacity())
		{	// reallocate
			const size_type _Oldsize = size();

			if (_Count > max_size() - _Oldsize)
			{
				_Xlength();
			}

			const size_type _Newsize = _Oldsize + _Count;
			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			const pointer _Newvec = this->_Getal().allocate(_Newcapacity);
			const pointer _Constructed_last = _Newvec + _Whereoff + _Count;
			pointer _Constructed_first = _Constructed_last;

			_TRY_BEGIN
				_Ucopy(_First, _Last, _Newvec + _Whereoff);
			_Constructed_first = _Newvec + _Whereoff;

			if (_One_at_back)
			{	// provide strong guarantee
				_Umove_if_noexcept(this->_Myfirst(), this->_Mylast(), _Newvec);
			}
			else
			{	// provide basic guarantee
				_Umove(this->_Myfirst(), _Where._Ptr, _Newvec);
				_Constructed_first = _Newvec;
				_Umove(_Where._Ptr, this->_Mylast(), _Newvec + _Whereoff + _Count);
			}
			_CATCH_ALL
				_Destroy(_Constructed_first, _Constructed_last);
			this->_Getal().deallocate(_Newvec, _Newcapacity);
			_RERAISE;
			_CATCH_END

				_Change_array(_Newvec, _Newsize, _Newcapacity);
		}
		else
		{	// Attempt to provide the strong guarantee for EmplaceConstructible failure.
			// If we encounter copy/move construction/assignment failure, provide the basic guarantee.
			// (For one-at-back, this provides the strong guarantee.)

			const pointer _Oldlast = this->_Mylast();
			const size_type _Affected_elements = static_cast<size_type>(_Oldlast - _Where._Ptr);

			if (_Count < _Affected_elements)
			{	// some affected elements must be assigned
				this->_Mylast() = _Umove(_Oldlast - _Count, _Oldlast, _Oldlast);
				_Move_backward_unchecked(_Where._Ptr, _Oldlast - _Count, _Oldlast);
				_Destroy(_Where._Ptr, _Where._Ptr + _Count);

				_TRY_BEGIN
					_Ucopy(_First, _Last, _Where._Ptr);
				_CATCH_ALL
					// glue the broken pieces back together

					_TRY_BEGIN
					_Umove(_Where._Ptr + _Count, _Where._Ptr + 2 * _Count, _Where._Ptr);
				_CATCH_ALL
					// vaporize the detached piece
					_Orphan_range(_Where._Ptr, _Oldlast);
				_Destroy(_Where._Ptr + _Count, this->_Mylast());
				this->_Mylast() = _Where._Ptr;
				_RERAISE;
				_CATCH_END

					_Move_unchecked(_Where._Ptr + 2 * _Count, this->_Mylast(), _Where._Ptr + _Count);
				_Destroy(_Oldlast, this->_Mylast());
				this->_Mylast() = _Oldlast;
				_RERAISE;
				_CATCH_END
			}
			else
			{	// affected elements don't overlap before/after
				const pointer _Relocated = _Where._Ptr + _Count;
				this->_Mylast() = _Umove(_Where._Ptr, _Oldlast, _Relocated);
				_Destroy(_Where._Ptr, _Oldlast);

				_TRY_BEGIN
					_Ucopy(_First, _Last, _Where._Ptr);
				_CATCH_ALL
					// glue the broken pieces back together

					_TRY_BEGIN
					_Umove(_Relocated, this->_Mylast(), _Where._Ptr);
				_CATCH_ALL
					// vaporize the detached piece
					_Orphan_range(_Where._Ptr, _Oldlast);
				_Destroy(_Relocated, this->_Mylast());
				this->_Mylast() = _Where._Ptr;
				_RERAISE;
				_CATCH_END

					_Destroy(_Relocated, this->_Mylast());
				this->_Mylast() = _Oldlast;
				_RERAISE;
				_CATCH_END
			}

			_Orphan_range(_Where._Ptr, _Oldlast);
		}
	}

public:
	template<class _Iter,
		class = enable_if_t<_Is_iterator_v<_Iter>>>
		iterator insert(const_iterator _Where, _Iter _First, _Iter _Last)
	{	// insert [_First, _Last) at _Where
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_Where._Getcont() == _STD addressof(this->_Get_data())
			&& _Where._Ptr >= this->_Myfirst()
			&& this->_Mylast() >= _Where._Ptr, "vector insert iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		_Adl_verify_range(_First, _Last);
		const size_type _Whereoff = static_cast<size_type>(_Where._Ptr - this->_Myfirst());
		_Insert_range(_Where, _Get_unwrapped(_First), _Get_unwrapped(_Last), _Iter_cat_t<_Iter>{});
		return (this->_Make_iterator_offset(_Whereoff));
	}

	iterator insert(const_iterator _Where, initializer_list<_Ty> _Ilist)
	{	// insert initializer_list at _Where
		return (insert(_Where, _Ilist.begin(), _Ilist.end()));
	}

	void assign(_CRT_GUARDOVERFLOW const size_type _Newsize, const _Ty& _Val)
	{	// assign _Newsize * _Val
		this->_Orphan_all();

		const size_type _Oldsize = size();
		const size_type _Oldcapacity = capacity();

		if (_Newsize > _Oldcapacity)
		{	// reallocate
			if (_Newsize > max_size())
			{
				_Xlength();
			}

			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			if (this->_Myfirst() != pointer())
			{	// destroy and deallocate old array
				_Destroy(this->_Myfirst(), this->_Mylast());
				this->_Getal().deallocate(this->_Myfirst(), _Oldcapacity);
			}

			_Buy(_Newcapacity);

			this->_Mylast() = _Ufill(this->_Myfirst(), _Newsize, _Val);
		}
		else if (_Newsize > _Oldsize)
		{
			_Fill_unchecked(this->_Myfirst(), this->_Mylast(), _Val);
			this->_Mylast() = _Ufill(this->_Mylast(), _Newsize - _Oldsize, _Val);
		}
		else
		{
			const pointer _Newlast = this->_Myfirst() + _Newsize;
			_Fill_unchecked(this->_Myfirst(), _Newlast, _Val);
			_Destroy(_Newlast, this->_Mylast());
			this->_Mylast() = _Newlast;
		}
	}

private:
	template<class _Iter>
	void _Assign_range(_Iter _First, _Iter _Last, input_iterator_tag)
	{	// assign [_First, _Last), input iterators
		this->_Orphan_all();

		pointer _Next = this->_Myfirst();

		for (; _First != _Last && _Next != this->_Mylast(); ++_First, (void)++_Next)
		{
			*_Next = *_First;
		}

		// Code size optimization: we've exhausted only the source, only the dest, or both.
		// If we've exhausted only the source: we Trim, then Append does nothing.
		// If we've exhausted only the dest: Trim does nothing, then we Append.
		// If we've exhausted both: Trim does nothing, then Append does nothing.

		// Trim.
		_Destroy(_Next, this->_Mylast());
		this->_Mylast() = _Next;

		// Append.
		for (; _First != _Last; ++_First)
		{
			emplace_back(*_First);	// performance note: emplace_back()'s strong guarantee is unnecessary here
		}
	}

	template<class _Iter>
	void _Assign_range(_Iter _First, _Iter _Last, forward_iterator_tag)
	{	// assign [_First, _Last), forward iterators
		const size_type _Newsize = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)));

		this->_Orphan_all();

		const size_type _Oldsize = size();
		const size_type _Oldcapacity = capacity();

		if (_Newsize > _Oldcapacity)
		{	// reallocate
			if (_Newsize > max_size())
			{
				_Xlength();
			}

			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			if (this->_Myfirst() != pointer())
			{	// destroy and deallocate old array
				_Destroy(this->_Myfirst(), this->_Mylast());
				this->_Getal().deallocate(this->_Myfirst(), _Oldcapacity);
			}

			_Buy(_Newcapacity);

			this->_Mylast() = _Ucopy(_First, _Last, this->_Myfirst());
		}
		else if (_Newsize > _Oldsize)
		{
			// performance note: traversing [_First, _Mid) twice
			const _Iter _Mid = _STD next(_First, static_cast<difference_type>(_Oldsize));
			_Copy_unchecked(_First, _Mid, this->_Myfirst());
			this->_Mylast() = _Ucopy(_Mid, _Last, this->_Mylast());
		}
		else
		{
			const pointer _Newlast = this->_Myfirst() + _Newsize;
			_Copy_unchecked(_First, _Last, this->_Myfirst());
			_Destroy(_Newlast, this->_Mylast());
			this->_Mylast() = _Newlast;
		}
	}

public:
	template<class _Iter,
		class = enable_if_t<_Is_iterator_v<_Iter>>>
		void assign(_Iter _First, _Iter _Last)
	{	// assign [_First, _Last)
		_Adl_verify_range(_First, _Last);
		_Assign_range(_Get_unwrapped(_First), _Get_unwrapped(_Last), _Iter_cat_t<_Iter>{});
	}

	void assign(initializer_list<_Ty> _Ilist)
	{	// assign initializer_list
		_Assign_range(_Ilist.begin(), _Ilist.end(), random_access_iterator_tag{});
	}

	vector& operator=(const vector& _Right)
	{	// assign _Right
		if (this != _STD addressof(_Right))
		{	// different, assign it
#pragma warning(push)
#pragma warning(disable: 4127)	// conditional expression is constant
			if (_Alty_traits::propagate_on_container_copy_assignment::value
				&& this->_Getal() != _Right._Getal())
			{	// reload array
				_Tidy();
			}
#pragma warning(pop)

			this->_Copy_alloc(_Right._Getal());

			assign(_Right._Myfirst(), _Right._Mylast());
		}

		return (*this);
	}

	vector& operator=(initializer_list<_Ty> _Ilist)
	{	// assign initializer_list
		_Assign_range(_Ilist.begin(), _Ilist.end(), random_access_iterator_tag{});
		return (*this);
	}

private:
	template<class _Lambda>
	void _Resize(const size_type _Newsize, _Lambda _Udefault_or_fill)
	{	// trim or append elements, provide strong guarantee
		const size_type _Oldsize = size();
		const size_type _Oldcapacity = capacity();

		if (_Newsize > _Oldcapacity)
		{	// reallocate
			if (_Newsize > max_size())
			{
				_Xlength();
			}

			const size_type _Newcapacity = _Calculate_growth(_Newsize);

			const pointer _Newvec = this->_Getal().allocate(_Newcapacity);
			const pointer _Appended_first = _Newvec + _Oldsize;
			pointer _Appended_last = _Appended_first;

			_TRY_BEGIN
				_Appended_last = _Udefault_or_fill(_Appended_first, _Newsize - _Oldsize);
			_Umove_if_noexcept(this->_Myfirst(), this->_Mylast(), _Newvec);
			_CATCH_ALL
				_Destroy(_Appended_first, _Appended_last);
			this->_Getal().deallocate(_Newvec, _Newcapacity);
			_RERAISE;
			_CATCH_END

				_Change_array(_Newvec, _Newsize, _Newcapacity);
		}
		else if (_Newsize > _Oldsize)
		{	// append
			const pointer _Oldlast = this->_Mylast();
			this->_Mylast() = _Udefault_or_fill(_Oldlast, _Newsize - _Oldsize);
			_Orphan_range(_Oldlast, _Oldlast);
		}
		else if (_Newsize == _Oldsize)
		{	// nothing to do, avoid invalidating iterators
		}
		else
		{	// trim
			const pointer _Newlast = this->_Myfirst() + _Newsize;
			_Orphan_range(_Newlast, this->_Mylast());
			_Destroy(_Newlast, this->_Mylast());
			this->_Mylast() = _Newlast;
		}
	}

public:
	void resize(_CRT_GUARDOVERFLOW const size_type _Newsize)
	{	// trim or append value-initialized elements, provide strong guarantee
		const auto _Lambda_default = [this](pointer _Dest, const size_type _Count)
		{
			return (_Udefault(_Dest, _Count));
		};

		_Resize(_Newsize, _Lambda_default);
	}

	void resize(_CRT_GUARDOVERFLOW const size_type _Newsize, const _Ty& _Val)
	{	// trim or append copies of _Val, provide strong guarantee
		auto _Lambda_fill = [this, &_Val](pointer _Dest, const size_type _Count)
		{
			return (_Ufill(_Dest, _Count, _Val));
		};

		_Resize(_Newsize, _Lambda_fill);
	}

private:
	void _Reallocate_exactly(const size_type _Newcapacity)
	{	// set capacity to _Newcapacity (without geometric growth), provide strong guarantee
		const size_type _Size = size();

		const pointer _Newvec = this->_Getal().allocate(_Newcapacity);

		_TRY_BEGIN
			_Umove_if_noexcept(this->_Myfirst(), this->_Mylast(), _Newvec);
		_CATCH_ALL
			this->_Getal().deallocate(_Newvec, _Newcapacity);
		_RERAISE;
		_CATCH_END

			_Change_array(_Newvec, _Size, _Newcapacity);
	}

public:
	void reserve(_CRT_GUARDOVERFLOW const size_type _Newcapacity)
	{	// increase capacity to _Newcapacity (without geometric growth), provide strong guarantee
		if (_Newcapacity > capacity())
		{	// something to do (reserve() never shrinks)
			if (_Newcapacity > max_size())
			{
				_Xlength();
			}

			_Reallocate_exactly(_Newcapacity);
		}
	}

	void shrink_to_fit()
	{	// reduce capacity to size, provide strong guarantee
		if (_Has_unused_capacity())
		{	// something to do
			if (empty())
			{
				_Tidy();
			}
			else
			{
				_Reallocate_exactly(size());
			}
		}
	}

	void pop_back()
	{	// erase element at end
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(!empty(), "vector empty before pop");
		_Orphan_range(this->_Mylast() - 1, this->_Mylast());
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		_Alty_traits::destroy(this->_Getal(), _Unfancy(this->_Mylast() - 1));
		--this->_Mylast();
	}

	iterator erase(const_iterator _Where)
	{	// erase element at _Where
		_Move_unchecked(_Where._Ptr + 1, this->_Mylast(), _Where._Ptr);
		_Alty_traits::destroy(this->_Getal(), _Unfancy(this->_Mylast() - 1));
		--this->_Mylast();
		return (iterator(_Where._Ptr, _STD addressof(this->_Get_data())));
	}

	iterator erase(const_iterator _First, const_iterator _Last)
	{	// erase [_First, _Last)
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(_First._Getcont() == _STD addressof(this->_Get_data())
			&& _Last._Getcont() == _STD addressof(this->_Get_data())
			&& _First._Ptr >= this->_Myfirst()
			&& _Last._Ptr >= _First._Ptr
			&& this->_Mylast() >= _Last._Ptr, "vector erase iterator outside range");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		if (_First._Ptr != _Last._Ptr)
		{	// something to do, invalidate iterators
			_Orphan_range(_First._Ptr, this->_Mylast());
			const pointer _Newlast = _Move_unchecked(_Last._Ptr, this->_Mylast(), _First._Ptr);
			_Destroy(_Newlast, this->_Mylast());
			this->_Mylast() = _Newlast;
		}

		return (iterator(_First._Ptr, _STD addressof(this->_Get_data())));
	}






	void _Change_array(const pointer _Newvec, const size_type _Newsize, const size_type _Newcapacity)
	{	// orphan all iterators, discard old array, acquire new array
		this->_Orphan_all();

		if (this->_Myfirst() != pointer())
		{	// destroy and deallocate old array
			_Destroy(this->_Myfirst(), this->_Mylast());
			this->_Getal().deallocate(this->_Myfirst(), capacity());
		}

		this->_Myfirst() = _Newvec;
		this->_Mylast() = _Newvec + _Newsize;
		this->_Myend() = _Newvec + _Newcapacity;
	}



};
