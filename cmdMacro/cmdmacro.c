#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

#define MAX_LINES 2048
#define CMD_LEN 30
#define BUFFER 2048
#define VAR_NAME 30
#define VAR_TYPE_LEN 30

const char quote = '\'';

char p_string[BUFFER] = {'\0'};
char h_holder[BUFFER] = {'\0'};
char xstring[BUFFER] = {'\0'};
char p_line[BUFFER] = {'\0'};

char sCommands[MAX_LINES][BUFFER];
char sArgs[MAX_LINES][BUFFER];
char strline[BUFFER] = {'\0'};

int rows = 0;
int pc = 0;
int gs_idx = -1;
int ret_idx = 0;

int l_start = 0;
int l_end = 0;
int l_counter = 0;

int pi = 0;
int s_pos = 0;
int e_pos = 0;

#include "sysvars.c"
#include "systools.c"
#include "eval.c"
#include "error.c"
#include "sysstrings.c"

//Prototypes
void load_script(char *);
void Execute(void);
int getbyte();

void do_setvar(void);
void do_var(void);
void do_write(int);
void do_beep(void);
void do_incDec(int);
void do_goto(void);
void do_readln(void);
void do_gosub(void);
void do_return(void);
void do_loop(void);
void do_endloop(void);

void load_script(char *filename){
    FILE *fp = NULL;
    int len = 0;
    char ch = '\0';
    int x = 0;
    int t = 0;

    char scmd[CMD_LEN] = {'\0'};
    char args[BUFFER] = {'\0'};

    fp = fopen(filename,"r");

    if(!fp){
        cmd_err(1,0);
    }

    while(fgets(strline,BUFFER,fp) != NULL){
        len = strlen(strline);

        if(strline[len-1] == '\n'){
            strline[len-1] = '\0';
        }

        if((strlen(strline) > 1) && (strline[0] != ';')){

            x = 0;
            t = 0;
            ch = strline[x];

            while(isspace(ch)){
                x++;
                ch = strline[x];
            }

            while(isalpha(ch) ||(ch == ':')){
                scmd[t] = toupper(ch);
                x++;
                t++;
                ch = strline[x];
            }
            scmd[t] = '\0';

            len = strlen(strline);
            t = 0;

            while(x < len){
                args[t] = ch;
                x++;
                t++;
                ch = strline[x];
            }

            args[t] = '\0';

            if(strlen(scmd) > 0){
                strcpy(sCommands[rows],scmd);
                strcpy(sArgs[rows],args);
                rows++;
            }
        }
    }

    fclose(fp);
}

void Execute(){
    while(pc < rows){

        s_pos = 0;
        e_pos = 0;

        switch(getbyte()){
            case 1:
                //Set variable
                do_setvar();
                break;
            case 2:
                do_beep();
                break;
            case 3:
                do_write(0);
                break;
            case 4:
                do_write(1);
                break;
            case 5:
                pc = rows;
                break;
            case 6:
                do_incDec(0);
                break;
            case 7:
                do_incDec(1);
                break;
            case 8:
                do_goto();
                break;
            case 9:
                do_readln();
                break;
            case 10:
                do_append();
                break;
            case 11:
                do_strcase(0);
                break;
            case 12:
                do_strcase(1);
                break;
            case 13:
                do_itoa();
                break;
            case 14:
                do_atoi();
                break;
            case 15:
                //LOOP
                do_loop();
                break;
            case 16:
                //ENDLOOP
                do_endloop();
                break;
            case 17:
                do_gosub();
                break;
            case 18:
                do_return();
                break;
            case 19:
                do_var();
                break;
            default:
                break;
        }
        pc++;
    }
}

