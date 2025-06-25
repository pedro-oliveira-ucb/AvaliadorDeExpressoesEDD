#include "expressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define STACK_MAX_SIZE 256
#define TOKEN_MAX_LEN 64
#define M_PI 3.14159265358979323846264338327950288

// Pilha para valores float
typedef struct {
    float data[ STACK_MAX_SIZE ];
    int top;
} FloatStack;

void float_stack_init( FloatStack * s ) { s->top = -1; }
int float_stack_push( FloatStack * s , float value ) {
    if ( s->top >= STACK_MAX_SIZE - 1 ) return -1; // Overflow
    s->data[ ++s->top ] = value;
    return 0;
}
float float_stack_pop( FloatStack * s ) {
    if ( s->top < 0 ) return NAN; // Underflow
    return s->data[ s->top-- ];
}

// Pilha para strings
typedef struct {
    char data[ STACK_MAX_SIZE ][ TOKEN_MAX_LEN ];
    int top;
} StringStack;

void string_stack_init( StringStack * s ) { s->top = -1; }
int string_stack_push( StringStack * s , const char * value ) {
    if ( s->top >= STACK_MAX_SIZE - 1 ) return -1; // Overflow
    strcpy( s->data[ ++s->top ] , value );
    return 0;
}
char * string_stack_pop( StringStack * s ) {
    if ( s->top < 0 ) return NULL; // Underflow
    return s->data[ s->top-- ];
}
char * string_stack_peek( StringStack * s ) {
    if ( s->top < 0 ) return NULL;
    return s->data[ s->top ];
}
int string_stack_is_empty( StringStack * s ) {
    return s->top == -1;
}

// --- Fun��es Auxiliares ---

// Verifica se um token � um n�mero
int is_number( const char * token ) {
    if ( !token ) return 0;
    char * end;
    strtod( token , &end );
    return end != token && *end == '\0';
}

// Retorna a preced�ncia de um operador/fun��o
int precedence( const char * op ) {
    if ( strcmp( op , "raiz" ) == 0 || strcmp( op , "sen" ) == 0 || strcmp( op , "cos" ) == 0 || strcmp( op , "tg" ) == 0 || strcmp( op , "log" ) == 0 ) return 4;
    if ( strcmp( op , "^" ) == 0 ) return 3;
    if ( strcmp( op , "*" ) == 0 || strcmp( op , "/" ) == 0 || strcmp( op , "%" ) == 0 ) return 2;
    if ( strcmp( op , "+" ) == 0 || strcmp( op , "-" ) == 0 ) return 1;
    return 0; // Para par�nteses
}

// Verifica se um operador � un�rio (uma fun��o)
int is_unary( const char * op ) {
    return strcmp( op , "raiz" ) == 0 || strcmp( op , "sen" ) == 0 || strcmp( op , "cos" ) == 0 || strcmp( op , "tg" ) == 0 || strcmp( op , "log" ) == 0;
}

// Verifica se um operador tem associatividade � direita
int is_right_associative( const char * op ) {
    return strcmp( op , "^" ) == 0;
}

// Converte graus para radianos
double to_radians( double degrees ) {
    return degrees * ( M_PI / 180.0 );
}

// Converte Infixa para P�s-fixada (Algoritmo Shunting-Yard)
char * getFormaPosFixa( char * StrInFixa ) {
    return NULL;
}

// Avalia uma express�o P�s-fixada
float getValorPosFixa( char * StrPosFixa ) {
    return NULL;
}

// Avalia uma express�o Infixa (convertendo primeiro)
float getValorInFixa( char * StrInFixa ) {
    return 0.0f;
}


// Converte P�s-fixada para Infixada
char * getFormaInFixa( char * StrPosFixa ) {
    return NULL;
}
