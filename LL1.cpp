#define MAX_VALUE 1001

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>

using namespace std;

/*产生式类*/
class Production
{
public:
    Production(string str);  //字符串的构造函数
    void Output();  //类成员函数：产生式输出函数
    string left;  //类成员：产生式左侧非终结符
    set<string> right;  //类成员：产生式右侧终结符集
};
Production::Production(string str)
{
    left = "";
    int len = str.length();
    int i;
    /*扫描产生式左边，复制进left中*/
    for (i = 0; i < len; i++)
    {
        if (str[i] == '-')
        {
            i += 2;
            break;
        }
        left += str[i];
    }
    /*扫描产生式右边，复制进right中*/
    string tmp = "";
    while (i < len)
    {
        if (str[i] == '|')
        {
            right.insert(tmp);
            tmp.clear();
        }
        else if (str[i] == '~')          //~表示空字epsilon
            tmp = "~";
        else
            tmp += str[i];
        i++;
    }
    right.insert(tmp);
}
void Production::Output()  
{
    cout << left << "->";
    set<string>::iterator iter = right.begin();
    if (right.begin() != right.end())
    {
        cout << iter->c_str();
        iter++;
    }
    for (; iter != right.end(); iter++)
        cout << "|" << iter->c_str();
}


vector<Production> vn_set;         //产生式（非终结符）集合
map<string, set<char>> first;     //firsr集合
map<string, set<char>> follow;    //follow集合
int vn_is_visited[MAX_VALUE] = { 0 };

void DFS(int i)
{
    if (vn_is_visited[i] == 1)   //已经访问 
        return;
    vn_is_visited[i] = 1;

    string& left = vn_set[i].left;
    set<string>& right = vn_set[i].right;
    set<string>::iterator iter = right.begin();
    for(;iter != right.end();iter++)
        for (int j = 0; j < iter->length(); j++)
        {
            if (!isupper(iter->at(j)) && iter->at(j) != '\'') //若当前不指向非终结符
            {
                first[left].insert(iter->at(j));
                break;
            }
            if (isupper(iter->at(j)))  //若当前指向非终结符
            {
                int k;
                if(j!=iter->length()-1&&iter->at(j+1)=='\'')
                    
            }
        }

    for (; it != right.end(); it++)
        for (int i = 0; i < it->length(); i++)
        {
            if (!isupper(it->at(i)) && it->at(i) != '\'')
            {
                first[left].insert(it->at(i));
                break;
            }
            if (isupper(it->at(i)))
            {
                int y;
                if (i != it->length() - 1 && it->at(i + 1) == '\'')
                    y = VN_dic[it->substr(i, 2)] - 1;
                else y = VN_dic[it->substr(i, 1)] - 1;
                string& tleft = VN_set[y].left;
                dfs(y);
                set<char>& temp = first[tleft];
                set<char>::iterator it1 = temp.begin();
                bool flag = true;
                for (; it1 != temp.end(); it1++)
                {
                    if (*it1 == '~') flag = false;
                    first[left].insert(*it1);
                }
                if (flag) break;
            }
            else continue;
        }
}

void makeFirst()
{

}

void make_first()
{
    memset(used, 0, sizeof(used));
    for (int i = 0; i < VN_set.size(); i++)
        dfs(i);
#define DEBUG
#ifdef DEBUG
    puts("***************FIRST集***********************");
    map<string, set<char> >::iterator it = first.begin();
    for (; it != first.end(); it++)
    {
        printf("FIRST(%s)={", it->first.c_str());
        set<char>& temp = it->second;
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for (; it1 != temp.end(); it1++)
        {
            if (flag) printf(",");
            printf("%c", *it1);
            flag = true;
        }
        puts("}");
    }
#endif
}


int main()
{
    cout << "==========================================" << endl;
    cout << "==== 同济大学2022年秋编译原理课程作业 ====" << endl;
    cout << "====        LL(1)语法分析程序         ====" << endl;
    cout << "====            Welcome!              ====" << endl;
    cout << "==========================================" << endl;
    cout << endl;

    while (1)
    {
        int n;
        cout << "请输入LL(1)文法产生式的数量（输入非正数退出程序）：";
        cin >> n;
        if (n <= 0)
            break;
        /*输入n个文法产生式*/
        string str;
        for (int i = 0; i < n; i++)
        {
            cin >> str;
            Production pro(str);
            vn_set.push_back(pro);
        }

    }
    return 0;
}