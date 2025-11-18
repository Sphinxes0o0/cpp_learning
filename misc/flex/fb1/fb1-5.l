%{
    #include "fb1-5.tab.h"
%}

%%

[0-9]+    { yylval = atoi(yytext); return NUMBER; }
"+"       { return ADD; }
"-"       { return SUB; }
"*"       { return MUL; }
"/"       { return DIV; }
"abs"     { return ABS; }
"\n"      { return EOL; }
[ \t]     { /* 忽略空白字符 */ }

%%

int yywrap(void)
{
    return 1;
}