#include "expressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define STACK_MAX_SIZE 256
#define TOKEN_MAX_LEN 64

//estava dando erro no meu visual studio, então defini aqui
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif // !1

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

// --- Funções Auxiliares ---

// Verifica se um token é um número
int is_number( const char * token ) {
    if ( !token ) return 0;
    char * end;
    strtod( token , &end );
    return end != token && *end == '\0';
}

// Retorna a precedência de um operador/função
int precedence( const char * op ) {
    if ( strcmp( op , "raiz" ) == 0 || strcmp( op , "sen" ) == 0 || strcmp( op , "cos" ) == 0 || strcmp( op , "tg" ) == 0 || strcmp( op , "log" ) == 0 ) return 4;
    if ( strcmp( op , "^" ) == 0 ) return 3;
    if ( strcmp( op , "*" ) == 0 || strcmp( op , "/" ) == 0 || strcmp( op , "%" ) == 0 ) return 2;
    if ( strcmp( op , "+" ) == 0 || strcmp( op , "-" ) == 0 ) return 1;
    return 0; // Para parênteses
}

// Verifica se um operador é unário (uma função)
int is_unary( const char * op ) {
    return strcmp( op , "raiz" ) == 0 || strcmp( op , "sen" ) == 0 || strcmp( op , "cos" ) == 0 || strcmp( op , "tg" ) == 0 || strcmp( op , "log" ) == 0;
}

// Verifica se um operador tem associatividade à direita
int is_right_associative( const char * op ) {
    return strcmp( op , "^" ) == 0;
}

// Converte graus para radianos
double to_radians( double degrees ) {
    return degrees * ( M_PI / 180.0 );
}

// Converte Infixa para Pós-fixada (Algoritmo Shunting-Yard)
char * getFormaPosFixa( char * StrInFixa ) {
    static char result[ 512 ];
    char temp_str[ 512 ];
    strcpy( temp_str , StrInFixa ); // Cópia para não modificar o original
    result[ 0 ] = '\0';

    StringStack op_stack;
    string_stack_init( &op_stack );

    char * token = strtok( temp_str , " " );
    while ( token != NULL ) {
        if ( is_number( token ) ) {
            strcat( result , token );
            strcat( result , " " );
        }
        else if ( is_unary( token ) ) {
            string_stack_push( &op_stack , token );
        }
        else if ( strcmp( token , "(" ) == 0 ) {
            string_stack_push( &op_stack , token );
        }
        else if ( strcmp( token , ")" ) == 0 ) {
            while ( !string_stack_is_empty( &op_stack ) && strcmp( string_stack_peek( &op_stack ) , "(" ) != 0 ) {
                strcat( result , string_stack_pop( &op_stack ) );
                strcat( result , " " );
            }
            string_stack_pop( &op_stack ); // Remove o "("
            if ( !string_stack_is_empty( &op_stack ) && is_unary( string_stack_peek( &op_stack ) ) ) {
                strcat( result , string_stack_pop( &op_stack ) );
                strcat( result , " " );
            }
        }
        else { // Operador binário
            while ( !string_stack_is_empty( &op_stack ) && strcmp( string_stack_peek( &op_stack ) , "(" ) != 0 ) {
                char * top_op = string_stack_peek( &op_stack );
                if ( ( precedence( top_op ) > precedence( token ) ) ||
                    ( precedence( top_op ) == precedence( token ) && !is_right_associative( token ) ) ) {
                    strcat( result , string_stack_pop( &op_stack ) );
                    strcat( result , " " );
                }
                else {
                    break;
                }
            }
            string_stack_push( &op_stack , token );
        }
        token = strtok( NULL , " " );
    }

    while ( !string_stack_is_empty( &op_stack ) ) {
        strcat( result , string_stack_pop( &op_stack ) );
        strcat( result , " " );
    }

    // Remove o espaço extra no final
    if ( strlen( result ) > 0 ) {
        result[ strlen( result ) - 1 ] = '\0';
    }

    return result;
}

// Avalia uma expressão Pós-fixada
float getValorPosFixa( char * StrPosFixa ) {
    FloatStack val_stack;
    float_stack_init( &val_stack );

    char temp_str[ 512 ];
    strcpy( temp_str , StrPosFixa );

    char * token = strtok( temp_str , " " );
    while ( token != NULL ) {
        if ( is_number( token ) ) {
            float_stack_push( &val_stack , atof( token ) );
        }
        else {
            if ( is_unary( token ) ) {
                float operand = float_stack_pop( &val_stack );
                float res = 0.0;
                if ( strcmp( token , "log" ) == 0 ) res = log10( operand );
                else if ( strcmp( token , "raiz" ) == 0 ) res = sqrt( operand );
                else if ( strcmp( token , "sen" ) == 0 ) res = sin( to_radians( operand ) );
                else if ( strcmp( token , "cos" ) == 0 ) res = cos( to_radians( operand ) );
                else if ( strcmp( token , "tg" ) == 0 ) res = tan( to_radians( operand ) );
                float_stack_push( &val_stack , res );
            }
            else { // Operador binário
                float op2 = float_stack_pop( &val_stack );
                float op1 = float_stack_pop( &val_stack );
                float res = 0.0;
                if ( strcmp( token , "+" ) == 0 ) res = op1 + op2;
                else if ( strcmp( token , "-" ) == 0 ) res = op1 - op2;
                else if ( strcmp( token , "*" ) == 0 ) res = op1 * op2;
                else if ( strcmp( token , "/" ) == 0 ) res = op1 / op2;
                else if ( strcmp( token , "^" ) == 0 ) res = pow( op1 , op2 );
                else if ( strcmp( token , "%" ) == 0 ) res = fmod( op1 , op2 );
                float_stack_push( &val_stack , res );
            }
        }
        token = strtok( NULL , " " );
    }
    return float_stack_pop( &val_stack );
}

// Avalia uma expressão Infixa (convertendo primeiro)
float getValorInFixa( char * StrInFixa ) {
    char * posfixa = getFormaPosFixa( StrInFixa );
    return getValorPosFixa( posfixa );
}


// Converte Pós-fixada para Infixada
char * getFormaInFixa( char * StrPosFixa ) {
    static char result[ 512 ];
    StringStack s_stack;
    string_stack_init( &s_stack );

    char temp_str[ 512 ];
    strcpy( temp_str , StrPosFixa );

    char * token = strtok( temp_str , " " );
    while ( token != NULL ) {
        if ( is_number( token ) ) {
            string_stack_push( &s_stack , token );
        }
        else if ( is_unary( token ) ) {
            char * operand = string_stack_pop( &s_stack );
            char new_expr[ TOKEN_MAX_LEN ];
            sprintf( new_expr , "%s ( %s )" , token , operand );
            string_stack_push( &s_stack , new_expr );
        }
        else { // Operador Binário
            char * op2 = string_stack_pop( &s_stack );
            char * op1 = string_stack_pop( &s_stack );
            char new_expr[ TOKEN_MAX_LEN ];
            sprintf( new_expr , "( %s %s %s )" , op1 , token , op2 );
            string_stack_push( &s_stack , new_expr );
        }
        token = strtok( NULL , " " );
    }
    strcpy( result , string_stack_pop( &s_stack ) );
    return result;
}
