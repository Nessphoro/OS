#include "Graphics/Console.h"
#include "Memory/Memory.h"
#include "String.h"

void Console::Initialise_NoGraphics()
{
    this->Width=80;
    this->Height=25;
    this->X=0;
    this->Y=0;
    this->Dirty=false;
    this->Color=15;
    this->Buffer=(unsigned short*)Memory::StaticMalloc(this->Height*this->Width*2);
    Memory::Memory_Clear((void*)this->Buffer,this->Width*this->Height*2);
}
void Console::Write(const unsigned char c)
{
    Dirty=true;
    if(c==0)
        return;
    else if(c==0x08||c==0x7F)
    {
        Buffer[Y*Width+X]=32;
        if(X!=0)
        {
            X--;
            Buffer[Y*Width+X]=32;
        }
    }
    else if(c == '\r')
    {
        X = 0;
    }
    else if(c=='\n')
    {
        Y++;
        X=0;
    }
    else if(c=='\t')
    {
        X = (X + 8) & ~(8 - 1);
    }
    else if(c>=32)
    {
        Buffer[Y*Width+X]=c|Color<<8;
        X++;
    }

	if(X >= Width)
    {
        X = 0;
        Y++;
    }

    Scroll();
}
void Console::Write(const char *s)
{
    Dirty=true;
    const char* string=s;
    while(*string!=0)
    {
        Write(*string++);
    }
}
void Console::WriteLine(const char* s)
{
    Dirty=true;
    Write(s);
    Write('\n');
}
void Console::WriteLine()
{
    Dirty=true;
    this->Write('\n');
}
void Console::WriteNumber(unsigned int number)
{
    Dirty=true;
    if(number==0)
    {
        this->Write("0x0");
        return;
    }

    const char* charSet="0123456789ABCDEF";
    char temp[9]={0,0,0,0,0,0,0,0,0};
    int index=0;
    while (number>0)
    {
        if(index>8)
            break;
        temp[index++]=charSet[number%16];
        number/=16;
    }

    this->Write("0x");
    for (unsigned int i=0;i<String::Length(temp);i++)
    {
        this->Write(temp[String::Length(temp)-i-1]);
    }
}





void Console::WriteSubsystem(const char* message,const char* subsystem,unsigned int importance)
{
    Dirty=true;
    int OldColor=Color;
    int ImportanceColor=GREEN;
    switch(importance)
    {
    case 1:
        ImportanceColor=GREEN;
    case 2:
        ImportanceColor=LIGHT_BROWN;
    case 3:
        ImportanceColor=RED;

    }
    Console::SetColor(ImportanceColor);
    Console::Write('[');
    Console::Write(subsystem);
    Console::Write(']');
	Console::SetColor(OldColor);
    Console::Write(message);
}
void Console::WriteLineSubsystem(const char* message,const char* subsystem,unsigned int importance)
{
    Dirty=true;
    int OldColor=Color;
    int ImportanceColor=GREEN;
    switch(importance)
    {
    case 1:
        ImportanceColor=GREEN;
        break;
    case 2:
        ImportanceColor=LIGHT_BROWN;
        break;
    case 3:
        ImportanceColor=RED;
        break;

    }
    Console::SetColor(ImportanceColor);
    Console::Write('[');
    Console::Write(subsystem);
    Console::Write('] ');
	Console::SetColor(OldColor);
    Console::WriteLine(message);
    
}
void Console::SetColor(const unsigned char c)
{
    Dirty=true;
    this->Color=c;
}
void Console::Scroll()
{
    unsigned int Temp;
    if(Y>=Height)
    {
        Dirty=true;
        Temp = Y - Height +1;
        Memory::Memory_Copy((void*)Buffer,(void*)((Buffer)+Temp*Width),(Height-Temp)*Width*2);
        Memory::Memory_Clear((void*)((Buffer)+(Height-1)*Width),Width*2);
        Y = Height-1;
    }
}
void Console::Clear()
{
    Dirty=true;
    Memory::Memory_Clear((void*)Buffer,Width*Height*2);
    Y=0;
    X=0;
}


