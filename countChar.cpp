#include<iostream>

using namespace std;

int CharCount(string String, char Char)
{
    int count = 0;
    while(true)
    {
        int a = String.find(Char);
        String.erase(0, a + 1);
        if(a==-1){return count;}
        count++;
    }
}

int main()
{
    string a = "(1,2,3,4)";
    cout<< CharCount(a, ',');
    return 1;
}