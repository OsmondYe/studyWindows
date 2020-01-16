#include "pch.h"
#include "helper.hpp"

template<typename Key, typename Value>
class base_dictionary {
public:
	virtual ~base_dictionary() {}
	virtual bool empty() =0;
	virtual int size() =0;
	virtual const Value& find(const Key& key)=0;
	virtual void erase(const Key& key) =0;
	virtual void insert(const Key& key, const Value& value)=0;
};


TEST(DSKVHash, ArrayList) {

}