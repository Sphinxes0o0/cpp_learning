%{
    enum yytokentype {
        NUMBER = 258,
        ADD = 259,
        SUB = 260,
        MUL = 261,
        DIV = 262,
        ABS = 263,
        EOL = 264,
    };
    int yylval;
    
    const char* token_name(int token) {
        switch (token) {
            case NUMBER: return "NUMBER";
            case ADD:    return "ADD";
            case SUB:    return "SUB";
            case MUL:    return "MUL";
            case DIV:    return "DIV";
            case ABS:    return "ABS";
            case EOL:    return "EOL";
            default:     return "UNKNOWN";
        }
    }
%}

%%
"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
"|"     { return ABS; }
[0-9]+   { yylval = atoi(yytext); return NUMBER; }
\n       { return EOL; }
[ \t]   { /**/  }
.       { printf("unknown token %s\n", yytext); }
%%


int main() {
    int token;
    while( (token = yylex()) ) {
        printf("token %s(%d)\n", token_name(token), token);
        if (token == NUMBER) printf("value=%d\n", yylval);
        else printf("\n");
    }
}
