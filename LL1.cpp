#define MAX_VALUE 1001

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>

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


vector<Production> vn_set;         //����ʽ�����ս��������
map<string, int> vn_dic;           //���ս������
map<string, set<char>> first;     //firsr����
map<string, set<char>> follow;    //follow����
int vn_is_visited[MAX_VALUE] = { 0 };

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
    for (; iter != right.end(); iter++)
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
void makeFirst()
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
void makeFollow()
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
        makeFirst();
        makeFollow();
    }
    return 0;
}