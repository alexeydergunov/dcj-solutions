#include "query_of_death.h"
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

const int timesToDetect = 30;

int findBad(int L, int R) {
	for (int i = L; i <= R; i++) {
		int cnt0 = 0;
		int cnt1 = 0;
		for (int k = 0; k < timesToDetect; k++) {
			int val = GetValue(i);
			if (val == 0) cnt0++;
			if (val == 1) cnt1++;
		}
		if (cnt0 != 0 && cnt1 != 0) {
			return i;
		}
	}
	cerr << "Couldn't find badIdx!!!" << endl;
	return -1;
}

void solveEasyKnowingBadIdx(int L, int R, int badIdx, int startSum) {
	int sum = 0;
	for (int i = L; i <= R; i++) {
		if (i != badIdx) {
			sum += GetValue(i);
		}
	}
	sum += GetValue(badIdx);	
	cout << sum + startSum << endl;
}

void solveEasy(int L, int R, int node1, int node2) {
	int curNode = MyNodeId();

	if (curNode == node2) {
		int badIdx = findBad(L, R);
		putInt(node1, badIdx);
		send(node1);
	}
	
	if (curNode == node1) {
		receive(node2);
		int badIdx = getInt(node2);
		solveEasyKnowingBadIdx(L, R, badIdx, 0);
	}
}

int main() {
	int nodes = NumberOfNodes();
	int curNode = MyNodeId();
	
	int N = (int) GetLength();

	if (N <= 1000) {
		solveEasy(0, N - 1, 0, 1);
		return 0;
	}

	solve(nodes, curNode);
}
//------------------------------------------------------------------------------

const ll mod = 1000 * 1000 * 1000 + 9;

ll powmod(ll a, ll n) {
	if (n == 0) return 1 % mod;
	if (n % 2 == 0) {
		ll t = powmod(a, n / 2);
		return t * t % mod;
	}
	return a * powmod(a, n - 1) % mod;
}

const ll prime = 239;
const ll invPrime = powmod(prime, mod - 2);

void getFromAndN(const int curNode, const int nodes, const int N, int& n, int& from) {
	n = N / nodes;
	from = curNode * n;
	if (curNode < N % nodes) {
		n++;
		from += curNode;
	} else {
		from += N % nodes;
	}
}

int getSumChecked(int L, int R) {
	int sum1 = 0;
	int sum2 = 0;
	ll hash1 = 0;
	ll hash2 = 0;

	ll curPower = 1;
	for (int i = L; i <= R; i++) {
		int val = GetValue(i);
		if (val == 1) {
			hash1 += curPower;
			sum1++;
		}
		curPower *= prime;
		curPower %= mod;
	}
	hash1 %= mod;

	for (int i = R; i >= L; i--) {
		curPower *= invPrime;
		curPower %= mod;
		int val = GetValue(i);
		if (val == 1) {
			hash2 += curPower;
			sum2++;
		}
	}
	hash2 %= mod;

	if (hash1 == hash2 && sum1 == sum2) {
		return sum1;
	} else {
		return -1;
	}
}

// step 1: [0, nodes/2) -> first wave
// step 2: nodes - 3 - first wave processor
// step 3: [nodes/2, nodes - 3) -> second wave
// step 4: nodes - 2 -> find badIdx
// step 5: nodes - 1 -> calc sum, print ans

void step1(const int nodes, const int curNode) {
	int N = (int) GetLength();
	
	int n = -1, from = -1;
	getFromAndN(curNode, nodes / 2, N, n, from);
	
	int sum = getSumChecked(from, from + n - 1);
	//cerr << "step 1: node " << curNode << " processed [" << from << ", " << from + n - 1 << "] and got sum = " << sum << endl;

	putInt(nodes - 3, sum);
	send(nodes - 3);
}

