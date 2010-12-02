#include "rtx.h"
#include "utils.h"

int _findChar (CHAR ch, const CHAR * str);

CHAR * rtx_sprintf(CHAR * str, const CHAR * format, void * params[])
{
	int i = 0, j = 0, k = 0;
	while (format[k] != '\0')
	{
		if(format[k] == '%')
		{
            k++;
            int spaces = format[k] - '0';
            if(spaces < 10 && spaces > 0)
                k++;
            else
                spaces = 0;
			if(params[i] != NULL)
			{
				if(format[k] == 'c')
				{
					//if((params[i])[0] == NULL)
					//	return ERROR;
                    
                    while(spaces > 1)
                    {
                        str[j] = ' ';
                        j++;
                        spaces--;
                    }  
                    
					str[j] = ((CHAR *)(params[i]))[0];
					i++;
					j++;
					k++;                      
				}
				else if(format[k] == 's')
				{
					CHAR * p_str = params[i];
					i++;
					int temp_i = 0;
					while (p_str[temp_i] != '\0')
					{
						str[j] = p_str[temp_i];
						j++;
						temp_i++;
					}
					k++;
				}
				else if(format[k] == 'i' || 
                        format[k] == 'd' || 
                        format[k] == 'u')
				{
					int size = 0, div = 1;
					int num = *((int *) params[i]);
					int neg = 0;
					i++;

					if(num < 0)
					{
						neg = 1;
						num *= -1;
					}

					while(num/div > 0)
					{
						size++;
						div*=10;
					}
					div/=10;

					if(size == 0)
					{
						str[j] = '0';
						j++;
					}
                    
                    if(size < spaces)
                    {	
                        while(spaces > size)
                        {
                            str[j] = ' ';
                            j++;
                            spaces--;
                        }
                    }
                    
					if(neg == 1)
					{
						str[j] = '-';
						j++;
					}
                    
                    while(size > 0)
                    {
                        int digit = num/div;
                        str[j] = '0' + digit;
                        j++;
                        size--;
                        num -= digit*div;
                        div/=10;
                    }
					k++;
				}
				else
				{
					str[j] = '%';
					j++;
					str[j] = format[k];
					j++;
					k++;
				}
			}
			else
			{
				return NULL;
			}		
		}
		else
		{
			str[j] = format[k];
			k++;
			j++;
		}
		str[j] = '\0';
	}
	return str;
}

CHAR * rtx_strcpy(CHAR * str, const CHAR * cpy_str)
{
	void * params[] = {cpy_str};
	return rtx_sprintf(str, "%s", params);
}

int rtx_strcmp(const CHAR * str1, const CHAR * str2)
{
	if(str1 == NULL || str2 == NULL)
		return -1;
	
	int k = 0;
	while (str1[k] != '\0' && str2[k] != '\0')
	{
		if(str1[k] != str2[k])
			return 1;
        k++;
	}
	return 0;
}

void rtx_strtok(const CHAR * str, CHAR * ret, const CHAR * delimiters)
{
    static char copy [100] = "";
    static BYTE i = 0;

    if ( str != NULL )
    {
        rtx_strcpy ( copy, str );
        i = 0;
    }

    BYTE j = 0;
    
    //skip leading characters to be ignored
    while ( _findChar( copy[i], delimiters ) == 1 )
    {
        i++;
    }
    
    while ( copy[i] != '\0' && _findChar( copy[i], delimiters ) == 0 )
    {
        ret[j] = copy [i];
        i++;
        j++;
    }
    ret[j]='\0';
}

int rtx_atoi (const CHAR * str, int *num_p)
{
    if (str == NULL || num_p == NULL)
    {
        return 0;
    }

    int neg, i, num, ret;
    i = num = ret = 0;
    neg = 1;
    
    while (str[i] == ' ')
    {
        i++;
    }

    if (str[i] == '-')
    {
        neg = -1;
        i++;
    }
    else if (str[i] == '+')
    {
        i++;
    }

    while (str[i] != '\0')
    {
        char tmp = str[i] - '0';    
        if (tmp < 0 || tmp > 10)
        {
            return 0;
        }
        num *= 10;
        num += tmp;
        ret = 1;
    }
    *num_p = neg*num;
    return ret;
}

int _findChar (CHAR ch, const CHAR * str)
{
    if (str == NULL)
    {
        return -1;
    }

    int i = 0;
    while ( str[i] != '\0' )
    {
        if ( ch == str[i] )
        {
            return 1;
        }
        i++;
    }
    return 0;
}
