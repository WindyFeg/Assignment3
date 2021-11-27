#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Contain
{
public:
    string Identifier;
    string TypeIn[20];
    string TypeOut;
    string Value;
    int nTypeIn;

    Contain() : Identifier(""), TypeOut("Undefined"), Value(""), nTypeIn(0) {}

    friend class Symbol;

    void AddContain(string identifier, int nTypeIn, string typeout, string value);
    
};

class Symbol
{
public:
    Contain contain;
    int level_of_block;
    int key;

    Symbol() : level_of_block(0), key(0){};
    Symbol(string name, int nTypeIn = 0);
    Symbol(string name, int level, int key);
    ~Symbol(){};

    friend class HashTable;
    friend int HASH_NAME(Symbol *Name)
    {
        string Res = "";
        Res += to_string(Name->level_of_block);
        for (int i = 0; i < Name->contain.Identifier.length(); i++)
        {
            Res += to_string(Name->contain.Identifier[i] - 48);
        }
        return stoi(Res);
    }
    
    string GetName();
    int GetLevel();
    int GetKey();
    bool operator==(Symbol &Sec);
};

class HashTable
{
public:
    Symbol *arr;
    int Size_of_HashTable;
    int c, c2;

    HashTable() : Size_of_HashTable(0), c(0), c2(0) {}

    bool CheckStringNum(string value);
    int CheckTypeOfAssign(string &name);
    void ASSIGN(string name,string Er, string value);
    void INSERT(string name, string Er = "", int nTypeIn = 0);
    int CharCount(string String, char Char);
    Symbol *FindSymbol(string name);

    void HASH_LINEAR_MAP(int size, int c)
    { // find
        Size_of_HashTable = size;
        arr = new Symbol[size];
        this->c = c;
    }
    /////////////////////////
    int HASH_DOUBLE1(int k)
    {
        return k % c;
    }
    int HASH_DOUBLE2(int k)
    {
        return 1 + (k % (this->Size_of_HashTable - 2));
    }
    int HASH_DOUBLE_P(int k, int i)
    {
        return (HASH_DOUBLE1(k) + this->c * i * HASH_DOUBLE2(k)) % this->Size_of_HashTable;
    }
    ////////////////////////

    int HASH_LINEAR(int k)
    {
        return k % this->Size_of_HashTable;
    }

    int HASH_LINEAR_P(int k, int i)
    {
        return (HASH_LINEAR(k) + this->c * i) % this->Size_of_HashTable;
    }
    /////////////////////////
    int HASH_QUADRATIC(int k)
    {
        return k % Size_of_HashTable;
    }
    int HASH_QUADRATIC(int k, int i)
    {
        return (HASH_QUADRATIC(k) + c * i + c2 * i * i) % Size_of_HashTable;
    }
    /////////////////////////
    bool CheckStringName();
    string ReturnType(Symbol *a);
    void AssignTypeOut(Symbol *x, Symbol *a, string Er);
    void AssignTypeIn(Symbol *x, int Type, int i, string cuts);
    bool InitType(Symbol *a);
};

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};
#endif