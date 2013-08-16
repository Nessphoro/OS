#pragma once
#include "EventHandler.h"

struct Registers
{
    unsigned int DS;      /* pushed the segs last */
    unsigned int EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;  /* pushed by 'pusha' */
    unsigned int INT, Error;    /* our 'push byte #' and ecodes do this */
    unsigned int EIP, CS, EFLAGS, OldESP, SS;   /* pushed by the processor automatically */ 
};

namespace IDT
{
#pragma pack(push)
#pragma pack(1)
    struct IDT_Entry
    {
        unsigned short Base_Low;
        unsigned short Selector;
        unsigned char Always0;
        unsigned char Flags;
        unsigned short Base_High;
    };
    struct IDT_Pointer
    {
        unsigned short Limit;
        void* Base;
    };
#pragma pack(pop)
    bool Initialize();
    extern EventHandler EventHandlers[];
    extern void SetGate(unsigned char number,unsigned int base,unsigned short selector,unsigned char flags);
    extern void DebugException(Registers* registers);
    extern void InternalHandler(Registers* registers);
}

extern "C" void ExceptionHandler(Registers* registers);
extern "C" void IRQHandler(Registers* registers);