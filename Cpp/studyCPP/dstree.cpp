#include "pch.h"
#include "helper.hpp"


template<typename T>
struct binary_tree_node {
	T element;
	binary_tree_node<T>* left_child;
	binary_tree_node<T>* right_child;

	binary_tree_node() {
		left_child = right_child = NULL;
	}

	binary_tree_node(const T& elem) {
		element(elem);
		left_child = right_child = NULL;
	}

	binary_tree_node(const T& elem, binary_tree_node* left, binary_tree_node* right) {
		element(elem);
		left_child = left;
		right_child = right;
	}

};


TEST(DsTree, basic) {

}