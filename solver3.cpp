#include <bits/stdc++.h>

using namespace std;

int N, M;                       // �ϐ��̐��A�߂̐�
vector<array<int, 3>> clauses;  // ���͂�cnf
vector<int> assigns, answer;  // ���݂̊��蓖�āA�������ꂽ�[�����蓖��
vector<int> satisfy_count;  // ���̐߂ɐ^�̃��e�������������݂��邩
vector<int> rest;  // ���̐߂ɖ����蓖�Ă̕ϐ����������݂��邩
unordered_set<int>
    rest_to_clauses[4];  // �����蓖�Ẵ��e�����̐�->�߂̔ԍ��̈ꗗ
unordered_set<int> satisfied;        // �[���ς݂̐߂̔ԍ��̈ꗗ
vector<vector<int>> lit_to_clauses;  // �ϐ�->������܂ސ߂̔ԍ��̈ꗗ
vector<int> counts;                  // �ϐ�->���͒��ɓo�ꂷ���
auto cmp = [](const int &l, const int &r) {
  return counts[l] == counts[r] ? l < r : counts[l] > counts[r];
};
set<int, function<bool(const int &, const int &)>> unassigned(
    cmp);  // �����蓖�Ă̕ϐ��i�o�ꂪ�������ɕ��ׂ�j

template <class T>
string to_string(T s);
template <class S, class T>
string to_string(pair<S, T> p);
string to_string(string s) { return s; }
string to_string(const char s[]) { return to_string(string(s)); }

template <class T>
string to_string(T v) {
  if (v.empty()) return "{}";
  string ret = "{";
  for (auto x : v) ret += to_string(x) + ",";
  ret.back() = '}';
  return ret;
}
template <class S, class T>
string to_string(pair<S, T> p) {
  return "{" + to_string(p.first) + "," + to_string(p.second) + "}";
}

void debug() { cerr << endl; }
template <class Head, class... Tail>
void debug(Head head, Tail... tail) {
  cerr << to_string(head) << " ";
  debug(tail...);
}

// lit�����蓖��
bool assign(int lit) {
  assigns[abs(lit)] = lit;
  unassigned.erase(abs(lit));
  bool ret = 1;
  for (int i : lit_to_clauses[abs(lit)]) {
    for (int j = 0; j < 3; ++j) {
      bool not_yet = satisfy_count[i] == 0;  // �܂��[������Ă��Ȃ��H
      if (lit == clauses[i][j]) {
        if (not_yet) rest_to_clauses[rest[i]].erase(i);
        rest[i]--;
        satisfy_count[i]++;
        if (not_yet) satisfied.insert(i);
      } else if (lit == -clauses[i][j]) {
        if (not_yet) rest_to_clauses[rest[i]].erase(i);
        rest[i]--;
        if (not_yet) rest_to_clauses[rest[i]].insert(i);
        if (not_yet && rest[i] == 0) {  // ��������
          ret = 0;
        }
      }
    }
  }
  return ret;
}

// lit�̊��蓖�Ă�������
void unassign(int lit) {
  for (int i : lit_to_clauses[abs(lit)]) {
    for (int j = 0; j < 3; ++j) {
      bool not_yet = satisfy_count[i] == 0;  // �܂��[������Ă��Ȃ��H
      if (lit == clauses[i][j]) {
        rest[i]++;
        satisfy_count[i]--;
        if (satisfy_count[i] == 0) {
          satisfied.erase(i);
          rest_to_clauses[rest[i]].insert(i);
        }
      } else if (lit == -clauses[i][j]) {
        if (not_yet) rest_to_clauses[rest[i]].erase(i);
        rest[i]++;
        if (not_yet) rest_to_clauses[rest[i]].insert(i);
      }
    }
  }
  unassigned.insert(abs(lit));
  assigns[abs(lit)] = 0;
}

// �ċA�ɂ��T��
bool solve() {
  vector<int> follow_list;  // ���̃X�e�b�v���ŒP�ʐ߂̂��ߊ��蓖�Ă��ϐ��̈ꗗ
  while (!rest_to_clauses[1].empty()) {  // �����蓖�Ă̕ϐ���1�̖��[���̐߂�
    int i = *rest_to_clauses[1].begin();
    int lit = 0;
    for (int j = 0; j < 3; ++j)
      if (clauses[i][j] != 0 && assigns[abs(clauses[i][j])] == 0) {
        lit = clauses[i][j];
        break;
      }
    bool f = assign(lit);
    follow_list.push_back(lit);
    if (!f) {  // ���蓖�ĂɎ��s
      for (int l : follow_list) unassign(l);
      return 0;  // �S�Ď������Ė߂�
    }
  }
  if (satisfied.size() == M) {  // �S�Ă̐߂��[������
    answer = assigns;
    return 1;
  } else {  // �V���ȕϐ��𕪊�����
    int next_select = *unassigned.begin();
    assign(next_select);
    bool ret1 = solve();
    if (ret1) return 1;
    unassign(next_select);
    assign(-next_select);
    bool ret2 = solve();
    if (ret2) return 1;
    unassign(-next_select);
    for (int l : follow_list) unassign(l);
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
  rest.resize(M);
  satisfy_count.resize(M);
  lit_to_clauses.resize(N + 1);
  counts.resize(N + 1);
  for (int i = 0; i < M; ++i) {
    int a, j = 0;
    while (ifs >> a, a != 0) {
      clauses[i][j++] = a;
      counts[abs(a)]++;
      lit_to_clauses[abs(a)].push_back(i);
    }
    rest[i] = j;
    rest_to_clauses[j].insert(i);
  }
  for (int i = 1; i <= N; ++i) unassigned.insert(i);
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