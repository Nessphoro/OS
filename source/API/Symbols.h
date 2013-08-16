#pragma once
#include "Multiboot.h"
namespace Symbols
{
#pragma pack(push)
#pragma pack(1)
    struct Symbol
    {

        unsigned int Name;
        unsigned int Value;
        unsigned int Size;
        unsigned char Info;
        unsigned char Other;
        unsigned short Section;
    };
#pragma pack(pop)

    /*Initialize the symbols search system*/
    void Initialize(MultibootInfo* multiboot);

    /*Find a symbol by its unmangled name*/
    Symbol* GetSymbol(const char* name);

     /*Find a symbol, by either mangled or unmangled name*/
    Symbol* GetSymbol(const char* name,bool isMangled);

    /*Returns nmangled symbol name*/
    const char* GetName(Symbol* symbol);

    /*Mangles a C++ symbol by Intel ABI specification*/
    char* Mangle(const char* symbolName);
    /*Unmangles a C++ symbol by Intel ABI specification*/
    char* Unmangle(const char* symboldName);
}