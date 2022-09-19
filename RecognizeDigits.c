#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RecognizeDigits.h"

#define STATES_AMOUNTS 7
#define CHARACTER_GROUPS 7

#define DECIMAL_INDEX 0
#define OCTAL_INDEX 1
#define HEXA_INDEX 2

#define DECIMAL_TYPE 0
#define OCTAL_TYPE 1
#define HEXA_TYPE 2

#define INVALID_CHAR 6
#define SIGN_CHAR 0
#define ZERO_CHAR 1
#define DECIMAL_CHAR 2
#define X_CHAR 3
#define HEXA_CHAR 4
#define AMP_CHAR 5

#define FIRST_STATE 0
#define OCTAL_STATE 2
#define DECIMAL_STATE 4
#define FIRST_HEXA_STATE 3
#define SECOND_HEXA_STATE 5

//PUNTO 1

int transitionTable[STATES_AMOUNTS][CHARACTER_GROUPS];
char nuberTokens[9] = {'1','2','3','4','5','6','7','8','9'};
char hexaLetters[12] = {'a','b','c','d','e','f','A','B','C','D','E','F'};

/*
filas (primer indice) -> cantidad de estados, 0 al 6, donde:
0 -> estado incial
4 y 5 -> estados finales
6 -> estado de rechazo

columnas (segundo indice)-> tipo de caracteres donde el numero representa:
0 -> signos '+' y '-'
1 -> numero '0'
2 -> numeros '1' al '9'
3 -> letras 'x' y 'X'
4 -> hexadecimales, numeros '1' al '9' y letras de la 'a' a la 'f' y 'A' a 'F'
5 -> caracter '&'
6 -> caracter invalido

*/
void loadTransitionTable(){
    //transitionTable[estado actual][tipo de caracter] = proximo estado
    transitionTable[0][SIGN_CHAR] = 1;
    transitionTable[0][ZERO_CHAR] = 2;
    transitionTable[0][DECIMAL_CHAR] = 4;
    transitionTable[0][X_CHAR] = 6;
    transitionTable[0][HEXA_CHAR] = 6;
    transitionTable[0][AMP_CHAR] = 6;
    transitionTable[0][INVALID_CHAR] = 6;

    transitionTable[1][SIGN_CHAR] = 6;
    transitionTable[1][ZERO_CHAR] = 4;
    transitionTable[1][DECIMAL_CHAR] = 4;
    transitionTable[1][X_CHAR] = 6;
    transitionTable[1][HEXA_CHAR] = 6;
    transitionTable[1][AMP_CHAR] = 6;
    transitionTable[1][INVALID_CHAR] = 6;

    transitionTable[2][SIGN_CHAR] = 6;
    transitionTable[2][ZERO_CHAR] = 4;
    transitionTable[2][DECIMAL_CHAR] = 4;
    transitionTable[2][X_CHAR] = 3;
    transitionTable[2][HEXA_CHAR] = 6;
    transitionTable[2][AMP_CHAR] = 6;
    transitionTable[2][INVALID_CHAR] = 6;

    transitionTable[3][SIGN_CHAR] = 6;
    transitionTable[3][ZERO_CHAR] = 6;
    transitionTable[3][DECIMAL_CHAR] = 5;
    transitionTable[3][X_CHAR] = 6;
    transitionTable[3][HEXA_CHAR] = 5;
    transitionTable[3][AMP_CHAR] = 6;
    transitionTable[3][INVALID_CHAR] = 6;

    transitionTable[4][SIGN_CHAR] = 6;
    transitionTable[4][ZERO_CHAR] = 4;
    transitionTable[4][DECIMAL_CHAR] = 4;
    transitionTable[4][X_CHAR] = 6;
    transitionTable[4][HEXA_CHAR] = 6;
    transitionTable[4][AMP_CHAR] = 0;
    transitionTable[4][INVALID_CHAR] = 6;

    transitionTable[5][SIGN_CHAR] = 6;
    transitionTable[5][ZERO_CHAR] = 5;
    transitionTable[5][DECIMAL_CHAR] = 5;
    transitionTable[5][X_CHAR] = 6;
    transitionTable[5][HEXA_CHAR] = 5;
    transitionTable[5][AMP_CHAR] = 0;
    transitionTable[5][INVALID_CHAR] = 6;

    transitionTable[6][SIGN_CHAR] = 6;
    transitionTable[6][ZERO_CHAR] = 6;
    transitionTable[6][DECIMAL_CHAR] = 6;
    transitionTable[6][X_CHAR] = 6;
    transitionTable[6][HEXA_CHAR] = 6;
    transitionTable[6][AMP_CHAR] = 6;
    transitionTable[6][INVALID_CHAR] = 6;

}

