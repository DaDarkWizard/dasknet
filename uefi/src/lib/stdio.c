#include "stdio.h"
#include "stdarg.h"

int sprintf(char *str, const char *format, ...)
{
    int j = 0;
    int i = 0;
    va_list x;
    va_start(x, format);
    for(; i >= 0;)
    {
        if(format[i] == NULL)
        {
            str[j] = NULL;
            va_end(x);
            return j;
        }

        if(format[i] == '%')
        {
            i++;
            if(format[i] == '%')
            {
                str[j++] = '%';
                i++;
            }
            else if(format[i] == 'd')
            {
                i++;
                char buf[20];
                int k = 0;
                long value = va_arg(x, int);
                if(value < 0)
                {
                    value *= -1;
                    str[j++] = '-';
                }
                while(value > 0)
                {
                    buf[k++] = value % 10 + '0';
                    value /= 10;
                }

                if(k == 0)
                {
                    str[j++] = '0';
                }
                else
                {
                    k--;
                    for(;k >= 0; k--)
                    {
                        str[j++] = buf[k];
                    }
                }
            }
            else if(format[i] == 's')
            {
                i++;
                char* value = va_arg(x, char*);
                for(int k = 0; value[k] != NULL && k >= 0; k++)
                {
                    str[j++] = value[k];
                }
            }
            else if(format[i] == NULL)
            {
                str[j] = NULL;
                return j;
            }
            else
            {
                i++;
            }
        }
        else
        {
            str[j++] = format[i++];
        }
    }
    va_end(x);
    return j;
}