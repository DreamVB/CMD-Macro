#include "proto.h"

void toUpperCase(char *s){
    while(*s){
        *s = toupper(*s);
        s++;
    }
}

int is_white(char c){
    if((c == ' ') ||(c == '\t')){
        return 1;
    }
    return 0;
}

void match(char c){
    int pi = e_pos;
    char s[6];
    char ch = p_string[pi];

    if(ch != c){
        strcpy(s, "\" \"");
        s[1] = c;
        strcpy(h_holder,s);
        cmd_err(4,pc);
    }else{
        e_pos++;
    }
}

int skip_white(int pi){
    e_pos = pi;
    char ch = p_string[pi];

    while(is_white(ch)){
        pi++;
        ch = p_string[pi];
    }
    e_pos = pi;
}

int get_number(){
    int pi = e_pos;
    int si = 0;
    char ch = '\0';
    char digits[10] = {'\0'};
    ch = p_string[pi];

    while(isdigit(ch)){
        digits[si] = ch;
        pi++;
        si++;
        ch = p_string[pi];
    }
    digits[si] = '\0';

    e_pos = pi;

    return atoi(digits);
}

