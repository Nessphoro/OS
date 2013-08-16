#include "String.h"

namespace String
{
    int Length(const char* string)
    {
        register const char* str=string;
        register int length=0;

        while(*str++!=0)
        {
            length++;
        }
        return length;
    }

    bool Compare(const char* string1,const char* string2)
    {
        int length1=Length(string1);
        int length2=Length(string2);

        if(length1!=length2)
            return false;

        for(int i=0;i<length1;i++)
        {
            if(string1[i]!=string2[i])
                return false;
        }

        return true;
    }

    bool Compare(const char* string1,const char* string2,int length)
    {
        for(int i=0;i<length;i++)
        {
            if(string1[i]!=string2[i])
                return false;
        }

        return true;
    }
}