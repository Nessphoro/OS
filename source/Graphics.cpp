#include "Graphics/Graphics.h"
#include "Memory/Memory.h"
#include "Globals.h"

namespace Graphics
{
    volatile unsigned char* volatile VRAM;
    unsigned int BBP;
    volatile Console* volatile MainConsole;

    void Initialise_NoGraphics()
    {
        VRAM=(unsigned char*)PhysicalToVirtual(0xB8000);
        BBP=8;
    }
    void SetConsole(Console* console)
    {
        MainConsole=console;
        MainConsole->Dirty=true;
    }
    Console* GetConsole()
    {
        return (Console*)MainConsole;
    }
    void RefreshConsole()
    {
        register volatile bool* volatile Dirty=&MainConsole->Dirty;
        if(*Dirty)
        {
            unsigned int Chars=MainConsole->Width*MainConsole->Height*2;

            register volatile unsigned short* volatile sVRAM=(unsigned short*)VRAM;
            register volatile unsigned short* volatile ConsoleBuffer=MainConsole->Buffer;
			Memory::Memory_Copy((void*)sVRAM,(void*)ConsoleBuffer,Chars);
            /*
			for(register unsigned int i=0;i<Chars;i++)
            {
                *sVRAM++=*ConsoleBuffer++;
            }
			*/
        }
        *Dirty=false;
    }
    void ForceRefresh()
    {
        volatile bool* volatile Dirty=&MainConsole->Dirty;
        unsigned int Chars=MainConsole->Width*MainConsole->Height*2;

        volatile unsigned short* volatile sVRAM=(unsigned short*)VRAM;
        volatile unsigned short* volatile ConsoleBuffer=MainConsole->Buffer;
		Memory::Memory_Copy((void*)sVRAM,(void*)ConsoleBuffer,Chars);
		/*
        for(register unsigned int i=0;i<Chars;i++)
        {
            *sVRAM++=*ConsoleBuffer++;
        }
		*/


        *Dirty=false;
    }
}