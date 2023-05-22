#define _CRT_SECURE_NO_WARNINGS
#include"OPGTable.h"
int main()
{
	OPGTable s;
	char sentence[100];//存放待检测的字符串
	char filename[100];//存放算符优先文法的文件名
	s.ReadProduction(filename);
	cout << "各非终结符的FIRSTVT集合如下：" << endl;
	s.FIRSTVT();
	cout << "\n各非终结符的LASTVT集合如下：" << endl;
	s.LASTVT();
	cout << "\n构造分析表如下:" << endl;
	s.MakeTable();
	do {
		cout << "\n请任意输入一个输入串(以#号键结束)：" << endl;
		cin >> sentence;
		cout << "\n分析表过程:" << endl;
		s.Analysis(sentence);
	} while (strlen(sentence));
	return 0;
}
