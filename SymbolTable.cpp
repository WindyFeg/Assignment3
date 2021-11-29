#include "SymbolTable.h"

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

Symbol::Symbol(string name, int nTypeIn, int level)
{
    this->contain.Identifier = name;
    for (auto &x : this->contain.TypeIn)
    {
        x = "Undefined";
    }
    this->level_of_block = level;
    this->key = HASH_NAME(this);
    this->contain.nTypeIn = nTypeIn;
}
// Symbol::Symbol(string name, int level, int key)
// {
//     this->contain.Identifier = name;
//     this->key = key;
//     this->level_of_block;
// }

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
        if (arr[j].contain.Identifier != "")
        {

            Res = Res + to_string(j) + " " + arr[j].contain.Identifier + "//" + to_string(arr[j].level_of_block) + ";";
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

long long HashTable::LOOK_UP(string name, int level, int HashType)
{
    int i = 0;
    for (i = level; i >= 0; i--)
    {
        for (int j = 0; j < Size_of_HashTable; j++)
        {
            if (arr[j].level_of_block == i && arr[j].contain.Identifier == name)
            {
                // return HASH_CHOSE(arr[j].key, HashType);
                return j;
            }
        }
    }
    return -1;
}

void HashTable::DeleteSymbolLevel(string Er, int level)
{
    // defaut level is 0 =>
    Symbol a;
    for (int i = 0; i < Size_of_HashTable; i++)
    {
        if (this->arr[i].level_of_block == level)
        {
            arr[i] = a;
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
    // Ki tu dau chu normal, con lai (chu normal, in hoa, _, so)
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
        // check behind
        if (value.find('(') != -1)
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

    AssignTypeOut(SymbolNeedAssign, SymbolNeedAssign2, Er); // Infer
    // check front
}

int HashTable::CheckType2(string cutslot)
{
    if (FindSymbol(cutslot) == NULL)
    {
        // string or int
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
        int a = String.find(Char);
        String.erase(0, a + 1);
        if (a == -1)
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
        // x(maybe) = a(co type)
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
            return 2;
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

void HashTable::INSERT(string name, string Er, int TypeHash, int level, int nTypeIn)
{
    CheckStringName(name,Er);
    Symbol *NewSymbol = new Symbol(name, nTypeIn, level);
    int index_in_map = HASH_CHOSE(NewSymbol->key, TypeHash);

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
            index_in_map = HASH_CHOSE_P(NewSymbol->key, i, TypeHash);
            if (arr[index_in_map].key == 0)
            {
                arr[index_in_map] = *NewSymbol;
                cout << i << endl;
                arr[index_in_map].contain.slot = i;
                return;
            }
            if (arr[index_in_map] == *NewSymbol)
            {
                throw Redeclared(Er);
                exit(1);
            }
        }
    }
}

void HashTable::CALL(string name, string Er)
{
    if (name.find('(') == -1 || name.find(')') == -1)
    {
        throw InvalidInstruction(Er);
        exit(1);
    }

    Symbol *SymbolNeedAssign2; // fun
    SymbolNeedAssign2 = FindSymbol(name.substr(0, name.find('(')));
    // Assign x(type = a) = a;

    if (SymbolNeedAssign2 == NULL)
    {
        throw Undeclared(name.substr(0, name.find('(')));
        exit(1);
    }
    if (SymbolNeedAssign2 != NULL)
    {
        // check behind
        if (name.find('(') != -1)
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
            // LINEAR
            line.erase(0, line.find(' ') + 1);
            BangBam.HASH_LINEAR_MAP(stol(line.substr(0, line.find(' '))), std::stol(line.substr(line.find(' ') + 1)));
            TypeHash = 0;
            break;
        }
        case 1:
        {
            // QUADRATIC
            line.erase(0, line.find(' ') + 1);
            long long a = stol(line.substr(0, line.find(' ')));
            line.erase(0, line.find(' ') + 1);
            BangBam.HASH_QUADRATIC_MAP(a, stol(line.substr(0, line.find(' '))), stol(line.substr(line.find(' ') + 1)));
            TypeHash = 2;
            break;
        }
        case 2:
        {
            // DOUBLE
            line.erase(0, line.find(' ') + 1);
            BangBam.HASH_DOUBLE_MAP(stol(line.substr(0, line.find(' '))), std::stol(line.substr(line.find(' ') + 1)));
            TypeHash = 1;
            break;
        }
        case 10:
        {
            // INSERT
            line.erase(0, line.find(' ') + 1);

            if (levelNow != 0 && line.find(' ') != -1)
            {
                throw InvalidDeclaration(line.substr(0, line.find(' ')));
                exit(1);
            }

            if (line.find(' ') == -1)
            {
                // dang add string
                BangBam.INSERT(line, line, TypeHash, levelNow);
            }
            else
            {
                // dang ham
                BangBam.INSERT(line.substr(0, line.find(' ')), line, TypeHash, levelNow, stol(line.substr(line.find(' ') + 1)));
            }

            break;
        }
        case 11:
        {
            // ASSIGN
            line.erase(0, line.find(' ') + 1);
            BangBam.ASSIGN(line.substr(0, line.find(' ')), line, line.substr(line.find(' ') + 1), TypeHash);
            // temporary we print out 0, then we count number of hash to print
            break;
        }
        case 12:
        {
            // Call
            // Undeclared, TypeMismatch, TypeCannotBeInferred.
            //  (0 1 3)
            line.erase(0, line.find(' ') + 1);
            BangBam.CALL(line, "CALL " + line);

            break;
        }
        case 13: // begin
        {
            levelNow++;
        }
        break;
        case 14: // end
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
        { // LOOKUP
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
            // PRINT
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