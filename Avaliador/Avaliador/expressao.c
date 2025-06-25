#include "expressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define STACK_MAX_SIZE 256
#define TOKEN_MAX_LEN 64

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

///@brief Estrutura de pilha para armazenar operandos (valores float). 
typedef struct {
    float data[ STACK_MAX_SIZE ];
    int top;
} FloatStack;

///@brief Inicializa a pilha de floats. 
void float_stack_init( FloatStack * s ) { s->top = -1; }

///@brief Adiciona um valor float ao topo da pilha. 
int float_stack_push( FloatStack * s , float value ) {
    if ( s->top >= STACK_MAX_SIZE - 1 ) return -1;
    s->data[ ++s->top ] = value;
    return 0;
}

///@brief Remove e retorna o valor float do topo da pilha.
float float_stack_pop( FloatStack * s ) {
    if ( s->top < 0 ) return NAN;
    return s->data[ s->top-- ];
}

///@brief Estrutura de pilha para armazenar tokens e operadores (strings). 
typedef struct {
    char data[ STACK_MAX_SIZE ][ TOKEN_MAX_LEN ];
    int top;
} StringStack;

///@brief Inicializa a pilha de strings. 
void string_stack_init( StringStack * s ) { s->top = -1; }

///@brief Adiciona uma string ao topo da pilha. 
int string_stack_push( StringStack * s , const char * value ) {
    if ( s->top >= STACK_MAX_SIZE - 1 ) return -1;
    strcpy( s->data[ ++s->top ] , value );
    return 0;
}

///@brief Remove e retorna a string do topo da pilha.
char * string_stack_pop( StringStack * s ) {
    if ( s->top < 0 ) return NULL;
    return s->data[ s->top-- ];
}

/// @brief Retorna a string do topo da pilha sem removê-la.
char * string_stack_peek( StringStack * s ) {
    if ( s->top < 0 ) return NULL;
    return s->data[ s->top ];
}

/// @brief Verifica se a pilha de strings está vazia. 
int string_stack_is_empty( StringStack * s ) {
    return s->top == -1;
}

///@brief Verifica se um token (string) é um número válido. 
int is_number( const char * token ) {
    if ( !token ) return 0;
    char * end;
    strtod( token , &end );
    return end != token && *end == '\0';
}

/// @brief Retorna a precedência numérica de um operador. 
int precedence( const char * op ) {
    if ( strcmp( op , "raiz" ) == 0 || strcmp( op , "sen" ) == 0 || strcmp( op , "cos" ) == 0 || strcmp( op , "tg" ) == 0 || strcmp( op , "log" ) == 0 ) return 4;
    if ( strcmp( op , "^" ) == 0 ) return 3;
    if ( strcmp( op , "*" ) == 0 || strcmp( op , "/" ) == 0 || strcmp( op , "%" ) == 0 ) return 2;
    if ( strcmp( op , "+" ) == 0 || strcmp( op , "-" ) == 0 ) return 1;
    return 0;
}

/// @brief Verifica se um operador é unário (função matemática). 
int is_unary( const char * op ) {
    return strcmp( op , "raiz" ) == 0 || strcmp( op , "sen" ) == 0 || strcmp( op , "cos" ) == 0 || strcmp( op , "tg" ) == 0 || strcmp( op , "log" ) == 0;
}

/// @brief Verifica se um operador tem associatividade à direita (ex: potência). 
int is_right_associative( const char * op ) {
    return strcmp( op , "^" ) == 0;
}

/// @brief Converte um ângulo de graus para radianos. 
double to_radians( double degrees ) {
    return degrees * ( M_PI / 180.0 );
}


///@brief Converte uma expressão da notação Infixa para a Pós-fixada.
char * getFormaPosFixa( char * StrInFixa ) {
    static char result[ 512 ];
    char temp_str[ 512 ];
    strcpy( temp_str , StrInFixa );
    result[ 0 ] = '\0';

    static StringStack op_stack;
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
            string_stack_pop( &op_stack );
            if ( !string_stack_is_empty( &op_stack ) && is_unary( string_stack_peek( &op_stack ) ) ) {
                strcat( result , string_stack_pop( &op_stack ) );
                strcat( result , " " );
            }
        }
        else {
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

    if ( strlen( result ) > 0 ) {
        result[ strlen( result ) - 1 ] = '\0';
    }

    return result;
}


/// @brief Calcula o valor numérico de uma expressão na notação Pós-fixada.
float getValorPosFixa( char * StrPosFixa ) {
    static FloatStack val_stack;
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
            else {
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


/// @brief Calcula o valor de uma expressão Infixa (através da conversão para Pós-fixa).
float getValorInFixa( char * StrInFixa ) {
    char * posfixa = getFormaPosFixa( StrInFixa );
    return getValorPosFixa( posfixa );
}


/// @brief Converte uma expressão da notação Pós-fixada para a Infixada.
char * getFormaInFixa( char * StrPosFixa ) {
    static char result[ 512 ];
    static StringStack s_stack;
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
            if ( !operand ) { return "Error: Malformed Expression"; }
            char new_expr[ TOKEN_MAX_LEN ];
            sprintf( new_expr , "%s( %s )" , token , operand );
            string_stack_push( &s_stack , new_expr );
        }
        else {
            char * op2 = string_stack_pop( &s_stack );
            char * op1 = string_stack_pop( &s_stack );
            if ( !op1 || !op2 ) { return "Error: Malformed Expression"; }
            char new_expr[ TOKEN_MAX_LEN ];
            sprintf( new_expr , "( %s %s %s )" , op1 , token , op2 );
            string_stack_push( &s_stack , new_expr );
        }
        token = strtok( NULL , " " );
    }

    char * final_expr = string_stack_pop( &s_stack );
    if ( final_expr && string_stack_is_empty( &s_stack ) ) {
        strcpy( result , final_expr );
    }
    else {
        strcpy( result , "Error: Malformed Expression" );
    }

    return result;
}
