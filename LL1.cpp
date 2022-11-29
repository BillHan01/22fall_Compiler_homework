#define MAX_VALUE 1001

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <iomanip>

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


/*定义全局变量*/
vector<Production> vn_set;         //产生式（非终结符）集合
map<string, int> vn_dic;           //非终结符索引
map<string, set<char>> first;     //first集合
map<string, set<char>> follow;    //follow集合
int vn_is_visited[MAX_VALUE] = { 0 };
map<string, map<char, string>> table;  //LL(1)分析表
vector<char> vt_set;              //终结符集合
string start;                  //文法的开始符号
set<string> vt;
stack<string> LL1_Stack;
int Output_Zahl = 0;   //打表次数

/*每次分析前，清空所有容器中的变量*/
void cleanData()
{
    vn_set.clear();
    vn_dic.clear();
    first.clear();
    follow.clear();
    memset(vn_is_visited, 0, sizeof(vn_is_visited));
    table.clear();
    vt_set.clear();
    start.clear();
    vt.clear();
    Output_Zahl = 0;
    while (!LL1_Stack.empty())
    {
        LL1_Stack.pop();
    }
}

/*初始化visit标志数组*/
void initVisit(vector<Production>& vn_set)
{
    string left;
    for (int i = 0; i < vn_set.size(); i++)
    {
        left = vn_set[i].left;
        if (!vn_dic[left])
            vn_dic[left] = i + 1;       //索引从1开始
    }
}

/*深度优先遍历构造FIRST*/
void DFS(int i)
{
    if (vn_is_visited[i] == 1)
        return;      //当前非终结符已经访问 
    vn_is_visited[i] = 1;

    string& left = vn_set[i].left;
    set<string>& right = vn_set[i].right;
    set<string>::iterator iter = right.begin();
    for (; iter != right.end(); iter++)
        for (int j = 0; j < iter->length(); j++)
        {
            if (!isupper(iter->at(j)) && iter->at(j) != '\'') //若当前不指向非终结符
            {
                first[left].insert(iter->at(j));
                break;
            }
            if (isupper(iter->at(j)))  //若当前指向非终结符
            {
                int index;
                if (j != iter->length() - 1 && iter->at(j + 1) == '\'')  //考虑非终结符形式为 P'
                    index = vn_dic[iter->substr(j, 2)] - 1;
                else
                    index = vn_dic[iter->substr(j, 1)] - 1;
                string& left_t = vn_set[index].left;

                DFS(index);   //递归

                set<char>& tmp = first[left_t];
                set<char>::iterator iter_1 = tmp.begin();
                bool flag = true;
                for (; iter_1 != tmp.end(); iter_1++)
                {
                    if (*iter_1 == '~')
                        flag = false;
                    first[left].insert(*iter_1);
                }
                if (flag == true)
                    break;
            }
            else
                continue;
        }
}

/*构造FIRST集合*/
void createFirst()
{
    for (int i = 0; i < vn_set.size(); i++)
        DFS(i);
}
/*打印FIRST集合*/
void printFirst()
{
    cout << endl;
    cout << "******************** FIRST set ********************" << endl;
    map<string, set<char>>::iterator iter = first.begin();
    for (; iter != first.end(); iter++)
    {
        cout << "FIRST(" << iter->first.c_str() << ") = {";
        set<char>& tmp = iter->second;
        set<char>::iterator iter_t = tmp.begin();
        bool flag = false;
        for (; iter_t != tmp.end(); iter_t++)
        {
            if (flag == true)
                cout << ",";
            cout << *iter_t;
            flag = true;
        }
        cout << " }" << endl;
    }
    cout << endl;
}

/*构造FOLLOW集合过程中，将str1的FOLLOW元素加到str2中*/
void append(const string& str1, const string& str2)
{
    set<char>& s1 = follow[str1];
    set<char>& s2 = follow[str2];
    set<char>::iterator iter = s1.begin();
    for (; iter != s1.end(); iter++)
        s2.insert(*iter);
}

