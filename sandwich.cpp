#include "sandwich.h"
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
	int N = (int) GetN();

	int n = (N + nodes - 1) / nodes;
	int from = curNode * n;
	int to = min(from + n, N);
	n = to - from;

	vector<ll> v(n);
	for (int i = 0; i < n; i++) {
		v[i] = GetTaste(from + i);
	}

	ll prefMin = 0;
	ll sum = 0;
	for (int i = 0; i < n; i++) {
		sum += v[i];
		prefMin = min(prefMin, sum);
	}

	ll suffMin = 0;
	sum = 0;
	for (int i = n - 1; i >= 0; i--) {
		sum += v[i];
		suffMin = min(suffMin, sum);
	}

	ll ans = 0;
	sum = 0;
	for (int i = 0; i < n; i++) {
		sum += v[i];
		if (sum < ans) {
			ans = sum;
		}
		if (sum > 0) {
			sum = 0;
		}
	}

	sum = 0;
	for (int i = 0; i < n; i++) {
		sum += v[i];
	}

	//cerr << "Node = " << curNode << endl;
	//cerr << from << " " << to << endl;
	//for (int i = 0; i < n; i++) cerr << v[i] << " "; cerr << endl;
	//cerr << prefMin << " " << suffMin << " " << ans << " " << sum << endl;

	vector<ll> prefMins(nodes), suffMins(nodes), localAns(nodes), sums(nodes);
	if (curNode == 0) {
		prefMins[0] = prefMin;
		suffMins[0] = suffMin;
		localAns[0] = ans;
		sums[0] = sum;
		for (int node = 1; node < nodes; node++) {
			Receive(node); prefMins[node] = GetLL(node);
			Receive(node); suffMins[node] = GetLL(node);
			Receive(node); localAns[node] = GetLL(node);
			Receive(node); sums[node] = GetLL(node);
		}
	} else {
		PutLL(0, prefMin); Send(0);
		PutLL(0, suffMin); Send(0);
		PutLL(0, ans); Send(0);
		PutLL(0, sum); Send(0);
	}

	if (curNode == 0) {
		ll globalAns = 0;
		for (int L = 0; L < nodes; L++) {
			for (int R = L; R < nodes; R++) {
				if (L == R) {
					globalAns = min(globalAns, localAns[L]);
				} else {
					sum = 0;
					sum += suffMins[L];
					for (int i = L + 1; i <= R - 1; i++) {
						sum += sums[i];
					}
					sum += prefMins[R];
					globalAns = min(globalAns, sum);
				}
			}
		}
		sum = 0;
		for (int i = 0; i < nodes; i++) {
			sum += sums[i];
		}
		cout << sum - globalAns << endl;
	}	
}
