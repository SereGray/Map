// This file is part of game engine TurnBasedGame
#include <iostream>
#include "CImg.h"
#include <vector>

using namespace std;

class point{
	uint32_t x,y; //   TODO: не инициализированны
	public:
		vector<uint32_t> smej; // смежные точки 
		vector<uint32_t> list_neighbor; // смежные точки принадлежащие другим игровым объектам                        NULL 
		bool border_map;
		uint16_t N_owner;
	point(){
	border_map=false;
	N_owner=0;
	}

	// получение координат вершины по номеру
	pair<uint32_t,uint32_t> getCoord(){
		return std::make_pair(x,y);
	}

	void setX(uint32_t X){
		x=X;
	}

	void setY(uint32_t Y){
		y=Y;
	}

};

class terrain{ //  клас предсавляющий изображение на карте территорию королевства и методы работы:
	public:

		uint32_t N;
		vector<uint32_t> list_v; // список вершин
		vector<uint32_t> borders; // список границ 
		// создание экземпляра из первой точки
		terrain(uint32_t num,uint32_t n){
			N=n;
			list_v.push_back(num);
			borders.push_back(num);
		}
		uint32_t my_N(){
			return N;
		}
};

class map{
	//friend class terrain;
	private:
		uint32_t width,height;
		vector<pair<uint32_t,uint32_t>> points;//TODO: not used ?
	public:
		 vector<point> tabSmej; // таблица смежности представляет из себя список всех вершин
		 vector<terrain> list_terrains;
	private:
		//  генерирует вектор координат ( ВНИМАНИЕ  повторяющихся)
void GenerateCoord(uint32_t p){
	for(uint32_t i=0;i<p;i++){
		int32_t cx=0,cy=0;
		cx=rand()%width;
		cy=rand()%height;
		cout<< "cx="<<cx<<" cy="<<cy<<endl;
		points.push_back(std::make_pair(cx,cy));
	}
}

uint32_t getNum(uint32_t x, uint32_t y){// получение номера вершины по координатам
	return x+y*width;
}

void GenerateTab(){
	uint32_t max=height*width; // maby uint64_t
	uint32_t w=0,h=0;
	point pNull;
	// заполняем таблицу нулевыми точками
	for(uint32_t i=0;i<max;++i){
		tabSmej.push_back(pNull);
	}
	// заполняем таблицу смежности
for(uint32_t i=0;i<max;++i){
	tabSmej[i].setX(h);
	tabSmej[i].setY(w);	
	// просматриваю таблицу вправо вниз добавляю 
	// к текущей точке следущую смежную и к следующей текущую
	// проверка правой границы
	if(w<width-1){
		tabSmej[i].smej.push_back(i+1);
		tabSmej[i+1].smej.push_back(i); 
	}
	// нижней границы
	if(h<height-1){
		tabSmej[i].smej.push_back(i+width);
		tabSmej[i+width].smej.push_back(i);
	}
	// опредление координаты на карте
	++w;
	if(w==width){
	w=0;
	++h;
	}
	}
}

// TODO: что будет если 2 начальых точки соседи ?(ничего?!?!!)	
// // генерация и добавление начальных точек к карте
void AddPoitsToMap( uint32_t po){ // ро - количество стартовых точек
	if(po>height*width) return;
	while(po>0){
		uint32_t x=rand()%width;
		uint32_t y=rand()%height;
		if(tabSmej[x+y*width].N_owner==0){
			tabSmej[getNum(x,y)].N_owner=po;
		}else{
			while(tabSmej[getNum(x,y)].N_owner!=0){
		x=rand()%width;
		y=rand()%height;
			}
			tabSmej[getNum(x,y)].N_owner=po;
		}
		terrain newKingdoom(getNum(x,y),po);
		cout<<" new kingd n="<<newKingdoom.my_N()<<endl;
		list_terrains.push_back(newKingdoom);
		--po;
	}
}

// обновление границ (решение влоб)
void RefreshBorders(terrain & terr){
	terr.borders.clear();
	for(auto numV: terr.list_v){// обходим все вершины королевства по номерам и пров
		//  условию границы  (список точек принадлежащ соседям не пуст или соседняя 
		//  точка никому не принадлежит 

	//  получаю вершину смотрю список смежных  и владельца
		// цикл проверяет соседние точки если соседняя точка не моя то значит проверяемая точка - гранинкая
		for (auto smej_V : tabSmej[numV].smej) {
			if (tabSmej[smej_V].N_owner != terr.my_N()) {
				terr.borders.push_back(numV);
				break; //  эта вершина граничная  выходим
			}
		}
	}
}

// вывод на экран карты
void MapToScreen(){
		// проходим по всем вершинам и форматируем в виде таблицы heigth x width
uint32_t k=0;
	for(uint32_t j=0;j<height;++j){
		for(uint32_t i=0;i<width;++i){
			cout << tabSmej[k].N_owner<<" . ";
			++k;
		}
		cout<<endl;
	}
}

// функц вывода карты в файл с помощью CImg.h
// TODO: this
void MapToFile() {
	using namespace cimg_library;
	//CImg img;

}

bool freeSpace(){
	static uint32_t maxIteration=100;
	if(--maxIteration==0)return false;
	for(point p: tabSmej){
		if(p.N_owner==0) return true;
	}
	return false;
}

void DjekstraPath(uint32_t numBorderV,uint32_t numTargetV, vector<uint32_t> &path){
	//считается что все вершины доступны иначе добавить вес ребра = бесконечности или др. промеж. варианты
uint32_t n=tabSmej.size();vector<uint32_t> dist(n, UINT32_MAX), parent(n);
dist[numBorderV] = 0; // // стартовая вершина
vector<bool> used(n);
for (uint32_t i = 0; i < n; ++i) {
	int64_t vertex = -1;
	for (uint32_t j = 0; j < n; ++j)
		if (!used[j] && (vertex == -1 || dist[j] < dist[vertex]))
			vertex = j;
		if (dist[vertex] == UINT32_MAX)
		break;
		used[vertex] = true;
		for (size_t j = 0; j < tabSmej[vertex].smej.size(); ++j) {
			uint32_t to = tabSmej[vertex].smej[j];
			const uint32_t len =1; // вес ребра
			if (dist[vertex] + len < dist[to]) {
				dist[to] = dist[vertex] + len;
				parent[to] = vertex;
			}
		}
	}
	//recover path to target from parent vector
	vector<uint32_t> tempPath;
	uint32_t currV=numTargetV;
	//пока не достигнем стартовой вершины 
	while(parent[currV]!=numBorderV){
		//восстанавливаем предка
		tempPath.push_back(currV);
		currV = parent[currV];
	}
	// add start vertex
	tempPath.push_back(numBorderV);
	for(uint32_t i=0;i<tempPath.size();++i){
		uint32_t temp= tempPath.back();
		path.push_back(temp);
		tempPath.pop_back();
	}

}

vector<vector<uint32_t>> createDxDTable(){
	
}

void Floid_Yorshell(vector<uint32_t> path){
	//create matrix nxn smej from tabSmej
	//TODO: smejDxD
	vector<vector<uint32_t>> smejDxD=createDxDTable();
	uint32_t n = smejDxD.size();
	for(uint32_t k=0;k<n;++k)
		for(uint32_t i=0;i<n;++i)
			for(uint32_t j=0;j<n;++j)
				smejDxD[i][j] = min(smejDxD[i][j], smejDxD[i][k]+smejDxD[k][k]);
}
	
void BalanceArea() {
	while (terrainsDisbalanced(1)) {
		std::sort(list_terrains.begin(), list_terrains.end(), [](terrain lkdm, terrain rkdm) { return lkdm.list_v.size() < rkdm.list_v.size(); });
		vector<terrain>::iterator kingdIterator = list_terrains.begin();
		while (kingdIterator != (list_terrains.end() - 1)) {
			terrain kingd = *kingdIterator;
			++kingdIterator;
			for (auto numBorderV : kingd.borders) {//any vertex from nim terrain
			// далее по алгоритму декстры ищем путь к наибольшей терр
			// по алгоритму флойда-уоршелла ищем все пути и выбираем наикороткий
			vector<uint32_t> path;
			//DjekstraPath();
			// TODO: break ;	
			}
		}
	}
}

bool terrainsDisbalanced(uint16_t offset){ // offset - допуск на равенство 
	uint16_t max=list_terrains[0].list_v.size();
	for(auto terr : list_terrains){
		if(max < terr.list_v.size())max=terr.list_v.size();
	}
	uint16_t min=list_terrains[0].list_v.size();
	for(auto terr : list_terrains){
		if(min > terr.list_v.size())min=terr.list_v.size();
	}
	if((max-min)>offset) return true;
	return false;
}
		
terrain get_minTerrain(){
	uint32_t min = 0 - 1;
	terrain res = list_terrains[0];
	for(auto terr : list_terrains){
		if(terr.list_v.size() < min) {
		       	min = terr.list_v.size();
			res = terr;
		}
	}
	return res;
}

void FillMap(){
	vector<uint32_t> iterOnBorders;		// список текущего положения итератора перебора 
					//по пограничным вершинам для всех королевств ( массив итераторов по одному на королевство)
	for(uint32_t i=0;i< list_terrains.size();++i) iterOnBorders.push_back(0);  //  установка начального значения итератора на 0

	while(freeSpace()){// пока свободные клетки не закончатся
	
	//1)обход окружности точек
	//добавление незанятых (окрашивание)
	//2)определение новых границ
	
	//Обход
		for(auto &kingd: list_terrains){
			// движение по окружности границы по их порядку начиная с правой
			if (iterOnBorders[kingd.my_N() - 1] >= kingd.borders.size()) {
				iterOnBorders[kingd.my_N() - 1] = 0;  // если итератор вышел за 
										//"границы королевства" то возвращаем на стартовую поз
			}
			//  если заграничная точка ничья то присваиваем (только 1)
			//  далее прохожу по границе numV - номер заграничной вершины(точки)
			// двигаюсь по списку смежности - по смежным вершинам вершины "tabSmej[kingd.borders[iterOnBorders[i]]]"
			for(uint32_t numV: tabSmej[kingd.borders[iterOnBorders[kingd.my_N() - 1]]].smej){
				if(tabSmej[numV].N_owner==0){
					tabSmej[numV].N_owner=kingd.my_N();
					kingd.list_v.push_back(numV);
					break; // quit if ok
				}
			}
			++iterOnBorders[kingd.my_N() - 1]; 	 // перемещаем итератор
		}	
		for(auto & kingd : list_terrains) RefreshBorders(kingd);
	}
	
	// выравниваем площадь
	//BalanceArea();
	
}
	public:
		map(uint32_t w,uint32_t h, uint32_t p): width(w), height(h){
			// создаем таблицу списков смежности
			cout<<" gen tab\n";
			GenerateTab();
			cout<<"map to scre\n";
			AddPoitsToMap(p); 
			cout<<"poins scre\n";
			MapToScreen();
			// определяем положения точек
		//	GenerateCoord(p);
			// заполняем территорию карты
			FillMap();//TODO: infinity loop there !!!!
			cout << endl;
			MapToScreen();
		}
		void PrintTabSmej(){
			uint32_t i=0;
			for(point p:tabSmej){
				cout << i <<" num smej:"<< p.smej.size() << endl;
				++i;
				for(uint32_t v: p.smej){
				cout << v <<" " ;
				}
			cout << endl;
			}
		}
};


int main(){
	cout<< " test "<< endl;
	map m(5,5,3);
//	m.PrintTabSmej();


}
