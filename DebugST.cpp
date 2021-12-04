#include "main.h"

class Contain
{
public:
    string Identifier;
    string *TypeIn;
    string TypeOut;
    string Value;
    int slot = 0;

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
    friend long long HASH_NAME(Symbol *Name)
    {
        string Res = "";
        Res += to_string(Name->level_of_block);
        for (int unsigned i = 0; i < Name->contain.Identifier.length(); i++)
        {
            Res += to_string(Name->contain.Identifier[i] - 48);
        }
        return stoll(Res);
    }

    string GetName();
    int GetLevel();
    int GetKey();
    bool operator==(Symbol &Sec);
};

class HashTable
{
public:
    Symbol *Allsymbol;
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
        Allsymbol = new Symbol[size];
        this->c = c;
    }
    void HASH_DOUBLE_M(int size, int c)
    {
        Size_of_HashTable = size;
        Allsymbol = new Symbol[size];
        this->c = c;
    }
    void HASH_QUADRATIC_M(int size, int c, int c2)
    {
        Size_of_HashTable = size;
        Allsymbol = new Symbol[size];
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
// alo
void Contain::AddContain(string identifier, int nTypeIn, string typeout, string value)
{
    this->Identifier = identifier;
    this->TypeOut = "Undefined";
    this->Value = value;
    this->TypeIn = new string[nTypeIn];
    for (int i = 0; i<nTypeIn ;i++)
    {
        TypeIn[i] = "Undefined";
    };
    this->nTypeIn = nTypeIn;
}

bool Symbol::operator==(Symbol &Sec)
{
    if (this->key == Sec.key)
        return true;
    return false;
}

Symbol::Symbol(string name, int nTypeIn, int level)
{
    this->contain.Identifier = name;
    this->contain.TypeIn = new string[nTypeIn];
    for (int i =0; i<nTypeIn ; i++)
    {
        this->contain.TypeIn[i] = "Undefined";
    }
    this->level_of_block = level;
    this->key = HASH_NAME(this);
    this->contain.nTypeIn = nTypeIn;
}

string Symbol::GetName()
{
    return this->contain.Identifier;
}
int Symbol::GetLevel()
{
    return this->level_of_block;
}
int Symbol::GetKey()
{
    return this->key;
}

bool HashTable::InitType(Symbol *a)
{
    if (a->contain.TypeOut == "Undefined")
    {
        return false;
    }
    return true;
}

long long HashTable::HASH_CHOSE(long long k, int HashType)
{
    if (HashType == 0)
    {
        return HASH_LINEAR(k);
    }
    if (HashType == 1)
    {
        return HASH_DOUBLE1(k);
    }
    if (HashType == 2)
    {
        return HASH_QUADRATIC(k);
    }
    return -1;
}

long long HashTable::HASH_CHOSE_P(long long k, int i, int HashType)
{
    if (HashType == 0)
    {
        return HASH_LINEAR_P(k, i);
    }
    if (HashType == 1)
    {
        return HASH_DOUBLE_P(k, i);
    }
    if (HashType == 2)
    {
        return HASH_QUADRATIC_P(k, i);
    }
    return -1;
};

void HashTable::AssignTypeIn(Symbol *x, int Type, int i, string cuts, string Er)
{
    if (Type == 0)
    {
        if (x->contain.TypeIn[i] == "Undefined" || x->contain.TypeIn[i] == "Number")
        {
            x->contain.TypeIn[i] = "Number";
        }
        else
        {
            throw TypeMismatch(Er);
        }
    }
    else if (Type == 1)
    {
        if (x->contain.TypeIn[i] == "Undefined" || x->contain.TypeIn[i] == "String")
        {
            x->contain.TypeIn[i] = "String";
        }
        else
        {
            throw TypeMismatch(Er);
        }
    }
    else if (Type == 4)
    {
        if (x->contain.TypeIn[i] == "Undefined" || x->contain.TypeIn[i] == FindSymbol(cuts)->contain.TypeOut)
        {
            x->contain.TypeIn[i] = FindSymbol(cuts)->contain.TypeOut;
        }
        else
        {
            throw TypeMismatch(Er);
        }
    }
    else
    {
        cout << "Something Wrong in Assign fun(1,2,3)";
    }
}

void HashTable::PRINT(int level)
{
    string Res = "";

    for (int j = 0; j < Size_of_HashTable; j++)
    {
        if (Allsymbol[j].contain.Identifier != "")
        {

            Res = Res + to_string(j) + " " + Allsymbol[j].contain.Identifier + "//" + to_string(Allsymbol[j].level_of_block) + ";";
        }
    }

    if (Res == "")
    {
        return;
    }
    Res.pop_back();
    cout << Res << endl;
    return;
}

void HashTable::AssignTypeOut(Symbol *x, Symbol *a, string Er)
{
    if (!InitType(x))
    {
        if (!InitType(a))
        {
            throw TypeCannotBeInferred("ASSIGN " + Er);
            exit(1);
        }
        x->contain.TypeOut = a->contain.TypeOut;
        return;
    }
    else
    {
        if (!InitType(a))
        {
            a->contain.TypeOut = x->contain.TypeOut;
            return;
        }
        else
        {
            if (x->contain.TypeOut == a->contain.TypeOut)
            {
                return;
            }
            else
            {
                cout << "Type missmatch";
            }
        }
    }
    return;
}

Symbol *HashTable::FindSymbol(string name)
{

    for (int i = 0; i < Size_of_HashTable; i++)
    {
        if (Allsymbol[i].GetName() == name)
        {
            return &Allsymbol[i];
        }
    }

    return NULL;
}

long long HashTable::LOOK_UP(string name, int level, int HashType)
{
    int i = 0;
    for (i = level; i >= 0; i--)
    {
        for (int j = 0; j < Size_of_HashTable; j++)
        {
            if (Allsymbol[j].level_of_block == i && Allsymbol[j].contain.Identifier == name)
            {
                return j;
            }
        }
    }
    return -1;
}

void HashTable::DeleteSymbolLevel(string Er, int level)
{
    Symbol a;
    for (int i = 0; i < Size_of_HashTable; i++)
    {
        if (this->Allsymbol[i].level_of_block == level)
        {
            Allsymbol[i] = a;
        }
    }

    return;
}

string HashTable::ReturnType(Symbol *a)
{
    return a->contain.TypeOut;
}

void HashTable::CheckStringName(string name,string Er)
{
    if('A' <=name[0] && name[0]<= 'Z' )
    {
        throw InvalidInstruction(Er);
            exit(1);
    } 
    int i=0;
    while (name[i]!='\0')
    {
        if(name[i]=='&' || name[i]=='@'|| name[i]=='$' ||name[i]=='*'||name[i]=='!'||name[i]=='%'||name[i]=='^')
        {
            throw InvalidInstruction(name);
            exit(1);
        }
        i++;
    }
    return;
}

void HashTable::CheckErForVar(Symbol *SymbolNeedAssign, string value, string name, string Er)
{
    int type = CheckType2(value);
    if (type == 5)
    {
        throw Undeclared(value);
        exit(1);
    }
    if (SymbolNeedAssign == NULL)
    {
        throw Undeclared(name);
        exit(1);
    }
    if (SymbolNeedAssign != NULL)
    {
        if (SymbolNeedAssign->contain.nTypeIn >= 1)
        {
            throw TypeMismatch("ASSIGN " + Er);
            exit(1);
        }
    }
}

void HashTable::CheckErForFunc(Symbol *SymbolNeedAssign, Symbol *SymbolNeedAssign2, string value, string name, string Er)
{
    if (SymbolNeedAssign2 == NULL)
    {
        throw Undeclared(value);
        exit(1);
    }
    if (SymbolNeedAssign2 != NULL)
    {
        if (value.find('(') !=string::npos)
        {
            value.erase(0, value.find('(') + 1);
            int n = CharCount(value, ',') + 1;

            if (n != SymbolNeedAssign2->contain.nTypeIn)
            {
                throw TypeMismatch("ASSIGN " + Er);
                exit(1);
            }

            for (int i = 0; i < n; i++)
            {
                string cutslot = value.substr(0, value.find(','));
                value.erase(0, value.find(',') + 1);
                if (i + 1 == n)
                {
                    cutslot.pop_back();
                }
                int isFuc = CheckType2(cutslot); 
                if (isFuc == 2)
                {
                    throw TypeMismatch("ASSIGN " + Er);
                    exit(1);
                }
                if (isFuc == 5)
                {
                    throw Undeclared(cutslot);
                    exit(1);
                }
                if (isFuc == 4)
                {
                    if (SymbolNeedAssign2->contain.TypeIn[i] == "Undefined" && FindSymbol(cutslot)->contain.TypeOut == "Undefined")
                    {
                        throw TypeCannotBeInferred("ASSIGN " + Er);
                        exit(1);
                    }
                }
                AssignTypeIn(SymbolNeedAssign2, isFuc, i, cutslot, "ASSIGN " + Er);
            }
        }
    }
    if (SymbolNeedAssign == NULL)
    {
        throw Undeclared(name);
        exit(1);
    }
    if (SymbolNeedAssign->contain.nTypeIn >= 1)
    {
        throw TypeMismatch("ASSIGN " + Er);
        exit(1);
    }

    AssignTypeOut(SymbolNeedAssign, SymbolNeedAssign2, Er);
}

int HashTable::CheckType2(string cutslot)
{
    if (FindSymbol(cutslot) == NULL)
    {
        if (CheckTypeOfAssign(cutslot) == 0)
        {
            return 0;
        };
        if (CheckTypeOfAssign(cutslot) == 1)
        {
            return 1;
        }
        else
            return 5;
    }
    if (FindSymbol(cutslot)->contain.nTypeIn == 0)
        return 4; 
    if (FindSymbol(cutslot)->contain.nTypeIn > 0)
        return 2; 
    return 5;
}

int HashTable::CharCount(string String, char Char)
{
    int count = 0;
    while (true)
    {
        int unsigned a = String.find(Char);
        String.erase(0, a + 1);
        if (a == string::npos)
        {
            return count;
        }
        count++;
    }
}

void HashTable::ASSIGN(string name, string Er, string value, int TypeHash)
{
    Symbol *SymbolNeedAssign;
    SymbolNeedAssign = FindSymbol(name);

    int typeAssign = CheckTypeOfAssign(value);
    switch (typeAssign)
    {
    case 0:
    {
        CheckErForVar(SymbolNeedAssign, value, name, Er);
        SymbolNeedAssign->contain.TypeOut = "Number";
        SymbolNeedAssign->contain.Value = value;
    }
    break;
    case 1:
    {
        CheckErForVar(SymbolNeedAssign, value, name, Er);
        SymbolNeedAssign->contain.TypeOut = "String";
        SymbolNeedAssign->contain.Value = value;
    }
    break;
    case 2:
    {
        Symbol *SymbolNeedAssign2; 
        SymbolNeedAssign2 = FindSymbol(value.substr(0, value.find('(')));

        CheckErForFunc(SymbolNeedAssign, SymbolNeedAssign2, value, name, Er);

        value.erase(0, value.find('(') + 1);

        int n = CharCount(value, ',') + 1;
        for (int i = 0; i < n; i++)
        {
            string cutslot = value.substr(0, value.find(','));
            value.erase(0, value.find(',') + 1);
            if (i + 1 == n)
            {
                cutslot.pop_back();
            }
            int Type = CheckTypeOfAssign(cutslot);
            AssignTypeIn(SymbolNeedAssign2, Type, i, cutslot, "ASSIGN " + Er);
        }
    }
    break;
    case 3:
    {
    }
    break;
    case 4:
    {
        Symbol *SymbolNeedAssign2;
        SymbolNeedAssign2 = FindSymbol(value);
        CheckErForVar(SymbolNeedAssign, value, name, Er);

        AssignTypeOut(SymbolNeedAssign, SymbolNeedAssign2, Er);
    }
    break;
    default:
        break;
    }

    cout << SymbolNeedAssign->contain.slot << endl;
    return;
}

bool HashTable::CheckStringNum(string value)
{
    char *end;
    strtol(value.c_str(), &end, 10);
    if (*end == '\0')
    {
        return true;
    }
    else
    {
        return false;
    }
}

int HashTable::CheckTypeOfAssign(string &name)
{
    unsigned int a = name.find('(');
    if (a != string::npos)
    {
        
        Symbol *SymbolNeedAssign;
        SymbolNeedAssign = FindSymbol(name.substr(0, a));

        if (SymbolNeedAssign->contain.TypeIn[0] == "Undefined")
        {
            return 2;
        }
        else
        {
            return 2;
        }
    }
    else
    {
       
        if (CheckStringNum(name))
        {
            return 0;
        }
        else
        {
            if (name.find('\'') == string::npos)
            {
                return 4;
            }
            return 1;
        }
    }
    return 5;
}

void HashTable::INSERT(string name, string Er, int TypeHash, int level, int nTypeIn)
{
    CheckStringName(name,Er);
    Symbol *NewSymbol = new Symbol(name, nTypeIn, level);
    int index_in_m = HASH_CHOSE(NewSymbol->key, TypeHash);

    if (Allsymbol[index_in_m].key == 0)
    {
        Allsymbol[index_in_m] = *NewSymbol;
        cout << 0 << endl;
        return;
    }
    else
    {
        for (int i = 0; i < Size_of_HashTable; i++)
        {
            index_in_m = HASH_CHOSE_P(NewSymbol->key, i, TypeHash);
            if (Allsymbol[index_in_m].key == 0)
            {
                Allsymbol[index_in_m] = *NewSymbol;
                cout << i << endl;
                Allsymbol[index_in_m].contain.slot = i;
                return;
            }
            if (Allsymbol[index_in_m] == *NewSymbol)
            {
                throw Redeclared(Er);
                exit(1);
            }
        }
        throw Overflow(Er);
    }
}

void HashTable::CALL(string name, string Er)
{
    if (name.find('(') == string::npos || name.find(')') == string::npos)
    {
        throw InvalidInstruction(Er);
        exit(1);
    }

    Symbol *SymbolNeedAssign2; 
    SymbolNeedAssign2 = FindSymbol(name.substr(0, name.find('(')));

    if (SymbolNeedAssign2 == NULL)
    {
        throw Undeclared(name.substr(0, name.find('(')));
        exit(1);
    }
    if (SymbolNeedAssign2 != NULL)
    {
        if (name.find('(') != string::npos)
        {
            name.erase(0, name.find('(') + 1);
            int n = CharCount(name, ',') + 1;

            if (n != SymbolNeedAssign2->contain.nTypeIn)
            {
                throw TypeMismatch(Er);
                exit(1);
            }

            for (int i = 0; i < n; i++)
            {
                string cutslot = name.substr(0, name.find(','));
                name.erase(0, name.find(',') + 1);
                if (i + 1 == n)
                {
                    cutslot.pop_back();
                }
                int isFuc = CheckType2(cutslot);
                if (isFuc == 2)
                {
                    throw TypeMismatch(Er);
                    exit(1);
                }
                if (isFuc == 5)
                {
                    throw Undeclared(cutslot);
                    exit(1);
                }
                if (isFuc == 4)
                {
                    if (SymbolNeedAssign2->contain.TypeIn[i] == "Undefined" && FindSymbol(cutslot)->contain.TypeOut == "Undefined")
                    {
                        throw TypeCannotBeInferred(Er);
                        exit(1);
                    }
                    if (SymbolNeedAssign2->contain.TypeIn[i] != "Undefined" && FindSymbol(cutslot)->contain.TypeOut == "Undefined")
                    {
                        FindSymbol(cutslot)->contain.TypeOut = SymbolNeedAssign2->contain.TypeIn[i];
                    }
                }
                AssignTypeIn(SymbolNeedAssign2, isFuc, i, cutslot, Er);
            }
        }
    }
    cout << SymbolNeedAssign2->contain.slot << endl;
    return;
}

int Convert(string input)
{
    if (input == "LINEAR")
    {
        return 0;
    }
    else if (input == "QUADRATIC")
    {
        return 1;
    }
    else if (input == "DOUBLE")
    {
        return 2;
    }
    else if (input == "INSERT")
    {
        return 10;
    }
    else if (input == "ASSIGN")
    {
        return 11;
    }
    else if (input == "CALL")
    {
        return 12;
    }
    else if (input == "BEGIN")
    {
        return 13;
    }
    else if (input == "END")
    {
        return 14;
    }
    else if (input == "LOOKUP")
    {
        return 15;
    }
    else if (input == "PRINT")
    {
        return 16;
    }
    else
    {
        return 20;
    }
}

void SymbolTable::run(string filename)
{
    ifstream file(filename);
    string line, input;

    HashTable BangBam;
    int levelNow = 0, TypeHash = -1;

    while (getline(file, line))
    {
        input = line.substr(0, line.find(' '));
        switch (Convert(input))
        {
        case 0:
        {
            line.erase(0, line.find(' ') + 1);
            BangBam.HASH_LINEAR_M(stol(line.substr(0, line.find(' '))), std::stol(line.substr(line.find(' ') + 1)));
            TypeHash = 0;
            break;
        }
        case 1:
        {
            line.erase(0, line.find(' ') + 1);
            long long a = stol(line.substr(0, line.find(' ')));
            line.erase(0, line.find(' ') + 1);
            BangBam.HASH_QUADRATIC_M(a, stol(line.substr(0, line.find(' '))), stol(line.substr(line.find(' ') + 1)));
            TypeHash = 2;
            break;
        }
        case 2:
        {
            line.erase(0, line.find(' ') + 1);
            BangBam.HASH_DOUBLE_M(stol(line.substr(0, line.find(' '))), std::stol(line.substr(line.find(' ') + 1)));
            TypeHash = 1;
            break;
        }
        case 10:
        {
            line.erase(0, line.find(' ') + 1);

            if (levelNow != 0 && line.find(' ') != string::npos)
            {
                throw InvalidDeclaration(line.substr(0, line.find(' ')));
                exit(1);
            }

            if (line.find(' ') == string::npos)
            {
                BangBam.INSERT(line, line, TypeHash, levelNow);
            }
            else
            {
                BangBam.INSERT(line.substr(0, line.find(' ')), line, TypeHash, levelNow, stol(line.substr(line.find(' ') + 1)));
            }

            break;
        }
        case 11:
        {
            line.erase(0, line.find(' ') + 1);
            BangBam.ASSIGN(line.substr(0, line.find(' ')), line, line.substr(line.find(' ') + 1), TypeHash);
          
            break;
        }
        case 12:
        {
          
            line.erase(0, line.find(' ') + 1);
            BangBam.CALL(line, "CALL " + line);

            break;
        }
        case 13: 
        {
            levelNow++;
        }
        break;
        case 14: 
        {
            levelNow--;
            if (levelNow < 0)
            {
                throw UnknownBlock();
                exit(1);
            }
            BangBam.DeleteSymbolLevel(line, levelNow + 1);
        }
        break;
        case 15:
        { 
            line.erase(0, line.find(' ') + 1);
            if (BangBam.FindSymbol(line) == NULL)
            {
                throw Undeclared(line);
                exit(1);
            }
            cout << BangBam.LOOK_UP(line, levelNow, TypeHash) << endl;
        }
        break;
        case 16:
        {
            
            BangBam.PRINT(levelNow);
        }
        break;
        default:
            cout << "Reinput pls";
            break;
        }
    }
    if (levelNow < 0)
    {
        throw UnknownBlock();
        exit(1);
    }
    else if (levelNow > 0)
    {
        throw UnclosedBlock(levelNow);
        exit(1);
    }
}

int main()
{
    SymbolTable b;
    string a;
    a = "test.txt";
    b.SymbolTable::run(a);

    // – InvalidDeclaration nếu khai báo hàm trong các khối có mức khác 0.
    // – Overflow nếu khai báo không thể tìm được vị trí trống phù hợp trong bảng băm.
}
