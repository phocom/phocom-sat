#include <bits/stdc++.h>

using namespace std;

int N, M;
vector<vector<int>> clauses;
vector<int> assigns;
vector<int> answer;
int decision = 0;

bool solve() {
  int true_count = 0;
  int next_select = 0;
  vector<int> follow_list;
  for (auto clause : clauses) {
    bool res_clause = 0;
    vector<int> unassigned;
    for (int lit : clause) {
      if (assigns[abs(lit)] != 0) {
        bool res_lit = (lit > 0) ^ (assigns[abs(lit)] < 0);
        if (res_lit) {
          res_clause = 1;
          break;
        }
      } else {
        unassigned.push_back(lit);
      }
    }
    if (res_clause) {
      true_count++;
    } else {
      if (unassigned.empty()) {
        for (int x : follow_list) assigns[abs(x)] = 0;
        return 0;
      } else {
        if (unassigned.size() == 1) {
          assigns[abs(unassigned[0])] = unassigned[0];
          follow_list.push_back(unassigned[0]);
        } else if (next_select == 0) {
          next_select = unassigned[0];
        }
      }
    }
  }
  if (true_count == M) {
    answer = assigns;
    return 1;
  } else {
    assigns[abs(next_select)] = next_select;
    decision++;
    bool ret1 = solve();
    if (ret1) {
      return 1;
    }
    assigns[abs(next_select)] = -next_select;
    bool ret2 = solve();
    if (ret2) {
      return 1;
    }
    for (int x : follow_list) assigns[abs(x)] = 0;
    assigns[abs(next_select)] = 0;
    decision--;
  }
  return 0;
}

int main() {
  string s;
  while (cin >> s, s != "cnf")
    ;
  cin >> N >> M;
  assigns.resize(N + 1);
  for (int i = 0; i < M; ++i) {
    clauses.push_back(vector<int>());
    int a;
    while (cin >> a, a != 0) {
      clauses.back().push_back(a);
    }
  }
  bool ret = solve();
  if (ret) {
    cout << "SAT" << endl;
    for (int x : answer) cout << x << " ";
    cout << endl;
  } else {
    cout << "UNSAT" << endl;
  }
  return 0;
}