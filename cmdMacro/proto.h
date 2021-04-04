//varibles

int get_stridx(char*);
void add_strvar(char *, char *);
void init_strvars(void);
char *get_strvartype();
char* get_varname();

void init_intvars(void);
void add_intvar(char *, int);
int get_intidx(char*);

int str_tovar(char *);
void add_vartype(char *, int);
void init_vtype(void);
int get_vType(char*);

//systools
void toUpperCase(char *);
int is_white(char);
int get_number();
void match(char);
int skip_white(int);

//Eval
int Expression();
int Term();
int Factor();
void Match2(char);
void Next();
void SkipWhite();
int GetNum();
int IsAddop(char ch);
int IsMultop(char ch);

//error.c
void cmd_err(int, int);

// sysstrings
void do_append(void);
void do_strcase(int);
void do_itoa(void);
void do_atoi(void);
