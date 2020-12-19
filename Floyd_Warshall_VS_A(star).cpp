#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>

using namespace std;

int main() {
	cout << " 0 1 2 \n 3 4 5 \n 6 7 8 \n";
	vector<vector<int>> map = { {0,0,0},{0,0,0},{0,0,0} };
	unsigned n = map.size() * map[0].size();
	vector<vector<int>> matrix, parent;
	// create matrix
	for (unsigned i = 0; i < map.size() * map[0].size(); ++i) {
		vector<int> v;
		for (unsigned j = 0; j < map.size() * map[0].size(); ++j) {
			v.push_back(UINT32_MAX);
		}
		matrix.push_back(v);
	}
	for (unsigned i = 0; i < map.size() * map[0].size(); ++i)matrix[i][i] = 0;
	// set matrix
	unsigned x = 0, y = 0, vertexNum = 0;
	for (vector<int> horiz : map) {
		x = 0;
		for (int i : horiz) {
			if (x < map[0].size() - 1) {
				matrix[vertexNum][vertexNum + 1] = 1;
				matrix[vertexNum + 1][vertexNum] = 1;
			}
			if (y < map.size() - 1) {
				matrix[vertexNum][vertexNum + map[0].size()] = 1;
				matrix[vertexNum + map[0].size()][vertexNum] = 1;
			}
			++x;
			++vertexNum;
		}
		++y;
	}
	// print matrix
	/*for(vector<int> v: matrix){
		for(int i: v){
			if(i<pow(2,15)){
				cout << i << " ";
			}else cout << "x"<<" ";
		}
		cout << endl;
	}*/
	// SET parent matrix to parent[i][j]=i
	for (int i = 0; i < n; ++i) {
		vector<int> p;
		for (int j = 0; j < n; ++j)p.push_back(i);
		parent.push_back(p);
	}
	// use alg floyd-warshall
	for (int k = 0; k < n; ++k)
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j) {
				if (matrix[i][j] > matrix[i][k] + matrix[k][j]) {
					matrix[i][j] = matrix[i][k] + matrix[k][j];
					parent[i][j] = k;
				}
			}
	// print matrix
	/*cout << "after floud-warshall algorithm"<<endl;
	for(vector<int> v: matrix){
		for(int i: v){
			if(i<pow(2,30)){
				cout << i << " ";
			}else cout << "x"<<" ";
		}
		cout << endl;
	}*/
	//print parents
	cout << "parents matrix:" << endl;
	for (vector<int> v : parent) {
		for (int i : v) {
			if (i < pow(2, 30)) {
				cout << i << " ";
			}
			else cout << "x" << " ";
		}
		cout << endl;
	}
	int s = 0, e = 0;
	cout << "enter start and end Vertex Number between 0 and " << n - 1 << endl;
	cin >> s >> e;
	bool revers = false;
	if (e < s) {  // switch at place
		e += s;
		s = e - s;
		e -= s;
		revers = true;
	}
	while (s >= n || e >= n || s < 0 || e < 0) {
		cout << "input err, try again ..." << endl;
		cin >> s >> e;
	}
	int j = e;
	vector<int> path;
	// recover path to changed vertex
	do {
		path.push_back(j);
		j = parent[s][j];
	} while (j != parent[s][j]);
	path.push_back(s);
	if(!revers)reverse(path.begin(), path.end());
	//print path
	for (int i : path) cout << i << " ";
	return 0;
}