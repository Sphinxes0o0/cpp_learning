%{
    int chars = 0;
    int lines = 0;
    int words = 0;
%}

%%
[a-zA-Z]+   { words++; chars += strlen(yytext); }
\n          { chars++; lines++; }
.           { chars++; }
%%

int main (int argc, char *argv[])
{
    yylex();
    printf("chars: %d, lines: %d, words: %d\n", chars, lines, words);

    return 0;
}
