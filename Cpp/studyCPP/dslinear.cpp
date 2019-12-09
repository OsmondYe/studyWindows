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
void node_traverse(const linked_list<elem>& l) {
	auto x = l.list;
	while (x) {
		cout << x->e << " ";
		x = x->next;
	}
	cout << endl;
}


/* 单链表转置
	算法取中间情况进行分析:
		- cur有效时,必须拿到nxt   nxt=cur->next;  
			只有这样,当cur->next变化为pre时,将来可以把cur后移
			cur=nxt 必然与nxt=cur->next成对出现
		- 进行转置时,需要pre(previous)	cur->next=pre
			转置结束 pre也要变动, pre=cur
*/
template <typename elem>
void node_reverse(linked_list<elem>& l) {	
	node<elem>* cur = l.list;	
	node<elem>* pre = NULL;
	node<elem>* nxt = NULL;
	if (!cur) {
		return;
	}
	while (cur){
		nxt = cur->next;
		//
		cur->next = pre;
		pre = cur;
		///
		cur = nxt;
	}
	l.list = pre;
}
/*
	while(cur) {
		 转置;
		 后移;
	}
	如果这样分析,nxt只是一个tmp
	后移前需要让 pre=cur,  把pre和cur当做整体进行后移

	再换一种简单的思路:
		一次操作的单位记为	
			一个窗口 wnd(pre,cur)
			wnd提供后移一个单位的操作
			wnd提供reverse操作

*/
template <typename elem>
void node_reverse2(linked_list<elem>& l) {
	node<elem>* cur = l.list;
	node<elem>* pre = NULL;
	if (!cur) {
		return;
	}
	while (cur) {
		node<elem>* nxt = cur->next;
		//  -窗口转置
		cur->next = pre;
		/// -窗口后移 , cur后移前需要把pre正确设置了
		pre = cur;
		cur = nxt;
	}
	l.list = pre;
}

TEST(DSLinear, LinkedList_Baisc) {
	vector<int> buf = { 1,2,3,4,5 };
	//vector<int> buf = { 1,2 };
	//for (size_t i = 0; i < 10000000; i++){
	linked_list<int> head = create_linked_list(buf);
	node_traverse(head);
	node_reverse(head);  node_traverse(head);
	node_reverse2(head);  node_traverse(head);
	
	clear_linked_list(head);
	//delete_linked_list(head);
	//}
}








