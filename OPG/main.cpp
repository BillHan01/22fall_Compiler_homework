#define _CRT_SECURE_NO_WARNINGS
#include"OPGTable.h"
int main()
{
	OPGTable s;
	char sentence[100];//��Ŵ������ַ���
	char filename[100];//�����������ķ����ļ���
	s.ReadProduction(filename);
	cout << "�����ս����FIRSTVT�������£�" << endl;
	s.FIRSTVT();
	cout << "\n�����ս����LASTVT�������£�" << endl;
	s.LASTVT();
	cout << "\n�������������:" << endl;
	s.MakeTable();
	do {
		cout << "\n����������һ�����봮(��#�ż�����)��" << endl;
		cin >> sentence;
		cout << "\n���������:" << endl;
		s.Analysis(sentence);
	} while (strlen(sentence));
	return 0;
}
