
%%
"+"     { printf("plus\n"); }
"-"     { printf("minus\n"); }
"*"     { printf("times\n"); }
"/"     { printf("divided\n"); }
"|"     { printf("ABS\n"); }
[0-9]+  { printf("number %s\n", yytext); }
\n      { printf("newline\n"); }
[ \t]   { /* ignore whitespace */ }
.       { printf("unknown %s\n", yytext); }
%%
