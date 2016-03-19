%option yylineno

%%

. { printf("Error type A at line %d: Mysterious characters \'%s\' [lexical error]\n", yylineno, yytext); }

%%
