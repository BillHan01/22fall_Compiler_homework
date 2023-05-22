#pragma once
#ifndef OPGTABLE_H_
#define OPGTABLE_H_
#include<iostream>
#include<fstream>
using namespace std;

struct Proce {  //用结构体数组来存放产生式
	char Left;
	char Right[100];
};

//存放每个非终结符的firstvt和lastvt
struct AVnSet {
	char VN;
	char VT[100];
};

class OPGTable
{
private:
	Proce pro[100];      //记录所有产生式
	int production_num;//总产生式个数
	char vt[50]; //记录所有终结符
	char vn[50]; //记录所有的非终结符
	AVnSet firstvt[100];  //存每一个非终结符的firstvt集合
	AVnSet lastvt[100];   //存每一个非终结符的lastvt集合
	char table[50][50]; //存放算符优先表
	char S[100];          //符号栈

	void GetVT();	//找到所有的终结符存放在vt字符数组中
	void GetVN();	//找到所有的非终结符存放在vn字符数组中
	int isVT(char ch); //判断是否是非终结符，默认大写字母为非终结符，其他为终结符
	char GetRelation(char a, char b); //找到a，b对应的优先关系
	bool Match(const string phrase); //为最左素短语匹配产生式
public:
	OPGTable() :production_num(0) //全部初始化为空
	{
		memset(vt, 0, sizeof(vt));
		memset(vn, 0, sizeof(vn));
		memset(firstvt, 0, sizeof(firstvt));
		memset(lastvt, 0, sizeof(lastvt));
		memset(pro, 0, sizeof(pro));
		memset(table, 0, sizeof(table));
		memset(S, 0, sizeof(S));
	}
	void ReadProduction(char filename[100]); //依次读入产生式
	void FIRSTVT();//求各非终结符的FIRSTVT集合
	void LASTVT();//求各非终结符的LASTVT集合
	void MakeTable(); //构造优先关系表
	int Analysis(char* str); //进行算符优先分析
};
#endif

