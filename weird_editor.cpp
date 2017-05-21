#include "weird_editor.h"
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

const int mod = 1000 * 1000 * 1000 + 7;

ll powmod(ll a, ll p) {
	if (p == 0) return 1;
	if (p % 2 == 0) {
		ll t = powmod(a, p / 2);
		return (t * t) % mod;
	}
	return (a * powmod(a, p - 1)) % mod;
}

ll inv(ll a) {
	return powmod(a, mod - 2);
}

ll sum10(ll k) {
	ll up = (powmod(10, k) - 1 + mod) % mod;
	ll downInv = inv(9);
	return (up * downInv) % mod;
}

const int threshold = (int) sqrt(1.0 * 1000 * 1000 * 1000);

int main() {
	int nodes = NumberOfNodes();
	int curNode = MyNodeId();

	int N = (int) GetNumberLength();
	if (N < nodes) {
		nodes = N;
		if (curNode >= nodes) return 0;
	}

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------


void solve(const int nodes, const int curNode) {
	int N = (int) GetNumberLength();

	int n = N / nodes;
	int from = curNode * n;
	if (curNode < N % nodes) {
		n++;
		from += curNode;
	} else {
		from += N % nodes;
	}

	vector<int> v(n);
	vector<int> lastLocalOccurence(10, -1);
	int maxDigit = -1;
	for (int i = 0; i < n; i++) {
		v[i] = (int) GetDigit(from + i);
		lastLocalOccurence[v[i]] = i;
		maxDigit = max(maxDigit, v[i]);
	}
	//cerr << "last occur: "; debugVector(lastLocalOccurence); cerr << endl;

	vector<int> localAnswer(10);
	int curDigit = maxDigit;
	for (int i = 0; i < n; i++) {
		if (v[i] == curDigit) {
			localAnswer[curDigit]++;
			if (i == lastLocalOccurence[curDigit]) {
				curDigit--;
				while (curDigit >= 0 && lastLocalOccurence[curDigit] < i) { // fixed: < i
					curDigit--;
				}
				if (curDigit < 0) {
					break;
				}
			}
		}
	}

	//cerr << "digits for this node = "; debugVector(v); cerr << endl;
	//cerr << "local answer = "; debugVector(localAnswer); cerr << endl;

	vector<vector<int> > answers(nodes, vector<int>(10));
	if (curNode == 0) {
		answers[0] = localAnswer;
		for (int node = 1; node < nodes; node++) {
			receive(node);
			for (int i = 0; i < 10; i++) {
				answers[node][i] = getInt(node);
			}
		}
	} else {
		for (int i = 0; i < 10; i++) {
			putInt(0, localAnswer[i]);
		}
		send(0);
	}

	vector<int> finalAns(10);
	if (curNode == 0) {
		int startNode = 0;
		for (curDigit = 9; curDigit >= 0; curDigit--) {
			int endNode = -1;
			for (int node = startNode; node < nodes; node++) {
				if (answers[node][curDigit] > 0) {
					endNode = node;
				}
			}
			if (endNode == -1) {
				continue;
			}
			for (int node = startNode; node <= endNode; node++) {
				finalAns[curDigit] += answers[node][curDigit];
			}
			startNode = endNode;
		}
		int sum = 0;
		for (int dig = 1; dig <= 9; dig++) {
			sum += finalAns[dig];
		}
		finalAns[0] = N - sum;
		//cerr << "final answer = "; debugVector(finalAns); cerr << endl;
		
		ll ans = 0;
		ll curPow10 = 1;
		for (int dig = 0; dig < 10; dig++) {
			while (finalAns[dig] > 0) {
				if (finalAns[dig] >= threshold) {
					ans += dig * curPow10 % mod * sum10(threshold) % mod;
					ans %= mod;
					finalAns[dig] -= threshold;
					curPow10 *= powmod(10, threshold);
					curPow10 %= mod;
				} else {
					ans += dig * curPow10 % mod;
					ans %= mod;
					finalAns[dig]--;
					curPow10 *= 10;
					curPow10 %= mod;
				}
			}
		}
		cout << ans << endl;
	}
}
