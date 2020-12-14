#pragma once
#include<unordered_set>
#include<utility>
#include<vector>

class Node
{
private:
	unsigned Num_;
	std::pair<int, int> CoordXY_;
	std::vector<int> 

};


class Path_Finder_Astar
{
	private:
		std::unordered_set<Node> Fired_Vertex_; // набор горящих вершин
		std::unordered_set<Node> Used_Vertex_; // набор вершин которые мы уже посмотрели
		std::vector<std::vector<int>> Adjacency_List_;
public:
	Path_Finder_Astar(std::vector<std::vector<int>>& adjacency_List) : Adjacency_List = adjacency_List{};

};

