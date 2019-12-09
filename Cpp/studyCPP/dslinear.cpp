#include "pch.h"
#include "helper.hpp"

//
//	Linked list
//
template <typename elem>
struct node {	
	elem e;
	node<elem>* next;


	node() { next = NULL; }
	node(elem e) { this->e = e; next = NULL; }
};

// 感觉链表有通用问题, 如果表头是个node的话,next=NULL, 到底链表为空还是只有一个元素
// 为了解决此问题, 独立设计表头
template <typename elem>
struct linked_list {
	linked_list() {
		list = NULL;
	}
	node<elem>* list;
};
//typedef 无法和template 一起使用

//头插法建表, 
template <typename elem>
linked_list<elem> create_linked_list(const std::vector<elem>& buf) {
	if (buf.empty()) {
		return linked_list<elem>();
	}

	linked_list<elem> rt;
	node<elem>* previous = NULL;
	for (auto i : buf) {
		node<elem>* x = new node<elem>(i);
		if (previous) {
			previous->next = x;
			previous = previous->next;
		}
		else {
			previous=x;
			rt.list = x;
		}
	}
	return rt;
}

template <typename elem>
void clear_linked_list(linked_list<elem>& l) {
	auto t = l.list;
	l.list = NULL;
	while (t) {
		auto x = t->next;
		delete t;
		t = x;
	}
}

template <typename elem>
void node_traverse(linked_list<elem>& l) {
	auto x = l.list;
	while (x) {
		cout << x->e << " ";
		x = x->next;
	}
	cout << endl;
}


template <typename elem>
node<elem>* node_reverse(node<elem>* head) {
	node<elem>* rt = head;
	if (head->next == NULL) {
		return head;
	}
	else {
		head = head->next;
	}

	node<elem>* pre = NULL;
	node<elem>* next = NULL;
	while (head)
	{
		next = head->next;
		head->next = pre;
		pre = head;
		head = next;
	}
	rt->next = pre;
	return rt;
}

TEST(DSLinear, LinkedList_Baisc) {
	vector<int> buf = { 1,2,3,4,5 };
	//for (size_t i = 0; i < 10000000; i++){
	linked_list<int> head = create_linked_list(buf);
	node_traverse(head);
	clear_linked_list(head);
	//delete_linked_list(head);
	//}
}








