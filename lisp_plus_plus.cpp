#include "lisp_plus_plus.h"
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

template<class T> void debugVector(vector<T> &v) {
	cerr << "[";
	for (int i = 0; i < sz(v); i++) {
		cerr << v[i];
		if (i != sz(v) - 1) cerr << " ";
	}
	cerr << "]" << endl;
}

int main() {
	int nodes = NumberOfNodes();
	int curNode = MyNodeId();

	int N = (int) GetLength();
	if (N < nodes) {
		nodes = N;
		if (curNode >= nodes) return 0;
	}

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------

void getFromAndN(const int N, const int nodes, const int curNode, int& from, int& n) {
	n = N / nodes;
	from = curNode * n;
	if (curNode < N % nodes) {
		n++;
		from += curNode;
	} else {
		from += N % nodes;
	}
}

void solve(const int nodes, const int curNode) {
	int N = (int) GetLength();

	vector<int> froms(nodes), ns(nodes);
	for (int i = 0; i < nodes; i++) {
		getFromAndN(N, nodes, i, froms[i], ns[i]);
	}
	int from = froms[curNode];
	int n = ns[curNode];

	int totalBalance = 0;
	int minBalance = 0;
	for (int i = 0; i < n; i++) {
		char ch = GetCharacter(from + i);
		if (ch == '(') totalBalance++; else totalBalance--;
		minBalance = min(minBalance, totalBalance);
	}

	vector<int> totals(nodes), mins(nodes);
	if (curNode == 0) {
		totals[0] = totalBalance;
		mins[0] = minBalance;
		for (int node = 1; node < nodes; node++) {
			receive(node);
			totals[node] = getInt(node);
			mins[node] = getInt(node);
		}
	 } else {
		putInt(0, totalBalance);
		putInt(0, minBalance);
		send(0);
	}

	if (curNode == 0) {
		//debugVector(froms);
		//debugVector(ns);
		//debugVector(totals);
		//debugVector(mins);
		int nodeToCheck = -1;
		totalBalance = 0;
		minBalance = 0;
		for (int i = 0; i < nodes; i++) {
			int prevBalance = totalBalance;
			totalBalance += totals[i];
			minBalance = min(minBalance, prevBalance + mins[i]);
			//cerr << "After node " << i << ": total = " << totalBalance  <<  ", minBalance = " << minBalance << endl; 
			if (minBalance < 0) {
				nodeToCheck = i;
				break;
			}
		}

		//cerr << "nodeToCheck = " << nodeToCheck << endl;

		if (nodeToCheck == -1) {
			if (totalBalance == 0) {
				cout << -1 << endl;
				return;
			} else if (totalBalance > 0) {
				cout << N << endl;
				return;
			} else {
				cerr << "totalBalance < 0" <<  endl;
				throw 0;
			}
		}

		int ans = 0;
		totalBalance = 0;
		for (int i = 0; i < nodeToCheck; i++) {
			totalBalance += totals[i];
			ans += ns[i];
		}

		bool found = false;
		for (int i = 0; i < ns[nodeToCheck]; i++) {
			char ch = GetCharacter(froms[nodeToCheck] + i);
			if (ch == '(') totalBalance++; else totalBalance--;
			if (totalBalance < 0) {
				found = true;
				break;
			}
			ans++;
		}

		if (!found) {
			cerr << "Not found" << endl;
			throw 0;
		}

		cout << ans << endl;
	}
}
