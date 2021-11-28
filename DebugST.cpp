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
        for (int i = 0; i < Name->contain.Identifier.length(); i++)
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
    Symbol *arr;
    int Size_of_HashTable;
    int c, c2;

    HashTable() : Size_of_HashTable(0), c(0), c2(0) {}

    int CheckType2(string cutslot);
    bool CheckStringNum(string value);
    int CheckTypeOfAssign(string &name);
    void ASSIGN(string name, string Er, string value);
    void INSERT(string name, string Er = "", int nTypeIn = 0);
    void CheckErForFunc(Symbol *SymbolNeedAssign, Symbol *SymbolNeedAssign2, string value, string name, string Er);
    void CheckErForVar(Symbol *SymbolNeedAssign, string value, string name, string Er);
    int CharCount(string String, char Char);
    Symbol *FindSymbol(string name);

    void HASH_LINEAR_MAP(int size, int c)
    { // find
        Size_of_HashTable = size;
        arr = new Symbol[size];
        this->c = c;
    }
    /////////////////////////
    long long HASH_DOUBLE1(int k)
    {
        return k % c;
    }
    long long HASH_DOUBLE2(int k)
    {
        return 1 + (k % (this->Size_of_HashTable - 2));
    }
    long long HASH_DOUBLE_P(int k, int i)
    {
        return (HASH_DOUBLE1(k) + this->c * i * HASH_DOUBLE2(k)) % this->Size_of_HashTable;
    }
    ////////////////////////

    long long HASH_LINEAR(long long k)
    {
        return k % this->Size_of_HashTable;
    }

    long long HASH_LINEAR_P(long long k, int i)
    {
        return (HASH_LINEAR(k) + this->c * i) % this->Size_of_HashTable;
    }
    /////////////////////////
    long long HASH_QUADRATIC(int k)
    {
        return k % Size_of_HashTable;
    }
    long long HASH_QUADRATIC(int k, int i)
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
//alo
void Contain::AddContain(string identifier, int nTypeIn, string typeout, string value)
{
    this->Identifier = identifier;
    this->TypeOut = "Undefined";
    this->Value = value;
    for (auto &x : TypeIn)
    {
        x = "Undefined";
    };
    this->nTypeIn = nTypeIn;
}

bool Symbol::operator==(Symbol &Sec)
{
    if (this->key == Sec.key)
        return true;
    return false;
}

