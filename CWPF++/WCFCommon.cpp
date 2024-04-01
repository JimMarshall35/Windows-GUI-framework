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

    Alignment ParseAlignment(const wchar_t* t, Alignment defaultVal /*= Alignment::TopLeft*/)
    {
        if (wcscmp(t, L"Top") == 0)
        {
            return Alignment::Top;
        }
        else if (wcscmp(t, L"TopLeft") == 0)
        {
            return Alignment::TopLeft;
        }
        else if (wcscmp(t, L"TopRight") == 0)
        {
            return Alignment::TopRight;
        }
        else if (wcscmp(t, L"Left") == 0)
        {
            return Alignment::Left;
        }
        else if (wcscmp(t, L"Middle") == 0)
        {
            return Alignment::Middle;
        }
        else if (wcscmp(t, L"Right") == 0)
        {
            return Alignment::Right;
        }
        else if (wcscmp(t, L"BottomLeft") == 0)
        {
            return Alignment::BottomLeft;
        }
        else if (wcscmp(t, L"Bottom") == 0)
        {
            return Alignment::Bottom;
        }
        else if (wcscmp(t, L"BottomRight") == 0)
        {
            return Alignment::BottomRight;
        }
        return defaultVal;
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