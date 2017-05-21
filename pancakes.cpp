#include "pancakes.h"
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

	int N = (int) GetStackSize();
	if (N < nodes) {
		nodes = N;
		if (curNode >= nodes) return 0;
	}

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------


void solve(const int nodes, const int curNode) {
	int N = (int) GetStackSize();
	//int D = (int) GetNumDiners();

	int n = N / nodes;
	int from = curNode * n;
	if (curNode < N % nodes) {
		n++;
		from += curNode;
	} else {
		from += N % nodes;
	}

	vector<int> v(n);
	for (int i = 0; i < n; i++) {
		v[i] = (int) GetStackItem(from + i);
	}

	int localAns = 0;
	for (int i = 1; i < n; i++) {
		if (v[i - 1] > v[i]) localAns++;
	}
	//cerr << curNode << " " << localAns << endl;
	
	vector<int> localAnswers(nodes), firstNum(nodes), lastNum(nodes);
	if (curNode == 0) {
		localAnswers[0] = localAns;
		firstNum[0] = v[0];
		lastNum[0] = v[n - 1];
		for (int node = 1; node < nodes; node++) {
			receive(node);
			localAnswers[node] = getInt(node);
			firstNum[node] = getInt(node);
			lastNum[node] = getInt(node);
		}
	} else {
		putInt(0, localAns);
		putInt(0, v[0]);
		putInt(0, v[n - 1]);
		send(0);
	}
	
	if (curNode == 0) {
		int finalAns = 1;
		for (int node = 0; node < nodes; node++) {
			finalAns += localAnswers[node];
		}
		for (int node = 1; node < nodes; node++) {
			if (lastNum[node - 1] > firstNum[node]) {
				finalAns++;
			}
		}
		cout << finalAns << endl;
	}
}
