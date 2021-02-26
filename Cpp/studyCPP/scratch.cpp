#include "pch.h"
#include "helper.hpp"

using namespace std;
/*
*  any code piece can be test here
*/

int mono_stack(vector<int>& h) {
    // 高度为x的,能够形成的最大面积是多少?
    map<int, int> m;
    for (int i : h) { m[i] = i; }

    auto update_m = [&m](int h, int v) {
        if (!m.count(h)) {
            m[h] = v;
            return;
        }
        if (m[h] < v) {
            m[h] = v;
        }
    };
    

    stack<int> s; // store index in h
    for (int i = 0; i < h.size(); ++i) {

        int a = h[i];

        while (!s.empty() && h[s.top()] > a) {
            int t = h[s.top()];
            int idx = s.top();
            // amend indx
            while (idx>0)
            {
                if (t <= h[idx - 1]) {
                    idx--;
                }
                else {
                    break;
                }
            }

            int w = i-idx;
            int square = t * w;
            update_m(t, square);
            s.pop();
        }

        s.push(i);
    }

    using pair_type = decltype(m)::value_type;

    return max_element(m.begin(), m.end(), [](pair_type& l, pair_type& r) {
        return l.second < r.second;
        })->second;
}



int mono_stack2(vector<int>& height) {
    int ret = 0;
    height.push_back(0);
    vector<int> index;
    for (int i = 0; i < height.size(); i++)
    {
        while (index.size() > 0 && height[index.back()] >= height[i])
        {
            int h = height[index.back()];
            index.pop_back();

            int sidx = index.size() > 0 ? index.back() : -1;
            int square = h * (i - sidx - 1);
            if (square > ret)
                ret = square;
        }
        index.push_back(i);
    }

    return ret;
}


int diagonalSum(vector<vector<int>>& mat) {
    int s = mat.size();
    int ans = 0;
    for (int i = 0; i < s; ++i) {
        auto& r = mat[i];
        ans += r[i];
        if (i != s - i - 1) {
            ans += r[s - i - 1];
        }
    }
    return ans;
}


vector<int> sortArrayByParityII(vector<int>& A) {
    int po = 1;
    int pe = 0;
    while (po < A.size() && pe < A.size()) {

        // find first even post that is odd value
        while (pe < A.size() && !(A[pe] & 1) ) {
            pe += 2;
        }

        // find first odd pos that hold even value
        while (po < A.size() && (A[po] & 1)) {
            po += 2;
        }

        if (pe < A.size() && po < A.size()) {
            swap(A[po], A[pe]);
        }
    }
    return A;

}


int countStudents(vector<int>& students, vector<int>& sandwiches) {
    int c[]{ 0,0 };


    for (auto i : students)  c[i]++;

    int i=0;
    for (; i < sandwiches.size() && c[sandwiches[i]] >0; ++i) {
        c[sandwiches[i]]--;
    }

    return sandwiches.size() - i;
    
}

vector<int> luckyNumbers(vector<vector<int>>& matrix) {
    set<int> r;

    for (auto& v : matrix) {
        r.insert(*min_element(v.begin(), v.end()));
    }


    int row = matrix.size();
    int col = matrix[0].size();

    vector<int> rt;
    // col
    for (int i = 0; i < col; ++i) {

        int max = matrix[0][i];
        // row
        for (int j=0; j < row; ++j) {
            if (max < matrix[j][i]) {
                max = matrix[j][i];
            }
        }
        if (r.count(max)) {
            rt.push_back(max);
        }
    }
    return rt;
}

bool canMakeArithmeticProgression(vector<int>& arr) {

    auto p = minmax_element(arr.begin(), arr.end());

    

    int d = (*p.second - *p.first) / (arr.size() - 1);

    if (0 != ((*p.second - *p.first) % (arr.size() - 1))) {
        return false;
    }
    

    for (auto i : arr) {
        if ((i - *p.first) % d != 0) {
            return false;
        }
    }

    return true;
}


vector<string> stringMatching(vector<string>& words) {
    sort(begin(words), end(words),[](string& l, string& r) {
        if (l.size() < r.size()) return true;
        if (l.size() > r.size()) return false;
        return l < r;
        });
    vector<string> rt;
    for (int i = 0; i < words.size(); i++) {
        auto s = words[i];
        for (int j = i + 1; j < words.size(); ++j) {
            if (words[j].find(s) != string::npos) {
                rt.push_back(s);
                break;
            }
        }
    }
    return rt;
}

bool closeStrings(string word1, string word2) {
    unordered_map<char, int> s1, s2;

    for (auto c : word1) s1[c]++;
    for (auto c : word2) s2[c]++;

    if (s1.size() != s2.size()) {
        return false;
    }

    set<int> r1, r2;
    for (auto& p : s1) r1.insert(p.second);
    for (auto& p : s2) r2.insert(p.second);

    return r1 == r2;

}

bool canFormArray(vector<int>& arr, vector<vector<int>>& pieces) {
    // value -> coordinate
    unordered_map<int, pair<int,int > > m; // wheather has value and store its [i,j]
    for (int i = 0; i < pieces.size(); ++i) {
        for (int j = 0; j < pieces[i].size(); ++j) {
            m[pieces[i][j]] = { i,j };
        }
    }

    for (auto i : arr) {
        if (!m.count(i)) return false;
    };

    // expected next pair
    bool same_row = false;
    int next_pos = -1;
    
    for (auto n : arr) {      
        int i = m[n].first;
        int j = m[n].second;

        // relate between n and the next value

        // not same row
        if (!same_row) {
            if (j != 0) {
                return false;
            }
            next_pos = 1;
            if (next_pos >= pieces[i].size()) {
                next_pos = -1;
                same_row = false;
            }
            else {
                next_pos = 1;
                same_row = true;
            }
        }
        else {
            if (j != next_pos) {
                return false;
            }
            // amend 
            next_pos++;
            if (next_pos >= pieces[i].size()) {
                same_row = false;
                next_pos = -1;
            }

        }
    }
    return true;
}


vector<vector<int>> diagonalSort(vector<vector<int>>& mat) {
    int m = mat.size();
    int n = mat[0].size();

    vector<int> v; v.reserve(n);
    // h
    for (int j = 0; j < n; ++j) {
        v.clear();
        int i = 0;
        int t = j;
        while (i < m && j < n) { v.push_back(mat[i++][j++]); }
        // sort
        sort(v.begin(), v.end());
        // write_back
        j = t; i = 0;
        auto it = v.begin();
        while (i < m && j < n) { mat[i++][j++] = *it++; }
        j = t;
    }

    // v
    for (int i = 1; i < m; ++i) {
        v.clear();
        int t = i;
        int j = 0;
        while (i < m && j < n) { v.push_back(mat[i++][j++]); }
        // sort
        sort(v.begin(), v.end());
        i = t; j = 0;
        auto it = v.begin();
        while (i < m && j < n) { mat[i++][j++] = *it++; }
        i = t;
    }

    return mat;
}



void scratch() {
    vector<vector<int>> mat{ {{3,3,1,1},{2,2,1,2},{1,1,1,2}} };
    auto x = diagonalSort(mat);
    //vector<int>arr{ 91,4,64,78 };
    //vector<vector<int>> pieces{ {78} ,{4,64},{91} };


    //bool xx = canFormArray(arr, pieces);

    cout << "get line \n";
}