int getCharacterType(char character, int actualState) {
    if (character == '+' || character == '-') {
        return SIGN_CHAR;
    }
    if (character == '0') {
        return ZERO_CHAR;
    }

    if (strchr(hexaLetters, character) != NULL) {
        return HEXA_CHAR;
    }

    if (strchr(nuberTokens, character) != NULL){
        //Si vengo de un estado que representa a los decimales, octales o es el estado inicial
        //y encuentro un numero del 1 al 9 entonces el caracter es de tipo decimal
        if (actualState == DECIMAL_STATE || actualState == FIRST_STATE || actualState == OCTAL_STATE){
            return DECIMAL_CHAR;
        }
        //En cambio si vengo de un estado que representa a los hexadecimales, entonces
        //el tipo de caracter pertenece al hexadecimal
        if (actualState == FIRST_HEXA_STATE || actualState == SECOND_HEXA_STATE) {
            return HEXA_CHAR;  
        }
        //por seguridad si hay algun caso de error o no contemplado retorno tipo invalido
        //debido a que no matchea el estado actual con el tipo de caracter encontrado
        return INVALID_CHAR;
    }

    if (character == 'x' || character == 'X') {
        return X_CHAR;
    }

    if (character == '&') {
        return AMP_CHAR;
    }
    
    return INVALID_CHAR;
}

int defineNextState(int actualState, char character, int* actualDigitType) {
    //printf("Obteniendo tipo de caracter para %c ...\n", character);
    int characterType = getCharacterType(character, actualState);
    //printf("Tipo de caracter obtenido -> %i\n", characterType);
    //defino el siguiente estado en base al estado actual y el tipo de caracter encontrado
    //y lo busco en la matrix de transicion cargada previamente
    int nextState = transitionTable[actualState][characterType]; 
    //printf("Proximo estado -> %i\n", nextState);
    switch (actualState) {
    case 0:
        switch (nextState) {
        case 1:
        case 4:
            //Si el estado actual es el incial y encuentro un caracter que me lleva al
            //estado 1 o 4 entonces quiere decir que el caracter es un signo o un digito
            //por ende el tipo de numero que estoy analizando seria uno de tipo Decimal
            *actualDigitType = DECIMAL_TYPE;
            break;
        case 2:
            //Si el estado actual es el incial y encuentro un caracter que me lleva al
            //estado 2 entonces quiere decir que el caracter es un 0, por ende el tipo
            //de numero que estoy analizando seria uno de tipo Octal
            *actualDigitType = OCTAL_TYPE;
            break;
        default:
            break;
        }
        break;
    case 2:
        //Si el estado actual es el numero 2 y el siguiente estado es el estado 3
        //eso quiere decir que el caracter encontrado es una x, por ende ahora el tipo
        //de numero que estoy analizando es uno del tipo Hexadecimal
        if (nextState == 3) {
            *actualDigitType = HEXA_TYPE;
        }
    default:
        break;
    }
    return nextState;
}


void recognizeDigitsFromString(char *stringDigits){
    printf("Caracteres a reconocer -> %s \n", stringDigits);
    //inicializo variables
    loadTransitionTable();
    int counters[3] = {0,0,0};
    int actualState = 0;
    int actualDigitType;
    char character;
    size_t i = 0;

    //ejecuto el analisis mientras no encuentre un caracter nulo, de salto de linea
    //o se haya llegado a un estado de rechazo
    while (stringDigits[i] != '\0' && stringDigits[i] != '\n' && actualState != 6) {
        character = stringDigits[i];
        //printf("caracter -> %c\n", character);
        actualState = defineNextState(actualState, character, &actualDigitType);
        //Si el siguiente estado definido vuelve a ser el estado 0, eso quiere decir que
        //se encontro un & y por ende hay un nuevo tipo de numero a analizar. Antes de
        //hacer este analisis aumento el contador del tipo de numero encontrado
        if(actualState == 0) {
            counters[actualDigitType] = counters[actualDigitType] + 1;
        }
        i++;
    }
    //Hay que contar el tipo de numero encontrado ultimo para no perderlo ya que en el while
    //se cortó por haber encontrado un caracter de salto. Pero hago esta suma sólo si el ultimo
    //estado no fue el de rechazo para evitar contar por duplicado erroneamente
    if(actualState != 6){
        counters[actualDigitType] = counters[actualDigitType] + 1;
    }
    printf("\n Valores decimales = %i \n", counters[DECIMAL_INDEX]);
    printf("\n Valores octales = %i \n", counters[OCTAL_INDEX]);
    printf("\n Valores hexadecimales = %i \n", counters[HEXA_INDEX]);
}
