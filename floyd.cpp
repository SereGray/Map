#include</usr/include/c++/8/iostream>
#include</usr/include/c++/8/vector>
#include</usr/include/c++/8/cmath>
#include<algorithm>

using namespace std;

int main(){	
	vector<vector<int>> map = { {0,0,0},{0,0,0},{0,0,0}};
	int n = map.size()*map[0].size();
	vector<vector<int>> matrix,parent;
	// create matrix
	for(int i=0;i<map.size()*map[0].size();++i){
		vector<int> v;
		for(int j=0;j<map.size()*map[0].size();++j){
			v.push_back(pow(2,15));
		}
		matrix.push_back(v);
	}
	for(int i=0;i<map.size()*map[0].size();++i)matrix[i][i]=0;
	// set matrix
	int x=0,y=0,vertexNum=0;
	for(vector<int> horiz: map){
		x=0;
		for(int i: horiz){
			if(x < map[0].size()-1){
				matrix[vertexNum][vertexNum+1]=1;
				matrix[vertexNum+1][vertexNum]=1;
			}
			if(y < map.size()-1){
				matrix[vertexNum][vertexNum+map[0].size()]=1;
				matrix[vertexNum+map[0].size()][vertexNum]=1;
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
	for(int i=0; i<n;++i){
		vector<int> p;
		for(int j=0;j<n;++j)p.push_back(i);
		parent.push_back(p);
	}
	// use alg floyd-warshall
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j){
				if(matrix[i][j]>matrix[i][k] + matrix[k][j]){
					matrix[i][j]= matrix[i][k] + matrix[k][j];
					parent[i][j]=k;
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
	cout<< "parents matrix:"<<endl;
	for(vector<int> v: parent){
		for(int i: v){
			if(i<pow(2,30)){
				cout << i << " ";
			}else cout << "x"<<" ";
		}
		cout << endl;
	}
	int s=0,e=0;
	cout<<"enter start and end Vertex Number between 0 and "<<n-1<< endl;
	cin >> s >> e;
	while(s >= n || e >= n || s <0 || e < 0){
		cout << "input err, try again ..."<<endl;
		cin >> s >> e;
	}
	int j=e;
	vector<int> path;
	// recover path to changed vertex
	do{
		path.push_back(j);
		j=parent[s][j];
	}while(j!=parent[s][j]);
	path.push_back(s);
	reverse(path.begin(),path.end());
	//print path
	for(int i: path) cout << i << " ";
	return 0;
}
