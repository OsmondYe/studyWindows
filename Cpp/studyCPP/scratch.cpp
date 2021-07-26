#include "pch.h"
#include <regex>
#include "helper.hpp"

using namespace std;

#define null -1

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// -1 is null, like [1,2,3,-1,-1,5,6]
TreeNode* tree_build(const vector<int>& v) {
    int i = 0;
    int len = v.size();
    if (!len) {
        return NULL;
    }
    TreeNode* root = new TreeNode(v[i++]);
    queue<TreeNode*> queue;
    queue.push(root);
    while (!queue.empty()) {
        TreeNode* cur = queue.front();
        if (i < len && v[i] != -1) {
            cur->left = new TreeNode(v[i]);
            queue.push(cur->left);
        }
        i++;
        if (i < len && v[i] != -1) {
            cur->right = new TreeNode(v[i]);
            queue.push(cur->right);
        }
        i++;
        queue.pop();
    }

    return root;
}

typedef pair<int, int> P;


vector<TreeNode*> rt;
void dfs(TreeNode* root, TreeNode* cur, int n) {
       
    if (n <=2) {
        cur->left = new TreeNode(0);
        cur->right = new TreeNode(0);
        rt.push_back(root);
        return;
    }
    TreeNode* r=NULL;
    // build header
    if (root == cur) {
        r = new TreeNode(root->val);
        cur = r;
        root = r;
        n -= 1;
    }    
    cur->left = new TreeNode(0);
    cur->right = new TreeNode(0);
    n -= 2;
    dfs(root, cur->left, n);
    dfs(root, cur->right, n);
}

vector<TreeNode*> allPossibleFBT(int n) {
    
    TreeNode dummy(0);
    dfs(&dummy, &dummy, n);
    return rt;
    
}

void scratch() {
    vector<int>  nums{ 0,2,1,5,3,4 };
    vector<string> words{ "abc","deq","mee","aqq","dkd","ccc" };
    vector<vector<int>> logs{ {283268890,14532},{283268891,14530},{283268889,14530},{283268892,14530},{283268890,14531} };

    while (true)
    {
        allPossibleFBT(7);
    }

}