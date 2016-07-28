#include "oops.h"
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

	ll mn = (ll) 8e18;
	ll mx = (ll) -8e18;
	for (int i = 0; i < n; i++) {
		ll x = GetNumber(from + i);
		mn = min(mn, x);
		mx = max(mx, x);
	}

	vector<ll> mins(nodes), maxs(nodes);
	if (curNode == 0) {
		mins[0] = mn;
		maxs[0] = mx;
		for (int node = 1; node < nodes; node++) {
			receive(node);
			mins[node] = getLL(node);
			receive(node);
			maxs[node] = getLL(node);
		}
	} else {
		putLL(0, mn);
		send(0);
		putLL(0, mx);
		send(0);
	}

	if (curNode == 0) {
		mn = *min_element(all(mins));
		mx = *max_element(all(maxs));
		ll ans = mx - mn;
		if (ans < 0) {
			ans = 0;
		}
		cout << ans << endl;
	}
}
