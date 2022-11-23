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
map<string, set<char>> first;     //firsr����
map<string, set<char>> follow;    //follow����
int vn_is_visited[MAX_VALUE] = { 0 };

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
    puts("***************FIRST��***********************");
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
    cout << "==== ͬ�ô�ѧ2022�������ԭ��γ���ҵ ====" << endl;
    cout << "====        LL(1)�﷨��������         ====" << endl;
    cout << "====            Welcome!              ====" << endl;
    cout << "==========================================" << endl;
    cout << endl;

    while (1)
    {
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

    }
    return 0;
}