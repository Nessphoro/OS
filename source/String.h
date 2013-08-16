#pragma once

namespace String
{
    /*
    Get length of a string
    */
    extern int Length(const char* string);
    /*
    Compare two strings
    */
    extern bool Compare(const char* string1,const char* string2);
    /*
    Compare two strings
    */
    extern bool Compare(const char* string1,const char* string2, int length);
}