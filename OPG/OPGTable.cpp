#define _CRT_SECURE_NO_WARNINGS
#include "OPGTable.h"
#include<iomanip>
#include<stack>
#include<string>

void OPGTable::GetVT()
{
	//找到所有的终结符存放在vt字符数组中
	int num = 0;
	int exited = 0;
	for (int i = 0; i < production_num; i++)
	{
		for (int k = 0; pro[i].Right[k] != '\0'; k++) {
			exited = 0;
			if (pro[i].Right[k] < 'A' || pro[i].Right[k]>'Z')
			{
				for (int j = 0; vt[j] != '\0'; j++) {
					if (vt[j] == pro[i].Right[k])
						exited = 1;
				}
				if (exited == 0)
					vt[num++] = pro[i].Right[k];
			}
		}
	}
}

void OPGTable::GetVN()
{
	int num = 0;
	int exited = 0;
	for (int i = 1; i < production_num; i++)
	{
		exited = 0;
		for (int j = 0; vn[j] != '\0'; j++) {
			if (vn[j] == pro[i].Left)
				exited = 1;
		}
		if (exited == 0)
			vn[num++] = pro[i].Left;
	}
}

//如果是终结符就返回1，其他情况返回0
int OPGTable::isVT(char ch)
{
	for (int i = 0; i < strlen(vt); i++) {
		if (vt[i] == ch)
			return 1;
	}
	return 0;
}


bool OPGTable::Match(const string phrase)
{
	int result;
	for (int i = 1; i < production_num; i++) {
		result = 1;
		if (phrase.size() != strlen(pro[i].Right))
			result = 0;
		else {
			for (int j = 0; j < phrase.size(); j++) {
				if (!isVT(phrase[j])) {
					if (isVT(pro[i].Right[j]))
						result = 0;
				}
				if (isVT(phrase[j]))
					if (pro[i].Right[j] != phrase[j])
						result = 0;
			}
		}
		if (result == 1)
			return true;
	}
	return false;
}

void OPGTable::ReadProduction(char filename[100])
{
	char production[100] = { 0 };//用于存放一个产生式
	cout << "请输入存放算符优先文法的文件名：";
	cin >> filename;
	ifstream fin(filename);
	if (!fin)
	{
		cout << "Cannot open the file.\n";
		exit(1);
	}
	cout << "您输入的文法为（第一个产生式为辅助产生式）：" << endl;
	while (fin)
	{
		fin.getline(production, 100); //读出一个产生式
		cout << production << endl;
		pro[production_num].Left = production[0];
		strcpy(pro[production_num].Right, production + 3);
		production_num++;
	}
	production_num -= 1;
	fin.close();
	GetVT();
	GetVN();
}

