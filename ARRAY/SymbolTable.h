#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Contain
{
public:
    string Identifier;
    string TypeOut;
    string Value;
    int slot = 0;
    string *TypeIn;

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
    long long key;

    Symbol() : level_of_block(0), key(0){};
    Symbol(string name, int nTypeIn = 0);
    Symbol(string name, int level, int key);
    ~Symbol(){};

    friend class HashTable;
    

    string GetName();
    int GetLevel();
    int GetKey();
    bool operator==(Symbol &Sec);
};

class HashTable
{
public:
    int Size_of_HashTable;
    int c, c2;

    HashTable() : Size_of_HashTable(0), c(0), c2(0) {}

    int CheckType2(string cutslot);
    bool CheckStringNum(string value);
    int CheckTypeOfAssign(string &name);
    void ASSIGN(string name, string Er, string value, int TypeHash);
    void INSERT(string name, string Er = "", int TypeHash = 0, int level = 0, int nTypeIn = 0);
    void PRINT(int level);
    void CheckErForFunc(Symbol *SymbolNeedAssign, Symbol *SymbolNeedAssign2, string value, string name, string Er);
    void CheckErForVar(Symbol *SymbolNeedAssign, string value, string name, string Er);
    int CharCount(string String, char Char);
    Symbol *FindSymbol(string name);
    void DeleteSymbolLevel(string name, int level);
    long long LOOK_UP(string name, int level, int Hashtype);
    void CALL(string name, string Er);

    void HASH_LINEAR_M(int size, int c)
    { 
        Size_of_HashTable = size;
        this->c = c;
    }
    void HASH_DOUBLE_M(int size, int c)
    {
        Size_of_HashTable = size;
        this->c = c;
    }
    void HASH_QUADRATIC_M(int size, int c, int c2)
    {
        Size_of_HashTable = size;
        this->c = c;
        this->c2 = c2;
    }
    long long HASH_DOUBLE1(long long k)
    {
        return k % this->Size_of_HashTable;
    }
    long long HASH_DOUBLE2(long long k)
    {
        return 1 + (k % (this->Size_of_HashTable - 2));
    }
    long long HASH_DOUBLE_P(long long k, int i)
    {
        return (HASH_DOUBLE1(k) + this->c * i * HASH_DOUBLE2(k)) % this->Size_of_HashTable;
    }

    long long HASH_LINEAR(long long k)
    {
        return k % this->Size_of_HashTable;
    }
    long long HASH_LINEAR_P(long long k, int i)
    {
        return (HASH_LINEAR(k) + this->c * i) % this->Size_of_HashTable;
    }
    long long HASH_QUADRATIC(long long k)
    {
        return k % Size_of_HashTable;
    }
    long long HASH_QUADRATIC_P(long long k, int i)
    {
        return (HASH_QUADRATIC(k) + c * i + c2 * i * i) % Size_of_HashTable;
    }
    long long HASH_CHOSE(long long k, int HashType);
    long long HASH_CHOSE_P(long long k, int i, int HashType);

    void CheckStringName(string name, string Er);
    string ReturnType(Symbol *a);
    void AssignTypeOut(Symbol *x, Symbol *a, string Er);
    void AssignTypeIn(Symbol *x, int Type, int i, string cuts, string Er);
    bool InitType(Symbol *a);
};

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};
#endif