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

template<typename Elem>
void insert_linked_list(linked_list<Elem>& l,Elem i) {
	
	node<Elem>* p = l.list;
	node<Elem>* n = new node<Elem>(i);
	
	if (!p) {
		//p = n; // Easy Error, p changed, but l.list is not changed
		l.list = n;
	}
	else {
		while (p ->next!= NULL) {
			p = p->next;
		}
		//p = n;	// easy error, set p->next , not p
		p->next =n ;
	}
	
}

template<typename Elem>
bool delete_linked_list(linked_list<Elem>& l, Elem i) {
	bool rt = false; // true, ok, false i not found in l
	if (l.list == NULL) {
		return rt;
	}
	node<Elem>* pre = NULL;
	node<Elem>* cur = l.list; 

	if (cur->e == i) {
		l.list = cur->next;
		delete cur;
		rt = true;
		return rt;
	}
	// init-state, pre(NULL), cur(begin)
	for (; cur != NULL; pre = cur, cur = cur->next) {
		if (cur->e != i) {
			continue;
		}
		else {
			pre->next = cur->next;		// alarm, pre maybe NULL		
			delete cur;
			rt = true;
			break;
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
	linked_list<int> l = create_linked_list(buf);
	node_traverse(l);
	node_reverse(l);  node_traverse(l);
	node_reverse2(l);  node_traverse(l);
	
	clear_linked_list(l);
	//delete_linked_list(head);

	int i = 0;
	while (i < 10) {
		insert_linked_list(l, i++);
	}
	node_traverse(l);

	// delete specific item in list
	i = 0;
	while (i++<100)
	{		
		auto x = rand() % 10;
		cout << "del:" << x << " " <<std::boolalpha<< delete_linked_list(l, x) << endl;
	}

	//}
}

template<typename T>
void reverse_print(node<T>* head) {
	if (head == NULL) {
		return;
	}
	reverse_print(head->next);
	cout << head->e << " ";
}


TEST(DSLinear, LinkedList_ReversePrint) {
	linked_list<int> l = create_linked_list(aux::getSorted(30));
	node_traverse(l);
	reverse_print(l.list); 


	clear_linked_list(l);  cout << endl;
	l = create_linked_list(aux::getRandom(30));
	node_traverse(l);
	reverse_print(l.list);
	cout << endl;
}

/*最大子数组
		
*/
TEST(DSLinear, MAX_SUB_ARRAY) {
	// 股票每日价格
	vector<int> v = {
		100,113,110,85,105,102,86,63,81,101,94,106,101,79,94,90,97
	};

	cout << "stock price of per day\n";
	aux::output(v);
	// 变换思路: 一段区间,股票delta值最大
	// 先计算每日差值
	vector<int> diff(v.size() - 1);
	// 有一次使用到了transform的2元操作, 值右区间-值左区间, 左右区级同增
	std::transform(v.begin(), v.end()-1, v.begin() + 1, diff.begin(), [](int l, int r) {return r - l; });
	cout << "diff of price of each day\n";
	aux::output(diff);
	//13 -3 -25 20 -3 -16 -23 18 20 -7 12 -5 -22 15 -4 7
	// 现在的目标转为为了: 在diff区间中寻找一个sub_array,∑最大

	// 方法1: 我来暴力求解
	std::map<std::pair<int, int>, int> range;
	auto i = 0;
	for (i; i < diff.size(); ++i) {
		for (int j = i+1; j < diff.size(); ++j) {
			range[make_pair(i, j)] = std::accumulate(&diff[i], &diff[j], 0);
		}
	}
	typedef std::pair<std::pair<int, int>, int> range_v;
	range_v max = make_pair(make_pair(-1, -1), 0);
	std::for_each(range.begin(), range.end(), [&max](range_v v) {
		if (v.second > max.second) {
			max = v;
		}
		});
	cout << max.first.first << "to" << max.first.second << endl;


}



