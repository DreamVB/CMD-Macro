#include "proto.h"

void do_append(){
    int pi = e_pos;
    int si = 0;
    int ndx = -1;
    int idx = -1;
    int vType = 0;
    char ch = '\0';
    char vname[VAR_NAME] = {'\0'};
    char vname2[VAR_NAME] = {'\0'};

    strcpy(p_string,sArgs[pc]);

    pi = skip_white(pi);
    match('(');
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    if(!isalnum(ch)){
        cmd_err(5,pc);
    }
    //Get varname
    strcpy(vname,get_varname());
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];
    match(',');
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];
    //Check for variable or qstr
    if(ch == quote){
        pi++;
        ch = p_string[pi];
        while(ch != quote){
            xstring[si] = ch;
            pi++;
            si++;
            ch = p_string[pi];
        }
        xstring[si] = '\0';
        //Get src var index.
        vType = get_vType(vname);

        //Make sure it's a string var
        if(vType != 2){
            cmd_err(8,pc);
        }else{
            ndx = get_stridx(vname);
            strcat(s_stack[ndx],xstring);
        }
        pi++;
        e_pos = pi;
        pi = skip_white(pi);
        ch = p_string[pi];
        match(')');

    }else if(isalpha(ch)){
        //Get var name
        strcpy(vname2,get_varname());
        vType = get_vType(vname2);

        if(vType != 2){
            cmd_err(8,pc);
        }
        ndx = get_stridx(vname);
        idx = get_stridx(vname2);

        strcat(s_stack[ndx],s_stack[idx]);
        pi = e_pos;
        pi = skip_white(pi);
        ch = p_string[pi];
        match(')');
    }else{
        cmd_err(7,pc);
    }
}

void do_strcase(int op){
    int pi = e_pos;
    int v_type = 0;
    int ndx = -1;
    int x = 0;
    int ch = '\0';
    char varname[VAR_NAME] = {'\0'};

    strcpy(p_string,sArgs[pc]);

    pi = skip_white(pi);
    ch = p_string[pi];
    match('(');

    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    if(!isalpha(ch)){
        cmd_err(5,pc);
    }
    //Get var name
    strcpy(varname,get_varname());
    //Get var type
    v_type = get_vType(varname);
    if(v_type != 2){
        cmd_err(8,pc);
    }
    ndx = get_stridx(varname);

    while(x < strlen(s_stack[ndx]))
    {
        if(op == 0){
            s_stack[ndx][x] = toupper(s_stack[ndx][x]);
        }
        if(op == 1){
            s_stack[ndx][x] = tolower(s_stack[ndx][x]);
        }
        x++;
    }
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];
    match(')');

}

void do_itoa(){
    int pi = e_pos;
    char v_name1[VAR_NAME] = {'\0'};
    char v_name2[VAR_NAME] = {'\0'};
    char s_num[10] = {'\0'};
    char ch = '\0';
    int ndx = -1;
    int v_type1 = 0;
    int v_type2 = 0;
    int value = 0;

    strcpy(p_string,sArgs[pc]);
    pi = skip_white(pi);
    ch = p_string[pi];

    match('(');
    //Get first varname.
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    if(!isalnum(ch)){
        cmd_err(5,pc);
    }
    //Get var name
    strcpy(v_name1,get_varname());
    //Get next var
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];
    match(',');
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    if(isalpha(ch)){
        //Get int var name.
        strcpy(v_name2,get_varname());
        //Get var types.
        v_type1 = get_vType(v_name1);
        if(v_type1 != 2){
            cmd_err(8,pc);
        }
        //Check next type
        v_type2 = get_vType(v_name2);
        if(v_type2 != 1){
            cmd_err(8,pc);
        }
        //Convert the int var to a string.
        ndx = get_stridx(v_name1);
        itoa(i_stack[get_intidx(v_name2)],s_num,10);
        strcpy(s_stack[ndx],s_num);
    }else if(isdigit(ch)){
        value = get_number();
        ndx = get_stridx(v_name1);
        itoa(value,s_num,10);
        strcpy(s_stack[ndx],s_num);
    }else{
        cmd_err(7,pc);
    }

    pi = e_pos;
    pi = skip_white(pi);
    match(')');
}

void do_atoi(){
    int pi = e_pos;
    char v_name1[VAR_NAME] = {'\0'};
    char v_name2[VAR_NAME] = {'\0'};
    char s_num[10] = {'\0'};
    char ch = '\0';
    int ndx = -1;
    int idx = -1;
    int si = 0;
    int v_type1 = 0;
    int v_type2 = 0;

    strcpy(p_string,sArgs[pc]);
    pi = skip_white(pi);
    ch = p_string[pi];

    match('(');
    //Get first varname.
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    if(!isalpha(ch)){
        cmd_err(5,pc);
    }
    //Get var name
    strcpy(v_name1,get_varname());
    //Get next var
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];
    match(',');
    pi = e_pos;
    pi = skip_white(pi);
    ch = p_string[pi];

    if(isalpha(ch)){
        //Get int var name.
        strcpy(v_name2,get_varname());
        //Get var types.
        v_type1 = get_vType(v_name1);
        if(v_type1 != 1){
            cmd_err(8,pc);
        }
        //Check next type
        v_type2 = get_vType(v_name2);
        if(v_type2 != 2){
            cmd_err(8,pc);
        }
        //Convert the int var to a string.
        ndx = get_intidx(v_name1);
        idx = get_stridx(v_name2);

        i_stack[ndx] = atoi(s_stack[idx]);

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
        ndx = get_intidx(v_name1);
        i_stack[ndx] = atoi(xstring);
        pi++;
        e_pos = pi;
    }else{
        cmd_err(7,pc);
    }

    pi = e_pos;
    pi = skip_white(pi);

    match(')');
}
