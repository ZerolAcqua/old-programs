//���ͷ�ļ�
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

//Ԥ���峣���ͱ������ͣ�ע�����޷ֺ�
#define OK 1
#define ERROR 0
#define MAX_SIZE 50
#define INCREASE_SIZE 50
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

//��ͨ��ʽ�ĺ궨��
#define PLANE 1
#define TRAIN 2
#define BUS 3
#define BOAT 4
//�����·��ģʽ�ĺ궨��
#define TIME 1
#define COST 2
//Dijkstra�еġ������
#ifndef INFINITY 
#define INFINITY 1000000
#endif

typedef char ElemType;//ͼ
typedef int SElemType;//ջ
typedef int Status;

//////ջ��ʵ��//////
struct SqStack {
	SElemType* base;// �洢�ռ��ַ 
	SElemType* top;// ջ��ָ�� 
	int stacksize;// ��ǰ��ʹ�õ����洢�ռ䣬��Ԫ��Ϊ��λ
};

//��ʼ��˳��ջS��������Ϊ��ջ
Status InitStack(SqStack& S)
{
	S.stacksize = 0;
	S.top = S.base = NULL;
	S.base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (S.base == NULL)
		return ERROR;		//�����ڴ�ʧ��
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}

//����˳��ջS
Status DestroyStack(SqStack& S)
{
	if (S.base == NULL)
	{
		return ERROR;	//�Ƿ�����Ч��˳��ջ
	}
	free(S.base);
	S.base = NULL;
	S.top = NULL;
	S.stacksize = 0;
	return OK;
}

//��˳��ջ��Ϊ��ջ
Status ClearStack(SqStack& S)
{
	if (S.base == NULL)
	{
		return ERROR;	//�Ƿ�����Ч��˳��ջ
	}
	S.top = S.base;
	return OK;
}

//�ж��Ƿ�Ϊ��ջ�����ǿ�ջ����true�����򷵻�false(���������ڵ�ջ)
bool StackEmpty(SqStack S)
{
	if (S.base == S.top && S.base != NULL)
	{
		return true;
	}
	else return false;
}

//����ջ��Ԫ�صĸ���(�����Ǵ��ڵ�ջ��)
int StackLength(SqStack S)
{
	return S.top - S.base;
}

//��e����ջ������Ԫ��
Status GetTop(SqStack S, SElemType& e)
{
	if (S.base == NULL)
	{
		return ERROR;	//�Ƿ��Ǵ��ڵ�ջ
	}
	e = *(S.top - 1);
	return OK;
}

//��eѹ��ջ������ջ��������ջ
Status Push(SqStack& S, SElemType e)
{
	if (S.base == NULL)
		return ERROR;	//�Ƿ��Ǵ��ڵ�ջ
	if (S.top - S.base == S.stacksize)
	{
		//���ӷ�����ڴ�����ջ�Ĵ�С
		S.base = (SElemType*)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
		if (S.base == NULL)
			return ERROR;	//����ʧ��
		S.top = S.base + S.stacksize;	//��ֹrealloc�ı��ַ��Ӱ��
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return OK;
}

//��ջ����ֵ���ظ�e
Status Pop(SqStack& S, SElemType& e)
{
	if (S.base == NULL || S.base == S.top)
		return ERROR;	//ջΪ�ջ򲻴���
	e = *--S.top;
	return OK;
}



//////ͼ��ʵ��//////

//·�߽��
typedef struct RouteNode
{
	int nextcity = 0;						//Ŀ�ĵس���
	float cost = 0;							//����
	float time = 0;							//��ʱ
	int trans = 0;							//��ͨ��ʽ  PLANE 1  TRAIN 2  BUS 3  BOAT 4
	string info;							//������Ϣ
	struct RouteNode* nextRoute = NULL;		//��һ��·��
}RouteNode;
//���н��
typedef struct City
{
	bool check = false;						//������־
	double latitude, longitude;				//γ�ȡ�����
	string country, city;					//���ң��ݣ�������
	RouteNode* firstroute = NULL;			//��һ��·��
}City, * CityList;
//ͼ�ṹ
typedef struct
{
	int size;
	CityList list = NULL;					//�ڽӱ�
	int citynum = 0, routenum = 0;			//������Ŀ��·����Ŀ
}CityGraph;


vector<int> tempcity;
vector<RouteNode*> temproute;


//����һ���ڽӱ�����ʾͼ������ͼ������
Status CreateGraph(CityGraph& G)
{
	G.citynum = 0;
	G.routenum = 0;
	G.list =new City[MAX_SIZE];	
	G.size = MAX_SIZE;						//�����ڴ�
	if (G.list == NULL)
		return ERROR;
	return OK;
}

//����������к�·�ߣ����иó��ж�Ӧ�ı���ӳ�
Status addlist(City& tail, RouteNode* add)
{
	RouteNode* p = tail.firstroute;			
	if (p == NULL)
		tail.firstroute = add;				//����·�ߣ���ֱ�����
	else									//�����ҵ����һ�����
	{
		for (; p->nextRoute != NULL; p = p->nextRoute)
		{
			;
		}
		p->nextRoute = add;
	}
	return OK;
}

//�ҵ�������list�е�λ��
int findcity(CityGraph G, string& city)
{
	for (int i = 0; i < G.citynum; i++)
	{
		if (G.list[i].city == city)
			return i;
	}
	return -1;
}


//��ͨ����string��intֵת��
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

//�����ڴ�
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
	//memcpy(p,G.list,G.list*(sizeof(City))) �ᱨ��
	delete[]G.list;
	G.size += INCREASE_SIZE;
	G.list = p;
	return OK;
}

