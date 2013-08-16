#include "Memory/Memory.h"
#include "Graphics/Graphics.h"
#include "Globals.h"
#include "EventHandler.h"

EventHandler::EventHandler()
{
    Memory::Memory_Clear(Handlers,1024);
}
void EventHandler::Invoke(void* Context)
{
    #ifdef INVOKE_TRACE
    *Graphics::GetConsole()<<"EventHandler Invoked"<<endl;
#endif
    for (int i = 0; i < 256; i++)
    {
        if(EventHandler::Handlers[i]==0)
            break;
        else
        {
#ifdef INVOKE_TRACE
            *Graphics::GetConsole()<<"Invoking function:"<<(unsigned int)EventHandler::Handlers[i]<<"\n";
#endif
            __asm volatile("pushl %0"::"b"(Context));
            EventHandler::Handlers[i]();
            __asm volatile("add $4, %ESP");
        }
    }
}
EventHandler& EventHandler::operator+=(void(*function)())
{
    for (int i = 0; i < 256; i++)
    {
        if(EventHandler::Handlers[i]==0)
        {
            EventHandler::Handlers[i]=function;
            break;
        }
    }

    return *this;
}
EventHandler& EventHandler::operator-=(void(*function)())
{
    int Position;
    for (int i = 0; i < 256; i++)
    {
        if(EventHandler::Handlers[i]==function)
        {
            Position=i;
            break;
        }
    }
    for(int i=Position;i<255;i++)
    {
        EventHandler::Handlers[i]=EventHandler::Handlers[i+1];
    }
    return *this;
}
EventHandler& EventHandler::operator+=(void(*function)(void *))
{
    for (int i = 0; i < 256; i++)
    {
        if(EventHandler::Handlers[i]==0)
        {
            EventHandler::Handlers[i]=((void(*)())function);
            break;
        }
    }
    return *this;
}
EventHandler& EventHandler::operator-=(void(*function)(void*))
{
    
    int Position;
    for (int i = 0; i < 256; i++)
    {
        if(EventHandler::Handlers[i]==((void(*)())function))
        {
            Position=i;
            break;
        }
    }
    for(int i=Position;i<255;i++)
    {
        EventHandler::Handlers[i]=EventHandler::Handlers[i+1];
    }
    return *this;
}
EventHandler& EventHandler::operator()()
{
    Invoke(0);
    return *this;
}
EventHandler& EventHandler::operator()(void* Context)
{
    Invoke(Context);
    return *this;
}