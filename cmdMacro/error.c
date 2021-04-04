#include "proto.h"

void cmd_err(int ncode, int lidx){
    int ln = (lidx + 1);

    printf("Command Macro v1.3\n");
    pc = rows;
    printf("%c",0x7);

    switch(ncode){
        case 0:
            printf("No Input Source Found.\n");
            break;
        case 1:
            printf("Cannot Load Script.\n");
            break;
        case 2:
            printf("Script File Is Empty.\n");
            break;
        case 3:
            printf("Script Error Line: [%d]",ln);
            break;
        case 4:
            printf("Expected %s\n",h_holder);
            printf("Line: %d\n",ln);
            break;
        case 5:
            printf("Variable not declared %s\n",h_holder);
            printf("Line: %d\n",ln);
            break;
        case 6:
            printf("Duplicate Declaration.\n");
            printf("Line: %d\n",ln);
            break;
        case 7:
            printf("Illegal character in input file.\n");
            printf("Line: %d\n",ln);
            break;
        case 8:
            printf("Types of actual and formal var parameters must be identical.\n");
            printf("Line: %d\n",ln);
            break;
        default:
            break;
    }
    memset(sArgs,0,sizeof sArgs);
    memset(sCommands,0,sizeof sCommands);
    memset(strline,0,sizeof strline);
    exit(1);
}
