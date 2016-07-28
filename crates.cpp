#include "crates.h"
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

	int N = (int) GetNumStacks();
	if (N < nodes) {
		nodes = N;
		if (curNode >= nodes) return 0;
	}

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------

const ll mod = 1000 * 1000 * 1000 + 7;

void getFrom(const int curNode, const int nodes, const int N, int& n, int& from) {
	n = N / nodes;
	from = curNode * n;
	if (curNode < N % nodes) {
		n++;
		from += curNode;
	} else {
		from += N % nodes;
	}
}

template<class T> void debugVector(int curNode, vector<T> &v) {
	cerr << curNode << ": "; for (int i = 0; i < sz(v); i++) cerr << v[i] << " "; cerr << endl;
}

void solve(const int nodes, const int curNode) {
	int N = (int) GetNumStacks();

	int n, from;
	getFrom(curNode, nodes, N, n, from);

	//cerr << curNode << ": " << from << " " << from + n << endl;

	vector<int> h(n);
	ll sum = 0;
	for (int i = 0; i < n; i++) {
		//cerr << curNode << ": ask  " << from + i + 1 << endl;
		h[i] = (int) GetStackHeight(from + i + 1);
		sum += h[i];
	}

	//cerr << curNode << ": from = " << from << " , to = " << from + n << ", elements = "; for (int i = 0; i < sz(h); i++) cerr << h[i] << " "; cerr << endl;
	//return;

	vector<ll> sums(nodes);
	if (curNode == 0) {
		sums[0] = sum;
		for (int node = 1; node < nodes; node++) {
			receive(node);
			sums[node] = getLL(node);
		}
	} else {
		putLL(0, sum);
		send(0);
	}

	ll globalSum;
	if (curNode == 0) {
		globalSum = 0;
		for (int i = 0; i < nodes; i++) {
			globalSum += sums[i];
		}
		for (int node = 1; node < nodes; node++) {
			putLL(node, globalSum);
			send(node);
		}
	} else {
		receive(0);
		globalSum = getLL(0);
	}

	int cntBigGlobal = globalSum % N;
	ll hBig = globalSum / N + 1;
	ll hSmall = globalSum / N;

	// [from, from + n)
	//int cntBig = max(0, cntBigGlobal - from);
	//int cntSmall = n - cntBig;

	//cerr << curNode << ": " << globalSum << " " << hBig << " " << hSmall << " " << cntBigGlobal << endl;
	//return;

	ll sumToLeft;
	ll sumToRight;
	if (curNode == 0) {
		sumToLeft = sums[0];
		for (int node = 1; node < nodes; node++) {
			putLL(node, sumToLeft);
			send(node);
			sumToLeft += sums[node];
		}
		sumToLeft = 0;

		sumToRight = 0;
		for (int node = nodes - 1; node >= 1; node--) {
			putLL(node, sumToRight);
			send(node);
			sumToRight += sums[node];
		}
	} else {
		receive(0);
		sumToLeft = getLL(0);
		receive(0);
		sumToRight = getLL(0);
	}

	//cerr << curNode << ": " << sumToLeft << " " << sumToRight << endl;
	//return;
	
	vector<ll> needSums(nodes);
	for (int i = 0; i < nodes; i++) {
		int nI, fromI;
		getFrom(i, nodes, N, nI, fromI);
		int cntBigLocal = min(nI, max(0, cntBigGlobal - fromI));
		int cntSmallLocal = nI - cntBigLocal;
		ll needSumI = hSmall * cntSmallLocal + hBig * cntBigLocal;
		//cerr << curNode << ": cntBigLocal cntSmallLocal i needSumI " << cntBigLocal << " " << cntSmallLocal << " " << i << " "  << needSumI << endl;
		needSums[i] = needSumI;
		if (i == curNode) {
			//cerr << curNode << ": fromI, nI: " << fromI << " " << fromI + nI << " , cnt big, small = " << cntBigLocal << ", " << cntSmallLocal  <<  endl;
		}
	}

	//debugVector(curNode, needSums);
	//return;

	ll balanceLeft = sumToLeft;
	for (int i = 0; i < curNode; i++) {
		balanceLeft -= needSums[i];
	}
	ll balanceRight = sumToRight;
	for (int i = curNode + 1; i < nodes; i++) {
		balanceRight -= needSums[i];
	}

	//cerr << curNode << ": balLeft = " << balanceLeft << ", balRight = " << balanceRight << endl;
	//return;

	vector<ll> v(n + 2);
	v[0] = balanceLeft;
	for (int i = 1; i <= n; i++) {
		v[i] = h[i - 1];
	}
	v[n + 1] = balanceRight;

	//debugVector(curNode, v);

	ll ans = 0;
	for (int i = 0; i <= n; i++) {
		ll needH;
		if (i == 0 || i == n + 1) {
			needH = 0;
		} else {
			needH = (from + (i - 1) < cntBigGlobal) ? hBig : hSmall;
		}
		ll add = abs(needH - v[i]);
		//cerr << "->" << i << " " << v[i] << " " << needH << " , added " << add << endl;
		if (needH > v[i]) {
			v[i] += add;
			v[i + 1] -= add;
		} else {
			v[i] -= add;
			v[i + 1] += add;
		}
		if (i != n) {
			ans += add;
			ans %= mod;
			if (ans < 0) ans += mod;
		}
	}

	//debugVector(curNode, v);
	//cerr << curNode << ": ans = " << ans << endl;

	if (v[n + 1] != 0) {
		cerr << "v[n + 1] != 0" << endl;
		throw 0;
	}
	
	vector<ll> answers(nodes);
	if (curNode == 0) {
		answers[0] = ans;
		for (int node = 1; node < nodes; node++) {
			receive(node);
			answers[node] = getLL(node);
		}
	} else {
		putLL(0, ans);
		send(0);
	}

	if (curNode == 0) {
		ll finalAns = 0;
		for (int i = 0; i < nodes; i++) {
			finalAns += answers[i];
			finalAns %= mod;
			if (finalAns < 0) finalAns += mod;
		}
		cout << finalAns << endl;		
	}
}
