#include<iostream>
#include<string>
using namespace std;

class Dictionary
{
    public:
    int level;
    string name;
    int key;

    Dictionary(): level(0), name(""), key(0){};

    void add(int level, string name)
    {
        this->level = level;
        this->name = name;
    }
};
class LINEAR
{
    private:
    int m = 19;
    Dictionary* ArrayDic = new Dictionary[m];
    
    public:
    int EncodeKey(string level,string Chu) // "abcd"
    {
        string NumNeedEncode = "";
        NumNeedEncode+=level;
        for(int i=0; i<Chu.length(); i++)
        {
            NumNeedEncode += to_string(Chu[i] - 48);
        }
        return stoi(NumNeedEncode);
    }

    int HashFunction(int NumNeedHash) //LINEAR
    {
        return NumNeedHash % this->m;
    }

    void  AddWord(int level, string name)
    {
        int encode = EncodeKey(to_string(level),name);
        int index = HashFunction(encode);
        while(ArrayDic[index].name!="")
        {
            if(ArrayDic[index].name == name){"Throw Error";}
            index++;
        }
        cout<< encode <<" ";
        cout<< index << " ";
        ArrayDic[index].add(level, name);

    }
};

int main()
{
    LINEAR ClassDic;
    ClassDic.AddWord(0, "a1");
    ClassDic.AddWord(0, "b2");
    ClassDic.AddWord(0, "rj");
}