/*构造FOLLOW集合*/
void createFollow()
{
    follow[start].insert('#');  //往文法开始符号的FOLLOW集中加'#'
    while (1)
    {
        int stopTag = 0;  //终止循环遍历标志
        for (int i = 0; i < vn_set.size(); i++)
        {
            string& left = vn_set[i].left;
            set<string>& right = vn_set[i].right;
            set<string>::iterator iter = right.begin();
            for (; iter != right.end(); iter++)
            {
                int flag = 1; //判断是否把产生式左部的FOLLOW集合元素加到产生式右部非终结符的FOLLOW集合中
                const string& str = *iter;
                for (int j = iter->length() - 1; j >= 0; j--)
                {
                    if (str[j] == '\'')
                    {
                        //若为P'非终结符
                        int index = vn_dic[iter->substr(j - 1, 2)] - 1;
                        if (flag == 1)
                        {
                            int fsize_before = follow[iter->substr(j - 1, 2)].size();
                            append(left, iter->substr(j - 1, 2));  //把产生式左部的FOLLOW集合元素加到产生式右部P'的FOLLOW集合中
                            int fsize_after = follow[iter->substr(j - 1, 2)].size();
                            if (fsize_after > fsize_before)
                                stopTag = 1;       //FOLLOW发生了变化，需要继续循环遍历
                            if (!vn_set[index].right.count("~"))
                                flag = 0;          //P'不含空字，产生式右部P'前面的非终结符，均不进行把产生式左部的FOLLOW集合元素加到产生式右部非终结符的FOLLOW集合中的操作
                        }

                        for (int k = j + 1; k < iter->length(); k++)
                        {
                            if (isupper(str[k]))
                            {
                                string vn;
                                if (k != iter->length() - 1 && str[k + 1] == '\'')
                                    vn = iter->substr(k, 2);
                                else
                                    vn = iter->substr(k, 1);

                                set<char>& from = first[vn];
                                set<char>& to = follow[iter->substr(j - 1, 2)];
                                int fsize_before = to.size();
                                set<char>::iterator it1 = from.begin();
                                for (; it1 != from.end(); it1++)
                                {
                                    if (*it1 != '~')
                                        to.insert(*it1);           //把后一个非终结符FIRST集合的非空字加入P'的FOLLOW集合
                                }
                                int fsize_after = follow[iter->substr(j - 1, 2)].size();
                                if (fsize_after > fsize_before)
                                    stopTag = true;      //FOLLOW发生了变化，需要继续循环遍历

                                if (!vn_set[vn_dic[vn] - 1].right.count("~"))
                                    break;         //遍历到当前非终结符已经不含空字，停止向后遍历
                            }
                            else if (str[k] != '\'')
                            {
                                int fsize_before = follow[iter->substr(j - 1, 2)].size();
                                follow[iter->substr(j - 1, 2)].insert(str[k]);  //若当前为终结符，直接加入P'的FOLLOW集合
                                int fsize_after = follow[iter->substr(j - 1, 2)].size();
                                if (fsize_after > fsize_before)
                                    stopTag = true;
                                break;
                            }
                        }
                        j--;
                    }
                    else if (isupper(str[j]))
                    {
                        //若为P非终结符
                        int index = vn_dic[iter->substr(j, 1)] - 1;
                        if (flag == 1)
                        {
                            int fsize_before = follow[iter->substr(j, 1)].size();
                            append(left, iter->substr(j, 1));   //把产生式左部的FOLLOW集合元素加到产生式右部P的FOLLOW集合中
                            if (!vn_set[index].right.count("~"))
                                flag = false;
                            int fsize_after = follow[iter->substr(j, 1)].size();
                            if (fsize_after > fsize_before)
                                stopTag = true;
                        }
                        for (int k = j + 1; k < iter->length(); k++)
                        {
                            if (isupper(str[k]))
                            {
                                string vn;
                                if (k != iter->length() - 1 && str[k + 1] == '\'')
                                    vn = iter->substr(k, 2);
                                else
                                    vn = iter->substr(k, 1);

                                set<char>& from = first[vn];
                                set<char>& to = follow[iter->substr(j, 1)];
                                set<char>::iterator it1 = from.begin();
                                int tt = follow[iter->substr(j, 1)].size();
                                for (; it1 != from.end(); it1++)
                                {
                                    if (*it1 != '~')
                                        to.insert(*it1);       //把后一个非终结符FIRST集合的非空字加入P'的FOLLOW集合
                                }

                                int tt1 = follow[iter->substr(j, 1)].size();
                                if (tt1 > tt) stopTag = true;
                                if (!vn_set[vn_dic[vn] - 1].right.count("~"))
                                    break;
                            }
                            else if (str[k] != '\'')
                            {
                                int fsize_before = follow[iter->substr(j, 1)].size();
                                follow[iter->substr(j, 1)].insert(str[k]);  //若当前为终结符，直接加入P'的FOLLOW集合
                                int fsize_after = follow[iter->substr(j, 1)].size();
                                if (fsize_after > fsize_before)
                                    stopTag = true;
                                break;
                            }
                        }
                    }
                    else
                        flag = 0; //当前为终结符
                }
            }
        }
        if (stopTag == 0)
            break;     //遍历最后一轮后FOLLOW集合均不发生变化，则退出循环
    }
}

