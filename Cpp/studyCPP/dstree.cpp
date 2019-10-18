#include "pch.h"
#include "helper.hpp"
/*
	height, depth:	index based on 1	
	walk through,  traversal

*/

template<typename T>
struct binary_tree_node {
	T element;
	binary_tree_node<T>* left_child;
	binary_tree_node<T>* right_child;

	binary_tree_node(): element(),left_child(nullptr), right_child(nullptr) {
	}

	binary_tree_node(const T& elem):element(elem), left_child(nullptr), right_child(nullptr) {
	}

	binary_tree_node(const T& elem, binary_tree_node* left, binary_tree_node* right) :element(elem), left_child(nullptr), right_child(nullptr) {
	}

};

template<typename T>
class binary_tree   {
	typedef typename T template_type;
	typedef typename binary_tree_node<T> node_type;
public:
	binary_tree() :root(nullptr) {

	}

	binary_tree(const std::vector<T>& v):binary_tree() {
		std::for_each(v.begin(), v.end(), [this](int i) {
			_insert(i);
		});
	}

	~binary_tree() {
		_clear();
	}

	bool empty() { return root == nullptr; }

	size_t size() { return _count(); } 

	size_t height() {
		return _height(root);
	}

	void preorder(const std::function<void(T)>& visit) {
		_preorder(root, visit);
	}

	void inorder(const std::function<void(T)>& visit) {
		_inorder(root, visit);
	}

	void postorder(const std::function<void(T)>& visit) {
		_postorder(root, visit);
	}

	void levelorder(const std::function<void(T)>& visit) {
		if (root == nullptr) {
			return;
		}
		std::queue<node_type*> q;
		q.push(root);
		while (!q.empty() ){
			node_type* cur = q.front();
			visit(cur->element);
			if (cur->left_child) q.push(cur->left_child);
			if (cur->right_child) q.push(cur->right_child);

			q.pop();
		}
	}

private:

	size_t _count() {
		size_t c = 0;
		auto fc = [&c](const template_type& t) {++c; };
		_preorder(root, fc);
		return c;
	}

	void _insert(const T& t) {
		if (root == nullptr) {
			root = new binary_tree_node<T>(t);
			return;
		}

		// walk through by each level, and find first NULL
		std::queue<node_type*> q;
		q.push(root);
		while (!q.empty())
		{
			node_type* cur = q.front();			

			if (cur->left_child == nullptr) {
				cur->left_child = new node_type(t);
				return;
			}
			else {
				q.push(cur->left_child);
			}

			if (cur->right_child == nullptr) {
				cur->right_child = new node_type(t);
				return;
			}
			else {
				q.push(cur->right_child);
			}
			q.pop();
		}
	}

	void _clear() {
		if (root == nullptr) {
			return;
		}

		std::queue<node_type*> q;
		q.push(root);
		while (!q.empty()) {
			node_type* cur = q.front();
			if (cur->left_child != nullptr) {
				q.push(cur->left_child);
			}
			if (cur->right_child != nullptr) {
				q.push(cur->right_child);
			}
			delete cur;
			q.pop();
		}
	}

	void _preorder(node_type* node, const std::function<void(T)>& visit) {
		if (node == nullptr) {
			return;
		}
		visit(node->element);
		_preorder(node->left_child, visit);
		_preorder(node->right_child, visit);
	}

	void _inorder(node_type* node, const std::function<void(T)>& visit) {
		if (node == nullptr) {
			return;
		}
		_inorder(node->left_child, visit);
		visit(node->element);
		_inorder(node->right_child, visit);
	}

	void _postorder(node_type* node, const std::function<void(T)>& visit) {
		if (node == nullptr) {
			return;
		}
		_postorder(node->left_child, visit);
		_postorder(node->right_child, visit);
		visit(node->element);
	}

	size_t _height(node_type* node) {
		if (node == nullptr) {
			return 0;
		}
		size_t l = _height(node->left_child);
		size_t r = _height(node->right_child);
		return std::max(l, r) + 1;
	}

private:
	node_type* root;

};


TEST(DsTree, basic) {
	// how to build a tree with {1,2,3,4,5,6,7,8,9,10}
	auto v = aux::getSorted(10);
	int i = 1000000;
	//while(--i)
	binary_tree<decltype(v)::value_type> bt(v);

	auto visit = [](int i) {cout << i << " "; };

	bt.preorder(visit); cout << endl;
	bt.inorder(visit); cout << endl;
	bt.postorder(visit); cout << endl;
	bt.levelorder(visit); cout << endl;

	cout <<"height():"<< bt.height() << endl;
	cout << "size():" << bt.size() << endl;


	string expr{ R"(a+b+c+d)" };
	vector<char> v2(expr.begin(), expr.end());

	binary_tree<char> bt2(v2);

	auto visit2 = [](char i) {cout << i << " "; };

	bt2.preorder(visit2); cout << endl;
	bt2.inorder(visit2); cout << endl;
	bt2.postorder(visit2); cout << endl;
	bt2.levelorder(visit2); cout << endl;



}