Symbol::Symbol(string name, int nTypeIn)
{
    this->contain.Identifier = name;
    for (auto &x : this->contain.TypeIn)
    {
        x = "Undefined";
    }
    this->level_of_block = 0;
    this->key = HASH_NAME(this);
    this->contain.nTypeIn = nTypeIn;
}
Symbol::Symbol(string name, int level, int key)
{
    this->contain.Identifier = name;
    this->key = key;
    this->level_of_block;
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

void HashTable::AssignTypeIn(Symbol *x, int Type, int i, string cuts)
{
    if (Type == 0)
    {
        x->contain.TypeIn[i] = "Number";
    }
    else if (Type == 1)
    {
        x->contain.TypeIn[i] = "String";
    }
    else if (Type == 4)
    {
        x->contain.TypeIn[i] = FindSymbol(cuts)->contain.TypeOut;
    }
    else
    {
        cout << "Something Wrong in Assign fun(1,2,3)";
    }
}

void HashTable::AssignTypeOut(Symbol *x, Symbol *a, string Er)
{
    if (!InitType(x))
    {
        // x = non a = non
        if (!InitType(a))
        {
            throw TypeCannotBeInferred("ASSIGN " + Er);
            exit(1);
        }
        // if x(non) = a(int)
        x->contain.TypeOut = a->contain.TypeOut;
        return;
    }
    else
    {
        // x = int
        if (!InitType(a))
        {
            a->contain.TypeOut = x->contain.TypeOut;
            return;
        }
        else
        {
            // if x(int) = a(int or string)
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
    // defaut level is 0 =>

    for (int i = 0; i < Size_of_HashTable; i++)
    {
        if (arr[i].GetName() == name)
        {
            return &arr[i];
        }
    }

    return NULL;
}
string HashTable::ReturnType(Symbol *a)
{
    return a->contain.TypeOut;
}

bool HashTable::CheckStringName()
{
    // Ki tu dau chu normal, con lai (chu normal, in hoa, _, so)
    return true;
}

void HashTable::CheckErForVar(Symbol *SymbolNeedAssign, string value, string name, string Er)
{
    int type = CheckType2(value); // 0 1 2 3 ok 5
    if(type == 5)
    {
        throw Undeclared(value);
    }
    if (SymbolNeedAssign == NULL)
    {
        throw Undeclared(name);
    }
    if (SymbolNeedAssign != NULL)
    {
        if (SymbolNeedAssign->contain.nTypeIn >= 1)
            throw TypeMismatch("ASSIGN " + Er);
    }
}

void HashTable::CheckErForFunc(Symbol *SymbolNeedAssign, Symbol *SymbolNeedAssign2, string value, string name, string Er)
{
    if (SymbolNeedAssign2 == NULL)
    {
        throw Undeclared(value);
    }
    if (SymbolNeedAssign2 != NULL)
    {
        // check behind
        if (value.find('(') != -1)
        {
            value.erase(0, value.find('(') + 1);
            int n = CharCount(value, ',') + 1;

            if(n != SymbolNeedAssign2->contain.nTypeIn){throw TypeMismatch("ASSIGN " + Er);}

            for (int i = 0; i < n; i++)
            {
                string cutslot = value.substr(0, value.find(',')); // 1
                value.erase(0, value.find(',') + 1);
                if (i + 1 == n)
                {
                    cutslot.pop_back();
                }
                int isFuc = CheckType2(cutslot); // 0 1 int string
                if (isFuc == 2)
                {
                    throw TypeMismatch("ASSIGN " + Er);
                }
                if(isFuc == 5)
                {
                    throw Undeclared(cutslot);
                }
                if(isFuc == 4)
                {
                    if(SymbolNeedAssign2->contain.TypeIn[i] == "Undefined" && FindSymbol(cutslot)->contain.TypeOut == "Undefined")
                    {
                        throw TypeCannotBeInferred("ASSIGN " + Er);
                        exit(1);
                    }  
                }
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

    AssignTypeOut(SymbolNeedAssign, SymbolNeedAssign2, Er); // Infer
    // check front
}

int HashTable::CheckType2(string cutslot)
{
    if (FindSymbol(cutslot) == NULL)
    {
        // string or int
        if(CheckTypeOfAssign(cutslot) == 0)
        {
            return 0;
        };
        if(CheckTypeOfAssign(cutslot) == 1)
        {
            return 1;
        }
        else return 5;
    }
    if (FindSymbol(cutslot)->contain.nTypeIn == 0)
        return 4; // a
    if (FindSymbol(cutslot)->contain.nTypeIn > 0) return 2; // fuction
    return 5;
}

int HashTable::CharCount(string String, char Char)
{
    int count = 0;
    while (true)
    {
        int a = String.find(Char);
        String.erase(0, a + 1);
        if (a == -1)
        {
            return count;
        }
        count++;
    }
}

void HashTable::ASSIGN(string name, string Er, string value)
{
    Symbol *SymbolNeedAssign;
    SymbolNeedAssign = FindSymbol(name);

    // 0 is Assign number ASSIGN x 3
    // 1 is Assign string '' ASSIGN x 'ed'
    // 2 is Assign Function haven't type yet ASSIGN x Fun(12,x, 'alo')
    // 3 is Assign Fuction have type ASSIGN x Fun(12,x, 'alo')
    // 4 is Assign Variable has it value x = a (a = 5)
    int typeAssign = CheckTypeOfAssign(value);
    switch (typeAssign)
    {
    case 0:
    {
        // x(maybe) = 2
        CheckErForVar(SymbolNeedAssign, value, name, Er);
        SymbolNeedAssign->contain.TypeOut = "Number";
        SymbolNeedAssign->contain.Value = value;
    }
    break;
    case 1:
    {
        // x(maybe) = 'abc' (but a is int??)
        CheckErForVar(SymbolNeedAssign, value, name, Er);
        SymbolNeedAssign->contain.TypeOut = "String";
        SymbolNeedAssign->contain.Value = value;
    }
    break;
    case 2:
    {
        Symbol *SymbolNeedAssign2; // fun
        SymbolNeedAssign2 = FindSymbol(value.substr(0, value.find('(')));
        // Assign x(type = a) = a;

        CheckErForFunc(SymbolNeedAssign, SymbolNeedAssign2, value, name, Er);

        // Check fun(1,2,3)
        value.erase(0, value.find('(') + 1); // 1,a,'b')

        int n = CharCount(value, ',') + 1; // 2
        // check cut each slot 1,a,'b') to store in array
        for (int i = 0; i < n; i++)
        {
            string cutslot = value.substr(0, value.find(',')); // 1
            value.erase(0, value.find(',') + 1);
            if (i + 1 == n)
            {
                cutslot.pop_back();
            }
            int Type = CheckTypeOfAssign(cutslot);
            // check each slot is what type??
            AssignTypeIn(SymbolNeedAssign2, Type, i, cutslot);
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
        // x(maybe) = a(co type)
        CheckErForVar(SymbolNeedAssign, value, name, Er);

        AssignTypeOut(SymbolNeedAssign, SymbolNeedAssign2, Er);
    }
    break;
    default:
        break;
    }
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
    int a = name.find('('), b = name.find(')');
    if (a != -1)
    {
        // sum(5,x) undefined
        // foo(’abc’) define
        Symbol *SymbolNeedAssign;
        SymbolNeedAssign = FindSymbol(name.substr(0, a));

        if (SymbolNeedAssign->contain.TypeIn[0] == "Undefined")
        {
            // Undefined
            return 2;
        }
        else
        {
            // Already check if input is true
            return 3;
        }
    }
    else
    {
        // 'abcd'
        // 1234
        if (CheckStringNum(name))
        {
            return 0;
        }
        else
        {
            if (name.find('\'') == -1)
            {
                return 4;
            }
            return 1;
        }
    }
    return 5;
}

void HashTable::INSERT(string name, string Er, int nTypeIn)
{
    Symbol *NewSymbol = new Symbol(name, nTypeIn);
    int index_in_map = HASH_LINEAR(NewSymbol->key);

    if (arr[index_in_map].key == 0)
    {
        arr[index_in_map] = *NewSymbol;
        cout << 0 << endl;
        return;
    }
    else
    {
        for (int i = 0; i < Size_of_HashTable; i++)
        {
            index_in_map = HASH_LINEAR_P(NewSymbol->key, i);
            if (arr[index_in_map].key == 0)
            {
                arr[index_in_map] = *NewSymbol;
                cout << i << endl;
                return;
            }
            if (arr[index_in_map] == *NewSymbol)
            {
                throw Redeclared("INSERT " + Er);
                exit(1);
            }
        }
    }
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

    while (getline(file, line))
    {
        input = line.substr(0, line.find(' '));
        switch (Convert(input))
        {
        case 0:
        {
            // LINEAR
            line.erase(0, line.find(' ') + 1);
            BangBam.HASH_LINEAR_MAP(stol(line.substr(0, line.find(' '))), std::stol(line.substr(line.find(' ') + 1)));
            break;
        }
        case 1:
        {

            break;
        }
        case 2:
        {

            break;
        }
        case 10:
        {
            // INSERT
            line.erase(0, line.find(' ') + 1);
            if (line.find(' ') == -1)
            {
                // dang add string
                BangBam.INSERT(line, line);
            }
            else
            {
                // dang ham
                BangBam.INSERT(line.substr(0, line.find(' ')), line, stol(line.substr(line.find(' ') + 1)));
            }

            break;
        }
        case 11:
        {
            // ASSIGN
            line.erase(0, line.find(' ') + 1);
            BangBam.ASSIGN(line.substr(0, line.find(' ')), line, line.substr(line.find(' ') + 1));
            cout << 0 << endl; // temporary we print out 0, then we count number of hash to print
            break;
        }
        default:
            cout << "Reinput pls";
            break;
        }
    }
}

int main()
{
    SymbolTable b;
    string a;
    a = "test.txt";
    b.SymbolTable::run(a);
}
