#pragma once
namespace GDT
{
#pragma pack(push)
#pragma pack(1)
    struct GDT_Entry
    {
        unsigned short Limit_Low;
        unsigned short Base_Low;
        unsigned char Base_Middle;
        unsigned char Access;
        unsigned char Granularity;
        unsigned char Base_High;
    };
    struct GDT_Pointer
    {
        unsigned short Limit;
        void* Base;
    };
#pragma pack(pop)
    bool Initialize();
    extern void SetGate(int number,unsigned int base,unsigned int limit,char access,char granularity);

}