#include "winning_move.h"
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
	for (int i = 0; i < sz(v); i++) cerr << v[i] << " "; cerr << endl;
}

int main() {
	int nodes = NumberOfNodes();
	int curNode = MyNodeId();

	int N = (int) GetNumPlayers();
	if (N < nodes) {
		nodes = N;
		if (curNode >= nodes) return 0;
	}

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------

int getHash(ll x, int nodes) {
	ll y = x ^ (x >> 3) ^ (x >> 13) ^ (x >> 23) ^ (x >> 33) ^ (x >> 43);
	return (int) (y % nodes);
}

void sendVector(vector<ll> &v, int target) {
	putInt(target, sz(v));
	for (int i = 0; i < sz(v); i++) {
		putLL(target, v[i]);
	}
	send(target);
}

void receiveIntoMap(map<ll, int> &mp, int source) {
	receive(source);
	int n = getInt(source);
	for (int i = 0; i < n; i++) {
		ll x = getLL(source);
		mp[x]++;
	}
}

void solve(const int nodes, const int curNode) {
	int N = (int) GetNumPlayers();

	int n = N / nodes;
	int from = curNode * n;
	if (curNode < N % nodes) {
		n++;
		from += curNode;
	} else {
		from += N % nodes;
	}

	map<ll, int> mp;
	for (int i = 0; i < n; i++) {
		ll x = GetSubmission(from + i);
		mp[x]++;
	}

	vector<vector<ll>> toSend(nodes);
	for (auto& it : mp) {
		ll x = it.first;
		int cnt = min(it.second, 2);
		int h = getHash(x, nodes);
		for (int i = 0; i < cnt; i++) {
			toSend[h].push_back(x);
		}
	}

	for (int node = 0; node < nodes; node++) {
		sendVector(toSend[node], node);
	}

	mp.clear();
	for (int node = 0; node < nodes; node++) {
		receiveIntoMap(mp, node);
	}

	ll candidate = 0;
	for (auto& it : mp) {
		if (it.second == 1) {
			candidate = it.first;
			break;
		}
	}
	putLL(0, candidate);
	send(0);

	if (curNode == 0) {
		vector<ll> candidates(nodes);
		for (int node = 0; node < nodes; node++) {
			receive(node);
			candidates[node] = getLL(node);
		}
		sort(all(candidates));
		ll ans = 0;
		for (int i = 0; i < nodes; i++) {
			if (candidates[i] > 0) {
				ans = candidates[i];
				break;
			}
		}
		cout << ans << endl;
	}
}
