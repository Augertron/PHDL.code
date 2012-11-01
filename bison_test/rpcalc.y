/* Reverse polish notation calculator. */
%{
    #define YYSTYPE double
    #include <stdio.h>
    #include <math.h>
    int yylex (void);
    void yyerror (char const *);
%}
%token NUM
%% /* Grammar rules and actions follow. */
input:
    /* empty */
    | input line
    ;
line:
    '\n'
    | exp '\n' { printf ("%.10g\n", $1); }
    ;
exp:
    NUM { $$ = $1; }
    | exp exp '+' { $$ = $1 + $2; }
    | exp exp '-' { $$ = $1 - $2; }
    | exp exp '*' { $$ = $1 * $2; }
    | exp exp '/' { $$ = $1 / $2; }
    | exp exp '^' { $$ = pow ($1, $2); } /* Exponentiation */
    | exp 'n' { $$ = -$1; } /* Unary minus */
    ;
%%

/* This is the so called epilogue section of the bison input file. */


/* Called by yyparse on error. */
void yyerror (char const *s)
{
    fprintf (stderr, "%s\n", s);
}

int main (void)
{
    return yyparse ();
}

