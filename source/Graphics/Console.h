/*Wrapper for the Console Interface*/
#pragma once
#define endl '\n'

enum Color
{
    BLACK=0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_BROWN,
    WHITE,

};


class Console
{
public:
    volatile unsigned short* Buffer;
    unsigned int Width;
    unsigned int Height;
    volatile unsigned int X;
    volatile unsigned int Y;
    unsigned char Color;
    volatile bool Dirty;


    void Write(const unsigned char c);
    void Write(const char* s);

    void WriteLine(const char* s);
    void WriteLine();
    void WriteMiddle(const char* s);

    void WriteSubsystem(const char* message,const char* subsystem,unsigned int importance);
    void WriteLineSubsystem(const char* message,const char* subsystem,unsigned int importance);

    void WriteNumber(unsigned int number);

    void SetColor(const unsigned char c);
    void Scroll();
    void Clear();

    void Initialise_NoGraphics();

    Console& operator<<(enum Color data)
    {
        this->SetColor(data);
        return *this;
    }
    Console& operator<<(int data)
    {
        this->WriteNumber((unsigned int)data);
        return *this;
    }
    Console& operator<<(unsigned int data)
    {
        this->WriteNumber(data);
        return *this;
    }
    Console& operator<<(const char* data)
    {
        this->Write(data);
        return *this;
    }
    Console& operator<<(const unsigned char data)
    {
        this->Write(data);
        return *this;
    }
    Console& operator<<(char data)
    {
        this->Write(data);
        return *this;
    }
    Console& operator<<(bool data)
    {
        if(data)
        {
            this->Write("true");
        }
        else
        {
            this->Write("false");
        }
        return *this;
    }
};
    