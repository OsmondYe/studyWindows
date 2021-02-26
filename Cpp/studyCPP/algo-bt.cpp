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
