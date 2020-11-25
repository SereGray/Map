#include <iostream>
#include "CImg.h"
#include <vector>

using namespace std;

class point{
	uint32_t x,y; //   TODO: не инициализированны
	public:
		vector<uint32_t> smej; // смежные точки 
		vector<uint32_t> list_neighbor; // смежные точки принадлежащие другим игровым объектам
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

class kingdoom{ //  клас struct? предсавляющий изображение на карте территорию королевства и методы работы:
	public:
		uint32_t N;
		vector<uint32_t> list_v; // список вершин
		vector<uint32_t> borders; // список границ 
		// создание экземпляра из первой точки
		kingdoom(uint32_t num,uint32_t n){
			N=n;
			list_v.push_back(num);
			borders.push_back(num);
		}
		uint32_t my_N(){
			return N;
		}
	};

class map{
	private:
		uint32_t width,height;
		vector<pair<uint32_t,uint32_t>> points;//TODO: not used ?
		vector<point> tabSmej; // таблица смежности представляет из себя список всех вершин
		vector<kingdoom> list_kingdooms; // список королевств

	public:
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
	uint64_t max=height*width;
	uint32_t w=0,h=0;
	point pNull;
	// заполняем таблицу нулевыми точками
	for(uint64_t i=0;i<max;++i){
		tabSmej.push_back(pNull);
	}
	// заполняем таблицу смежности
for(uint64_t i=0;i<max;++i){
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
		kingdoom newKingdoom(getNum(x,y),po);
		cout<<" new kingd n="<<newKingdoom.my_N()<<endl;
		list_kingdooms.push_back(newKingdoom);
		--po;
	}
}

// обновление границ (решение влоб)
void RefreshBorders(kingdoom kingd){
	kingd.borders.clear();
	for(auto numV: kingd.list_v){// обходим все вершины королевства по номерам и пров
		//  условию границы  (список точек принадлежащ соседям не пуст или соседняя 
		//  точка никому не принадлежит 
	//TODO: test this
	//  получаю вершину смотрю список соседей  и владельца
		if(tabSmej[numV].list_neighbor.size()>0 || tabSmej[numV].N_owner != 0){
			// добавить к списку границ
				kingd.borders.push_back(numV);
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

void FillMap(){
uint32_t i=0;				// счетчик королевств
vector<uint32_t> iterOnBorders;		// список из текущего положения итератора перебора 
					// границ королевств
for(uint32_t i=0;i< list_kingdooms.size();++i) iterOnBorders.push_back(0);
while(freeSpace()){// пока свободные клетки не закончатся
	
	//1)обход окружности точек
	//добавление незанятых (окрашивание)
	//2)определение новых границ
	
	//Обход
	for(auto kingd: list_kingdooms){
		cout<<kingd.my_N()<<" for started loop ";
		// движение по окружности границы по их порядку начиная с правой
		if(iterOnBorders[i]>=kingd.borders.size())iterOnBorders[i]=0;
		//  если заграничная точка ничья то присваиваем (только 1)
		//  далее прохожу по границе numV - номер заграничной вершины(точки)
		cout<<tabSmej[kingd.borders[iterOnBorders[i]]].list_neighbor.size()<<" <-size smej list";
		for(uint32_t numV: tabSmej[kingd.borders[iterOnBorders[i]]].smej){
			cout <<" "<< numV;
			if(tabSmej[numV].N_owner==0){
				tabSmej[numV].N_owner=kingd.my_N();
				cout<<"added point N="<< numV << " to kingdoom N="<<kingd.my_N()<<endl;
				break; // quit if ok
			}
		}
		cout<<" done loop kingdoom , i="<<i<<endl;
		++i;
	}	
for(uint32_t i=0;i< this->list_kingdooms.size();++i) ++iterOnBorders[i];// перемещаем итератор
cout << " refresh borders"<<endl;
// обновление границ TODO: check there !
for(auto kingd : list_kingdooms) RefreshBorders(kingd);
}
			//Если площади областей не равны то
			//сортируем 
			//цикл пока не равны
			//берем самую маленькую:w
			//площадь(выбираем точку на гранце)
			//строим цепочку до самой большой
			//тянем к себе точки
			//
			

}
	public:
		vector<uint32_t> list_smej;
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