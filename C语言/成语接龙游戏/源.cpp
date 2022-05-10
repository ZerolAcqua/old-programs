#define _CRT_SECURE_NO_WARNINGS 1
#define LENGTH 30
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"

struct chengyu_link
{
	char word[40] = { 0 };					//注意一个字需要两个字节来存储！！
	chengyu_link* pLast_chengyu = NULL; 	//链表		
};

//主要函数
void search_chengyu(FILE* fp);
void competition(FILE* fp);
void link(FILE* fp);

//辅助函数
char* get_keyword();
int compare(char* keyword,char* chengyu);
char* find_end(char* beginning);
char* search_next_chengyu(FILE* fp, char* last);
char* link_chengyu(FILE* fp, char* last);
chengyu_link* search_link_chengyu(char (*detect_chengyu)[LENGTH], int num_chengyu, chengyu_link* last, chengyu_link* now);
void print_link(chengyu_link* second_end);

int main()
{	
	FILE* fp = fopen("debug/成语库.dat", "r");
	if (fp == NULL) {
		printf("成语库无法打开！");
		return 0;
	}
	int function;
	while (true)
	{
		printf("\n选择功能：\n0-退出\n1-成语查询\n2-人机接龙\n3-自动接龙\n");
		//实现功能的选择
		scanf_s("%d", &function);
		switch (function) {
		case 0:	printf("已退出\n"); {fclose(fp); return 0; }
		case 1: search_chengyu(fp); break;
		case 2: competition(fp); break;
		case 3: link(fp); break;
		default: printf("错误\n"); break;
		}

	}

	

}


//主要函数

/***********************************************************************************
 *			 search_chengyu函数要实现对一个或多个关键字的搜索				       *
 ***********************************************************************************/
void search_chengyu(FILE* fp)
{
	char chengyu[40];
	char* keyword = get_keyword();
	int num_chengyu = 0;
	while ((fscanf(fp, "%s", chengyu)) !=EOF)
		if (compare(keyword, chengyu))
		{
			printf("%s\n", chengyu);
			num_chengyu++;
		}
	printf("成语个数：%d\n\n", num_chengyu);
	free(keyword);
	fseek(fp, 0L, SEEK_SET);
}
/***********************************************************************************
 *		    	competition函数要将用户和自己的成语进行接龙的匹配                  *
 *				也就是需要一个特殊的search_chengyu对第一个字检索                   *
 ***********************************************************************************/
void competition(FILE* fp)
{

	char chengyu[40];
	char chengyu_in[40];
	char* point = chengyu_in;
	char test[5];
	char* assist;
	printf("请输入一个成语(若要结束请输入0):");
	scanf("%s", chengyu_in);
	if (chengyu_in[0] == '0')     //退出
		return;
	///////////这里是对输入的成语的正确性进行判断//////////
	while (true)
	{
		if ((fscanf(fp, "%s", chengyu)) != EOF)
		{
			if (!strcmp(chengyu_in, chengyu))
			{
				fseek(fp, 0L, SEEK_SET);
				break;
			}
		}
		else
		{
			fseek(fp, 0L, SEEK_SET);
			printf("请输入正确的成语,现在认输还来得及(若要结束请输入0)：");
			scanf("%s", chengyu_in);
			if (chengyu_in[0] == '0')		 //退出
				return;
		}
	}
	while (true)
	{


		point = find_end(point);
		if (!(assist=search_next_chengyu(fp, point)))     ///电脑作答完毕
			return;
		strcpy(test, assist);

		printf("请输入一个成语(若要结束请输入0):");
		scanf("%s", chengyu_in);
		if (chengyu_in[0] == '0')		 //退出
			return;
		///////////这里是对输入的成语的正确性进行判断//////////
		while (true)
		{
			while (true)
			{
				if ((fscanf(fp, "%s", chengyu)) != EOF)
				{
					if (!strcmp(chengyu_in, chengyu))
					{
						fseek(fp, 0L, SEEK_SET);
						break;
					}
				}
				else
				{
					fseek(fp, 0L, SEEK_SET);
					printf("请输入正确的成语,现在认输还来得及(若要结束请输入0)：");
					scanf("%s", chengyu_in);
					if (chengyu_in[0] == '0')		 //退出
						return;
				}
			}
			if (test[0] != chengyu_in[0] || test[1] != chengyu_in[1])
			{
				printf("别想骗我！！(若要结束请输入0)：");
				scanf("%s", chengyu_in);
				if (chengyu_in[0] == '0')		 //退出
					return;
			}
			else
			{
				point =chengyu_in;
				break;
			}
				
		}
	}
}
/***********************************************************************************
 *                  这个函数要求找到两个成语之间的最短接龙                         *
 *                此函数体会比较复杂，可能需要再编写几个函数                       *
 ***********************************************************************************/