void OPGTable::FIRSTVT()
{
	bool matrix[50][50]; //二维布尔矩阵F[P,a]
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			matrix[i][j] = 0;
		}
	}
	char s_vt[100];
	char s_vn[100];  //使用两个栈共同存储一个终结符和一个非终结符,即存放使FIRSTVT 为真的符号栈
	int top_vt = 0, top_vn = 0; //指向上面两个栈的栈顶
	int  i, j, k; //辅助循环变量
	int num = 1; //记录扫过的产生式个数
	char temp_vt, temp_vn;
	//第一遍扫描找到P->a...，P->Qa...形式的产生式，把所有初值为真的数组元素F[P, a]的符号对(P, a)全都放在STACK之中。
	for (num = 1; num < production_num; num++)
	{
		if (isVT(pro[num].Right[0]) == 1) {
			for (i = 0; vn[i] != pro[num].Left; i++)
				;
			for (j = 0; vt[j] != pro[num].Right[0]; j++)
				;
			if (matrix[i][j] == 0) {
				matrix[i][j] = 1;
				s_vn[top_vn++] = vn[i];
				s_vt[top_vt++] = vt[j];
			}
		}
		else if (isVT(pro[num].Right[0]) == 0 && isVT(pro[num].Right[1]) == 1) {
			for (i = 0; vn[i] != pro[num].Left; i++)
				;
			for (j = 0; vt[j] != pro[num].Right[1]; j++)
				;
			if (matrix[i][j] == 0) {
				matrix[i][j] = 1;
				s_vn[top_vn++] = vn[i];
				s_vt[top_vt++] = vt[j];
			}
		}
	}
	//利用栈和布尔矩阵不断分析:如果栈STACK不空，栈顶出栈，记此项为(Q, a)
	//对于每个形如P→Q…的产生式，若F[P, a]为假，则变其值为真，且将(P, a)推进STACK栈，直到栈空分析完成
	while (top_vn != 0 && top_vt != 0) {
		temp_vn = s_vn[--top_vn];
		temp_vt = s_vt[--top_vt];
		for (num = 1; num < production_num; num++) {
			if (pro[num].Right[0] == temp_vn && pro[num].Left != temp_vn) {
				for (i = 0; vn[i] != pro[num].Left; i++)
					;
				for (j = 0; vt[j] != temp_vt; j++)
					;
				if (matrix[i][j] == 0) {
					matrix[i][j] = 1;
					s_vn[top_vn++] = vn[i];
					s_vt[top_vt++] = temp_vt;
				}
			}
		}
	}
	//将得到的矩阵输入到firstvt中
	for (i = 0; i < strlen(vn); i++) {
		firstvt[i].VN = vn[i];
		for (j = 0; j < strlen(vt); j++) {
			if (matrix[i][j] == 1) {
				for (k = 0; firstvt[i].VT[k] != '\0'; k++)
					;
				firstvt[i].VT[k] = vt[j];
			}
		}
	}
	//输出各个非终结符的firstvt集合
	for (int i = 0; i < strlen(vn); i++)
	{
		cout << "FIRSTVT(";
		cout << firstvt[i].VN << ")" << "=" << "{";
		cout << firstvt[i].VT[0];
		for (int j = 1; j < strlen(firstvt[i].VT); j++)
			cout << "," << firstvt[i].VT[j];
		cout << "}" << endl;
	}
}

void OPGTable::LASTVT()
{
	bool matrix[50][50];
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			matrix[i][j] = 0;
		}
	}
	char s_vt[100];
	char s_vn[100];  //使用两个栈共同存储一个终结符和一个非终结符，符号栈
	int top_vt = 0, top_vn = 0; //指向上面两个栈的栈顶
	int num = 1; //记录扫过的产生式个数
	char temp_vt, temp_vn;
	int i, j, k;  //辅助数组

	//第一遍扫描找到P->...a，P->...aQ形式的产生式,并将lastvt对应入栈
	for (num = 1; num < production_num; num++)
	{
		k = strlen(pro[num].Right);
		if (isVT(pro[num].Right[k - 1])) {
			for (i = 0; vn[i] != pro[num].Left; i++)
				;
			for (j = 0; vt[j] != pro[num].Right[k - 1]; j++)
				;
			if (matrix[i][j] == 0) {
				matrix[i][j] = 1;
				s_vn[top_vn++] = vn[i];
				s_vt[top_vt++] = vt[j];
			}
		}
		else if (isVT(pro[num].Right[k - 1]) == 0 && isVT(pro[num].Right[k - 2])) {
			for (i = 0; vn[i] != pro[num].Left; i++)
				;
			for (j = 0; vt[j] != pro[num].Right[k - 2]; j++)
				;
			if (matrix[i][j] == 0) {
				matrix[i][j] = 1;
				s_vn[top_vn++] = vn[i];
				s_vt[top_vt++] = vt[j];
			}
		}
	}
	//利用栈和布尔矩阵不断分析
	while (top_vn != 0 && top_vt != 0) {
		temp_vn = s_vn[--top_vn];
		temp_vt = s_vt[--top_vt];
		for (num = 1; num < production_num; num++) {
			k = strlen(pro[num].Right);
			if (pro[num].Right[k - 1] == temp_vn && pro[num].Left != temp_vn) {
				for (i = 0; vn[i] != pro[num].Left; i++)
					;
				for (j = 0; vt[j] != temp_vt; j++)
					;
				if (matrix[i][j] == 0) {
					matrix[i][j] = 1;
					s_vn[top_vn++] = vn[i];
					s_vt[top_vt++] = temp_vt;
				}
			}
		}
	}
	//将得到的矩阵输入到lastvt中
	for (i = 0; i < strlen(vn); i++) {
		lastvt[i].VN = vn[i];
		for (j = 0; j < strlen(vt); j++) {
			if (matrix[i][j] == 1) {
				for (k = 0; lastvt[i].VT[k] != '\0'; k++)
					;
				lastvt[i].VT[k] = vt[j];
			}
		}
	}
	//输出每一个非终结符的LASTVT集合
	for (int i = 0; i < strlen(vn); i++)
	{
		cout << "LASTVT(";
		cout << lastvt[i].VN << ")" << "=" << "{";
		cout << lastvt[i].VT[0];
		for (j = 1; j < strlen(lastvt[i].VT); j++)
			cout << "," << lastvt[i].VT[j];
		cout << "}" << endl;
	}
}

