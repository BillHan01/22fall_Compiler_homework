/*LL(1) 语法分析器生成程序*/
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


/*定义全局变量*/
vector<Production> vn_set;         //产生式（非终结符）集合
map<string, int> vn_dic;           //非终结符索引
map<string, set<char>> first;     //first集合
map<string, set<char>> follow;    //follow集合
int vn_is_visited[MAX_VALUE] = { 0 };
//vector<map<char, string>> table;  //LL(1)分析表
map<string, map<char, string>> table;  //LL(1)分析表
//vector<char> letter;        //存储终结符
vector<char> vt_set;              //终结符集合
string start;                  //文法的开始符号


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
                int flag = 1;
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
                            append(left, iter->substr(j - 1, 2));
                            int fsize_after = follow[iter->substr(j - 1, 2)].size();
                            if (fsize_after > fsize_before) 
                                stopTag = 1;       //FOLLOW发生了变化，需要继续循环遍历
                            if (!vn_set[index].right.count("~"))
                                flag = false;
                        }
                        
                        for (int k = j + 1; k < iter->length(); k++)
                        {
                            if (isupper(str[k]))
                            {
                                string id;
                                if (k != iter->length() - 1 && str[k + 1] == '\'')
                                    id = iter->substr(k, 2);
                                else 
                                    id = iter->substr(k, 1);
                                set<char>& from = first[id];
                                set<char>& to = follow[iter->substr(j - 1, 2)];
                                int tt = to.size();
                                set<char>::iterator it1 = from.begin();
                                for (; it1 != from.end(); it1++)
                                    if (*it1 != '~')
                                        to.insert(*it1);
                                int tt1 = follow[iter->substr(j - 1, 2)].size();
                                if (tt1 > tt) stopTag = true;
                                if (!vn_set[vn_dic[id] - 1].right.count("~"))
                                    break;
                            }
                            else if (str[k] != '\'')
                            {
                                int tt = follow[iter->substr(j - 1, 2)].size();
                                follow[iter->substr(j - 1, 2)].insert(str[k]);
                                int tt1 = follow[iter->substr(j - 1, 2)].size();
                                if (tt1 > tt)
                                    stopTag = true;
                                break;
                            }
                            else 
                                continue;
                        }
                        j--;
                    }
                    else if (isupper(str[j]))
                    {
                        int x = vn_dic[iter->substr(j, 1)] - 1;
                        if (flag)
                        {
                            int tt = follow[iter->substr(j, 1)].size();
                            append(left, iter->substr(j, 1));
                            if (!vn_set[x].right.count("~"))
                                flag = false;
                            int tt1 = follow[iter->substr(j, 1)].size();
                            if (tt1 > tt) 
                                stopTag = true;
                        }
                        for (int k = j + 1; k < iter->length(); k++)
                        {
                            if (isupper(str[k]))
                            {
                                string id;
                                if (k != iter->length() - 1 && str[k + 1] == '\'')
                                    id = iter->substr(k, 2);
                                else id = iter->substr(k, 1);
                                set<char>& from = first[id];
                                set<char>& to = follow[iter->substr(j, 1)];
                                set<char>::iterator it1 = from.begin();
                                int tt = follow[iter->substr(j, 1)].size();
                                for (; it1 != from.end(); it1++)
                                    if (*it1 != '~')
                                        to.insert(*it1);
                                int tt1 = follow[iter->substr(j, 1)].size();
                                if (tt1 > tt) stopTag = true;
                                if (!vn_set[vn_dic[id] - 1].right.count("~"))
                                    break;
                            }
                            else if (str[k] != '\'')
                            {
                                int tt = follow[iter->substr(j, 1)].size();
                                follow[iter->substr(j, 1)].insert(str[k]);
                                int tt1 = follow[iter->substr(j, 1)].size();
                                if (tt1 > tt) 
                                    stopTag = true;
                                break;
                            }
                            else 
                                continue;
                        }
                    }
                    else flag = false;
                }
            }
        }
        if (stopTag == 0)
            break;
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
                /*
                if (!isupper(iter->at(j)) && iter->at(j) != '\'')
                {
                    if (is_visited[iter->at(j)]) 
                        continue;
                    is_visited[iter->at(j)] = true;
                    vt_set.push_back(iter->at(j));
                }*/
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
                
                if (it->at(0) == '~' && belongToFollow(vt_set[j],left))
                    tmp[vt_set[j]] = *it;        //若b属于FOLLOW(P)，置[P,b]为空字
            }
        //table.push_back(tmp);
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
            //if (table[i].count(vt_set[j]))
              //  printf("%7s%3s", table[i][vt_set[j]].c_str(), "|");
            if(table[vn_set[i].left].count(vt_set[j]))
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
