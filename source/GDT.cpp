#include "Boot/GDT.h"

extern "C" void GDT_Flush(void*);
namespace GDT
{
    GDT_Entry Entires[5];
    GDT_Pointer Pointer;
    bool Initialize()
    {
        Pointer.Limit=sizeof(GDT_Entry)*5-1;
        Pointer.Base=&Entires[0];
        SetGate(0, 0, 0, 0, 0);                // Null segment
        SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
        SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
        SetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
        SetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

        GDT_Flush(&Pointer);

        return true;
    }
    bool Deinitialize()
    {
        return true;
    }

    void SetGate(int number,unsigned int base,unsigned int limit,char access,char granularity)
    {
        Entires[number].Base_Low    = (base & 0xFFFF);
        Entires[number].Base_Middle = (base >> 16) & 0xFF;
        Entires[number].Base_High   = (base >> 24) & 0xFF;

        Entires[number].Limit_Low   = (limit & 0xFFFF);
        Entires[number].Granularity = (limit >> 16) & 0x0F;

        Entires[number].Granularity |= granularity & 0xF0;
        Entires[number].Access      = access;
    }
}