void OPGTable::MakeTable() //构造优先关系表 
{
	int i = 0, i1 = 0, i2 = 0, t = 0, k = 0;//循环中辅助变量
	for (i = 1; i <= strlen(vt); i++)
	{
		table[0][i] = vt[i - 1]; //第一列的终结符
		table[i][0] = vt[i - 1]; //第一行的终结符 
	}
	for (int i = 0; i < production_num; i++)//遍历所有的产生式
	{
		int length = strlen(pro[i].Right);
		for (int j = 0; j < length - 1; j++)//遍历产生式的右部
		{
			//如果Xi和Xi+1均为终结符，形如P->...ab... ,置Xi = Xi+1
			if (isVT(pro[i].Right[j]) && isVT(pro[i].Right[j + 1]))
			{
				for (i1 = 0; i1 <= strlen(vt); i1++)//遍历终结符
					for (i2 = 0; i2 <= strlen(vt); i2++)
						if (table[0][i1] == pro[i].Right[j] && table[i2][0] == pro[i].Right[j + 1])//a=b
						{
							if (table[i1][i2] != 0) //说明存在冲突
							{
								cout << "该文法不是算符优先文法！" << endl;
								return;
							}
							else//赋值=
								table[i1][i2] = '=';
						}
			}
			//如果j<n-2且Xi和Xi+2都为终结符,形如P->...aQb...，置Xi = Xi+2
			if (j < length - 2 && isVT(pro[i].Right[j]) && isVT(pro[i].Right[j + 2]) && isVT(pro[i].Right[j + 1]) == 0)
			{
				for (i1 = 0; i1 <= strlen(vt); i1++)//遍历终结符
					for (i2 = 0; i2 <= strlen(vt); i2++)
						if (table[0][i1] == pro[i].Right[j] && table[i2][0] == pro[i].Right[j + 2])//a=b
						{
							if (table[i1][i2] != 0)
							{
								cout << "该文法不是算符优先文法！" << endl;
								return;
							}
							else
							{
								table[i1][i2] = '=';
							}

						}
			}
			//如果Xi为终结符而Xi+1为非终结符，形如P->...aQ...，为FIRSTVT(Q)中的每个终结符b置  a<b ；
			if (isVT(pro[i].Right[j]) && isVT(pro[i].Right[j + 1]) == 0) {
				for (i1 = 0; table[0][i1] != pro[i].Right[j]; i1++)//找到a在终结符集合中的位置i1
					;
				for (k = 0; firstvt[k].VN != pro[i].Right[j + 1]; k++)//找到Q在firtstvt数组中的位置k
					;
				for (t = 0; t < strlen(firstvt[k].VT); t++) {//遍历firstvt集合
					for (i2 = 0; i2 <= strlen(vt); i2++) {
						if (table[i2][0] == firstvt[k].VT[t])
						{
							if (table[i1][i2] != 0)
							{
								cout << "该文法不是算符优先文法！" << endl;
								return;
							}
							else
								table[i1][i2] = '<';
						}
					}
				}
			}
			//如果Xi为非终结符而Xi+1为终结符，形如P->...Qa...，为LASTVT(Q)中的每个终结符b置 b>a ；
			if (isVT(pro[i].Right[j]) == 0 && isVT(pro[i].Right[j + 1])) {
				for (t = 0; lastvt[t].VN != pro[i].Right[j]; t++)//找到Q在lastvt数组中的位置t
					;
				for (i2 = 0; table[0][i2] != pro[i].Right[j + 1]; i2++)//找到a在table中的位置i2
					;
				for (k = 0; k < strlen(lastvt[t].VT); k++)//遍历lastvt集合
					for (i1 = 0; i1 <= strlen(vt); i1++) {
						if (table[0][i1] == lastvt[t].VT[k] && table[i2][0] == pro[i].Right[j + 1])
						{
							if (table[i1][i2] != 0)
							{
								cout << "该文法不是算符优先文法！" << endl;
								return;
							}
							else
								table[i1][i2] = '>';
						}
					}
			}
		}
	}
	//输出优先关系表
	cout << "----------------------------------------------------------" << endl;
	for (int i = 0; i <= strlen(vt); i++)
	{
		for (int j = 0; j <= strlen(vt); j++)
			cout << table[i][j] << "    ";
		cout << endl;
	}
	cout << "----------------------------------------------------------" << endl;
}

