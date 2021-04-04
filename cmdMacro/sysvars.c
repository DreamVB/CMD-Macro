#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include "proto.h"

#define MAX_VARS 100
#define MAX_VAR_TYPE 200

//String variables
char s_name[MAX_VARS][VAR_NAME] = {'\0'};
char s_stack[MAX_VARS][BUFFER] = {'\0'};
int s_maxvars = 0;

char i_name[MAX_VARS][VAR_NAME] = {'\0'};
int i_stack[MAX_VARS] = {0};
int i_maxvars = 0;

//All variables types
char n_vars[MAX_VAR_TYPE][VAR_NAME] = {'\0'};
int t_vars[MAX_VAR_TYPE] = {0};
int var_count = 0;

int str_tovar(char *var){
    if(strcmp(var,"INTEGER") == 0){
        return 1;
    }else if(strcmp(var,"STRING") == 0){
        return 2;
    }else{
        return 0;
    }
}

void init_strvars(){
    int x = 0;
    while(x < MAX_VARS){
        s_name[x][0] = '\0';
        s_stack[x][0] = '\0';
        x++;
    }
}

void init_intvars(){
    int x = 0;
    while(x < MAX_VARS){
        i_name[x][0] = '\0';
        i_stack[x] = 0;
        x++;
    }
}

void init_vtype(){
    int x = 0;
    while(x < MAX_VAR_TYPE){
        n_vars[x][0] = '\0';
        t_vars[x] = 0;
        x++;
    }
}

void add_strvar(char *name, char *data){
    strcpy(s_name[s_maxvars],name);
    strcpy(s_stack[s_maxvars],data);
    s_maxvars++;
}

void add_intvar(char *name, int data){
    strcpy(i_name[i_maxvars],name);
    i_stack[i_maxvars] = data;
    i_maxvars++;
}

void add_vartype(char *name, int type){
    strcpy(n_vars[var_count],name);
    t_vars[var_count] = type;
    var_count++;
}

int get_stridx(char* name){
    int x = 0;
    int idx = -1;
    while(x < s_maxvars){
        if(strcmp(s_name[x],name) == 0){
            idx = x;
            break;
        }
        x++;
    }
    return idx;
}

int get_intidx(char* name){
    int x = 0;
    int idx = -1;
    while(x < i_maxvars){
        if(strcmp(i_name[x],name) == 0){
            idx = x;
            break;
        }
        x++;
    }
    return idx;
}

int get_vType(char* name){
    int x = 0;
    int idx = -1;
    while(x < var_count){
        if(strcmp(n_vars[x],name) == 0){
            idx = x;
            break;
        }
        x++;
    }
    if(idx == -1)
    {
        return 0;
    }else{
        return t_vars[idx];
    }

}

char* get_varname(){
    int pi = e_pos;
    int si  = 0;
    char ch = '\0';
    static char temp[VAR_NAME];

    ch = p_string[pi];

    while(isalpha(ch)){
        temp[si] = toupper(ch);
        pi++;
        si++;
        ch = p_string[pi];
    }
    e_pos = pi;
    temp[si] = '\0';

    return temp;
}

char *get_strvartype(){
    int pi = e_pos;
    int si = 0;
    char ch = '\0';
    static char temp[VAR_TYPE_LEN] = {'\0'};

    ch = p_string[pi];

    while(isalpha(ch)){
        temp[si] = toupper(ch);
        pi++;
        si++;
        ch = p_string[pi];
    }
    temp[si] = '\0';
    e_pos = pi;

    return temp;
}