void link(FILE* fp)
{	
	int num_chengyu, layer_num = 0;
	char detect_chengyu[31200][LENGTH];
	/////////读入数组/////////
	for (num_chengyu = 0;; num_chengyu++)
	{
		if (fscanf(fp, "%s", detect_chengyu[num_chengyu]) == EOF)
		{
			printf("转存成功\n");
			break;
		}
	}
	fseek(fp, 0L, SEEK_SET);
	chengyu_link beginning, end;
	/////////输入首尾成语////////
	printf("请输入开始的成语：");
	scanf("%s", beginning.word);
	printf("请输入结束的成语：");
	scanf("%s", end.word);
	char* x = find_end(beginning.word);
	if (x[0] == end.word[0] && x[1] == end.word[1])
	{
		printf("你当我傻?\n");
		return;
	}
		
	chengyu_link* last_layer = &beginning;
	for (int i = 0; i < num_chengyu; i++)
	{
		if (!strcmp(detect_chengyu[i], beginning.word))
		{
			strcpy(detect_chengyu[i], "");
			break;
		}
	}
	////////////////////////////
	int last_num = 1;
	int last_time = 0;
	int now_time = 0;
	int space_time = 0;
	int last_space_time = 0;
	chengyu_link* space;
	space = (chengyu_link*)malloc(35000 * sizeof(chengyu_link));
	for (int i = 0; i <= 6; i++)
	{
		for (last_time = 0; last_time < last_num; last_time++)
		{
			for (;; space_time++, now_time++)
			{
				if (search_link_chengyu(detect_chengyu, num_chengyu, last_layer + last_time, space + space_time) == NULL)
					break;
				static char* end_now;
				end_now = find_end((space + space_time)->word);
				if (end_now[0] == end.word[0] && end_now[1] == end.word[1])
				{
					print_link(space + space_time);
					printf("%s\n", end.word);

					free(space);
					return;
				}
			}

		}

		last_layer = space + last_space_time;
		last_space_time = space_time;
		last_num = now_time;

		now_time = 0;
	}
	printf("无法接龙orz\n");
	free(space);
	return;
}
	



//辅助函数

////////////////用于读取一个关键字//////////////////
char* get_keyword()
{
	char* word = (char*)malloc(3);
	printf("请输入一个关键字：");
	scanf("%s", word);
	return word;
}
//////////////////用于比较关键字////////////////////
///////////若含有关键字返回1，否则返回0/////////////
int compare(char* keyword, char* chengyu)
{
	int i ;
	for (i = 0; chengyu[i] != '\0'; i += 2)
	{
		if (chengyu[i] > '\0')
			i++;					//处理“，”的问题
		if (keyword[0] == chengyu[i] && keyword[1] == chengyu[i + 1])
			return 1;
	}
	return 0;

}
///////////////把成语读至最后一个字/////////////////
char* find_end(char* beginning)
{
	int i;
	for (i = 0; beginning[i + 2] != '\0'; i += 2)
	{
		if (beginning[i] > '\0')
			i++;				//处理“，”的问题
	}
	return beginning + i;
}
///////////////////搜索接龙成语/////////////////////
///////若成功，返回电脑给出成语的最后一个字/////////
////////////////若失败，返回空指针//////////////////
char* search_next_chengyu(FILE* fp,char* last) 
{
	char chengyu[40];
	srand((unsigned)time(NULL));
	int i = rand() % 10 + 1;
	int n = 0;
	int test;
	while (true) 
	{
		if ((test=(fscanf(fp, "%s", chengyu))) != EOF)
		{
			if (chengyu[0] == last[0] && chengyu[1] == last[1])
			{
				n++;
				
			}
		}
		if (n==0&&test==EOF)
		{
			printf("天哪你是什么魔鬼?!\n");
			fseek(fp, 0L, SEEK_SET);
			return NULL;
		}
		else if(test==EOF)
			fseek(fp, 0L, SEEK_SET);
		if (n == i)
		{
			printf("%s\n", chengyu);
			fseek(fp, 0L, SEEK_SET);
			return find_end(chengyu);
		}
	}
}
//////////////搜索由首汉字连接成语//////////////////
////////若成功，返回电脑给出成语的第一个字//////////
///////////////若失败，返回空指针///////////////////
char* link_chengyu(FILE* fp, char* first)
{
	char* chengyu=(char*)malloc(40*sizeof(char));
	char* last = find_end(first);
	while (true)
	{
		if ((fscanf(fp, "%s", chengyu)) != EOF)
		{
			if (chengyu[0] == last[0] && chengyu[1] == last[1])
			{
				fseek(fp, 0L, SEEK_SET);
				return chengyu;
			}
		}
		else
		{
			return NULL;
		}
	}
}
/////////////从链最后反序打印出成语/////////////////
//////////////////并回收空间////////////////////////
void print_link(chengyu_link* second_end)
{
	char store[10][40];
	int i;
	chengyu_link* temp = second_end;
	for (i = 0; temp->pLast_chengyu != NULL; i++)
	{
		strcpy(store[i], temp->word);
		temp = temp->pLast_chengyu;
	}
	strcpy(store[i], temp->word);
	for (int n = i; n >= 0; n--)
	{
		printf("%s->\n", store[n]);
	}

}
///////////////////连接成语搜索/////////////////////
chengyu_link* search_link_chengyu(char (*detect_chengyu)[LENGTH], int num_chengyu, chengyu_link* last, chengyu_link* now)
{
	static int i = 0;
	char* end = find_end(last->word);
	i = 0;
	for (; i < num_chengyu; i++)
	{
		if (detect_chengyu[i][0] == end[0] && detect_chengyu[i][1] == end[1])
		{
			now->pLast_chengyu = last;
			strcpy(now->word, detect_chengyu[i]);
			strcpy(detect_chengyu[i], "");
			return now;
		}		
	}
	i = 0;
	return NULL;	
}

	//int i;
	//char a[35000][40];
	//for (i = 0; fscanf(fp, "%s", a[i]) == EOF; i++);
	//i--;
	//chengyu_link start, end;
	//printf("开始：");
	//scanf("%s", start.word);
	//printf("结束：");
	//scanf("%s", end.word);
