#include "pch.h"
using namespace aux;


namespace {
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
            TreeNode* cur= queue.front();
            if (i < len && v[i]!= -1) {
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

    void tree_dump_path(const vector<vector<int>> path) {
        for (auto& p : path) {
            for (auto i : p) {
                cout << i << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

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

// 257. Binary Tree Paths
vector<string> binaryTreePaths(TreeNode* root) {
    vector<string> rt;
    if (!root) {
        return rt;
    }

    stack<TreeNode*> ss;
    TreeNode* cur = root;
    string trace;
    while (!ss.empty() || cur)
    {
        while (cur!=NULL)
        {
            trace += to_string(cur->val) + "->";
            ss.push(cur);
            cur = cur->left;
        }
        //
        rt.push_back(trace);
        auto* top = ss.top();
        ss.pop();
        trace.pop_back(); trace.pop_back();
        cur = top->right;
    }

    return rt;
}

vector<vector<int>> enumAllPath(TreeNode * root) {
    vector<vector<int>> rt;
    if (!root) {
        return rt;
    }    
    // variant of pre_order, <root,left,right> , using iterative, not recursive

    stack<TreeNode*> ss;
    ss.push(root);
    vector<int> trace;
    TreeNode* cur = root;
    while (!ss.empty() || cur)
    {
        while (cur)
        {
            trace.push_back(cur->val);
            ss.push(cur);
            cur = cur->left;
        }
        auto top = ss.top();
        ss.pop();
        cur = top->right;
    }



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

// determine if the tree has a root-to-leaf path, such that adding up all the values along the path equals the given sum
bool hasPathSum(TreeNode* root, int sum) {
    if (!root) {
        return false;
    }
    if (root && !root->left && !root->right && root->val== sum ) {
        return true;
    }
    int partial_sum = sum - root->val;
    return hasPathSum(root->left, partial_sum) || hasPathSum(root->right, partial_sum);
}


// enum all path form root to leaf that adding up all the values along the path equals the given sum
vector<vector<int>> pathSum(TreeNode* root, int sum) {
    vector<vector<int>> rt;
    if (!root) {
        return rt;
    }
    // enum all path that from root to leaves
    std::queue<TreeNode*> queue;
    std::queue<int> partial_sum;
    queue.push(root); partial_sum.push(sum - root->val);
    while (!queue.empty())
    {

    }
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

         

    root = tree_build({ 5,4,8,11,-1,13,4,7,2,-1,-1,-1,1 });
    EXPECT_EQ(true, hasPathSum(root, 22));

    root = tree_build({ 5,4,8,11,-1,13,4,7,2,-1,-1,5,1 });

    auto y = binaryTreePaths(root);

    auto x = enumAllPath(root);
    tree_dump_path(x);
    x = pathSum(root, 22);

}