#pragma once
#ifndef OPGTABLE_H_
#define OPGTABLE_H_
#include<iostream>
#include<fstream>
using namespace std;

struct Proce {  //�ýṹ����������Ų���ʽ
	char Left;
	char Right[100];
};

//���ÿ�����ս����firstvt��lastvt
struct AVnSet {
	char VN;
	char VT[100];
};

class OPGTable
{
private:
	Proce pro[100];      //��¼���в���ʽ
	int production_num;//�ܲ���ʽ����
	char vt[50]; //��¼�����ս��
	char vn[50]; //��¼���еķ��ս��
	AVnSet firstvt[100];  //��ÿһ�����ս����firstvt����
	AVnSet lastvt[100];   //��ÿһ�����ս����lastvt����
	char table[50][50]; //���������ȱ�
	char S[100];          //����ջ

	void GetVT();	//�ҵ����е��ս�������vt�ַ�������
	void GetVN();	//�ҵ����еķ��ս�������vn�ַ�������
	int isVT(char ch); //�ж��Ƿ��Ƿ��ս����Ĭ�ϴ�д��ĸΪ���ս��������Ϊ�ս��
	char GetRelation(char a, char b); //�ҵ�a��b��Ӧ�����ȹ�ϵ
	bool Match(const string phrase); //Ϊ�����ض���ƥ�����ʽ
public:
	OPGTable() :production_num(0) //ȫ����ʼ��Ϊ��
	{
		memset(vt, 0, sizeof(vt));
		memset(vn, 0, sizeof(vn));
		memset(firstvt, 0, sizeof(firstvt));
		memset(lastvt, 0, sizeof(lastvt));
		memset(pro, 0, sizeof(pro));
		memset(table, 0, sizeof(table));
		memset(S, 0, sizeof(S));
	}
	void ReadProduction(char filename[100]); //���ζ������ʽ
	void FIRSTVT();//������ս����FIRSTVT����
	void LASTVT();//������ս����LASTVT����
	void MakeTable(); //�������ȹ�ϵ��
	int Analysis(char* str); //����������ȷ���
};
#endif

