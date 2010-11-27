#include "rtx.h"

CHAR * rtx_spritf(CHAR * str, const CHAR * format, void * params[])
{
	int i = 0, j = 0, k = 0;
	while (format[k] != '\0')
	{
		if(format[k] == '%')
		{
			k++;
			if(params[i] != NULL)
			{
				if(format[k] == 'c')
				{
					//if((params[i])[0] == NULL)
					//	return ERROR;
					str[j] = (params[i])[0];
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
				else if(format[k] == 'i')
				{
					int size = 0, div = 1;
					int num = params[i];
					i++;
					while(num/div > 0)
					{
						size++;
						div*=10;
					}
					div/=10;
					
					while(size > 0)
					{
						int digit = num/div;
						str[j] = 0x(30 + digit);
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
				return ERROR;
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