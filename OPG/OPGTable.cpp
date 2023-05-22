#define _CRT_SECURE_NO_WARNINGS
#include "OPGTable.h"
#include<iomanip>
#include<stack>
#include<string>

void OPGTable::GetVT()
{
	//�ҵ����е��ս�������vt�ַ�������
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

//������ս���ͷ���1�������������0
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
	char production[100] = { 0 };//���ڴ��һ������ʽ
	cout << "����������������ķ����ļ�����";
	cin >> filename;
	ifstream fin(filename);
	if (!fin)
	{
		cout << "Cannot open the file.\n";
		exit(1);
	}
	cout << "��������ķ�Ϊ����һ������ʽΪ��������ʽ����" << endl;
	while (fin)
	{
		fin.getline(production, 100); //����һ������ʽ
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
	bool matrix[50][50]; //��ά��������F[P,a]
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			matrix[i][j] = 0;
		}
	}
	char s_vt[100];
	char s_vn[100];  //ʹ������ջ��ͬ�洢һ���ս����һ�����ս��,�����ʹFIRSTVT Ϊ��ķ���ջ
	int top_vt = 0, top_vn = 0; //ָ����������ջ��ջ��
	int  i, j, k; //����ѭ������
	int num = 1; //��¼ɨ���Ĳ���ʽ����
	char temp_vt, temp_vn;
	//��һ��ɨ���ҵ�P->a...��P->Qa...��ʽ�Ĳ���ʽ�������г�ֵΪ�������Ԫ��F[P, a]�ķ��Ŷ�(P, a)ȫ������STACK֮�С�
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
	//����ջ�Ͳ������󲻶Ϸ���:���ջSTACK���գ�ջ����ջ���Ǵ���Ϊ(Q, a)
	//����ÿ������P��Q���Ĳ���ʽ����F[P, a]Ϊ�٣������ֵΪ�棬�ҽ�(P, a)�ƽ�STACKջ��ֱ��ջ�շ������
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
	//���õ��ľ������뵽firstvt��
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
	//����������ս����firstvt����
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
	char s_vn[100];  //ʹ������ջ��ͬ�洢һ���ս����һ�����ս��������ջ
	int top_vt = 0, top_vn = 0; //ָ����������ջ��ջ��
	int num = 1; //��¼ɨ���Ĳ���ʽ����
	char temp_vt, temp_vn;
	int i, j, k;  //��������

	//��һ��ɨ���ҵ�P->...a��P->...aQ��ʽ�Ĳ���ʽ,����lastvt��Ӧ��ջ
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
	//����ջ�Ͳ������󲻶Ϸ���
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
	//���õ��ľ������뵽lastvt��
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
	//���ÿһ�����ս����LASTVT����
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

