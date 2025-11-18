%{
#include <stdio.h>
#include <string.h>
%}

%%
"colour"          { printf("color"); }
"flavour"         { printf("flavor"); }
"clever"          { printf("smart"); }
"conservative"    { printf("liberal"); }
.                 { printf("%s", yytext); }  
%%

int main(int argc, char *argv[])
{
    yylex();
    return 0;
}

int yywrap(void)
{
    return 1;
}