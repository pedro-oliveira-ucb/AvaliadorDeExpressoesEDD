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

    /// Testes obrigatorios
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

    printf( "\n\n--- Testes com base na tabela (Posfixa -> Infixa -> Valor) ---\n" );
    testar_expressao_posfixa( "3 4 + 5 *" );
    testar_expressao_posfixa( "7 2 * 4 + " );
    testar_expressao_posfixa( "8 5 2 4 + * + " );
    testar_expressao_posfixa( "6 2 / 3 + 4 * " );
    testar_expressao_posfixa( "9 5 2 8 * 4 + * +" );
    testar_expressao_posfixa( "2 3 + log 5 / " );
    testar_expressao_posfixa( "10 log 3 ^ 2 +" );
    testar_expressao_posfixa( "45 60 + 30 cos *" );
    testar_expressao_posfixa( "0.5 45 sen 2 ^ + " );

    /// Testes adicionais
    printf( "\n\n--- Testes de Borda e Complexos (Posfixa -> Infixa -> Valor) ---\n" );
    // Testes de funções unárias
    testar_expressao_posfixa( "100 raiz" );
    testar_expressao_posfixa( "90 sen" );
    testar_expressao_posfixa( "0 cos" );
    testar_expressao_posfixa( "45 tg" );
    testar_expressao_posfixa( "100 log" );

    // Testes de operadores binários
    testar_expressao_posfixa( "10 3 %" ); // Módulo
    testar_expressao_posfixa( "2 3 ^" );   // Potência
    testar_expressao_posfixa( "2 3 2 ^ ^" ); // Associatividade à direita da potência: 2^(3^2) = 512

    // Expressões mais longas e com precedência mista
    testar_expressao_posfixa( "5 1 2 + 4 * + 3 -" ); // 5 + (1+2)*4 - 3 = 14
    testar_expressao_posfixa( "3 5 * 10 + 2 / 4 2 ^ -" ); // ((3*5)+10)/2 - 4^2 = -3.5

    // Teste de expressão malformada
    printf( "----------------------------------------\n" );
    printf( "Teste de expressao malformada (deve retornar erro):\n" );
    testar_expressao_posfixa( "5 * + 2" );

    printf( "\n----------------------------------------\n" );
    printf( "TESTES CONCLUIDOS\n" );

    system( "pause" );
    return 0;
}