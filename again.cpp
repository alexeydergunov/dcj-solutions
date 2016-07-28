#include "again.h"
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

	/*int N = (int) GetN();
	if (N < nodes) {
		nodes = N;
		if (curNode >= nodes) return 0;
	}*/

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------

const int mod = 1000 * 1000 * 1000 + 7;

void solve(const int nodes, const int curNode) {
	int N = (int) GetN();

	int n = N / nodes;
	int from = curNode * n;
	if (curNode < N % nodes) {
		n++;
		from += curNode;
	} else {
		from += N % nodes;
	}

	vector<int> sumsA(nodes), sumsB(nodes);
	for (int i = 0; i < n; i++) {
		int id = (from + i) % nodes;
		sumsA[id] += GetA(from + i);
		sumsB[id] += GetB(from + i);
		if (sumsA[id] >= mod) sumsA[id] -= mod;
		if (sumsB[id] >= mod) sumsB[id] -= mod;
	}

	//debugVector(sumsA);
	//debugVector(sumsB);

	vector<int> a(nodes), b(nodes);
	if (curNode == 0) {
		for (int i = 0; i < nodes; i++) {
			a[i] += sumsA[i];
			b[i] += sumsB[i];
			a[i] %= mod;
			b[i] %= mod;
		}
		for (int node = 1; node < nodes; node++) {
			receive(node);
			for (int i = 0; i < nodes; i++) {
				a[i] += getInt(node);
				b[i] += getInt(node);
				a[i] %= mod;
				b[i] %= mod;
			}
		}
	} else {
		for (int i = 0; i < nodes; i++) {
			putInt(0, sumsA[i]);
			putInt(0, sumsB[i]);
		}
		send(0);
	}

	if (curNode == 0) {
		//debugVector(a);
		//debugVector(b);
		ll result = 0;
		for (int i = 0; i < nodes; i++) {
			for (int j = 0; j < nodes; j++) {
				if ((i + j) % nodes != 0) {
					result = (result + ll(a[i]) * b[j]) % mod;
				}
			}
		}
		cout << result << endl;
	}
}