void step2(const int nodes, const int curNode) {
	if (curNode != nodes - 3) throw 0;
	
	int badNode = -1;
	int sum = 0;
	for (int node = 0; node < nodes / 2; node++) {
		receive(node);
		int thatSum = getInt(node);
		//cerr << "step 2: node " << curNode << " got sum = " << thatSum << " from node " << node << endl;
		if (thatSum == -1) {
			badNode = node;
		} else {
			sum += thatSum;
		}
	}
	
	int N = (int) GetLength();
	int n = -1, from = -1;
	getFromAndN(badNode, nodes / 2, N, n, from);
	int L = from;
	int R = from + n - 1;
	//cerr << "step 2: new segment for processing will be [" << L << ", " << R << "]" << endl;
	
	for (int node = nodes / 2; node < nodes - 3; node++) {
		putInt(node, L);
		putInt(node, R);
		send(node);
	}
	
	putInt(nodes - 1, sum);
	send(nodes - 1);
}

void step3(const int nodes, const int curNode) {
	receive(nodes - 3);
	int L = getInt(nodes - 3);
	int R = getInt(nodes - 3);
	//cerr << "step 3: node " << curNode << " got L = " << L << ", R = " << R << " from node " << nodes - 3 << endl;
	int length = R - L + 1;
	
	int n = -1, from = -1;
	getFromAndN(curNode - nodes / 2, nodes / 2 - 3, length, n, from);
	from += L;
	//cerr << "step 3: node " << curNode << " will process [" << from << ", " << from + n - 1 << "]" << endl;
	
	int sum = getSumChecked(from, from + n - 1);
	//cerr << "step 3: node " << curNode << " processed [" << from << ", " << from + n - 1 << "] and got sum = " << sum << endl;
	putInt(nodes - 2, sum);
	putInt(nodes - 2, from);
	putInt(nodes - 2, from + n - 1);
	send(nodes - 2);
}

void step4(const int nodes, const int curNode) {
	if (curNode != nodes - 2) throw 0;
	
	int L = -1;
	int R = -1;
	
	int sum = 0;
	for (int node = nodes / 2; node < nodes - 3; node++) {
		receive(node);
		int thatSum = getInt(node);
		int thatL = getInt(node);
		int thatR = getInt(node);
		//cerr << "step 4: node " << curNode << " got sum = " << thatSum << ", L = " << thatL << ", R = " << thatR << " from node " << node << endl;
		if (thatSum == -1) {
			L = thatL;
			R = thatR;
		} else {
			sum += thatSum;
		}
	}
	//cerr << "step 4: node " << curNode << " will use L = " << L << ", R = " << R << " for calculating badIdx" << endl;
		
	int badIdx = findBad(L, R);
	//cerr << "step 4: node " << curNode << " calculated badIdx = " << badIdx << endl;
	putInt(nodes - 1, sum);
	putInt(nodes - 1, badIdx);
	putInt(nodes - 1, L);
	putInt(nodes - 1, R);
	send(nodes - 1);
}

void step5(const int nodes, const int curNode) {
	if (curNode != nodes - 1) throw 0;

	receive(nodes - 3);
	int sum1 = getInt(nodes - 3);
	//cerr << "step 5: node " << curNode << " got sum = " << sum1 << " from node " << nodes - 3 << endl;
	
	receive(nodes - 2);
	int sum2 = getInt(nodes - 2);
	int badIdx = getInt(nodes - 2);
	int L = getInt(nodes - 2);
	int R = getInt(nodes - 2);
	//cerr << "step 5: node " << curNode << " got sum = " << sum2 << " from node " << nodes - 2 << endl;
	//cerr << "step 5: node " << curNode << " got L = " << L << ", R = " << R << ", badIdx = " << badIdx << " from node " << nodes - 2 << endl;

	int sum = sum1 + sum2;
	solveEasyKnowingBadIdx(L, R, badIdx, sum);
}

void solve(const int nodes, const int curNode) {
	if (curNode < nodes / 2) {
		step1(nodes, curNode);
	}

	if (curNode == nodes - 3) {
		step2(nodes, curNode);
	}
	
	if (nodes / 2 <= curNode && curNode < nodes - 3) {
		step3(nodes, curNode);
	}
	
	if (curNode == nodes - 2) {
		step4(nodes, curNode);
	}
	
	if (curNode == nodes - 1) {
		step5(nodes, curNode);
	}
}
