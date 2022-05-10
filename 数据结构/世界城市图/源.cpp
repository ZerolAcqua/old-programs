//添加头文件
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include<stdio.h>
#include <windows.h>//
#include<string.h>
#include <vector>
#include <math.h>
#include <sstream>
#include<iostream>
#include <shellapi.h>

int count;
using namespace std;

//预定义常量和变量类型，注意有无分号
#define OK 1
#define ERROR 0
#define MAX_SIZE 50
#define INCREASE_SIZE 50
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

//交通方式的宏定义
#define PLANE 1
#define TRAIN 2
#define BUS 3
#define BOAT 4
//对最短路径模式的宏定义
#define TIME 1
#define COST 2
//Dijkstra中的“无穷大”
#ifndef INFINITY 
#define INFINITY 1000000
#endif

typedef char ElemType;//图
typedef int SElemType;//栈
typedef int Status;

//////栈的实现//////
struct SqStack {
	SElemType* base;// 存储空间基址 
	SElemType* top;// 栈顶指针 
	int stacksize;// 当前可使用的最大存储空间，以元素为单位
};

//初始化顺序栈S，将其置为空栈
Status InitStack(SqStack& S)
{
	S.stacksize = 0;
	S.top = S.base = NULL;
	S.base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (S.base == NULL)
		return ERROR;		//分配内存失败
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}

//销毁顺序栈S
Status DestroyStack(SqStack& S)
{
	if (S.base == NULL)
	{
		return ERROR;	//是否是有效的顺序栈
	}
	free(S.base);
	S.base = NULL;
	S.top = NULL;
	S.stacksize = 0;
	return OK;
}

//将顺序栈置为空栈
Status ClearStack(SqStack& S)
{
	if (S.base == NULL)
	{
		return ERROR;	//是否是有效的顺序栈
	}
	S.top = S.base;
	return OK;
}

//判断是否为空栈，若是空栈返回true，否则返回false(包括不存在的栈)
bool StackEmpty(SqStack S)
{
	if (S.base == S.top && S.base != NULL)
	{
		return true;
	}
	else return false;
}

//返回栈中元素的个数(必须是存在的栈！)
int StackLength(SqStack S)
{
	return S.top - S.base;
}

//用e返回栈顶部的元素
Status GetTop(SqStack S, SElemType& e)
{
	if (S.base == NULL)
	{
		return ERROR;	//是否是存在的栈
	}
	e = *(S.top - 1);
	return OK;
}