//��ȡ����֮�����Ҫ���·��
RouteNode* getRoute(CityGraph& G, int tail, int head, int mode)
{
	RouteNode* temp = NULL;
	RouteNode* p;
	//ʱ������̵Ļ�
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
	//���������ٻ�
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

//�������ĳ������к�Ŀ�ĵس��У��Լ������Ϣ�����ڽӱ�
Status BuildGraph(CityGraph& G, string& head, string& tail, string& info, int trans, float time, float cost)
{
	int intail = 0, inhead = 0;				//�洢���еı��

	RouteNode* next = new RouteNode;
	if (next == NULL)
		return ERROR;

	next->nextRoute = NULL;
	next->cost = cost;
	next->time = time;
	next->trans = trans;
	next->nextcity = findcity(G, head);
	next->info = info;
											//����·�߽��
	intail = findcity(G, tail);
	addlist(G.list[intail], next);
											//��ӵ��ڽӱ���
	return OK;
}

//�����ж�ȡ������ͼ��
Status fileCity(CityGraph& G, ifstream& fcity)
{
	if (!fcity.is_open())					//�ļ��Ƿ��
	{
		cout << "fail to open the file"<<endl;
		return ERROR;
	}
	int n = 0;
	int front, rear, length;				//���Լ�¼����λ����֮��ļ��
	string temp;							//������ʱ�洢һ���ַ���
	for (G.citynum = n = 0; getline(fcity, temp); n++)
	{
		front = 0; rear = 0;
		rear = temp.find(',', front);		//�ҵ�����λ��
		length = rear - front;				//������
		G.list[n].country = temp.substr(front, length);
											//��ȡ�ַ���
		front = rear + 1;
		rear = temp.find(',', front);
		length = rear - front;
		G.list[n].city = temp.substr(front, length);
		front = rear + 1;
		rear = temp.find(',', front);
		length = rear - front;
		istringstream(temp.substr(front, length).c_str())>> G.list[n].latitude;
											//����ʵ���ַ���ֱ��ת������
		front = rear + 1;
		istringstream(temp.substr(front).c_str()) >> G.list[n].longitude;

		G.citynum++;						//��¼������Ŀ

		if (G.citynum == G.size)
			if (!renew(G))
				return ERROR;
				
	}
	
	return OK;
}

//����·��ȡ������ͼ��
Status fileroute(CityGraph& G, ifstream& froute)
{
	if (!froute.is_open())					//�ļ��Ƿ��
	{
		cout << "fail to open the file" << endl;
		return ERROR;
	}
	int n = 0;
	int front, rear, length;				//���Լ�¼����λ����֮��ļ��
	string temp;							//������ʱ�洢һ���ַ���
	string head, tail, info;				//���������,�Լ�·��Ϣ
	int trans;
	float time;
	float cost;
	for (n = 0; getline(froute, temp); n++)
	{
		front = 0; rear = 0;
		rear = temp.find(',', front);		//�ҵ�����λ��
		length = rear - front;				//������
		tail= temp.substr(front, length);	//��ȡ�ַ���

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
											//����ʵ���ַ���ֱ��ת������
		front = rear + 1;
		rear = temp.find(',', front);
		length = rear - front;
		istringstream(temp.substr(front, length).c_str()) >> cost;

		front = rear + 1;
		info = temp.substr(front);

		BuildGraph(G, head, tail, info, trans, time, cost);
											//�����ڽӱ�
	}


	G.routenum = n;							//��¼·����Ŀ
	return OK;
}

//����ͼ
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
	delete[]G.list;							//��Ӧnew
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

//��ȱ���,��start��Ӧ�ĳ��п�ʼ����
//��startȡ0����Ϊ�ӵ�һ�����п�ʼ����
Status DFSTrasvers(CityGraph& G, int start)
{
	int i = 0;
	RouteNode* p = NULL;
	//�������ı�־��Ϊfalse
	for (i = 0; i < G.citynum; i++)
	{
		G.list[i].check = false;
	}

	//��start��ʼ�ݹ�ر���ͼ
	DFS(G, start);

	//����ͨͼʱ�ı�����δ����DFS�Ľڵ㣬������DFS
	for (i = 0; i < G.citynum; i++)
	{
		if(G.list[i].check==false)
			DFS(G, i);
	}
	cout << endl;

	return OK;
}
//ջ�ķ�ʽ
Status DFSTrasversEX(CityGraph& G, int start)
{
	int i = 0;
	int n = 0;
	RouteNode* p = NULL;
	//�������ı�־��Ϊfalse
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
	//����ͨͼʱ�ı�����δ����DFS�Ľڵ㣬������DFS
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

//���·������Dijkstra�㷨
Status ShortestPath(CityGraph G, string ori, string des, int mode)
{
	int i;
											
	int* parent = new int[G.citynum];		//path���ڴ�����֪���ܵ����·��
	float* path = new float[G.citynum];			//parent���Դ洢���·�ߣ�����һ�����У�
	bool* finish = new bool[G.citynum];		//finish���ڴ����Ѿ�������·���ĳ���(�����㼯)
	//Ѱ���������Ӧ�ĳ��б��
	int start = findcity(G, ori);
	int end = findcity(G, des);
	
	if (start < 0 || end < 0||start==end)
		return ERROR;

	//��ʼ��parent��finish��path
	for (i = 0; i < G.citynum; i++)
	{
		parent[i] = -1;
		finish[i] = false;
		path[i] = INFINITY;
	}
	

	RouteNode* p = NULL;
	int city = 0;							//Ҫ���µ�·������
	int min = INFINITY;						//������ʱ��¼��Сֵ�ı���

	
	if (mode == TIME)						//���·������ʱ������
	{
		//�������м�����ɵ㼯�������·��0
		finish[start] = true;
		path[start] = 0;

		city = start;

		while (true)
		{
			for (p = G.list[city].firstroute;
				p != NULL; p = p->nextRoute)
			{
				//������֪·����˫�ף���֪���·����㣩
				//���￼�ǵ����������м䲻ͬ�Ľ�ͨ��ʽ����������ֻ��ѡ����С�ģ��ϴ�Ļᱻ���ǵ�
				//���еĵ�city���·��+city��ĳ���е�·��<���ó��е�·�� 			
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
				//�ҵ���δ����㼯����С�����·��min����Ӧ����
				if (!finish[i] && path[i] < min)
				{
					city = i;
					min = path[i];
				}
			}
			//����㼯finish��
			if (min < INFINITY)
			{
				finish[city] = true;
				if (city == end)
				{
					break;					//˵���Ѿ��ҵ�����������·��
											//����Ҫ�ټ��������������·��
				}
			}
			else
			{								//˵��ʣ�µĵ㶼ΪINFINITY,���޷�����
				cout << "û��·���ɵ���";
				return OK;
			}
		}
	}
	else if (mode == COST)					//���·��������������
	{
		//�������м�����ɵ㼯�������·��0
		finish[start] = true;
		path[start] = 0;

		city = start;

		while (true)
		{
			for (p = G.list[city].firstroute;
				p != NULL; p = p->nextRoute)
			{
				//������֪·����˫�ף���֪���·����㣩
				//���￼�ǵ����������м䲻ͬ�Ľ�ͨ��ʽ����������ֻ��ѡ����С��
				//���еĵ�city���·��+city��ĳ���е�·��<���ó��е�·�� 			
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
				//�ҵ���δ����㼯����С�����·��min����Ӧ����
				if (!finish[i] && path[i] < min)
				{
					city = i;
					min = path[i];
				}
			}
			//����㼯finish��
			if (min < INFINITY)
			{
				finish[city] = true;
				if (city == end)
				{
					break;					//˵���Ѿ��ҵ�����������·��
											//����Ҫ�ټ��������������·��
				}
			}
			else
			{
				cout << "û��·���ɵ���";	//˵��ʣ�µĵ㶼ΪINFINITY,���޷�����
				return OK;
			}
		}
	}
	else
		return ERROR;


	//·���洢�������
	tempcity.clear();
	int n;
	n = end;
	for (i = 0;n!=start; i++)
	{
		tempcity.insert(tempcity.begin(),n);
		n = parent[n];
	}
	tempcity.insert(tempcity.begin(), n);

	//�洢·������Ϣ
	temproute.clear();
	for(i=0;i<tempcity.size()-1;i++)
	{
		temproute.push_back(getRoute(G, tempcity[i], tempcity[i+1], mode));
	}

	//��ӡ
	if (mode == TIME)
		cout << "�����ʱ��" << path[end] << endl;
	else 
		cout << "���ٻ��ѣ�" << path[end] << endl;
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
		<style type='text/css'>body, html{width: 100%;height: 100%;margin:0;font-family:'΢���ź�';}#allmap{height:100%;width:100%;}#r-result{width:100%;}</style>\
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

	//�ر��ļ�
	fclose(fp3);
}


int main()
{
	//---------------------------------�ļ���ȡ����----------------------------------//
	

	CityGraph G;
	CreateGraph(G);

	//�������ѡ�������ļ���·��
	//char a[100];
	//cout << "������е��ļ�;";
	//cin >> a;
	//ifstream fcity(a);
	//cout << "����·�ߵ��ļ�:";
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
	cout << "����ͼ��ʱ��"<<overTime - startTime << "ms" << endl;
	fcity.close();
	froute.close();


	string ori;
	string des;
	int mode;
	//�������,�ӵ�һ�����п�ʼ
	startTime = GetTickCount();
	DFSTrasvers(G, 0);
	overTime = GetTickCount();
	cout << "������ȣ��ݹ飩��ʱ��"<<overTime - startTime << "ms" << endl;

	startTime = GetTickCount();
	DFSTrasversEX(G, 0);
	overTime = GetTickCount();
	cout << "������ȣ��ǵݹ飩��ʱ��" << overTime - startTime << "ms" << endl;
	//----------------------------------���·������----------------------------------//
	cout << "������㣺" << endl;
	getline(cin , ori);
	cout << "�����յ㣺" << endl;
	getline(cin, des);
	cout << "����ģʽ(time����1  cost����2)" << endl;
	cin >> mode;
	startTime = GetTickCount();
	ShortestPath(G, ori, des, mode);
	overTime = GetTickCount();
	cout <<"���·����ʱ��"<< overTime - startTime << "ms" << endl;


	if (tempcity.size() < 2)
		return 5;

	//----------------------------------���ӻ�����----------------------------------//
	
	const char* str = "graph.htm";
	visible(str, G,  ori,  des,  tempcity, temproute);


	//�Զ����ļ�
	ShellExecute(NULL, L"open", L"Firefox.exe", L"graph.htm", NULL, SW_SHOW);


	//----------------------------------------------------------------------------//
	//����
	destroy(G);
}