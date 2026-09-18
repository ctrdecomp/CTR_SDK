// Filename: nstd_Printf.cpp
//
// Project: Horizon

#include <nn/nstd/nstd_Printf.h>
#include <string>

namespace {
    
template <typename Type>
struct TVSNPrintfImpl
{
    typedef typename Type::char_type CharT;
    struct dst_string{
        size_t len;  // remaining capacity
        CharT* cur;  // current write cursor
        CharT* base; // start of destination
    };

    static void string_put_char (dst_string* p, CharT c)
    {
        if (p->len) 
        {
            *p->cur = c, --p->len;
        }
        ++p->cur;
    }
    static void string_fill_char (dst_string* p, CharT c, int n)
    {
        if (n > 0) 
        {
            size_t i, k = p->len;

            if (p->len > n)
            {
                k = n;
            }
            for (i = 0; i < k; ++i) 
            {
                p->cur[i] = c;
            }
            p->len -= k;
            p->cur += n;
        }
    }
    static void string_put_string (dst_string* p, const CharT* s, int n)
    {
        if (n > 0) 
        {
            size_t i, k = p->len;

            if (p->len > n)
            {
                k = n;
            }
            for (i = 0; i < k; ++i)
            {
                 p->cur[i] = s[i];
            }
            p->len -= k;
            p->cur += n;
        }
    }
    static s32 TVSNPrintf (CharT* dst, size_t len, const CharT* fmt, va_list vlist)
    {
        CharT   buf[24];
        int     n_buf;
        CharT   prefix[2];
        int     n_prefix;

        const CharT *s = fmt;

        dst_string str;
        str.len = len, str.cur = str.base = dst;

        while (*s)
        {
            if (*s != '%')
            {
                string_put_char(&str, *s++);
            }
            else {
                enum
                {
                    flag_blank = 000001,
                    flag_plus = 000002,
                    flag_sharp = 000004,
                    flag_minus = 000010,
                    flag_zero = 000020,
                    flag_l1 = 000040,
                    flag_h1 = 000100,
                    flag_l2 = 000200,
                    flag_h2 = 000400,
                    flag_unsigned = 010000,
                    flag_end
                };
                int     flag = 0, width = 0, precision = -1, radix = 10;
                CharT    hex_char = 'a' - 10;
                const CharT *p_start = s;

                for(;;)
                {
                    switch (*++s)
                    {
                    case '+':
                        if (s[-1] != ' ')
                        {
                            break;
                        }
                        flag |= flag_plus;
                        continue;
                    case ' ':
                        flag |= flag_blank;
                        continue;
                    case '-':
                        flag |= flag_minus;
                        continue;
                    case '0':
                        flag |= flag_zero;
                        continue;
                    }
                    break;
                }

                if (*s == '*')
                {
                    ++s, width = va_arg(vlist, int);
                    if (width < 0)
                    {
                        width = -width, flag |= flag_minus;
                    }
                }
                else
                {
                    while ((*s >= '0') && (*s <= '9'))
                    {
                        width = (width * 10) + *s++ - '0';
                    }
                }

                if (*s == '.')
                {
                    ++s, precision = 0;
                    if (*s == '*')
                    {
                        ++s, precision = va_arg(vlist, int);
                        if (precision < 0)
                        {
                            precision = -1;
                        }
                    }
                    else
                    {
                        while ((*s >= '0') && (*s <= '9'))
                        {
                            precision = (precision * 10) + *s++ - '0';
                        }
                    }
                }

                switch (*s)
                {
                case 'h':
                    if (*++s != 'h')
                    {
                        flag |= flag_h1;
                    }
                    else
                    {
                        ++s, flag |= flag_h2;
                    }
                        break;
                case 'l':
                    if (*++s != 'l')
                    {
                        flag |= flag_l1;
                    }
                    else
                    {
                        ++s, flag |= flag_l2;
                    }
                        break;
                    }

                    switch (*s)
                    {
                    case 'd':
                    case 'i':
                        goto put_integer;
                    case 'o':
                        radix = 8;
                        flag |= flag_unsigned;
                        goto put_integer;
                    case 'u': // unsigned number
                        flag |= flag_unsigned;
                        goto put_integer;
                    case 'X': // signed hex
                        hex_char = 'A' - 10;
                        goto put_hexadecimal;
                    case 'x': // unsigned hex
                        goto put_hexadecimal;
                    case 'p': // pointer
                        flag |= flag_sharp;
                        precision = 8;
                        goto put_hexadecimal;

                    case 'c': // character
                        if (precision >= 0)
                        {
                            goto put_invalid;
                        }
                        {
                            int     c = va_arg(vlist, int);
                            width -= 1;
                            if (flag & flag_minus)
                            {
                                string_put_char(&str, (CharT)c);
                                string_fill_char(&str, ' ', width);
                            }
                            else
                            {
                                CharT    pad = (CharT)((flag & flag_zero) ? '0' : ' ');
                                string_fill_char(&str, pad, width);
                                string_put_char(&str, (CharT)c);
                            }
                            ++s;
                        }
                        break;

                    case 's': // string
                        {
                            int     n_buf2 = 0;
                            const CharT *p_buf = va_arg(vlist, const CharT *);
                            if (precision < 0)
                            {
                                while (p_buf[n_buf2])
                                {
                                    ++n_buf2;
                                }
                            }
                            else
                            {
                                while ((n_buf2 < precision) && p_buf[n_buf2])
                                {
                                    ++n_buf2;
                                }
                            }
                            width -= n_buf2;
                            if (flag & flag_minus)
                            {
                                string_put_string(&str, p_buf, n_buf2);
                                string_fill_char(&str, ' ', width);
                            }
                            else
                            {
                                CharT    pad = (CharT)((flag & flag_zero) ? '0' : ' ');
                                string_fill_char(&str, pad, width);
                                string_put_string(&str, p_buf, n_buf2);
                            }
                            ++s;
                        }
                        break;

                    case 'n': // number
                        {
                            int     pos = str.cur - str.base;
                            if (flag & flag_h2)
                            {
                                ;
                            }
                            else if (flag & flag_h1)
                            {
                                *va_arg(vlist, signed short *) = (signed short)pos;
                            }
                            else if (flag & flag_l2)
                            {
                                *va_arg(vlist, u64 *) = (u64)pos;
                            }
                            else
                            {
                                *va_arg(vlist, signed int *) = (signed int)pos;
                            }
                        }
                        ++s;
                        break;

                    case '%': // output
                        if (p_start + 1 != s)
                        {
                            goto put_invalid;
                        }
                        string_put_char(&str, *s++);
                        break;

                    default: // invalid
                        goto put_invalid;
put_invalid:
    string_put_string(&str, p_start, s - p_start);
    break;

put_hexadecimal:
    radix = 16;
    flag |= flag_unsigned;
put_integer:
{
    u64     val = 0;
    n_prefix = 0;

    if (flag & flag_minus)
    {
        flag &= ~flag_zero;
    }

    if (precision < 0)
    {
        precision = 1;
    }
    else
    {
        flag &= ~flag_zero;
    }

    if (flag & flag_unsigned)
    {
        if (flag & flag_h2)
        {
            val = va_arg(vlist, unsigned char);
        }
        else if (flag & flag_h1)
        {
            val = va_arg(vlist, unsigned short);
        }
        else if (flag & flag_l2)
        {
            val = va_arg(vlist, u64);
        }
        else
        {
            val = va_arg(vlist, unsigned long);
        }

        flag &= ~(flag_plus | flag_blank);
        if (flag & flag_sharp)
        {
            if (radix == 16)
            {
                if (val != 0)
                {
                    prefix[0] = (CharT)(hex_char + (10 + 'x' - 'a'));
                    prefix[1] = '0';
                    n_prefix = 2;
                }
            }
            else if (radix == 8)
            {
                prefix[0] = '0';
                n_prefix = 1;
            }
        }
    }
    else
    {
        if (flag & flag_h2)
        {
            val = va_arg(vlist, char);
        }
        else if (flag & flag_h1)
        {
            val = va_arg(vlist, short);
        }
        else if (flag & flag_l2)
        {
            val = va_arg(vlist, u64);
        }
        else
        {
            val = va_arg(vlist, long);
        }

        if ((val >> 32) & 0x80000000)
        {
            val = ~val + 1;
            prefix[0] = '-';
            n_prefix = 1;
        }
        else
        {
            if (val || precision)
            {
                if (flag & flag_plus)
                {
                    prefix[0] = '+';
                    n_prefix= 1;
                }
                else if (flag & flag_blank)
                {
                    prefix[0] = ' ';
                    n_prefix = 1;
                }
            }
        }
    }
    n_buf = 0;
    switch (radix)
    {
        case 8:
            while (val != 0)
            {
                int     d = (int)(val & 0x07);
                val >>= 3;
                buf[n_buf++] = (CharT)(d + '0');
            }
            break;
        case 10:
            if ((val >> 32) == 0)
            {
                u32     v = (u32)val;
                while (v != 0)
                {
                    u32     r = v / 10;
                    int     d = (int)(v - (r * 10));
                    v = r;
                    buf[n_buf++] = (CharT)(d + '0');
                }
            }
            else
            {
                while (val != 0)
                {
                    u64     r = val / 10;
                    int     d = (int)(val - (r * 10));
                    val = r;
                    buf[n_buf++] = (CharT)(d + '0');
                }
            }
            break;
        case 16:
            while (val != 0)
            {
                int     d = (int)(val & 0x0f);
                val >>= 4;
                buf[n_buf++] = (CharT)((d < 10) ? (d + '0') : (d + hex_char));
            }
            break;
    }
    if ((n_prefix > 0) && (prefix[0] == '0'))
    {
        n_prefix = 0;
        buf[n_buf++] = '0';
    }
}
goto put_to_stream;

put_to_stream:
{
    int     n_pad = precision - n_buf;
    if (flag & flag_zero)
    {
        if (n_pad < width - n_buf - n_prefix)
        {
            n_pad = width - n_buf - n_prefix;
        }
    }
    if (n_pad > 0)
    {
        width -= n_pad;
    }
    width -= n_prefix + n_buf;
    if (!(flag & flag_minus))
        string_fill_char(&str, ' ', width);
    while (n_prefix > 0)
    {
        string_put_char(&str, prefix[--n_prefix]);
    }
    string_fill_char(&str, '0', n_pad);
    while (n_buf > 0)
    {
        string_put_char(&str, buf[--n_buf]);
    }
    if (flag & flag_minus)
        string_fill_char(&str, ' ', width);
    ++s;
                }
            break;
            }
        }
    }

    if (str.len > 0)
    {
        *str.cur = '\0';
    }
    else if (len > 0)
    {
        str.base[len - 1] = '\0';
    }
    return str.cur - str.base;
}
};
} // namespace

