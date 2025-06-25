#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "expressao.h"

// Linux Build:
// gcc expressao.c main.c -o expressao
// ./expressao

// Windows Build
// gcc expressao.c main.c -o expressao.exe
// expressao.exe

void testar_expressao_infixa( char * infixa ) {
    printf( "----------------------------------------\n" );
    printf( "Expressao Infixa Original: %s\n" , infixa );

    // Converte para Pós-fixa
    char * posfixa = getFormaPosFixa( infixa );
    printf( " > Convertida para Posfixa: %s\n" , posfixa );

    // Converte a Pós-fixa de volta para Infixa
    char * infixa_convertida = getFormaInFixa( posfixa );
    printf( " > Posfixa convertida para Infixa: %s\n" , infixa_convertida );

    // Calcula o valor a partir da Infixa original
    float valor = getValorInFixa( infixa );
    printf( " > Valor calculado: %f\n" , valor );
}

void testar_expressao_posfixa( char * posfixa ) {
    printf( "----------------------------------------\n" );
    printf( "Expressao Posfixa Original: %s\n" , posfixa );

    // Converte para Infixa
    char * infixa = getFormaInFixa( posfixa );
    printf( " > Convertida para Infixa: %s\n" , infixa );

    // Calcula o valor a partir da Posfixa original
    float valor = getValorPosFixa( posfixa );
    printf( " > Valor calculado: %f\n" , valor );
}


int main( ) {
    printf( "INICIANDO TESTES DO AVALIADOR DE EXPRESSOES\n\n" );

    printf( "--- Testes com base na tabela (Infixa -> Posfixa -> Valor) ---\n" );
    testar_expressao_infixa( "( 3 + 4 ) * 5" );
    testar_expressao_infixa( "7 * 2 + 4" );
    testar_expressao_infixa( "8 + ( 5 * ( 2 + 4 ) )" );
    testar_expressao_infixa( "( 6 / 2 + 3 ) * 4" );
    testar_expressao_infixa( "9 + ( 5 * ( 2 + 8 * 4 ) )" );
    testar_expressao_infixa( "log ( 2 + 3 ) / 5" );
    testar_expressao_infixa( "( log ( 10 ) ) ^ 3 + 2" );
    testar_expressao_infixa( "( 45 + 60 ) * cos ( 30 )" );
    testar_expressao_infixa( "sen ( 45 ) ^ 2 + 0.5" );

    printf( "\n\n--- Testes Adicionais (Posfixa -> Infixa -> Valor) ---\n" );
    testar_expressao_posfixa( "3 4 + 5 *" ); // Teste 1
    testar_expressao_posfixa( "10 2 8 * + 3 -" ); // (10 + (2*8)) - 3
    testar_expressao_posfixa( "90 sin 1 +" ); // sen(90) + 1
    testar_expressao_posfixa( "1 10 log +" ); // 1 + log(10)

    printf( "\n----------------------------------------\n" );
    printf( "TESTES CONCLUIDOS\n" );

    system( "pause" );
    return 0;
}