void OPGTable::MakeTable() //�������ȹ�ϵ�� 
{
	int i = 0, i1 = 0, i2 = 0, t = 0, k = 0;//ѭ���и�������
	for (i = 1; i <= strlen(vt); i++)
	{
		table[0][i] = vt[i - 1]; //��һ�е��ս��
		table[i][0] = vt[i - 1]; //��һ�е��ս�� 
	}
	for (int i = 0; i < production_num; i++)//�������еĲ���ʽ
	{
		int length = strlen(pro[i].Right);
		for (int j = 0; j < length - 1; j++)//��������ʽ���Ҳ�
		{
			//���Xi��Xi+1��Ϊ�ս��������P->...ab... ,��Xi = Xi+1
			if (isVT(pro[i].Right[j]) && isVT(pro[i].Right[j + 1]))
			{
				for (i1 = 0; i1 <= strlen(vt); i1++)//�����ս��
					for (i2 = 0; i2 <= strlen(vt); i2++)
						if (table[0][i1] == pro[i].Right[j] && table[i2][0] == pro[i].Right[j + 1])//a=b
						{
							if (table[i1][i2] != 0) //˵�����ڳ�ͻ
							{
								cout << "���ķ�������������ķ���" << endl;
								return;
							}
							else//��ֵ=
								table[i1][i2] = '=';
						}
			}
			//���j<n-2��Xi��Xi+2��Ϊ�ս��,����P->...aQb...����Xi = Xi+2
			if (j < length - 2 && isVT(pro[i].Right[j]) && isVT(pro[i].Right[j + 2]) && isVT(pro[i].Right[j + 1]) == 0)
			{
				for (i1 = 0; i1 <= strlen(vt); i1++)//�����ս��
					for (i2 = 0; i2 <= strlen(vt); i2++)
						if (table[0][i1] == pro[i].Right[j] && table[i2][0] == pro[i].Right[j + 2])//a=b
						{
							if (table[i1][i2] != 0)
							{
								cout << "���ķ�������������ķ���" << endl;
								return;
							}
							else
							{
								table[i1][i2] = '=';
							}

						}
			}
			//���XiΪ�ս����Xi+1Ϊ���ս��������P->...aQ...��ΪFIRSTVT(Q)�е�ÿ���ս��b��  a<b ��
			if (isVT(pro[i].Right[j]) && isVT(pro[i].Right[j + 1]) == 0) {
				for (i1 = 0; table[0][i1] != pro[i].Right[j]; i1++)//�ҵ�a���ս�������е�λ��i1
					;
				for (k = 0; firstvt[k].VN != pro[i].Right[j + 1]; k++)//�ҵ�Q��firtstvt�����е�λ��k
					;
				for (t = 0; t < strlen(firstvt[k].VT); t++) {//����firstvt����
					for (i2 = 0; i2 <= strlen(vt); i2++) {
						if (table[i2][0] == firstvt[k].VT[t])
						{
							if (table[i1][i2] != 0)
							{
								cout << "���ķ�������������ķ���" << endl;
								return;
							}
							else
								table[i1][i2] = '<';
						}
					}
				}
			}
			//���XiΪ���ս����Xi+1Ϊ�ս��������P->...Qa...��ΪLASTVT(Q)�е�ÿ���ս��b�� b>a ��
			if (isVT(pro[i].Right[j]) == 0 && isVT(pro[i].Right[j + 1])) {
				for (t = 0; lastvt[t].VN != pro[i].Right[j]; t++)//�ҵ�Q��lastvt�����е�λ��t
					;
				for (i2 = 0; table[0][i2] != pro[i].Right[j + 1]; i2++)//�ҵ�a��table�е�λ��i2
					;
				for (k = 0; k < strlen(lastvt[t].VT); k++)//����lastvt����
					for (i1 = 0; i1 <= strlen(vt); i1++) {
						if (table[0][i1] == lastvt[t].VT[k] && table[i2][0] == pro[i].Right[j + 1])
						{
							if (table[i1][i2] != 0)
							{
								cout << "���ķ�������������ķ���" << endl;
								return;
							}
							else
								table[i1][i2] = '>';
						}
					}
			}
		}
	}
	//������ȹ�ϵ��
	cout << "----------------------------------------------------------" << endl;
	for (int i = 0; i <= strlen(vt); i++)
	{
		for (int j = 0; j <= strlen(vt); j++)
			cout << table[i][j] << "    ";
		cout << endl;
	}
	cout << "----------------------------------------------------------" << endl;
}

char OPGTable::GetRelation(char a, char b) //�ҵ�a��b��Ӧ�Ĺ�ϵ
{
	int i, j;
	for (i = 0; table[0][i] != a; i++);
	for (j = 0; table[j][0] != b; j++);
	return table[i][j];
}

int OPGTable::Analysis(char* str) //strΪ���봮
{
	char a;   //�������봮���ַ�
	int pos = 0;		//��¼��ʱ�������봮��һ��λ��
	char ch; //������ʱ����
	int step = 1; //��¼�����Ĳ���
	int top_vt, top;		//top_vtָ��ջ�����ս��,topָ�����ջ��
	cout << "����" << "\t\t����ջ" << "\t\t���봮" << "\t\t����" << endl;
	a = str[0];   //���봮�ĵ�һ���ַ�����a
	top = 1;   //ջ����1��ʼ
	S[top] = '#'; //ջ����ѹ#
	while (a != '#')
	{
		//����
		a = str[pos++];
		//�ҵ�ջ���ս��
		if (isVT(S[top]))
			top_vt = top;
		else
			top_vt = top - 1;
		//��ջ����һ���ս��S[top_vt]���ȼ��������봮��һ���ַ�a�����ȼ�����Լ
		while (GetRelation(S[top_vt], a) == '>')
		{
			//�ҳ������ض���phrase
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
			//��Լ�����ض���
			if (Match(phrase)) {
				cout << step++ << "\t\t" << S + 1 << "\t\t" << left << setw(16) << str + pos - 1 << "��Լ" << endl;
				for (int i = top_vt + 2; i <= top; i++)
					S[i] = 0;
				top = top_vt + 1;
				S[top] = 'N';   //ջ������Щ������ĳһ����ѡ��Ӧ��ȣ��͹�Լ��N��
			}
			else {
				cout << "\n��������޷�ʶ��" << endl;
				return -1;
			}
		}
		//��ջ����һ���ս��S[top_vt]���ȼ����ڻ�������봮��һ���ַ�a�����ȼ����ƽ�
		if (GetRelation(S[top_vt], a) == '<' || GetRelation(S[top_vt], a) == '=')
		{
			cout << step++ << "\t\t" << S + 1 << "\t\t" << left << setw(16) << str + pos - 1;
			if (a != '#')
			{
				cout << "�ƽ�" << endl;
			}
			top = top + 1; //�ƽ�����a�ͱ����ջ���ս��
			S[top] = a;
		}
		else
		{
			cout << "\n��������޷�ʶ��" << endl;
			return -1;
		}
	}
	cout << "����" << endl;
	cout << "\n��ϲ���������ɹ�!" << endl;
	return 1;
}