namespace nn {
namespace nstd {

s32 TVSNPrintf (char* dst, size_t len, const char* fmt, va_list vlist)
{
    return TVSNPrintfImpl<std::char_traits<char> >::TVSNPrintf (dst, len, fmt, vlist);
}

s32 TSNPrintf (char* dst, size_t len, const char* fmt, ...)
{
    va_list vlist;
    s32     ret;

    va_start (vlist, fmt);
    ret = TVSNPrintf (dst, len, fmt, vlist);
    va_end (vlist);

    return ret;
}

s32 TSPrintf (char* dst, const char* fmt, ...)
{
    va_list vlist;
    s32     ret;

    va_start (vlist, fmt);
    ret = TVSNPrintf (dst, 0x7FFFFFFF, fmt, vlist);
    va_end (vlist);

    return ret;
}

/* wchar_t's */

s32 TVSNPrintf(wchar_t* dst, size_t len, const wchar_t* fmt, va_list vlist)
{
    return TVSNPrintfImpl<std::char_traits<wchar_t> >::TVSNPrintf(dst, len, fmt, vlist);
}

s32 TSNPrintf(wchar_t *dst, size_t len, const wchar_t *fmt, ...)
{
    va_list vlist;
    s32     ret;

    va_start (vlist, fmt);
    ret = TVSNPrintfImpl<std::char_traits<wchar_t> >::TVSNPrintf(dst,len,fmt,vlist);
    va_end (vlist);

    return ret;
}

s32 TSPrintf (wchar_t* dst, const wchar_t* fmt, ...)
{
    va_list vlist;
    s32     ret;

    va_start (vlist, fmt);
    ret = TVSNPrintf(dst, 0x7FFFFFFF, fmt, vlist);
    va_end (vlist);

    return ret;
}

}
}


