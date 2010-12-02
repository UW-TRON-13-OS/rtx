#include "utils.h"
#include "trace.h"

int main ()
{
    char str [100];
    void *params [11];
    char tok [20];
    char *tok_p, *str_p;
    tok_p = tok;
    str_p = str;
    int num, ret;

    // test sprintf
    params[0] = NULL;
    trace (ALWAYS, rtx_sprintf (str,"Simple string",params));

    tok_p = "fastly";
    num = -14;
    ret = 20;
    params[0] = tok_p;
    params[1] = &num;
    params[2] = &ret;
    params[3] = NULL;
    trace (ALWAYS, rtx_sprintf (str,"Mix %s, %4d, %6d",params)); 
    trace (ALWAYS, rtx_sprintf (str,"Too many params %s, %4d",params));
    trace (ALWAYS, rtx_sprintf (str,"Too few params %s %4d %d %d",params));
    trace (ALWAYS, rtx_sprintf (str,"Mismatch %s, %s, %d", params));

    trace (ALWAYS, rtx_strcpy (str_p, tok_p));//should be fine 
    str_p = "Really long string thing";
    trace (ALWAYS, rtx_strcpy (tok_p, str_p));//should fail 
    
    trace_uint (ALWAYS,"0", rtx_strcmp("abc","abc")); //0 
    trace_uint (ALWAYS,"1", rtx_strcmp("","abc")); //1
    trace_uint (ALWAYS, "1",rtx_strcmp("abc","")); //1 
    trace_uint (ALWAYS, "1",rtx_strcmp("ab","abc"));//1 
    trace_uint (ALWAYS, "1",rtx_strcmp("absdfaccc","abc")); //1
    trace_uint (ALWAYS, "1",rtx_strcmp("absdfaccc","lkppmklm")); //1

    rtx_strtok (str_p, tok_p, " ");
    trace (ALWAYS, tok_p);
    rtx_strtok (NULL, tok_p, " ");
    trace (ALWAYS, tok_p);
    rtx_strtok (NULL, tok_p, "");
    trace (ALWAYS, tok_p);
    rtx_strtok (NULL, tok_p, " ");
    trace (ALWAYS, tok_p);

    trace_uint (ALWAYS, "1",rtx_atoi("14", &num));
    trace_uint (ALWAYS, "14",num);
    trace_uint (ALWAYS, "1",rtx_atoi("-14", &num));
    trace_uint (ALWAYS, "-14",num);
    trace_uint (ALWAYS, "0",rtx_atoi("", &num));
    trace_uint (ALWAYS, "0",rtx_atoi("1sdf4", &num));

    while (1)
    {
    }
    return 0;
}
/*
CHAR * rtx_sprintf(CHAR * str, const CHAR * format, void * params[]);

CHAR * rtx_strcpy(CHAR * str, const CHAR * cpy_str);

int rtx_strcmp(const CHAR * str1, const CHAR * str2);

void rtx_strtok(CHAR * str, const CHAR * delimiters);

int rtx_atoi (const CHAR * str, int *num_p);
*/

