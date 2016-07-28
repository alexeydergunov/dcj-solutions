#include "almost_sorted.h"
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

	int N = (int) NumberOfFiles();
	if (N < nodes) {
		nodes = N;
		if (curNode >= nodes) return 0;
	}

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------

void solve(const int nodes, const int curNode) {
	const ll mask = (1 << 20) - 1;

	int N = (int) NumberOfFiles();
	int d = (int) MaxDistance();

	int n = (N + nodes - 1) / nodes;
	int from = curNode * n;
	int to = min(from + n, N);

	int L = max(0, from - d);
	int R = min(N, to + d);
	n = R - L;

	vector<ll> v(n);
	for (int i = 0; i < n; i++) {
		v[i] = Identifier(L + i);
	}

	sort(all(v));

	int shift = from - L;
	ll ans = 0;
	for (int i = shift, j = 0; j < to - from; i++, j++) {
		ans += (L + i) * v[i];
		ans &= mask;
	}

	vector<ll> a(nodes);
	if (curNode == 0) {
		a[0] = ans;
		for (int node = 1; node < nodes; node++) {
			receive(node); a[node] = getLL(node);
		}
	} else {
		putLL(0, ans); send(0);
	}

	if (curNode == 0) {
		ans = 0;
		for (int i = 0; i < nodes; i++) {
			ans += a[i];
			ans &= mask;
		}
		cout << ans << endl;		
	}
}