int getbyte(){
    int ndx = 0;
    int slen = 0;
    int v_type = 0;
    int b = 0;

   if(strcmp(sCommands[pc],"VAR") == 0){
        b = 1;
   } else if(strcmp(sCommands[pc],"BEEP") == 0){
        b = 2;
   }else if(strcmp(sCommands[pc],"WRITE") == 0){
        b = 3;
   }else if(strcmp(sCommands[pc],"WRITELN") == 0){
        b = 4;
   }else if(strcmp(sCommands[pc],"EXIT") == 0){
        b = 5;
   }else if(strcmp(sCommands[pc],"INC") == 0){
        b = 6;
   }else if(strcmp(sCommands[pc],"DEC") == 0){
        b = 7;
   }else if(strcmp(sCommands[pc],"GOTO") == 0){
        b = 8;
   }else if(strcmp(sCommands[pc],"READLN") == 0){
        b = 9;
   }else if(strcmp(sCommands[pc],"APPENDSTR") == 0){
        b = 10;
   }else if(strcmp(sCommands[pc],"STRUPPER") == 0){
        b = 11;
   }else if(strcmp(sCommands[pc],"STRLOWER") == 0){
        b = 12;
   }else if(strcmp(sCommands[pc],"ITOA") == 0){
        b = 13;
   }else if(strcmp(sCommands[pc],"ATOI") == 0){
        b = 14;
   }else if(strcmp(sCommands[pc],"LOOP") == 0){
        b = 15;
   }else if(strcmp(sCommands[pc],"ENDLOOP") == 0){
        b = 16;
   }else if(strcmp(sCommands[pc],"GOSUB") == 0){
        b = 17;
   }else if(strcmp(sCommands[pc],"RETURN") == 0){
        b = 18;
   }
   else{
        slen = strlen(sCommands[pc]);

        if(sCommands[pc][slen-1] == ':'){
            return -1;
        }

        //Get variable type
        v_type = get_vType(sCommands[pc]);

        //Check variable type
        if(v_type == 1){
           ndx = get_intidx(sCommands[pc]);
        }else if(v_type == 2){
            ndx = get_stridx(sCommands[pc]);
        }else{
        }

        if(ndx != -1){
            b = 19;
        }else{
            cmd_err(5,pc);
        }
    }
   return b;
}

void do_setvar(){
int pi = 0;
int ndx = 0;
char ch = '\0';
char varname[VAR_NAME] = {'\0'};
char s_vartype[VAR_TYPE_LEN] = {'\0'};
int v_type = 0;

    strcpy(p_string,sArgs[pc]);

    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    if(isalpha(ch)){
        strcpy(varname,get_varname());
        //
        ndx = get_stridx(varname);
        //get vartype
        pi = e_pos;
        pi = skip_white(pi);
        ch = p_string[pi];
        match(':');
        pi = e_pos;
        pi = skip_white(pi);

        strcpy(s_vartype,get_strvartype());

        if(ndx == -1){
            v_type = str_tovar(s_vartype);
            add_vartype(varname,v_type);

            switch(v_type){
                case 1:
                    add_intvar(varname,0);
                    break;
                case 2:
                    add_strvar(varname,"");
                    break;
                default:
                    break;
            }

        }else{
            cmd_err(6,pc);
        }
    }
}

void do_var(){
    char varname[VAR_NAME] = {'\0'};
    char temp[VAR_NAME] = {'\0'};
    int si = 0;
    int ndx = 0;
    int idx = 0;
    char ch = '\0';
    int pi = 0;
    int v_type = 0;
    int value = 0;

    //Get assigning variable name
    strcpy(varname,sCommands[pc]);
    //Get variable type
    v_type = get_vType(varname);

    if(v_type == 1){
        ndx = get_intidx(varname);
    }else if(v_type == 2){
        ndx = get_stridx(varname);
    }

    if(ndx == -1){
        cmd_err(5,pc);
    }
    //Get assigning variable statement
    strcpy(p_string,sArgs[pc]);
    //
    pi = skip_white(pi);
    //
    match(':');
    match('=');

    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    if(v_type == 1){
        i_stack[ndx] = Expression();
        return;
    }

    if(ch == quote){
        //Get quoted string
        pi++;
        ch = p_string[pi];
        while(ch != quote){
            xstring[si] = ch;
            pi++;
            si++;
            ch = p_string[pi];
        }
        xstring[si] = '\0';
        pi++;
        e_pos = pi;
        //Assign string to variable.
        strcpy(s_stack[ndx],xstring);
    } else if(isalpha(ch)){
         strcpy(temp,get_varname());
         idx = get_stridx(temp);
         if(idx == -1){
            cmd_err(5,pc);
         }
         strcpy(s_stack[ndx],s_stack[idx]);
    }else
    {

    }
}

void do_beep(){
    strcpy(p_string,sArgs[pc]);

    e_pos = skip_white(e_pos);
    match('(');
    e_pos = skip_white(e_pos);
    match(')');

    printf("%c",0x7);
}