extern "C" {

s32 nnnstdTVSNPrintf (char* dst, size_t len, const char* fmt, va_list vlist)
{
    return nn::nstd::TVSNPrintf(dst, len, fmt, vlist);
}

s32 nnnstdTSNPrintf(char* dst, size_t len, const char* fmt, ...)
{
    va_list vlist;
    s32     ret;

    va_start (vlist, fmt);
    ret = nn::nstd::TSNPrintf(dst, len, fmt, vlist);
    va_end (vlist);

    return ret;
}

s32 nnnstdTSPrintf (char* dst, const char* fmt, ...)
{
    va_list vlist;
    s32     ret;

    va_start (vlist, fmt);
    ret = nn::nstd::TSPrintf(dst, fmt, vlist);
    va_end (vlist);

    return ret;
}

/* wchar_t */

s32 nnnstdTVSNWPrintf (wchar_t* dst, size_t len, const wchar_t* fmt, va_list vlist)
{
    return nn::nstd::TVSNPrintf(dst, len, fmt, vlist);
}

s32 nnnstdTSNWPrintf (wchar_t* dst, size_t len, const wchar_t* fmt, ...)
{
    va_list vlist;
    s32     ret;

    va_start (vlist, fmt);
    ret = nn::nstd::TSNPrintf(dst, len, fmt, vlist);
    va_end (vlist);

    return ret;
}

s32 nnnstdTSWPrintf (wchar_t* dst, const wchar_t* fmt, ...)
{
    va_list vlist;
    s32     ret;

    va_start (vlist, fmt);
    ret = nn::nstd::TSPrintf(dst, fmt, vlist);
    va_end (vlist);

    return ret;
}
}