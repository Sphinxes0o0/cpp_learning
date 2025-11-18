%option noyywrap

%{
int chars = 0;
int lines = 0;
int words = 0;

int totchars = 0;
int totwords = 0;
int tottlines = 0;

%}


%%
[a-zA-Z]+ { words++; chars += strlen(yytext); }
\n        { chars++; lines++; }
.         { chars++; }
%%

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        yylex();
        printf("Total: %d chars, %d words, %d lines\n", chars, words, lines);
        return 0;
    }

    for (i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "Can't open %s\n", argv[i]);
            continue;
        }

        yyrestart(fp);
        yylex();
        fclose(fp);
        printf("%s: %d chars, %d words, %d lines\n", argv[i], chars, words, lines);
        totchars += chars; chars = 0;
        totwords += words; words = 0;
        tottlines += lines; lines = 0;
    }

    if (argc > 1)
        printf("Total: %d chars, %d words, %d lines\n", totchars, totwords, tottlines);

    return 0;
}