//将e压入栈顶，若栈满则扩充栈
Status Push(SqStack& S, SElemType e)
{
	if (S.base == NULL)
		return ERROR;	//是否是存在的栈
	if (S.top - S.base == S.stacksize)
	{
		//增加分配的内存扩大栈的大小
		S.base = (SElemType*)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
		if (S.base == NULL)
			return ERROR;	//分配失败
		S.top = S.base + S.stacksize;	//防止realloc改变基址的影响
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return OK;
}

//出栈，将值返回给e
Status Pop(SqStack& S, SElemType& e)
{
	if (S.base == NULL || S.base == S.top)
		return ERROR;	//栈为空或不存在
	e = *--S.top;
	return OK;
}



//////图的实现//////

//路线结点
typedef struct RouteNode
{
	int nextcity = 0;						//目的地城市
	float cost = 0;							//花费
	float time = 0;							//耗时
	int trans = 0;							//交通方式  PLANE 1  TRAIN 2  BUS 3  BOAT 4
	string info;							//其他信息
	struct RouteNode* nextRoute = NULL;		//下一条路线
}RouteNode;
//城市结点
typedef struct City
{
	bool check = false;						//遍历标志
	double latitude, longitude;				//纬度、经度
	string country, city;					//国家（州）、城市
	RouteNode* firstroute = NULL;			//第一条路线
}City, * CityList;
//图结构
typedef struct
{
	int size;
	CityList list = NULL;					//邻接表
	int citynum = 0, routenum = 0;			//城市数目、路径数目
}CityGraph;


vector<int> tempcity;
vector<RouteNode*> temproute;


//创建一个邻接表来表示图，输入图的引用
Status CreateGraph(CityGraph& G)
{
	G.citynum = 0;
	G.routenum = 0;
	G.list =new City[MAX_SIZE];	
	G.size = MAX_SIZE;						//开辟内存
	if (G.list == NULL)
		return ERROR;
	return OK;
}

//输入出发城市和路线，进行该城市对应的表的延长
Status addlist(City& tail, RouteNode* add)
{
	RouteNode* p = tail.firstroute;			
	if (p == NULL)
		tail.firstroute = add;				//若无路线，则直接添加
	else									//否则找到最后一个结点
	{
		for (; p->nextRoute != NULL; p = p->nextRoute)
		{
			;
		}
		p->nextRoute = add;
	}
	return OK;
}

//找到城市在list中的位置
int findcity(CityGraph G, string& city)
{
	for (int i = 0; i < G.citynum; i++)
	{
		if (G.list[i].city == city)
			return i;
	}
	return -1;
}


//交通工具string和int值转换
int gettrans(string trans)
{
	if (trans == "plane")
		return PLANE;
	else if (trans == "train")
		return TRAIN;
	else if (trans == "bus")
		return BUS;
	else  if (trans == "boat")
		return BOAT;
	else
		return 0;
}
const char* gettransname(int trans)
{
	if (trans = BUS)
		return "bus";
	else if (trans == BOAT)
		return "boat";
	else if (trans == TRAIN)
		return "train";
	else if (trans == PLANE)
		return "plane";
}

//扩大内存
Status renew(CityGraph& G)
{
	int i = 0;
	CityList p = new City[G.size + INCREASE_SIZE];
	if (p == NULL)
		return ERROR;
	for (i = 0; i < G.citynum; i++)
	{
		p[i].latitude = G.list[i].latitude;
		p[i].longitude = G.list[i].longitude;
		p[i].country = G.list[i].country;
		p[i].city = G.list[i].city;
	}
	//memcpy(p,G.list,G.list*(sizeof(City))) 会报错
	delete[]G.list;
	G.size += INCREASE_SIZE;
	G.list = p;
	return OK;
}

//获取两点之间符合要求的路径
RouteNode* getRoute(CityGraph& G, int tail, int head, int mode)
{
	RouteNode* temp = NULL;
	RouteNode* p;
	//时间上最短的弧
	if (mode == TIME)
	{
		for (p = G.list[tail].firstroute; p != NULL; p = p->nextRoute)
		{
			if (p->nextcity == head)
			{
				temp = p;
				break;
			}
		}
		for (p = p->nextRoute; p != NULL; p = p->nextRoute)
		{
			if (p->nextcity == head)
			{
				if (temp->time > p->time)
					temp = p;
			}
		}
	}
	//花费上最少弧
	else if (mode == COST)
	{
		for (p = G.list[tail].firstroute; p != NULL; p = p->nextRoute)
		{
			if (p->nextcity == head)
			{
				temp = p;
				break;
			}
		}
		for (p = p->nextRoute; p != NULL; p = p->nextRoute)
		{
			if (p->nextcity == head)
			{
				if (temp->cost > p->cost)
					temp = p;
			}
		}
	}
	return temp;
}

//按所给的出发城市和目的地城市，以及相关信息构造邻接表
Status BuildGraph(CityGraph& G, string& head, string& tail, string& info, int trans, float time, float cost)
{
	int intail = 0, inhead = 0;				//存储城市的编号

	RouteNode* next = new RouteNode;
	if (next == NULL)
		return ERROR;

	next->nextRoute = NULL;
	next->cost = cost;
	next->time = time;
	next->trans = trans;
	next->nextcity = findcity(G, head);
	next->info = info;
											//构造路线结点
	intail = findcity(G, tail);
	addlist(G.list[intail], next);
											//添加到邻接表中
	return OK;
}

//将城市读取并存入图中
Status fileCity(CityGraph& G, ifstream& fcity)
{
	if (!fcity.is_open())					//文件是否打开
	{
		cout << "fail to open the file"<<endl;
		return ERROR;
	}
	int n = 0;
	int front, rear, length;				//用以记录逗号位置与之间的间隔
	string temp;							//用以暂时存储一行字符串
	for (G.citynum = n = 0; getline(fcity, temp); n++)
	{
		front = 0; rear = 0;
		rear = temp.find(',', front);		//找到逗号位置
		length = rear - front;				//计算间隔
		G.list[n].country = temp.substr(front, length);
											//截取字符串
		front = rear + 1;
		rear = temp.find(',', front);
		length = rear - front;
		G.list[n].city = temp.substr(front, length);
		front = rear + 1;
		rear = temp.find(',', front);
		length = rear - front;
		istringstream(temp.substr(front, length).c_str())>> G.list[n].latitude;
											//用以实现字符串直接转浮点数
		front = rear + 1;
		istringstream(temp.substr(front).c_str()) >> G.list[n].longitude;

		G.citynum++;						//记录城市数目

		if (G.citynum == G.size)
			if (!renew(G))
				return ERROR;
				
	}
	
	return OK;
}

//将线路读取并存入图中
Status fileroute(CityGraph& G, ifstream& froute)
{
	if (!froute.is_open())					//文件是否打开
	{
		cout << "fail to open the file" << endl;
		return ERROR;
	}
	int n = 0;
	int front, rear, length;				//用以记录逗号位置与之间的间隔
	string temp;							//用以暂时存储一行字符串
	string head, tail, info;				//起讫点城市,以及路信息
	int trans;
	float time;
	float cost;
	for (n = 0; getline(froute, temp); n++)
	{
		front = 0; rear = 0;
		rear = temp.find(',', front);		//找到逗号位置
		length = rear - front;				//计算间隔
		tail= temp.substr(front, length);	//截取字符串

		front = rear + 1;
		rear = temp.find(',', front);
		length = rear - front;
		head = temp.substr(front, length);

		front = rear + 1;
		rear = temp.find(',', front);
		length = rear - front;
		trans = gettrans(temp.substr(front, length));

		front = rear + 1;
		rear = temp.find(',', front);
		length = rear - front;
		istringstream(temp.substr(front, length).c_str()) >> time;
											//用以实现字符串直接转浮点数
		front = rear + 1;
		rear = temp.find(',', front);
		length = rear - front;
		istringstream(temp.substr(front, length).c_str()) >> cost;

		front = rear + 1;
		info = temp.substr(front);

		BuildGraph(G, head, tail, info, trans, time, cost);
											//构造邻接表
	}


	G.routenum = n;							//记录路线数目
	return OK;
}

//销毁图
Status destroy(CityGraph& G)
{
	int i = 0;
	RouteNode* p = NULL, * temp = NULL;
	for (i = 0; i < G.size; i++)
	{	
		for (p = G.list[i].firstroute; p != NULL; p = temp)
		{
			temp = p->nextRoute;
			delete p;
		}
	}
	delete[]G.list;							//对应new
	return true;
}

//DFS
Status DFS(CityGraph& G, int Node)
{
	if (G.list[Node].check == false)
	{
		cout << G.list[Node].city << endl;
		G.list[Node].check = true;
	}
	else
	{
		return OK;
	}
	RouteNode* p = G.list[Node].firstroute;
	for (; p != NULL; p = p->nextRoute)
	{
		DFS(G, p->nextcity);
	}
}

//深度遍历,从start对应的城市开始遍历
//若start取0，即为从第一个城市开始遍历
Status DFSTrasvers(CityGraph& G, int start)
{
	int i = 0;
	RouteNode* p = NULL;
	//将遍历的标志置为false
	for (i = 0; i < G.citynum; i++)
	{
		G.list[i].check = false;
	}

	//从start开始递归地遍历图
	DFS(G, start);

	//非连通图时的遍历出未进行DFS的节点，并进行DFS
	for (i = 0; i < G.citynum; i++)
	{
		if(G.list[i].check==false)
			DFS(G, i);
	}
	cout << endl;

	return OK;
}
//栈的方式
Status DFSTrasversEX(CityGraph& G, int start)
{
	int i = 0;
	int n = 0;
	RouteNode* p = NULL;
	//将遍历的标志置为false
	for (i = 0; i < G.citynum; i++)
	{
		G.list[i].check = false;
	}
	SqStack S;
	InitStack(S);
	Push(S, start);
	

	while (!StackEmpty(S))
	{
		Pop(S, n);
		if (G.list[n].check == false)
		{
			cout << G.list[n].city << endl;
			G.list[n].check = true;
		}

		p = G.list[n].firstroute;
		for (; p != NULL; p = p->nextRoute)
		{
			if (G.list[p->nextcity].check == false)
			{
				Push(S, p->nextcity);
			}
		}
	}
	//非连通图时的遍历出未进行DFS的节点，并进行DFS
	for (i = 0; i < G.citynum; i++)
	{
		if (G.list[i].check == false)
		{
			Push(S, i);
			while (!StackEmpty(S))
			{
				Pop(S, n);
				if (G.list[n].check == false)
				{
					cout << G.list[n].city << endl;
					G.list[n].check = true;
				}

				p = G.list[n].firstroute;
				for (; p != NULL; p = p->nextRoute)
				{
					if (G.list[p->nextcity].check == false)
					{
						Push(S, p->nextcity);
					}
				}
			}
		}
	}
	cout << endl;
	DestroyStack(S);
	return OK;
}

//最短路径——Dijkstra算法
Status ShortestPath(CityGraph G, string ori, string des, int mode)
{
	int i;
											
	int* parent = new int[G.citynum];		//path用于储存已知可能的最短路径
	float* path = new float[G.citynum];			//parent用以存储最短路线（的上一个城市）
	bool* finish = new bool[G.citynum];		//finish用于储存已经求得最短路径的城市(红蓝点集)
	//寻找起讫点对应的城市编号
	int start = findcity(G, ori);
	int end = findcity(G, des);
	
	if (start < 0 || end < 0||start==end)
		return ERROR;

	//初始化parent、finish和path
	for (i = 0; i < G.citynum; i++)
	{
		parent[i] = -1;
		finish[i] = false;
		path[i] = INFINITY;
	}
	

	RouteNode* p = NULL;
	int city = 0;							//要更新的路径城市
	int min = INFINITY;						//搜索临时记录最小值的变量

	
	if (mode == TIME)						//最短路径——时间最少
	{
		//将起点城市加入完成点集，置最短路径0
		finish[start] = true;
		path[start] = 0;

		city = start;

		while (true)
		{
			for (p = G.list[city].firstroute;
				p != NULL; p = p->nextRoute)
			{
				//更新已知路径和双亲（已知最短路径结点）
				//这里考虑到了两个城市间不同的交通方式，程序最终只会选择最小的，较大的会被覆盖掉
				//已有的到city最短路径+city到某城市的路径<到该城市的路径 			
				if (!finish[p->nextcity] &&
					path[city] + p->time < path[p->nextcity])
				{
					path[p->nextcity] = path[city] + p->time;
					parent[p->nextcity] = city;
				}
			}

			min = INFINITY;
			for (i = 0; i < G.citynum; i++)
			{
				//找到还未加入点集的最小的最短路径min和相应城市
				if (!finish[i] && path[i] < min)
				{
					city = i;
					min = path[i];
				}
			}
			//加入点集finish中
			if (min < INFINITY)
			{
				finish[city] = true;
				if (city == end)
				{
					break;					//说明已经找到了所求的最短路径
											//不需要再继续管其他的最短路径
				}
			}
			else
			{								//说明剩下的点都为INFINITY,即无法到达
				cout << "没有路径可到达";
				return OK;
			}
		}
	}
	else if (mode == COST)					//最短路径——花费最少
	{
		//将起点城市加入完成点集，置最短路径0
		finish[start] = true;
		path[start] = 0;

		city = start;

		while (true)
		{
			for (p = G.list[city].firstroute;
				p != NULL; p = p->nextRoute)
			{
				//更新已知路径和双亲（已知最短路径结点）
				//这里考虑到了两个城市间不同的交通方式，程序最终只会选择最小的
				//已有的到city最短路径+city到某城市的路径<到该城市的路径 			
				if (!finish[p->nextcity] &&
					path[city] + p->cost < path[p->nextcity])
				{
					path[p->nextcity] = path[city] + p->cost;
					parent[p->nextcity] = city;
				}
			}

			min = INFINITY;
			for (i = 0; i < G.citynum; i++)
			{
				//找到还未加入点集的最小的最短路径min和相应城市
				if (!finish[i] && path[i] < min)
				{
					city = i;
					min = path[i];
				}
			}
			//加入点集finish中
			if (min < INFINITY)
			{
				finish[city] = true;
				if (city == end)
				{
					break;					//说明已经找到了所求的最短路径
											//不需要再继续管其他的最短路径
				}
			}
			else
			{
				cout << "没有路径可到达";	//说明剩下的点都为INFINITY,即无法到达
				return OK;
			}
		}
	}
	else
		return ERROR;


	//路径存储输出部分
	tempcity.clear();
	int n;
	n = end;
	for (i = 0;n!=start; i++)
	{
		tempcity.insert(tempcity.begin(),n);
		n = parent[n];
	}
	tempcity.insert(tempcity.begin(), n);

	//存储路径的消息
	temproute.clear();
	for(i=0;i<tempcity.size()-1;i++)
	{
		temproute.push_back(getRoute(G, tempcity[i], tempcity[i+1], mode));
	}

	//打印
	if (mode == TIME)
		cout << "最短用时：" << path[end] << endl;
	else 
		cout << "最少花费：" << path[end] << endl;
	for (i = 0; i < tempcity.size(); i++)
	{
		cout << G.list[tempcity[i]].country << " "
			<< G.list[tempcity[i]].city << " "
			<< "->" << endl;
	}


	delete[] parent;
	delete[] path;
	return OK;
}

void visible(const char* file, CityGraph& G, string& ori, string& des, vector<int>& tempcity, vector<RouteNode*>&temproute)
{
	int i, j;
	FILE* fp3;
	fp3 = fopen("graph.htm", "w");
	if (fp3 == NULL)
	{
		cout << "error3!";
		return;
	}

	string s = "<!DOCTYPE html><html><head>\
		<style type='text/css'>body, html{width: 100%;height: 100%;margin:0;font-family:'微软雅黑';}#allmap{height:100%;width:100%;}#r-result{width:100%;}</style>\
		<script type='text/javascript' src='http://api.map.baidu.com/api?v=2.0&ak=nSxiPohfziUaCuONe4ViUP2N'></script>";

	fprintf(fp3, "%s<title>Shortest path from %s to %s</title></head><body>\
		<div id='allmap'></div></div></body></html><script type='text/javascript'>\
		var map = new BMap.Map('allmap');\
		var point = new BMap.Point(0, 0);\
		map.centerAndZoom(point, 2);\
		map.enableScrollWheelZoom(true);", s.c_str(), ori.c_str(), des.c_str());

	i = j = 0;

	int bycity, bycity2;

	for (bycity = 0; bycity < tempcity.size() - 1; )
	{
		fprintf(fp3, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\
			map.addOverlay(marker%d);\n\
			var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\
			marker%d.addEventListener(\"click\", function(){\
			this.openInfoWindow(infoWindow%d);}); ", j, G.list[tempcity[bycity]].longitude, G.list[tempcity[bycity]].latitude, j, j, G.list[tempcity[bycity]].country.c_str(), G.list[tempcity[bycity]].city.c_str(), j, j);

		j++;
		bycity2 = bycity;
		bycity++;

		fprintf(fp3, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\
			map.addOverlay(marker%d);\n\
			var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\
			marker%d.addEventListener(\"click\", function(){\
			this.openInfoWindow(infoWindow%d);}); ", j, G.list[tempcity[bycity]].longitude, G.list[tempcity[bycity]].latitude, j, j, G.list[tempcity[bycity]].country.c_str(), G.list[tempcity[bycity]].city.c_str(), j, j);

		fprintf(fp3, "var contentString0%d = '%s, %s --> %s, %s (%s - %.0f hours - $%.0f -%s )';\
			var path%d = new BMap.Polyline([new BMap.Point(%.4f, %.4f),new BMap.Point(%.4f, %.4f)], {strokeColor:'#18a45b', strokeWeight:8, strokeOpacity:0.8});\
			map.addOverlay(path%d);\
			path%d.addEventListener(\"click\", function(){\
			alert(contentString0%d);});", j, G.list[tempcity[bycity2]].city.c_str(), G.list[tempcity[bycity2]].country.c_str(), G.list[tempcity[bycity]].city.c_str(), G.list[tempcity[bycity]].country.c_str(),
			gettransname(temproute[bycity2]->trans), temproute[bycity2]->time, temproute[bycity2]->cost, temproute[bycity2]->info.c_str(), j,
			G.list[tempcity[bycity2]].longitude, G.list[tempcity[bycity2]].latitude, G.list[tempcity[bycity]].longitude, G.list[tempcity[bycity]].latitude, j, j, j);

	}
	fprintf(fp3, "</script>\n");

	//关闭文件
	fclose(fp3);
}


int main()
{
	//---------------------------------文件读取部分----------------------------------//
	

	CityGraph G;
	CreateGraph(G);

	//这里可以选择输入文件的路径
	//char a[100];
	//cout << "输入城市的文件;";
	//cin >> a;
	//ifstream fcity(a);
	//cout << "输入路线的文件:";
	//cin >> a;
	//ifstream froute(a);

	ifstream fcity("cities.csv");
	ifstream froute("routes.csv");
	if(!fcity.is_open())
		cout<<"ERROR";
	DWORD startTime = GetTickCount();
	fileCity(G, fcity);
	fileroute(G, froute);
	DWORD overTime = GetTickCount();
	cout << "构建图用时："<<overTime - startTime << "ms" << endl;
	fcity.close();
	froute.close();


	string ori;
	string des;
	int mode;
	//深度优先,从第一个城市开始
	startTime = GetTickCount();
	DFSTrasvers(G, 0);
	overTime = GetTickCount();
	cout << "深度优先（递归）用时："<<overTime - startTime << "ms" << endl;

	startTime = GetTickCount();
	DFSTrasversEX(G, 0);
	overTime = GetTickCount();
	cout << "深度优先（非递归）用时：" << overTime - startTime << "ms" << endl;
	//----------------------------------最短路径部分----------------------------------//
	cout << "输入起点：" << endl;
	getline(cin , ori);
	cout << "输入终点：" << endl;
	getline(cin, des);
	cout << "输入模式(time——1  cost——2)" << endl;
	cin >> mode;
	startTime = GetTickCount();
	ShortestPath(G, ori, des, mode);
	overTime = GetTickCount();
	cout <<"最短路径用时："<< overTime - startTime << "ms" << endl;


	if (tempcity.size() < 2)
		return 5;

	//----------------------------------可视化部分----------------------------------//
	
	const char* str = "graph.htm";
	visible(str, G,  ori,  des,  tempcity, temproute);


	//自动打开文件
	ShellExecute(NULL, L"open", L"Firefox.exe", L"graph.htm", NULL, SW_SHOW);


	//----------------------------------------------------------------------------//
	//销毁
	destroy(G);
}