void do_write(int lnReturn){
    int pi = 0;
    int si = 0;
    char ch = 0;
    int ndx = -1;
    int v_type = 0;
    char varname[VAR_NAME] = {'\0'};
    char vname2[VAR_NAME] = {'\0'};
    char sNum[10] = {'\0'};
    strcpy(p_string,sArgs[pc]);

    match('(');
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    //Check for variable
    if(isalpha(ch)){
        strcpy(varname,get_varname());
        //Get variable type.
        v_type = get_vType(varname);

        if(v_type == 1){
            ndx = get_intidx(varname);
        }else if(v_type == 2){
            ndx = get_stridx(varname);
        }else{

        }

        if(ndx == -1){
            strcpy(h_holder,varname);
            cmd_err(5,pc);
        }

        if(v_type == 1){
            if(lnReturn == 1){
                printf("%d\n",i_stack[ndx]);
            }
            else{
                printf("%d",i_stack[ndx]);
            }
        }
        //String type
        if(v_type == 2){
            if(lnReturn == 1){
                printf("%s\n",s_stack[ndx]);
            }
            else{
                printf("%s",s_stack[ndx]);
            }
        }

        pi = e_pos;
        pi = skip_white(pi);
        match(')');

    }else if(ch == quote){
        pi++;
        ch = p_string[pi];
        while(ch != quote){
            xstring[si] = ch;
            pi++;
            si++;
            ch = p_string[pi];
        }
        xstring[si] = '\0';

        pi++;
        e_pos = pi;
        pi = skip_white(pi);

        ch = p_string[pi];

        if(ch != ','){

            if(lnReturn == 1){
                printf("%s\n",xstring);
            }else{
                printf("%s",xstring);
            }

            match(')');
        }else{
            pi++;
            pi = skip_white(pi);

            ch = p_string[pi];

            if(!isalpha(ch)){
                cmd_err(5,pc);
            }

            strcpy(varname,get_varname());

            v_type = get_vType(varname);

            if(v_type == 1){
                ndx = get_intidx(varname);
                itoa(i_stack[ndx],sNum,10);
                strcat(xstring,sNum);

                if(lnReturn == 1){
                    printf("%s\n",xstring);
                }else{
                    printf("%s",xstring);
                }
                pi = e_pos;
                pi = skip_white(pi);
                ch = p_string[pi];
                match(')');
            }else if(v_type == 2){
                ndx = get_stridx(varname);
                strcat(xstring,s_stack[ndx]);

                if(lnReturn == 1){
                    printf("%s\n",xstring);
                }else{
                    printf("%s",xstring);
                }
                pi = e_pos;
                pi = skip_white(pi);

                ch = p_string[pi];
                match(')');
            }else{
                cmd_err(5,pc);
            }

        }
    }else if(isdigit(ch)){
        printf("NUMBER HERE\n");
    }else{

    }
}

void do_incDec(int op){
    int pi = 0;
    int ndx = -1;
    int idx = -1;
    int val = 0;
    char ch = '\0';
    char varname[VAR_NAME] = {'\0'};
    char vname2[VAR_NAME] = {'\0'};

    strcpy(p_string,sArgs[pc]);

    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    match('(');

    //Get varname
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    if(isalpha(ch)){
        strcpy(varname,get_varname());
    }

    ndx = get_intidx(varname);
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    if(ch == ','){
        //Get var value
        pi++;
        pi = skip_white(pi);
        ch = p_string[pi];

        if(isdigit(ch)){
            val = get_number();
            if(ndx == -1){
                cmd_err(5,pc);
            }

            if(op == 0){
                i_stack[ndx] += val;
            }
            if(op == 1){
                i_stack[ndx] -= val;
            }
            pi = e_pos;
            pi = skip_white(pi);
            ch = p_string[pi];
            match(')');
        }else if(isalpha(ch)){
            strcpy(vname2,get_varname());

            idx = get_intidx(vname2);

            if(idx == -1){
                cmd_err(5,pc);
            }else if(ndx == -1){
                cmd_err(5,pc);
            }

            if(op == 0){
                i_stack[ndx] += i_stack[idx];
            }
            if(op == 1){
                i_stack[ndx] -= i_stack[idx];
            }
            pi = e_pos;
            pi = skip_white(pi);
            ch = p_string[pi];
            match(')');
        }else{

        }
    }else{
        //Get var index
        if(ndx == -1){
            cmd_err(5,pc);
        }
        if(op == 0){
            i_stack[ndx]++;
        }
        if(op == 1){
            i_stack[ndx]--;
        }
        match(')');
    }
}

