#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<map>
#include<queue>

using namespace std;

vector<int> Djekstra(int, int, vector<vector<int>> &);
void recoveryPath(int, int, vector<vector<int>>&, vector<int>&);

int main() {
	cout << " 0  1  2  3  4 \n 5  6  7  8  9 \n 10 11 12 13 14 \n 15 16 17 18 19 \n 20 21 22 23 24 \n";
	vector<vector<int>> mapa = { {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0} };
	int n = mapa.size() * mapa[0].size();
	vector<vector<int>> matrix, parent, listAdjacent;
	// create matrix
	for (unsigned i = 0; i < mapa.size() * mapa[0].size(); ++i) {
		vector<int> v;
		for (unsigned j = 0; j < mapa.size() * mapa[0].size(); ++j) {
			v.push_back((int)pow(2, 15));
		}
		matrix.push_back(v);
	}
	for (unsigned i = 0; i < mapa[0].size() * mapa.size(); ++i) listAdjacent.push_back({});
	for (unsigned i = 0; i < mapa.size() * mapa[0].size(); ++i)matrix[i][i] = 0;
	// set matrix
	unsigned x = 0, y = 0, vertexNum = 0;
	for (vector<int> horiz : mapa) {
		x = 0;
		for (int i : horiz) {
			if (x < mapa[0].size() - 1) {
				matrix[vertexNum][vertexNum + 1] = 1;
				matrix[vertexNum + 1][vertexNum] = 1;
				listAdjacent[vertexNum].push_back(vertexNum + 1);
				listAdjacent[vertexNum + 1].push_back(vertexNum);
			}
			if (y < mapa.size() - 1) {
				matrix[vertexNum][vertexNum + mapa[0].size()] = 1;
				matrix[vertexNum + mapa[0].size()][vertexNum] = 1;
				listAdjacent[vertexNum].push_back(vertexNum + +mapa[0].size());
				listAdjacent[vertexNum + +mapa[0].size()].push_back(vertexNum);
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
	// print listAdjacent
	/*for (auto v : listAdjacent) {
		for (int i : v) {
			cout << i << " ";
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
	//cout << "parents matrix:" << endl;
	/*for (vector<int> v : parent) {
		for (int i : v) {
			if (i < pow(2, 30)) {
				cout << i << " ";
			}
			else cout << "x" << " ";
		}
		cout << endl;
	}*/
	int s = 0, e = 0;
	cout << "enter start and end Vertex Number between 0 and " << n - 1 << endl;
	cin >> s >> e;
	/*bool revers = false; // not need for Djektra alg TODO: replace this mehanism
	if (e < s) {  // switch at place
		e += s;
		s = e - s;
		e -= s;
		revers = true;
	}*/
	while (s >= n || e >= n || s < 0 || e < 0) {
		cout << "input err, try again ..." << endl;
		cin >> s >> e;
	}
	//int j = e;
	vector<int> path;
	// recover path to changed vertex
	recoveryPath(s, e, parent, path);
	/*do {
		path.push_back(j);
		j = parent[s][j];
	} while (j != parent[s][j]);
	*/
	path.push_back(e);
	//if(!revers)reverse(path.begin(), path.end());
	//print path
	cout << "Floyd - Warshall path ->";
	for (int i : path) cout << i << " ";
	// TODO: insert A_Star() here
	path.clear();
	path= Djekstra(s, e, listAdjacent);
	cout << "\nDjeckstra path ->";
	for (int i : path) cout << i << " ";
	return 0;
}

void recoveryPath(int a, int b, vector<vector<int>> & parent, vector<int> & path){
	if (parent[a][b] == a) {
		path.push_back(a);
	}
	else {
		recoveryPath(a, parent[a][b], parent, path);
		recoveryPath(parent[a][b], b, parent, path);
	}
}

vector<int> Djekstra(int start, int goal, vector<vector<int>>& matrix) {
	vector < pair<int, int>> frontier;  // TODO: wrapper class to implement good priorityQueue with lambda
	frontier.push_back(make_pair(start,0));
	map<int, int> came_from, cost_so_far;
	const int default_cost = 1;
	came_from[start] = start;
	cost_so_far[start] = 0;
	while (!frontier.empty()) {
		sort(frontier.begin(), frontier.end(), [](pair<int, int> lpair, pair<int, int> rpair) { return lpair.second > rpair.second; }); //lambda
		pair<int, int> current = frontier.back();
		frontier.pop_back();
		if (current.first == goal)
			break;
		for (int next : matrix[current.first]) {
			if (cost_so_far.find(next) == cost_so_far.end() || (cost_so_far[current.first] + default_cost) < cost_so_far[next]) {
				cost_so_far[next] = cost_so_far[current.first] + 1;
				frontier.push_back(make_pair(next, cost_so_far[next]));
				sort(frontier.begin(), frontier.end(), [](pair<int, int> lpair, pair<int, int> rpair) { return lpair.second > rpair.second; });
				came_from[next] = current.first;
			}
		}
	}
	//TODO: return Path
	vector<int> path;
	path.push_back(goal);
	while (goal != start) {
		goal = came_from[goal];
		path.push_back(goal);
	}
	reverse(path.begin(), path.end());
	return path;
}