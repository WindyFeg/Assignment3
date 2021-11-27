#include "SymbolTable.h"

void Contain::AddContain(string identifier, int nTypeIn, string typeout, string value)
{
    this->Identifier = identifier;
    this->TypeOut = typeout;
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
    if (a->contain.TypeIn[0] == "Undefined")
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

void HashTable::AssignTypeOut(Symbol *x, Symbol *a)
{
    if (!InitType(x))
    {
        // x = non a = non
        if (!InitType(a))
        {
            cout << "TypeCannotBeInferred";
        }
        // if x(non) = a(int)
        x->contain.TypeOut = a->contain.TypeOut;
    }
    else
    {
        // x = int
        if (!InitType(a))
        {
            a->contain.TypeOut = x->contain.TypeOut;
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

    for (int i = 0; i <Size_of_HashTable; i++)
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

void HashTable::ASSIGN(string name, string value)
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
        SymbolNeedAssign->contain.TypeOut = "Number";
        SymbolNeedAssign->contain.Value = value;
    }
    break;
    case 1:
    {
        // x(maybe) = 'abc' (but a is int??)
        SymbolNeedAssign->contain.TypeOut = "String";
        SymbolNeedAssign->contain.Value = value;
    }
    break;
    case 2:
    {
        Symbol *SymbolNeedAssign2; // fun
        SymbolNeedAssign2 = FindSymbol(value.substr(0, value.find('(')));
        // Assign x(type = a) = a;
        AssignTypeOut(SymbolNeedAssign, SymbolNeedAssign2);

        // Check fun(1,2,3)
        value.erase(0, value.find('(') + 1); // 1,a,'b')

        int n = CharCount(value, ',') + 1; // 2
        // check cut each slot 1,a,'b') to store in array
        for (int i = 0; i < n; i++)
        {
            string cutslot = value.substr(0, value.find(',')); // 1
            value.erase(0, value.find(',') + 1);
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
        Symbol *SymbolNeedAssign2;
        SymbolNeedAssign2 = FindSymbol(value);
        // x(maybe) = a(co type)
        AssignTypeOut(SymbolNeedAssign, SymbolNeedAssign2);
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

void HashTable::INSERT(string name, int nTypeIn)
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
                cout << "Redeclared";
                exit(1);
            }
        }
    }
}

void SymbolTable::run(string filename)
{
    cout << "success";
}