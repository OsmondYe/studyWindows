#include "pch.h"
using namespace aux;


namespace {
    struct TreeNode {
        int val;
        TreeNode* left;
        TreeNode* right;
        TreeNode(int x) : val(x), left(NULL), right(NULL) {}
    };
}


int maxDepth(TreeNode* root) {
    if (!root) {
        return 0;
    }
    return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}

int maxDepth_Iteratively(TreeNode* root) {
    int depth = 0;
    cout << "TBD" << endl;
    return depth;
}

// good test, remove all leaves that value is targer
TreeNode* removeLeafNodes(TreeNode* root, int target) {
    if (root == NULL) {
        return NULL;
    } 
    if (root->val != target) {
        root->left = removeLeafNodes(root->left, target);
        root->right =removeLeafNodes(root->right, target);
    }
    else {
        if (root->left){
            root->left = removeLeafNodes(root->left, target);
        }
        if (root->right) {
            root->right = removeLeafNodes(root->right, target);
        }
        if (!root->left && !root->right) {
            delete root;
            root = NULL;
        }
    }
    return root;
}


TEST(AlgoTree, LeetCode) {
    TreeNode* root = new TreeNode(1);
    root->left= new TreeNode(1);
    root->right= new TreeNode(1);

    root= removeLeafNodes(root, 1);


    root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->left->left = new TreeNode(2);
    root->left->left->left = new TreeNode(2);
    root->left->left->left->left = new TreeNode(2);
    root = removeLeafNodes(root, 2);
}