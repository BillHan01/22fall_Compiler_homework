#define MAX_VALUE 1001

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include<stack>
#include<algorithm>
using namespace std;

/*����ʽ��*/
class Production
{
public:
    Production(string str);  //�ַ����Ĺ��캯��
    void Output();  //���Ա����������ʽ�������
    string left;  //���Ա������ʽ�����ս��
    set<string> right;  //���Ա������ʽ�Ҳ��ս����
};
Production::Production(string str)
{
    left = "";
    int len = str.length();
    int i;
    /*ɨ�����ʽ��ߣ����ƽ�left��*/
    for (i = 0; i < len; i++)
    {
        if (str[i] == '-')
        {
            i += 2;
            break;
        }
        left += str[i];
    }
    /*ɨ�����ʽ�ұߣ����ƽ�right��*/
    string tmp = "";
    while (i < len)
    {
        if (str[i] == '|')
        {
            right.insert(tmp);
            tmp.clear();
        }
        else if (str[i] == '~')          //~��ʾ����epsilon
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


/*����ȫ�ֱ���*/
vector<Production> vn_set;         //����ʽ�����ս��������
map<string, int> vn_dic;           //���ս������
map<string, set<char>> first;     //firsr����
map<string, set<char>> follow;    //follow����
set<string> vt;
int vn_is_visited[MAX_VALUE] = { 0 };
map<string, map<char, string>> table;  //LL(1)������
vector<char> vt_set;              //�ս������
stack<string> LL1_Stack;


/*ÿ�η���ǰ��������������еı���*/
void cleanData()
{
   
    vn_set.clear();
    vn_dic.clear();
    first.clear();
    follow.clear();
    memset(vn_is_visited, 0, sizeof(vn_is_visited));
}

/*��ʼ��visit��־����*/
void initVisit(vector<Production>& vn_set)
{
    string left;
    for (int i = 0; i < vn_set.size(); i++)
    {
        left = vn_set[i].left;
        if (!vn_dic[left])
            vn_dic[left] = i + 1;       //������1��ʼ
    }
}

/*������ȱ�������FIRST*/
void DFS(int i)
{
    if (vn_is_visited[i] == 1)   //�Ѿ����� 
        return;
    vn_is_visited[i] = 1;

    string& left = vn_set[i].left;
    set<string>& right = vn_set[i].right;
    set<string>::iterator iter = right.begin();
    for(;iter != right.end();iter++)
        for (int j = 0; j < iter->length(); j++)
        {
            if (!isupper(iter->at(j)) && iter->at(j) != '\'') //����ǰ��ָ����ս��
            {
                first[left].insert(iter->at(j));
                break;
            }
            if (isupper(iter->at(j)))  //����ǰָ����ս��
            {
                int k;
                if (j != iter->length() - 1 && iter->at(j + 1) == '\'')  //���Ƿ��ս����ʽΪ P'
                    k = vn_dic[iter->substr(j, 2)] - 1;
                else
                    k = vn_dic[iter->substr(j, 1)] - 1;
                string& left_t = vn_set[k].left;

                DFS(k);

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

/*���첢���FIRST���Ϲ��ܺ���*/
void createFirst()
{
    for (int i = 0; i < vn_set.size(); i++)
        DFS(i);

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

/*����FOLLOW���Ϲ����У���str1��FOLLOWԪ�ؼӵ�str2��*/
void append(const string& str1, const string& str2)
{
    set<char>& s1 = follow[str1];
    set<char>& s2 = follow[str2];
    set<char>::iterator iter = s1.begin();
    for (; iter != s1.end(); iter++)
        s2.insert(*iter);
}

/*���첢���FOLLOW���Ϲ��ܺ���*/
void createFollow()
{
    while (1)
    {
        int goon = 0;
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
                        int index = vn_dic[iter->substr(j - 1, 2)] - 1;
                        if (flag == 1)
                        {
                            int tt = follow[iter->substr(j - 1, 2)].size();
                            append(left, iter->substr(j - 1, 2));
                            int tt1 = follow[iter->substr(j - 1, 2)].size();
                            if (tt1 > tt) 
                                goon = true;
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
                                else id = iter->substr(k, 1);
                                set<char>& from = first[id];
                                set<char>& to = follow[iter->substr(j - 1, 2)];
                                int tt = to.size();
                                set<char>::iterator it1 = from.begin();
                                for (; it1 != from.end(); it1++)
                                    if (*it1 != '~')
                                        to.insert(*it1);
                                int tt1 = follow[iter->substr(j - 1, 2)].size();
                                if (tt1 > tt) goon = true;
                                if (!vn_set[vn_dic[id] - 1].right.count("~"))
                                    break;
                            }
                            else if (str[k] != '\'')
                            {
                                int tt = follow[iter->substr(j - 1, 2)].size();
                                follow[iter->substr(j - 1, 2)].insert(str[k]);
                                int tt1 = follow[iter->substr(j - 1, 2)].size();
                                if (tt1 > tt)
                                    goon = true;
                                break;
                            }
                            else continue;
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
                                goon = true;
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
                                if (tt1 > tt) goon = true;
                                if (!vn_set[vn_dic[id] - 1].right.count("~"))
                                    break;
                            }
                            else if (str[k] != '\'')
                            {
                                int tt = follow[iter->substr(j, 1)].size();
                                follow[iter->substr(j, 1)].insert(str[k]);
                                int tt1 = follow[iter->substr(j, 1)].size();
                                if (tt1 > tt) 
                                    goon = true;
                                break;
                            }
                            else continue;
                        }
                    }
                    else flag = false;
                }
            }
        }
        if (goon == 0)
            break;
    }

    cout << endl;
    cout << "******************** FOLLOW set ********************" << endl;
    map<string, set<char> >::iterator iter = follow.begin();
    for (; iter != follow.end(); iter++)
    {
        cout << "FOLLOW(" << iter->first.c_str() << ")={ ";
        set<char>& tmp = iter->second;
        tmp.insert('#');
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


/*����ַ��Ƿ������ַ�����FIRST����*/
int belongToFirst(char ch, const string& str)
{
    for (int i = 0; i < str.length(); i++)
    {
        int containEpsilon = 0; //�Ƿ��������
        if (!isupper(str[i]) && str[i] != '\'')
        {
            //Ϊ�ս�������Ǹ��ս��Ϊch�����򷵻�0
            if (str[i] == ch)
                return 1;
            else
                return 0;
        }
        else if (isupper(str[i]))
        {
            string tmp;
            if (i != str.length() - 1 && str[i + 1] == '\'')
                tmp = str.substr(i, 2);       //�������� P' �ķ��ս��
            else
                tmp = str.substr(i, 1);       //һ��ķ��ս��
            set<char>::iterator iter = first[tmp].begin(); //�������ս��FIRST��
            for (; iter != first[tmp].end(); iter++)
            {
                if (*iter == '~')
                    containEpsilon = 1;
                if (*iter == ch)
                    return 1;
            }
            if (!containEpsilon)
                break;           //�����п��֣��򲻿������ҵ����˳�ѭ��
        }
    }
    return 0;
}

/*����ַ��Ƿ������ַ�����FOLLOW����*/
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
    map<char, string> tmp; //�������ս����Ӧ��һ�б�

    /*�����ս������vn_set*/
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
                        continue;   //�Ѿ��ڼ�����
                    is_visited[iter->at(j)] = true;
                    vt_set.push_back(iter->at(j));  //�����ս������
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
    vt_set.push_back('#');  //�ѽ�����־#�ӵ��ս��������

    /*����LL(1)������������*/
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
                    tmp[vt_set[j]] = *it;             //�� P->������a����FIRST(��)������[P,a]Ϊ��

                if (it->at(0) == '~' && belongToFollow(vt_set[j], left))
                    tmp[vt_set[j]] = *it;        //��b����FOLLOW(P)����[P,b]Ϊ����
            }
        
            //table.push_back(tmp);
            table[vn_set[i].left] = tmp;  //��ɸ÷��ս����Ӧ�еĹ���

    }

    /*��ӡLL(1)������*/
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

