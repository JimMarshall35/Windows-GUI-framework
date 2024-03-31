#include "WCFCommon.h"
#include <string>
namespace CWPF
{

    HMENU GetControlID()
    {
        static int i = 1;
        return (HMENU)i++;
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
}