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

TEST(AlgoTree, LeetCode) {

}