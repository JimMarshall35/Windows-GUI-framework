#include "WCFCommon.h"
#include <string>
#include <cassert>
namespace CWPF
{

    int GetControlID()
    {
        static int i = 1;
        return i++;
    }

    Length ParseLength(const wchar_t* val)
    {
        Length l = {};
        if (wcslen(val) == 1 && val[0] == '*')
        {
            l.Type = LengthType::Stretch;
            l.Value = 0.0;
        }
        else if (wcscmp(val, L"auto") == 0)
        {
            l.Type = LengthType::Auto;
            l.Value = 0.0;
        }
        else
        {
            l.Type = LengthType::Fixed;
            try
            {
                l.Value = std::stof(val);
            }
            catch (...)
            {
                assert(false);
                l.Type = LengthType::Invalid;
            }
        }
        return l;
    }


    HorizontalAlignment ParseHAlignment(const wchar_t* t, HorizontalAlignment defaultVal)
    {
        if (wcscmp(t, L"Left") == 0)
        {
            return HorizontalAlignment::Left;
        }
        else if (wcscmp(t, L"Middle") == 0)
        {
            return HorizontalAlignment::Middle;
        }
        else if (wcscmp(t, L"Right") == 0)
        {
            return HorizontalAlignment::Right;
        }
        return defaultVal;
    }

    VerticalAlignment ParseVAlignment(const wchar_t* t, VerticalAlignment defaultVal/* = VerticalAlignment::Top*/)
    {
        if (wcscmp(t, L"Top") == 0)
        {
            return VerticalAlignment::Top;
        }
        else if (wcscmp(t, L"Middle") == 0)
        {
            return VerticalAlignment::Middle;
        }
        else if (wcscmp(t, L"Bottom") == 0)
        {
            return VerticalAlignment::Bottom;
        }
        return defaultVal;
    }

    bool ParseColourAttribString(const wchar_t* str, Colour& c)
    {
        wchar_t buf[32];
        const wchar_t* read = str;
        wchar_t* write = buf;
        for (int i = 0; i < 3; i++)
        {
            while (*read  != '\0' && *read != ',')
            {
                *write++ = *read++;
            }
            read++;
            *write = '\0';
            int v = _wtoi(buf);
            write = buf;
            if (v > 255)
            {
                return false;
            }
            switch (i)
            {
            case 0:
                c.r = v;
                break;
            case 1:
                c.g = v;
                break;
            case 2:
                c.b = v;
                break;
            }
        }
        
        return true;
    }
}