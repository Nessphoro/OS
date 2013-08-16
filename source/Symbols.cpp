#include "API/Symbols.h"
#include "String.h"
#include "Graphics/Console.h";
#include "Graphics/Graphics.h"
#include "Globals.h"


namespace Symbols
{
    unsigned int SymbolsStart;
    unsigned int SymbolsEnd;
    unsigned int TotalSymbols;

    unsigned int SymbolNamesStart;
    unsigned int SymbolNamesEnd;

    void Initialize(MultibootInfo* multiboot)
    {
        ELFSectionHeader* Headers=(ELFSectionHeader*)multiboot->TableUnition.Elf_sec.Addr;
        for (int i = 0; i < multiboot->TableUnition.Elf_sec.Num; i++)
        {
            ELFSectionHeader Header=Headers[i];
            if(Header.Type==2)
            {
                SymbolsStart=Header.Address+KernelVirtualBase;
                SymbolsEnd=Header.Address+KernelVirtualBase+Header.Size;
                TotalSymbols=(SymbolsEnd-SymbolsStart)/sizeof(Symbol);
                ELFSectionHeader StringTable=Headers[Header.Link];
                SymbolNamesStart=StringTable.Address+KernelVirtualBase;
                SymbolNamesEnd=StringTable.Address+KernelVirtualBase+StringTable.Size;
                break;
            }
        }
    }

    Symbol* GetSymbol(const char* name)
    {
        return GetSymbol(name,false);
    }

    Symbol* GetSymbol(const char* name,bool isMangled)
    {
        Symbol* start=(Symbol*)SymbolsStart;
        char* mangledName=(isMangled?(char*)name:Mangle(name));
        
        for(int i=0;i<TotalSymbols;i++)
        {
            Symbol symbol=start[i];
            bool result=String::Compare(GetName(&symbol),mangledName);
            if(result)
                return &symbol;
        }

        return 0;
    }

    const char* GetName(Symbol* symbol)
    {
        return (const char*)(SymbolNamesStart+symbol->Name);
    }

    /*Mangles a C++ symbol by Intel ABI specification*/
    char* Mangle(const char* symbolName)
    {
        return "";
    }
    /*Unmangles a C++ symbol by Intel ABI specification*/
    char* Unmangle(const char* symboldName)
    {
        return "";
    }
}