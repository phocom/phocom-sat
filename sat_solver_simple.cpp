#include <bits/stdc++.h>

using namespace std;

int N, M;
vector<array<int, 3>> clauses;
vector<int> assigns;
vector<int> answer;

bool solve() {
  int true_count = 0;
  int next_select = 0;
  vector<int> follow_list;
  for (auto &clause : clauses) {
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
    bool ret1 = solve();
    if (ret1) return 1;
    assigns[abs(next_select)] = -next_select;
    bool ret2 = solve();
    if (ret2) return 1;
    for (int x : follow_list) assigns[abs(x)] = 0;
    assigns[abs(next_select)] = 0;
  }
  return 0;
}

void read_problem(char filename[]) {
  ifstream ifs(filename);
  string s;
  while (ifs >> s, s != "cnf")
    ;
  ifs >> N >> M;
  assigns.resize(N + 1);
  clauses.resize(M);
  for (int i = 0; i < M; ++i) {
    int a, j = 0;
    while (ifs >> a, a != 0) {
      clauses[i][j++] = a;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "needs filename" << endl;
    return 1;
  }
  read_problem(argv[1]);
  bool ret = solve();
  if (ret) {
    cout << "SAT" << endl;
    for (int i = 1; i <= N; ++i)
      cout << (answer[i] == 0 ? i : answer[i]) << " ";
    cout << endl;
  } else {
    cout << "UNSAT" << endl;
  }
  return 0;
}