#define TRUE 1
#define FALSE 0

#include "proto.h"
#include <math.h>

int IsAddop(char ch)
{
    if((ch == '+') || (ch == '-')
       || (ch == '>') || (ch == '<' ||
                          (ch == '=')))
    {   return TRUE;
    }
    return FALSE;
}

int IsMultop(char ch)
{
    if((ch == '*') || (ch == '/')
       || (ch == '%') || (ch == '^' ||
                          (ch == '|')))
    {   return TRUE;
    }
    return FALSE;
}

int Expression()
{   char ch  = '\0';
    int pi = 0;
    int Value = 0;

    SkipWhite();

    pi = e_pos;
    ch = p_string[pi];
    if(IsAddop(ch))
    {   Value = 0;
    }
    else
    {   Value = Term();
        pi = e_pos;
        ch = p_string[pi];
    }
    while(IsAddop(ch))
    {   switch(ch)
        {    case '+':
                 Match2('+');
                 Value = Value + Term();
                 break;
             case '-':
                 Match2('-');
                 Value = Value - Term();
                 break;
             case '>':
                Match2('>');
                Value = Value > Term();
                break;
             case '<':
                Match2('<');
                Value = Value < Term();
                break;
             case '=':
                Match2('=');
                Value = Value == Term();
                break;
             default:
                 break;
        }
        pi = e_pos;
        ch = p_string[pi];
    }
    return Value;
}

int Term()
{   char ch = '\0';
    int pi = 0;
    int Value = 0;

    Value = Factor();
    pi = e_pos;
    ch = p_string[pi];

    while(IsMultop(ch))
    {    switch(ch)
         {    case '*':
                  Match2('*');
                  Value = Value * Factor();
                  break;
              case '/':
                  Match2('/');
                  Value = Value / Factor();
                  break;
              case '^':
                  Match2('^');
                  Value = Value ^ Factor();
                  break;
              case '%':
                 Match2('%');
                 Value = (int)Value % (int)Factor();
                 break;
              default:
                  break;
         }
         pi = e_pos;
         ch = p_string[pi];
    }
    return Value;
}

int Factor()
{   char ch = '\0';
    int pi = 0;
    int value = 0;
    int si = 0;
    int ndx = -1;
    char varname[VAR_NAME] = {'\0'};

    pi = e_pos;
    ch = p_string[pi];
    if(ch == '(')
    {   Match2('(');
        value = Expression();
        Match2(')');
    }
    else if(isalpha(ch)){

        while(isalpha(ch)){
            varname[si] = toupper(ch);
            si++;
            pi++;
            ch = p_string[pi];
        }

        varname[si] = '\0';
        e_pos = pi;

        SkipWhite();

        ndx = get_intidx(varname);
        if(ndx != -1){
            value = i_stack[ndx];
        }

    }else
    {   value = GetNum();
    }
    return value;
}

void Match2(char x)
{   char ch = '\0';
    int pi = 0 ;

    pi = e_pos;
    ch = p_string[pi];
    if(ch != x)
    {
        //abort
    }
    else
    {   Next();
        SkipWhite();
    }
}

void Next()
{
    e_pos++;
}

void SkipWhite()
{   char ch;
    int pi;

    pi = e_pos;
    ch = p_string[pi];
    while(ch == ' ' || ch == '\t')
    {   Next();
        pi = e_pos;
        ch = p_string[pi];
    }
}

int GetNum()
{   char ch = '\0';
    int pi = 0;
    int Value=0;

    pi = e_pos;
    ch = p_string[pi];

    if(!isdigit(ch))
    {
        //Abort
    }

    while(isdigit(ch))
    {   Value = 10 * Value + ch - '0';
        Next();
        pi = e_pos;
        ch = p_string[pi];
    }

    SkipWhite();

    return Value;
}

