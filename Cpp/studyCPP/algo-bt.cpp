#include "pch.h"

using namespace aux;


// bt:  backtracking method

class Solution_CombiniationSum2 {
public:
    using citer = vector<int>::const_iterator;
    using container = vector<int>;

    vector<vector<int>> rt;

    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        vector<int> dummy;
        sort(candidates.begin(), candidates.end());
        backtracking_recur(candidates, dummy, target);

        // eliminate duplicate elem;    

        return  eliminiate_duplcated(rt);
    }
    void backtracking_recur(const container con, vector<int> v, int target) {

        if (target == 0) {
            rt.push_back(v);
            return;
        }

        if (target < 0)  return;

        for (int i = 0; i < con.size(); i++) {
            int c = con[i];
            // build a new container without i;
            container new_con;
            for (int j = 0; j < con.size(); j++) {
                if (j != i) {
                    new_con.push_back(con[j]);
                }
            }
            v.push_back(c);
            backtracking_recur(new_con, v, target - c);
            v.pop_back();
        }






    }

    vector<vector<int>> eliminiate_duplcated(vector<vector<int>>& con) {
        auto comp = [](const container& l, const container& r) {
            if (l.size() < r.size())
                return true;
            if (l.size() > r.size()) {
                return false;
            }

            // how to judege l and r have the same value?
            map<int, int> p;
            for (auto i : l) {
                p[i]++;
            }
            for (auto i : r) {
                p[i]--;
            }
            for (auto i : p) {
                if (i.second != 0) {
                    return true;
                }
            }
            return false;
        };

        set<container, decltype(comp)> x(comp);
        for (auto& i : con) {
            x.insert(i);
        }

        return vector<vector<int>>(x.begin(), x.end());

    }
};
// leetcode 40
TEST(AlgoBT, DISABLED_CombiniationSum2) {

    vector<vector<int>> rt;

    Solution_CombiniationSum2 s;

    vector<int> candidates{ 10,1,2,7,6,1,5 };

    rt = s.combinationSum2(candidates, 8);


    if (rt.empty()) {

    }


}



class Solution_Permutation {
public:

    vector<string> rt;

    vector<string> perform(const string& in) {
        string buf;
        back_tracking(in, buf);
        return rt;
    }

    void back_tracking(string remainder, string& proc) {
        if (remainder.empty()) {
            rt.push_back(proc);
        }

        for (int i = 0; i < remainder.size(); i++) {
            string tmp = remainder;

            char c = tmp[i];
            tmp.erase(i, 1);
            proc.push_back(c);
            back_tracking(tmp, proc);
            proc.pop_back();
        }

    }


};

TEST(AlogBT, DISABLED_Permutation) {
    Solution_Permutation s;
    auto rt=s.perform("abc");
    for (auto s : rt) {
        cout << s << endl;
    }
}




// in:  "[XXX]"
// out: "xxx"
string parse(string& s, int& i) {
    string r;
    int l = s.size();
    while (i < l) {
        char c = s[i];
        if ('[' == c) {
            i++;
            continue;
        }
        if (']' == c) {
            i++;
            return r;
        }
        if (!isdigit(c)) {
            r.push_back(c);
            i++;
            continue;
        }
        // the format is XXX[
        string digit_buf(1, c);
        i++;
        while (i < l && isdigit(s[i])) {
            digit_buf += s[i++];
        }
        int n = stoi(digit_buf);
        //
        string ss= parse(s, i);
        while (n--) {
            r += ss;
        }
    }
    return r;
}

string decodeString(string s) {
    int index = 0;
    return parse(s, index);
}


string defangIPaddr(string address) {
    const string s{ "[. ]" };
    string rt;
    for (int pos = 0; pos != string::npos;) {
        int p2 = address.find('.', pos);
        if (p2 != string::npos) {
            rt.append(address, pos, p2 - pos);
            rt += s;
            pos = p2;
            pos++;
        }
        else {
            rt.append(address, pos);
            pos = p2;
        }
        
    }
    return rt;
}

vector<string> commonChars(const vector<string>& A) {
    vector<string> rt;
    if (A.size() < 2) {
        return rt;
    }
    unordered_map<char, int> m;
    unordered_map<char, int> m2;
    for (auto c : A[0]) {
        m[c]++;
    }
    for (auto c : A[1]) {
        if (m.count(c) && m[c]) {
            m[c]--;
            m2[c]++;
        }
    }
    for (int i = 2; i < A.size(); i++) {
        m = m2;
        m2.clear();
        for (auto c : A[i]) {
            if (m.count(c) && m[c]) {
                m[c]--;
                m2[c]++;
            }
        }
    }
    for (auto& p : m2) {
        for (int i = 0; i < p.second; i++) {
            string s;
            s += p.first;
            rt.push_back(s);
        }
    }
    return rt;
}

set<string> split(const string& s) {
    set<string> rt;
    string t;

    istringstream iss(s);

    while (iss >> t) rt.insert(t);


    
    return rt;

}


vector<string> findOcurrences(string text, string first, string second) {
    vector<string> rt;
    string key = first + ' ' + second;
    auto pos = text.find(key, 0);


    while (pos != string::npos) {
        pos += key.size();
        // ignore ' '
        while (pos < text.size() && text[pos] == ' ') {
            pos++;
        }

        auto p = pos;
        string s;
        while (p < text.size() && text[p] != ' ') {
            s.push_back(text[p]);
            p++;
        }
        rt.push_back(s);

        pos = text.find(key, pos);
    }
    return rt;
}


bool canFormArray(const vector<int>& arr, const vector<vector<int>>& pieces) {
    unordered_map<int, int>  m; //(price -> id);
    for (int i = 0; i < pieces.size(); i++) {
        auto& p = pieces[i];
        for (int j = 0; j < p.size(); ++j) {
            m[p[j]] = i;
        }
    }

    vector<int> ids;
    for (int i : arr) {
        if (!m.count(i)) {
            return false;
        }
        ids.push_back(m[i]);
    }

    return !all_of(ids.begin(), ids.end(), [&ids](int i) {return i == ids[0]; });

}


int findKthPositive(const vector<int>& arr, int k) {
    set<int> c(arr.begin(), arr.end());
    int ans = 0;
    while (k--) {
        ans++;
        if (!c.count(ans)) {
            continue;
        }
        else {
            while (c.count(ans)) { ans++; };

        }

    }

    return ans;
}


vector<int> createTargetArray(const vector<int>& N, const vector<int>& index) {
    vector<int> rt;
    auto nums = N;
    for (int i = 0; i < index.size(); ++i) {
        rt.insert(rt.begin() + index[i], nums[i]);
    }
    return rt;
}

TEST(AlgoBT, Permutation2) {

    aux::output(createTargetArray({ 0,1,2,3,4 }, { 0,1,2,2,1 }));

    getchar();

}