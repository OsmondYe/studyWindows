#include "pch.h"
#include "helper.hpp"




template<typename T>
class LinearList {
public:
	virtual ~LinearList() {}
	virtual bool empty() const = 0;
	virtual int size() const = 0;
	virtual T& get(int index)const  = 0;
	virtual int indexof(const T& elem)const = 0;
	virtual void erase(int index) = 0;
	virtual void insert(int index, const T& elem) = 0;
	virtual void output(std::ostream& os) = 0;
};


template<typename T>
class ArrayList : public LinearList<T> {
public:
	ArrayList(): elems_(NULL),length_(0),capacity_(0) {}
	ArrayList(int capacity = 10) :capacity_(capacity),length_(0),elems_(new T[capacity_]) {}
	ArrayList(const ArrayList& rh) {
		this->length_ = rh.length_;
		this->capacity_ = rh.capacity_;
		this->elems_ = new T[this->capacity_];
		std::copy(rh.elems_, rh.elems_ + rh.length_, this->elems_);
	}
	~ArrayList(){delete[] elems_;}
public: // override
	virtual bool empty() const override { return length_ == 0; }
	virtual int size() const override { return length_; }
	virtual typename T& get(int index) const override { return elems_[index]; }
	virtual int indexof(const T& elem) const override {
		return (elems_ + length_)-std::find(elems_, elems_ + length_, elem);
	}
	virtual void erase(int index) override {}
	virtual void insert(int index, const T& elem) override {}
	virtual void output(std::ostream& os) override {}

private:
	typename T* elems_;
	int length_;
	int capacity_;
};


TEST(DSLinear, ArrayList) {

	ArrayList<int> m(100);


}