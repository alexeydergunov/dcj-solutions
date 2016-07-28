#include "rps.h"
#include "message.h"
#include <bits/stdc++.h>
using namespace std;
#define all(v) (v).begin(),(v).end()
#define sz(v) ((int)((v).size()))
typedef long long ll;
typedef pair<int, int> ii;
//------------------------------------------------------------------------------
void putChar(int target, char value) { PutChar(target, value); }
void putInt (int target, int  value) { PutInt (target, value); }
void putLL  (int target, ll   value) { PutLL  (target, value); }

char getChar(int source) { return GetChar(source); }
int  getInt (int source) { return GetInt (source); }
ll   getLL  (int source) { return GetLL  (source); }

void send   (int target) {        Send   (target); }
int  receive(int source) { return Receive(source); }

void solve(const int nodes, const int curNode);
//------------------------------------------------------------------------------

int main() {
	int nodes = NumberOfNodes();
	int curNode = MyNodeId();

	int N = 1 << ((int) GetN());
	if (nodes > N / 2) {
		nodes = N / 2;
	}

	while ((nodes & (nodes - 1)) != 0) {
		nodes--;
	}

	if (curNode == 0)  {
		//cerr << "N = " << N << ", nodes = " << nodes << endl;
	}
	if (N % nodes != 0) {
		throw 0;
	}

	if (curNode >= nodes) return 0;

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------

bool wins(char a, char b) {
	if (a == b) {
		cerr << "a == b in wins()";
		throw 0;
	}
	if (a == 'R' && b == 'S') return true;
	if (a == 'S' && b == 'P') return true;
	if (a == 'P' && b == 'R') return true;
	return false;
}

void emulate(vector<pair<char, int>> &v) {
	while (sz(v) > 1) {
		vector<pair<char, int>> v2;
		v2.reserve(sz(v) / 2);
		for (int i = 0; i < sz(v); i += 2) {
			char c1 = v[i].first;
			char c2 = v[i + 1].first;
			if (c1 == c2) {
				v2.push_back(v[i]);
			} else {
				if (wins(c1, c2)) {
					v2.push_back(v[i]);
				} else {
					v2.push_back(v[i + 1]);
				}
			}
		}
		v = v2;
		//cerr << "in emulate(): "; for (int i = 0; i < sz(v); i++) cerr << "(" << v[i].first << " " << v[i].second << ") "; cerr << endl;
	}
}

void solve(const int nodes, const int curNode) {
	int N = 1 << ((int) GetN());

	int n = N / nodes;
	vector<pair<char, int>> v(n);
	for (int i = 0; i < n; i++) {
		int id = curNode * n + i;
		char c = GetFavoriteMove(id);
		v[i] = make_pair(c, id);
	}

	//cerr << curNode << ": "; for (int i = 0; i < sz(v); i++) cerr << "(" << v[i].first << " " << v[i].second << ") "; cerr << endl;

	emulate(v);

	char winC = v[0].first;
	int winI = v[0].second;

	v.clear();
	v.resize(nodes);
	if (curNode == 0) {
		v[0] = make_pair(winC, winI);
		for (int node = 1; node < nodes; node++) {
			receive(node);
			char c = getChar(node);
			receive(node);
			int idx = getInt(node);
			v[node] = make_pair(c, idx);
		}
	} else {
		putChar(0, winC);
		send(0);
		putInt(0, winI);
		send(0);
	}
	
	if (curNode == 0) {
		//cerr << curNode << ": "; for (int i = 0; i < sz(v); i++) cerr << "(" << v[i].first << " " << v[i].second << ") "; cerr << endl;
		emulate(v);
		cout << v[0].second << endl;
	}
}
