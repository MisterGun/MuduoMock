#pragma once

#include <string>
#include <vector>
#include <stdarg.h>

#pragma warning(disable : 4127)

#if _MSC_VER < 1500
#define vsnprintf(buf, length, format, ap) _vsnprintf(buf, length, format, ap)
#else
#define vsnprintf(buf, length, format, ap) _vsnprintf_s(buf, length, _TRUNCATE, format, ap)
#endif



namespace StringHelper
{

    using namespace std;

        
    static const char* spaces = " \t\n\v\f\r";    
    inline void TrimLeft(string &s)
    {
        size_t pos = s.find_first_not_of(spaces);
        if(string::npos == pos)
        {
            s = "";
        }
        else
        {
            s = s.substr(pos);
        }
    }

    inline void TrimRight(string &s)
    {
        size_t pos = s.find_last_not_of(spaces);
        if(string::npos == pos)
        {
            s = "";
        }
        else
        {
            s = s.substr(0, pos+1);
        }
    }

    inline void Trim(string &s)
    {
        TrimLeft(s);
        TrimRight(s);
    }

    inline void Upper(string &s)
    {
        for(size_t i=0; i<s.length(); i++)
        {
            s[i] = (char)toupper(s[i]);
        }
    }

    inline void Lower(string &s)
    {
        for(size_t i=0; i<s.length(); i++)
        {
            s[i] = (char)tolower(s[i]);
        }
    }

    inline void ReplaceAll(string &s, const string& src, const string &dst)
    {
        int nPos = 0;
        
        while((nPos=s.find(src, nPos)) != -1)
        {
            s.replace(nPos, src.length(), dst.c_str());
            nPos += dst.length();
        }
    }

    inline bool EndsWith(const string &s, const string& suffix)
    {
        size_t pos = s.rfind(suffix);
        if(pos == string::npos)
            return false;

        return pos+suffix.length() == s.length();
    }

    inline bool BeginsWith(const string &s, const string &prefix)
    {
        size_t pos = s.find(prefix);
        return pos == 0;
    }

    //sep为空字符串时，取出连续空白符分隔的字符串
    inline vector<string> Split(const string &s, const string &sep="", unsigned int nMaxSplit=-1)
    {
        vector<string> vecStr;

        if(!sep.empty())
        {
            string::size_type iBeginPos = 0;
            string::size_type iEndPos = 0;
            while((iEndPos=s.find(sep, iBeginPos)) != string::npos)
            {
                if(vecStr.size() == nMaxSplit)
                {
                    break;
                }
                vecStr.push_back(s.substr(iBeginPos, iEndPos-iBeginPos));
                iBeginPos = iEndPos + sep.length();
                
            }
            if(vecStr.size() != nMaxSplit)
            {
                vecStr.push_back(s.substr(iBeginPos));
            }
            
        }
        else
        {
            string::size_type iBeginPos = 0;
            string::size_type iEndPos = 0;

            while((iEndPos=s.find_first_of(spaces, iBeginPos)) != string::npos)
            {
                if(vecStr.size() == nMaxSplit)
                {
                    break;
                }

                vecStr.push_back(s.substr(iBeginPos, iEndPos-iBeginPos));
                iBeginPos = s.find_first_not_of(spaces, iEndPos);
            }
            if(vecStr.size() != nMaxSplit)
            {
                vecStr.push_back(s.substr(iBeginPos));
            }
        }
        
        
        return vecStr;
    }

    inline void InternalStringPrintf(string& output, const char* format, va_list ap)
    {   
        char smallBuf[128];
        int nCharsWritten = vsnprintf(smallBuf, sizeof(smallBuf), format, ap);
        //if smallBuf is enough
        if(nCharsWritten >= 0)
        {
            output.append(smallBuf, nCharsWritten);
            return;
        }
        
        size_t length = sizeof(smallBuf);
        while(true)
        {
            length *= 2;
            char *buf = new char[length];

            nCharsWritten = vsnprintf(buf, length, format, ap);
            if(nCharsWritten >= 0)
            {
                output.append(buf, nCharsWritten);
                delete []buf;
                return;
            }
            delete []buf;
        }
    }
    
    inline void FormatString(string& output, const char* format, ...) 
    {
        va_list ap;
        va_start(ap, format);
        output.erase();
        InternalStringPrintf(output, format, ap);
        va_end(ap);
    }
    
    inline void AppendString(string& output, const char* format, ...) 
    {
        va_list ap;
        va_start(ap, format);
        InternalStringPrintf(output, format, ap);
        va_end(ap);
    }
    
    inline string Format(const char* format, ...) 
    {
        va_list ap;
        va_start(ap, format);
        std::string output;
        InternalStringPrintf(output, format, ap);
        va_end(ap);
        return output;
    }

    inline string ToString(int i)
    {
        return Format("%d", i);
    }

    inline string ToString(double d)
    {
        //防止格式化出-0
        if(d == 0)
        {
            d = 0; 
        }
        return Format("%g", d);
    }
}