/*�Բ���ʽ�ұ߽��д���*/
void String_trennen(string vn)
{
    string new_vn(vn.rbegin(), vn.rend());   //�����ж�
    string::iterator p = vn.begin();
    
    while (p != vn.end())
    {
        if (*(p + 1) == '\'')
        {
            string temp(1, *p);
            temp += *(p + 1);
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
//����LL(1)�ķ������﷨����
void LL1_analysieren()    
{
    cout << "������LL(1)�ķ���" << endl;
    string Next_LL1;    //���������LL(1)�ķ�

    cin >> Next_LL1;
    Next_LL1 += "#";        //���#
    LL1_Stack.push("#");    //����#
    LL1_Stack.push(vn_set[1].left);   //�����׸����ս��
    string::iterator p = Next_LL1.begin();
    while (p != Next_LL1.end())
    {
        string X = LL1_Stack.top();   //ȡջ��Ԫ��
        if (vt.find(X) != vt.end())    //XΪ�ս��
        {
            if (string(1, *p) == X) //ƥ��ɹ�
            {
                p++;
                LL1_Stack.pop();    //ָ����ƣ���ջ��Ԫ�س�ջ
                continue;
            }
            else                       //ջ���ս����ָ���ս����ͬ������
            {
                cout << "ERROR!" << endl;
                return;
            }
        }
        else if (X == "#")          
        {
            if (string(1, *p) == X)  //ջ����ָ���Ϊ#���ɹ�
            {
                cout << "YES" << endl;
                break;
            }
            else
            {
                cout << "ERROR!" << endl;
                return;
            }
        }
        else if (table[X][*p]!="")   //XΪ���ս�����ұ��
        {
            LL1_Stack.pop();    
            String_trennen(table[X][*p]);  //����ڲ���ʽ�Ҳ��ջ
            continue;
        }
        else if (table[X][*p] == "")
        {
            cout << "ERROR!" << endl;
            return;
        }
    }
}
int main()
{
    cout << "==========================================" << endl;
    cout << "==== ͬ�ô�ѧ2022�������ԭ��γ���ҵ ====" << endl;
    cout << "====        LL(1)�﷨��������         ====" << endl;
    cout << "====            Welcome!              ====" << endl;
    cout << "==========================================" << endl;
    cout << endl;

    while (1)
    {
        cleanData();
        int n;
        cout << "������LL(1)�ķ�����ʽ������������������˳����򣩣�";
        cin >> n;
        if (n <= 0)
            break;
        /*����n���ķ�����ʽ*/
        string str;
        for (int i = 0; i < n; i++)
        {
            cin >> str;
            Production pro(str);
            vn_set.push_back(pro);
        }
        initVisit(vn_set);
        createFirst();
        createFollow();
        createTable();
        LL1_analysieren();
    }
    return 0;
}