/*打印FOLLOW集合*/
void printFollow()
{
    cout << endl;
    cout << "******************** FOLLOW set ********************" << endl;
    map<string, set<char> >::iterator iter = follow.begin();
    for (; iter != follow.end(); iter++)
    {
        cout << "FOLLOW(" << iter->first.c_str() << ")={ ";
        set<char>& tmp = iter->second;
        //tmp.insert('#');
        set<char>::iterator iter_t = tmp.begin();
        bool flag = false;
        for (; iter_t != tmp.end(); iter_t++)
        {
            if (flag == 1)
                cout << ",";
            cout << *iter_t;
            flag = 1;
        }
        cout << " }" << endl;
    }
    cout << endl;
}


/*检查字符是否属于字符串的FIRST集合*/
int belongToFirst(char ch, const string& str)
{
    for (int i = 0; i < str.length(); i++)
    {
        int containEpsilon = 0; //是否包含空字
        if (!isupper(str[i]) && str[i] != '\'')
        {
            //为终结符，除非该终结符为ch，否则返回0
            if (str[i] == ch)
                return 1;
            else
                return 0;
        }
        else if (isupper(str[i]))
        {
            string tmp;
            if (i != str.length() - 1 && str[i + 1] == '\'')
                tmp = str.substr(i, 2);       //处理诸如 P' 的非终结符
            else
                tmp = str.substr(i, 1);       //一般的非终结符
            set<char>::iterator iter = first[tmp].begin(); //遍历非终结符FIRST集
            for (; iter != first[tmp].end(); iter++)
            {
                if (*iter == '~')
                    containEpsilon = 1;
                if (*iter == ch)
                    return 1;
            }
            if (!containEpsilon)
                break;           //不含有空字，则不可能再找到，退出循环
        }
    }
    return 0;
}

/*检查字符是否属于字符串的FOLLOW集合*/
int belongToFollow(char ch, const string& str)
{
    set<char>::iterator iter = follow[str].begin();
    for (; iter != follow[str].end(); iter++)
    {
        if (*iter == ch)
            return 1;
    }
    return 0;
}

void createTable()
{
    map<char, string> tmp; //单个非终结符对应的一行表

    /*构造终结符集合vn_set*/
    bool is_visited[1001] = {};
    for (int i = 0; i < vn_set.size(); i++)
    {
        set<string>& right = vn_set[i].right;
        set<string>::iterator iter = right.begin();
        for (; iter != right.end(); iter++)
            for (int j = 0; j < iter->length(); j++)
                if (!isupper(iter->at(j)) && iter->at(j) != '\'' && iter->at(j) != '~')
                {
                    if (is_visited[iter->at(j)])
                        continue;   //已经在集合中
                    is_visited[iter->at(j)] = true;
                    vt_set.push_back(iter->at(j));  //加入终结符集合
                }
    }
    vt_set.push_back('#');  //把结束标志#加到终结符集合中

    /*构造LL(1)分析表主过程*/
    for (int i = 0; i < vn_set.size(); i++)
    {
        tmp.clear();
        string& left = vn_set[i].left;
        set<string>& right = vn_set[i].right;
        set<string>::iterator it = right.begin();
        for (; it != right.end(); it++)
            for (int j = 0; j < vt_set.size(); j++)
            {
                //cout << *it << " " <<  vt_set[j] << endl;
                if (belongToFirst(vt_set[j], *it))
                    tmp[vt_set[j]] = *it;             //对 P->α，若a属于FIRST(α)，则置[P,a]为α

                if (it->at(0) == '~' && belongToFollow(vt_set[j], left))
                    tmp[vt_set[j]] = *it;        //若b属于FOLLOW(P)，置[P,b]为空字
            }
        table[vn_set[i].left] = tmp;  //完成该非终结符对应行的构造
    }

    /*打印LL(1)分析表*/
    cout << endl;
    cout << "**************** LL1 Analysis Table ****************" << endl;
    for (int i = 0; i < (vt_set.size() + 1) * 10; i++)
        cout << "-";
    cout << endl;
    cout << "|        |";
    for (int i = 0; i < vt_set.size(); i++)
        cout << "    " << vt_set[i] << "    |";
    cout << endl;
    for (int i = 0; i < (vt_set.size() + 1) * 10; i++)
        cout << "-";
    cout << endl;
    for (int i = 0; i < vn_set.size(); i++)
    {
        printf("|%5s%4s", vn_set[i].left.c_str(), "|");
        for (int j = 0; j < vt_set.size(); j++)
        {
            if (table[vn_set[i].left].count(vt_set[j]))
                printf("%7s%3s", table[vn_set[i].left][vt_set[j]].c_str(), "|");
            else
                cout << "         |";
        }
        cout << endl;
        for (int i = 0; i < (vt_set.size() + 1) * 10; i++)
            cout << "-";
        cout << endl;
    }
    cout << endl;
}


