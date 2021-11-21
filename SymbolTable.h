#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable
{
private:
 string Name;
 int Level;
 int Index;
public:
    SymbolTable() {}
    void run(string filename);
};
#endif