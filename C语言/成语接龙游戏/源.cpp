#define _CRT_SECURE_NO_WARNINGS 1
#define LENGTH 30
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"

struct chengyu_link
{
	char word[40] = { 0 };					//ע��һ������Ҫ�����ֽ����洢����
	chengyu_link* pLast_chengyu = NULL; 	//����		
};

//��Ҫ����
void search_chengyu(FILE* fp);
void competition(FILE* fp);
void link(FILE* fp);

//��������
char* get_keyword();
int compare(char* keyword,char* chengyu);
char* find_end(char* beginning);
char* search_next_chengyu(FILE* fp, char* last);
char* link_chengyu(FILE* fp, char* last);
chengyu_link* search_link_chengyu(char (*detect_chengyu)[LENGTH], int num_chengyu, chengyu_link* last, chengyu_link* now);
void print_link(chengyu_link* second_end);

int main()
{	
	FILE* fp = fopen("debug/�����.dat", "r");
	if (fp == NULL) {
		printf("������޷��򿪣�");
		return 0;
	}
	int function;
	while (true)
	{
		printf("\nѡ���ܣ�\n0-�˳�\n1-�����ѯ\n2-�˻�����\n3-�Զ�����\n");
		//ʵ�ֹ��ܵ�ѡ��
		scanf_s("%d", &function);
		switch (function) {
		case 0:	printf("���˳�\n"); {fclose(fp); return 0; }
		case 1: search_chengyu(fp); break;
		case 2: competition(fp); break;
		case 3: link(fp); break;
		default: printf("����\n"); break;
		}

	}

	

}


//��Ҫ����

/***********************************************************************************
 *			 search_chengyu����Ҫʵ�ֶ�һ�������ؼ��ֵ�����				       *
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
	printf("���������%d\n\n", num_chengyu);
	free(keyword);
	fseek(fp, 0L, SEEK_SET);
}
/***********************************************************************************
 *		    	competition����Ҫ���û����Լ��ĳ�����н�����ƥ��                  *
 *				Ҳ������Ҫһ�������search_chengyu�Ե�һ���ּ���                   *
 ***********************************************************************************/
void competition(FILE* fp)
{

	char chengyu[40];
	char chengyu_in[40];
	char* point = chengyu_in;
	char test[5];
	char* assist;
	printf("������һ������(��Ҫ����������0):");
	scanf("%s", chengyu_in);
	if (chengyu_in[0] == '0')     //�˳�
		return;
	///////////�����Ƕ�����ĳ������ȷ�Խ����ж�//////////
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
			printf("��������ȷ�ĳ���,�������仹���ü�(��Ҫ����������0)��");
			scanf("%s", chengyu_in);
			if (chengyu_in[0] == '0')		 //�˳�
				return;
		}
	}
	while (true)
	{


		point = find_end(point);
		if (!(assist=search_next_chengyu(fp, point)))     ///�����������
			return;
		strcpy(test, assist);

		printf("������һ������(��Ҫ����������0):");
		scanf("%s", chengyu_in);
		if (chengyu_in[0] == '0')		 //�˳�
			return;
		///////////�����Ƕ�����ĳ������ȷ�Խ����ж�//////////
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
					printf("��������ȷ�ĳ���,�������仹���ü�(��Ҫ����������0)��");
					scanf("%s", chengyu_in);
					if (chengyu_in[0] == '0')		 //�˳�
						return;
				}
			}
			if (test[0] != chengyu_in[0] || test[1] != chengyu_in[1])
			{
				printf("����ƭ�ң���(��Ҫ����������0)��");
				scanf("%s", chengyu_in);
				if (chengyu_in[0] == '0')		 //�˳�
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
 *                  �������Ҫ���ҵ���������֮�����̽���                         *
 *                �˺������Ƚϸ��ӣ�������Ҫ�ٱ�д��������                       *
 ***********************************************************************************/
void link(FILE* fp)
{	
	int num_chengyu, layer_num = 0;
	char detect_chengyu[31200][LENGTH];
	/////////��������/////////
	for (num_chengyu = 0;; num_chengyu++)
	{
		if (fscanf(fp, "%s", detect_chengyu[num_chengyu]) == EOF)
		{
			printf("ת��ɹ�\n");
			break;
		}
	}
	fseek(fp, 0L, SEEK_SET);
	chengyu_link beginning, end;
	/////////������β����////////
	printf("�����뿪ʼ�ĳ��");
	scanf("%s", beginning.word);
	printf("����������ĳ��");
	scanf("%s", end.word);
	char* x = find_end(beginning.word);
	if (x[0] == end.word[0] && x[1] == end.word[1])
	{
		printf("�㵱��ɵ?\n");
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
	printf("�޷�����orz\n");
	free(space);
	return;
}
	



//��������

////////////////���ڶ�ȡһ���ؼ���//////////////////
char* get_keyword()
{
	char* word = (char*)malloc(3);
	printf("������һ���ؼ��֣�");
	scanf("%s", word);
	return word;
}
//////////////////���ڱȽϹؼ���////////////////////
///////////�����йؼ��ַ���1�����򷵻�0/////////////
int compare(char* keyword, char* chengyu)
{
	int i ;
	for (i = 0; chengyu[i] != '\0'; i += 2)
	{
		if (chengyu[i] > '\0')
			i++;					//��������������
		if (keyword[0] == chengyu[i] && keyword[1] == chengyu[i + 1])
			return 1;
	}
	return 0;

}
///////////////�ѳ���������һ����/////////////////
char* find_end(char* beginning)
{
	int i;
	for (i = 0; beginning[i + 2] != '\0'; i += 2)
	{
		if (beginning[i] > '\0')
			i++;				//��������������
	}
	return beginning + i;
}
///////////////////������������/////////////////////
///////���ɹ������ص��Ը�����������һ����/////////
////////////////��ʧ�ܣ����ؿ�ָ��//////////////////
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
			printf("��������ʲôħ��?!\n");
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
//////////////�������׺������ӳ���//////////////////
////////���ɹ������ص��Ը�������ĵ�һ����//////////
///////////////��ʧ�ܣ����ؿ�ָ��///////////////////
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
/////////////����������ӡ������/////////////////
//////////////////�����տռ�////////////////////////
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
///////////////////���ӳ�������/////////////////////
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
	//printf("��ʼ��");
	//scanf("%s", start.word);
	//printf("������");
	//scanf("%s", end.word);