void do_goto(){
    int pi = e_pos;
    int x = 0;
    int si = 0;
    int slen = 0;
    int lb_idx = -1;
    char label[80] = {'\0'};
    char ch = '\0';
    //Get goto label
    strcpy(p_string,sArgs[pc]);

    pi = skip_white(pi);
    e_pos = pi;

    ch = p_string[pi];

    if(isalnum(ch)){
        while(isalnum(ch)){
            label[si] = toupper(ch);
            pi++;
            si++;
            ch = p_string[pi];
        }
        label[si] = ':';
        label[si+1] = '\0';
        e_pos = pi;
    }

    while(x < rows){
        slen = strlen(sCommands[x]);
        if(sCommands[x][slen-1] == ':'){
            if(strcmp(sCommands[x],label) == 0){
                lb_idx = x;
                break;
            }
        }
        x++;
    }
    if(lb_idx != -1){
        pc = lb_idx;
    }
}

void do_readln(){
    int pi = e_pos;
    char vname[VAR_NAME] = {'\0'};
    char in_data[2048] = {'\0'};
    char ch = '\0';
    int vType = 0;
    int ndx = -1;

    strcpy(p_string,sArgs[pc]);

    pi = skip_white(pi);
    e_pos = pi;
    ch = p_string[pi];

    match('(');
    pi = e_pos;
    pi = skip_white(pi);
    //Get variable name
    ch = p_string[pi];

    if(isalpha(ch)){
        strcpy(vname,get_varname());
        //
        //Get variable type
        vType = get_vType(vname);

        if(vType == 1){
            //Show the input box
            gets(in_data);
            //Store in variable
            ndx = get_intidx(vname);
            if(ndx == -1){
                cmd_err(5,pc);
            }
            i_stack[ndx] = atoi(in_data);

            pi = e_pos;
            pi = skip_white(pi);
            ch = p_string[pi];
           match(')');
        }

        if(vType == 2){
            //Show the input box
            gets(in_data);
            //Store in variable
            ndx = get_stridx(vname);
            if(ndx == -1){
                cmd_err(5,pc);
            }
            strcpy(s_stack[ndx],in_data);

            pi = e_pos;
            pi = skip_white(pi);
            ch = p_string[pi];
           match(')');
        }
    }
}

void do_gosub(){
    int pi = e_pos;
    int slen = 0;
    int si = 0;
    int x = 0;

    char gs_label[80] = {'\0'};

    char ch = '\0';

    strcpy(p_string,sArgs[pc]);

    pi = skip_white(pi);
    ch = p_string[pi];

    if(!isalpha(ch)){
        cmd_err(7,pc);
    }
    //Get gosub label
    while(isalpha(ch)){
        gs_label[si] = toupper(ch);
        si++;
        pi++;
        ch = p_string[pi];
    }

    gs_label[si] = ':';
    gs_label[si+1] = '\0';
    e_pos = pi;

    while(x < rows){
        slen = strlen(sCommands[x]);
        if(sCommands[x][slen-1] == ':'){
            if(strcmp(sCommands[x],gs_label) == 0){
                gs_idx = x;
                break;
            }
        }
        x++;
    }

    ret_idx = pc;

    if(gs_idx != -1){
        pc = gs_idx;
    }
}

void do_return(){
    pc = ret_idx;
}


void do_loop(){
    int pi = e_pos;
    int vtype = 0;
    int ndx = -1;
    int ch = '\0';
    char varname[VAR_NAME] = {'\0'};

    l_end = pc;
    l_counter = 0;

    strcpy(p_string,sArgs[pc]);

    pi = skip_white(pi);
    ch = p_string[pi];
    if(isdigit(ch)){
        //Get number
        l_start = get_number();
    }
    if(isalpha(ch)){
        strcpy(varname,get_varname());
        vtype = get_vType(varname);

        if(vtype != 1){
            cmd_err(8,pc);
        }
        //Get var value
        ndx = get_intidx(varname);
        l_start = i_stack[ndx];
    }

}

void do_endloop(){
    if(l_counter < l_start){
        pc = l_end;
    }
    l_counter++;
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        cmd_err(0,0);
    }

    load_script(argv[1]);
    if(rows == 0){
        cmd_err(3,0);
    }

    init_strvars();
    init_intvars();
    init_vtype();

    Execute();
    return 0;
}