/*把产生式右边拆分成单个符号*/
void splitProduction(string vn)
{
    string new_vn(vn.rbegin(), vn.rend());   //需要倒序入栈
    string::iterator p = new_vn.begin();

    while (p != new_vn.end())
    {
        if (*p == '\'')     //特判'的情况
        {
            string temp(1, *(p + 1));
            temp += *(p);
            LL1_Stack.push(temp);
            p += 2;
            continue;
        }
        else
        {
            string temp(1, *p);
            LL1_Stack.push(temp);
            p++;
            continue;
        }
    }
    return;
}

/*构造vt集合*/
void vt_initialisieren()
{
    for (int i = 0; i < vt_set.size(); i++)
    {
        vt.insert(string(1, vt_set[i]));
    }
}

/*栈的输出*/
void outputStack(string& X, string::iterator p)
{
    stack<string> LL1_copy = LL1_Stack;
    int size = 0;
    stack<string> LL1_Output;
    cout <<  setw(2) << Output_Zahl;
    cout << " ";
    Output_Zahl++;
    while (!LL1_copy.empty())
    {
        LL1_Output.push(LL1_copy.top());
        LL1_copy.pop();

    }
    while (!LL1_Output.empty())
    {
        cout << LL1_Output.top();
        size += LL1_Output.top().size();
        LL1_Output.pop();
    }
    cout  << setw(30 - size) << "" ;
    int length_Ausdruck = 0;
    char p_value = *p;
    while (*p != '\0')
    {
        cout << *p;
        length_Ausdruck++;
        p++;
    }
    cout << setw(30 - length_Ausdruck) << "";
    if (table[X][p_value] != "")
        cout << X << "->" << table[X][p_value];
    cout << endl;
    return;
}

//输入LL(1)文法进行语法分析
void analyseLL1()
{
    cout << "请输入要分析的句子：" << endl;
    string Next_LL1;    //即将输入的LL(1)文法
    cin >> Next_LL1;
    cout << endl << setw(5) << "栈" << setw(32) << "输入" << setw(32) << "产生式" << endl << endl;
    Next_LL1 += "#";        //添加#
    Next_LL1 += '\0';
    LL1_Stack.push("#");    //加入#
    LL1_Stack.push(vn_set[0].left);   //加入首个非终结符
    vt_initialisieren();
    string::iterator p = Next_LL1.begin();
    string X;
    outputStack(X, p);
    while (p != Next_LL1.end())
    {
        X = LL1_Stack.top();   //取栈顶元素
        if (vt.find(X) != vt.end() && X != "#")    //X为终结符且不为#
        {
            if (string(1, *p) == X) //匹配成功
            {
                p++;
                LL1_Stack.pop();    //指针后移，且栈顶元素出栈
                outputStack(X, p);
                continue;
            }
            else                       //栈顶终结符与指针终结符不同，报错
            {
                cout << "ERROR!" << endl;
                return;
            }
        }
        else if (X == "#")
        {
            if (string(1, *p) == X)  //栈顶及指针均为#，匹配成功
            {
                cout << "Erfolg!" << endl;
                break;
            }
            else                    //指针指向#，栈顶不是#，报错
            {
                cout << "ERROR!" << endl;
                return;
            }
        }
        else if (table[X][*p] != "")   //X为非终结符，找表格
        {
            if (table[X][*p] == "~") //空字特判
            {
                LL1_Stack.pop();        //直接出栈
                outputStack(X, p);       //打表
                continue;
            }
            else                     //不为空字情况
            {
                LL1_Stack.pop();
                splitProduction(table[X][*p]);  //表格内产生式右侧进栈
                outputStack(X, p);               //打表
                continue;
            }
        }
        else if (table[X][*p] == "")           //表格为空，报错
        {
            cout << "ERROR!" << endl;
            return;
        }
    }
    return;
}


int main()
{
    while (1)
    {
        system("cls");
        cout << "==========================================" << endl;
        cout << "==== 同济大学2022年秋编译原理课程作业 ====" << endl;
        cout << "====        LL(1)语法分析程序         ====" << endl;
        cout << "====            Welcome!              ====" << endl;
        cout << "==========================================" << endl;
        cout << endl;
        cleanData();
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
            if (i == 0)
                start = pro.left;   //默认第一个输入的产生式左部为文法开始符号
        }
        initVisit(vn_set);
        createFirst();
        printFirst();
        createFollow();
        printFollow();
        createTable();
        analyseLL1();
        system("pause");

    }
    return 0;
}

/*
测试案例1：
5
E->TE'
E'->+TE'|~
T->FT'
T'->*FT'|~
F->(E)|i
*/

