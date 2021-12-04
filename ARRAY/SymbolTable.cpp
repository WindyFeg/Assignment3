#include "SymbolTable.h"

void Contain::AddContain(string identifier, int nTypeIn, string typeout, string value)
{
    this->Identifier = identifier;
    this->TypeOut = "Undefined";
    this->Value = value;
    
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
    
    this->level_of_block = level;
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
        
    }
    else if (Type == 1)
    {
        
    }
    else if (Type == 4)
    {
       
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
            
        }
    }
    return -1;
}

void HashTable::DeleteSymbolLevel(string Er, int level)
{
    Symbol a;
    for (int i = 0; i < Size_of_HashTable; i++)
    {
       
    }

    return;
}

string HashTable::ReturnType(Symbol *a)
{
    return a->contain.TypeOut;
}



void HashTable::CheckErForVar(Symbol *SymbolNeedAssign, string value, string name, string Er)
{
    int type = CheckType2(value); // 0 1 2 3 ok 5
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
                    exit(1);
                }
                if (isFuc == 5)
                {
                    throw Undeclared(cutslot);
                    exit(1);
                }
                if (isFuc == 4)
                {
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

    AssignTypeOut(SymbolNeedAssign, SymbolNeedAssign2, Er); // Infer
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
        return 4; // a
    if (FindSymbol(cutslot)->contain.nTypeIn > 0)
        return 2; // fuction
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

        int n = CharCount(value, ',') + 1; // 2
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
        // check behind
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
                string cutslot = name.substr(0, name.find(',')); // 1
                name.erase(0, name.find(',') + 1);
                if (i + 1 == n)
                {
                    cutslot.pop_back();
                }
                int isFuc = CheckType2(cutslot); // 0 1 int string
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
    cout<<1;
    int a = 4;
    string *b = new string[a];
    b[1] = "as";
}