char OPGTable::GetRelation(char a, char b) //找到a，b对应的关系
{
	int i, j;
	for (i = 0; table[0][i] != a; i++);
	for (j = 0; table[j][0] != b; j++);
	return table[i][j];
}

int OPGTable::Analysis(char* str) //str为输入串
{
	char a;   //读入输入串的字符
	int pos = 0;		//记录此时读到输入串哪一个位置
	char ch; //辅助临时变量
	int step = 1; //记录分析的步骤
	int top_vt, top;		//top_vt指向栈顶的终结符,top指向符号栈顶
	cout << "步骤" << "\t\t符号栈" << "\t\t输入串" << "\t\t动作" << endl;
	a = str[0];   //输入串的第一个字符赋给a
	top = 1;   //栈顶从1开始
	S[top] = '#'; //栈里面压#
	while (a != '#')
	{
		//读入
		a = str[pos++];
		//找到栈顶终结符
		if (isVT(S[top]))
			top_vt = top;
		else
			top_vt = top - 1;
		//若栈顶第一个终结符S[top_vt]优先级高于输入串第一个字符a的优先级，规约
		while (GetRelation(S[top_vt], a) == '>')
		{
			//找出最左素短语phrase
			while (1) {
				ch = S[top_vt];
				if (isVT(S[top_vt - 1]))
					top_vt = top_vt - 1;
				else
					top_vt = top_vt - 2;
				if (GetRelation(S[top_vt], ch) == '<')
					break;
			}
			string	phrase = "";
			for (int i = top_vt + 1; i <= top; i++) {
				phrase = phrase + S[i];
			}
			//规约最左素短语
			if (Match(phrase)) {
				cout << step++ << "\t\t" << S + 1 << "\t\t" << left << setw(16) << str + pos - 1 << "归约" << endl;
				for (int i = top_vt + 2; i <= top; i++)
					S[i] = 0;
				top = top_vt + 1;
				S[top] = 'N';   //栈顶的这些符号与某一个候选对应相等，就规约到N上
			}
			else {
				cout << "\n输入句子无法识别" << endl;
				return -1;
			}
		}
		//若栈顶第一个终结符S[top_vt]优先级低于或等于输入串第一个字符a的优先级，移进
		if (GetRelation(S[top_vt], a) == '<' || GetRelation(S[top_vt], a) == '=')
		{
			cout << step++ << "\t\t" << S + 1 << "\t\t" << left << setw(16) << str + pos - 1;
			if (a != '#')
			{
				cout << "移进" << endl;
			}
			top = top + 1; //移进来的a就变成了栈顶终结符
			S[top] = a;
		}
		else
		{
			cout << "\n输入句子无法识别" << endl;
			return -1;
		}
	}
	cout << "接受" << endl;
	cout << "\n恭喜您，分析成功!" << endl;
	return 1;
}