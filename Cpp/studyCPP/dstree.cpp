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

	binary_tree_node() : element(), left_child(nullptr), right_child(nullptr) {
	}

	binary_tree_node(const T& elem) :element(elem), left_child(nullptr), right_child(nullptr) {
	}

	binary_tree_node(const T& elem, binary_tree_node* left, binary_tree_node* right) :element(elem), left_child(nullptr), right_child(nullptr) {
	}

};

template<typename T>
class binary_tree {
	typedef typename T template_type;
	typedef typename binary_tree_node<T> node_type;
public:
	binary_tree() :root(nullptr) {

	}

	binary_tree(const std::vector<T>& v) :binary_tree() {
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
		while (!q.empty()) {
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

TEST(DSTree, basic) {
	// how to build a tree with {1,2,3,4,5,6,7,8,9,10}
	auto v = aux::getSorted(10);
	int i = 1000000;
	//while(--i)
	binary_tree<decltype(v)::value_type> bt(v);

	auto visit = [](int i) {cout << i << " "; };

	cout << "preorder:\n";
	bt.preorder(visit); cout << endl;
	cout << "inorder:\n";
	bt.inorder(visit); cout << endl;
	cout << "postorder:\n";
	bt.postorder(visit); cout << endl;
	cout << "levelorder:\n";
	bt.levelorder(visit); cout << endl;

	cout << "height():" << bt.height() << endl;
	cout << "size():" << bt.size() << endl;
}


//
//	Bineary Search Tree  (BST)
//

/*
	search, minimum, maximum, predecessor, successor
	intsert delete

	bst:  x is a node
		if y in x.left
			y.key<=x.key
		if y in x.right
			y.key>=x.key

*/

struct bst_node {
	int key;
	bst_node* left;
	bst_node* right;
	bst_node* parent;

	bst_node(int key) {
		this->key = key;
		left = right = parent = NULL;
	}
};

struct bst_tree
{
	bst_node* root;

	bst_tree() :root(NULL) {}
};


void bst_inorder_walk(bst_node* root) {
	if (root != NULL) {
		bst_inorder_walk(root->left);
		cout << root->key << " ";
		bst_inorder_walk(root->right);
	}
}

bst_node* bst_search(bst_node* x, int k) {
	if (x == NULL || x->key == k) {
		return x;
	}
	if (k < x->key) {
		return bst_search(x->left, k);
	}
	else {
		return bst_search(x->right, k);
	}
}

bst_node* bst_search_iterative(bst_node* x, int k) {
	while (x != NULL && x->key != k) {
		if (k < x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	return x;
}

bst_node* bst_minimun(bst_node* x) {
	while (x != NULL && x->left != NULL)
	{
		x = x->left;
	}
	return x;
}

bst_node* bst_maximun(bst_node* x) {
	while (x != NULL && x->right != NULL)
	{
		x = x->right;
	}
	return x;
}

// the next node  in  the in_order_walk sequence
// x->right ==NULL, from bottom to top, find first parent that p.left=x
bst_node* bst_successor(bst_node* x) {
	if (x->right != NULL) {
		return bst_minimun(x);
	}
	bst_node* y = x->parent;
	while (y != NULL && x == y->right)
	{
		x = y;
		y = y->parent;
	}
	return y;

}

// the previous node in the in_order_walk sequence
bst_node* bst_predecessor(bst_node* x) {
	if (x->left != NULL) {
		return bst_minimun(x);
	}
	bst_node* y = x->parent;
	while (y != NULL && x == y->left)
	{
		x = y;
		y = y->parent;
	}
	return y;
}

void bst_insert(bst_tree* T, bst_node* z) {
	bst_node* y = NULL;
	bst_node* x = T->root;
	// find insert point y;
	while (x != NULL)
	{
		y = x;
		if (z->key < x->key) {
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}
	z->parent = y;
	if (y == NULL) {
		// tree is empty
		T->root = z;
	}
	else if (z->key < y->key)
	{
		y->left = z;
	}
	else
	{
		y->right = z;
	}
}


// helper in bst_delete
void bst_transplant(bst_tree* T, bst_node* u, bst_node* v) {
	if (u->parent == NULL) {
		T->root = v;
	}
	else if (u == u->parent->left) {
		u->parent->left = v;
	}
	else
	{
		u->parent->right = v;
	}
	if (v != NULL) {
		v->parent = u->parent;
	}
}

void bst_delete(bst_tree* T, bst_node* z) {
	if (z->left == NULL) {
		bst_transplant(T, z, z->right);
	}
	else if (z->right == NULL) {
		bst_transplant(T, z, z->left);
	}
	else {
		auto y = bst_minimun(z->right);
		if (y->parent != z) {
			bst_transplant(T, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		bst_transplant(T, z, y);
		y->left = z->left;
		y->left->parent = y;
	}
}

TEST(DSTree, bst) {
	vector<int> v{ 15,6,18,3,7,17,20,2,4,13,9 };

	//
	bst_tree* bstTree = new bst_tree();
	for (auto i : v) {
		bst_insert(bstTree, new bst_node(i));
	}

	// search
	for (auto i : v) {
		auto x = bst_search(bstTree->root, i);
		if (x == NULL) {
			cout << i << "is null" << endl;
		}
		else {
			cout << i << "is:" << x << endl;
		}
	}

	bst_inorder_walk(bstTree->root); cout << endl;

	cout << "min:" << bst_minimun(bstTree->root)->key << endl;
	cout << "max:" << bst_maximun(bstTree->root)->key << endl;
}

//
//  red black tree
//

struct rbt_node  {
	enum Color {
		black = 1,
		red =2
	};
	int key;
	rbt_node* left;
	rbt_node* right;
	rbt_node* parent;
	Color color;

	rbt_node(int k) :key(k), color(black) {
		left = right = parent = NULL;
	}
};

struct rbt_tree {
	rbt_node* root;
	rbt_node* nil; // sentry, black node,for each nil child

	rbt_tree(): root(NULL) {
		nil = new rbt_node(-1);		
		root = nil;
	}
};

void rbt_left_rotat(rbt_tree* T, rbt_node* x) {
	auto y = x->right;		// set y
	x->right = y->left;		// rotate anti-clockwise, y up, x down

	if (y->left != T->nil) {   // x.right <- y.left
		y->left->parent = x;		
	}
	y->parent = x->parent;  // up_y
	if (x->parent == T->nil) {	// consider root
		T->root = y;
	}
	else if(x == x->parent->left){ // x is a left sub tree
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}

	// puy x on y.left
	y->left = x;
	x->parent = y;
}

void rbt_insert_fixup(rbt_tree* T, rbt_node* z) {

}

void rbt_insert(rbt_tree* T, rbt_node* z) {
	auto y = T->nil;
	auto x = T->root;

	while (x!=T->nil)
	{
		y = x;
		if (z->key < x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}

	}
	z->parent = y;
	if (y == T->nil) {
		T->root = z;
	}
	else if (z->key<y->key) {
		y->left = z;
	}
	else {
		y->right = z;
	}
	z->left = z->right = T->nil;
	z->color = rbt_node::red; // new insert node is red
}



TEST(DSTree, rbt) {

}