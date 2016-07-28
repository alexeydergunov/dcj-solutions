#include "majority.h"
#include "message.h"
#include <bits/stdc++.h>
using namespace std;
#define all(v) (v).begin(), (v).end()
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

	int N = (int) GetN();
	if (N < nodes) {
		nodes = N;
		if (curNode >= nodes) return 0;
	}

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------

void solve(const int nodes, const int curNode) {
	int total = (int) GetN();
	int len = total / nodes + (curNode < total % nodes ? 1 : 0);

	vector<int> a(len);
	for (int i = 0; i < len; i++) {
		a[i] = (int) GetVote(curNode + i * nodes);
	}
	sort(all(a));

	int candidate = -1;
	for (int i = 0; i < len; ) {
		int j = i;
		while (j < len && a[i] == a[j]) {
			j++;
		}
		if (j - i >= len / 2 + 1) {
			candidate = a[i];
		}
		i = j;
	}

	vector<int> candidates;
	if (candidate != -1) {
		candidates.push_back(candidate);
	}

	for (int otherId = 0; otherId < curNode; otherId++) {
		putInt(otherId, candidate);
		send(otherId);
		receive(otherId);
		int otherCandidate = getInt(otherId);
		if (otherCandidate != -1) {
			candidates.push_back(otherCandidate);
		}
	}
	for (int otherId = curNode + 1; otherId < nodes; otherId++) {
		receive(otherId);
		int otherCandidate = getInt(otherId);
		if (otherCandidate != -1) {
			candidates.push_back(otherCandidate);
		}
		putInt(otherId, candidate);
		send(otherId);
	}

	sort(all(candidates));
	candidates.resize(unique(all(candidates)) - candidates.begin());

	vector<int> counts(sz(candidates));
	for (int x : a) {
		int index = lower_bound(all(candidates), x) - candidates.begin();
		if (index >= 0 && index < sz(candidates) && candidates[index] == x) {
			counts[index]++;
		}
	}

	if (curNode == 0) {
		for (int otherId = 1; otherId < nodes; otherId++) {
			receive(otherId);
			for (int i = 0; i < sz(counts); i++) {
				int add = getInt(otherId);
				counts[i] += add;
			}
		}
	} else {
		for (int i = 0; i < sz(counts); i++) {
			putInt(0, counts[i]);
		}
		send(0);
	}

	if (curNode == 0) {
		for (int i = 0; i < sz(counts); i++) {
			if (counts[i] >= total / 2 + 1) {
				cout << candidates[i] << endl;
				return;
			}
		}
		cout << "NO WINNER" << endl;
	}
}
