#include "TODO.h"
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

	int N = (int) GetN();
	if (N < nodes) {
		nodes = N;
		if (curNode >= nodes) return 0;
	}

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------


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

	vector<ll> v(n);
	for (int i = 0; i < n; i++) {
		v[i] = GetXXX(from + i);
	}

	
}
