#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ResolveOperation.h"
#include "CharToIntConverter.h"
#include "IntToCharHelper.h"
#include "Stack.h"

//Defino si un caracter es un operador
bool isOperator (char operator) {
    return  operator == '+' || operator == '-' || operator == '*';
}

//Obtengo la precedencia del operador
int getPrecedence(char ch) {
    if (ch == '+' || ch == '-') {
        return 1;
    }
    if (ch == '*') {
        return 2;
    }
    return 0;
}

//Cuando obtengo un operador tengo que hacer la operacion con dos operandos de la pila de valores
void processOperator(char operator, struct stack *valuesStack, bool* thereIsError) {
    int valueA, valueB;
    //Si la pila de valores está vacia es que hay un error en la expresion ingresada
    if (isEmpty(valuesStack)) {
        printf("Expression error, could not get second operation value. Values Stack is empty\n");
        *thereIsError = true;
        return;
    } else {
        //El valor de la pila que está arriba de todo es el segundo operando (fue el último que ingresó)
        valueB = pop(valuesStack);
    }
    //Tengo que sacar el primer operando para la operacion, si la pila está vacia es que hay un error en la expresion ingresada
    if (isEmpty(valuesStack)) {
        printf("Expression error, could not get first operation value. Values Stack is empty\n");
        *thereIsError = true;
        return;
    } else {
        //El siguiente valor en la pila de valores corresponde al primer operando para la operacion
        valueA = pop(valuesStack);
    }

    //Ya con los operandos y el operador puedo hacer la cuenta, dependiendo del operador que sea
    int operationResult = 0;
    if (operator == '+') {
        operationResult = valueA + valueB;
    } else if (operator == '-') {
        operationResult = valueA - valueB;
    } else if (operator == '*') {
        operationResult = valueA * valueB;
    } else {
        printf("Operator error, invalid operator %c\n", operator);
        *thereIsError = true;
    }
    //Ingreso el resultado de la cuenta en la pila de valores
    push(valuesStack, operationResult);
}

//PUNTO 3

int resolveOperation(char *operation) {
    printf("Operacion a resolver = %s \n", operation);
    
    size_t i = 0; //Indice
    char nextToken = operation[i]; //Primer caracter de la operacion recibida por parametro
    struct stack * operatorStack = newStack(100); //Inicializo la pila de operandos
    struct stack * valuesStack = newStack(100); //Inicializo la pila de valores
    bool thereIsError = false; //Flag que determina si hubo algun error en la operatoria
    bool previousTokenWasADigit = false; //Flag que uso para saber si el anterior caracter leido tambien fue un digito
                                         //Esto es necesario por si recibo caracteres que representan numeros de mas de un digito

    //Voy a iterar mientras el caracter leido no sea un espacio o un fin de linea
    while (nextToken != '\0' && nextToken != '\n') {        
        //Pregunto si el token leido es un digito
        if (nextToken >= '0' && nextToken <= '9') {
            if (previousTokenWasADigit){
                int previousDigit = pop(valuesStack);
                char previousCharDigit [sizeof(int)*8+1];
                itoa(previousDigit, previousCharDigit, 10);
                char* newCharDigit;
                newCharDigit = malloc(strlen(previousCharDigit)+2);
                strcpy(newCharDigit, previousCharDigit);
                strcat(newCharDigit, &nextToken);
                int value = convertToInt(newCharDigit);
                push(valuesStack, value);
                free(newCharDigit);
            } else {
                int value = convertToInt(&nextToken);
                push(valuesStack, value);
                //Como el caracter fue un digito debo setear este flag en true para la siguiente iteracion
                previousTokenWasADigit = true;
            }        
        //Si no es un digito preguto si el caracter leido es un operador
        } else if (isOperator(nextToken)) {
            //Si el caracter es un operador de mayor precedencia al que ya existe en la pila de operadores, o es el primero
            //Entonces lo ingreso directamente a la pila de operadores
            if (isEmpty(operatorStack) || getPrecedence(nextToken) > getPrecedence(peek(operatorStack))) {
                push(operatorStack ,nextToken);
            } else {
                //Como el caracter es de menor o igual precedencia al siguiente en la pila de operandos,
                //Tengo que sacar el operando de la pila y realizar la operacion correspondiente,
                //Este proceso se tiene que repetir hasta que haya un operando de mayor precedencia al caracter leido o se vacie la pila de operandos
                while (!isEmpty(operatorStack) && getPrecedence(nextToken) <= getPrecedence(peek(operatorStack))) {
                    char toProcess = pop(operatorStack);
                    processOperator(toProcess, valuesStack, &thereIsError);
                }
                //Como ya se resolvieron todas las operaciones de menor o igual precedencia, ahora se prosigue a guardar el operando leido en la pila
                push(operatorStack, nextToken);
            }
            //Como el caracter no fue un digito debo setear este flag en true para la siguiente iteracion
            previousTokenWasADigit = false;
        } else {
            printf("Expression error, the input is invalid!\n");
            thereIsError = true;
        }

        i++;
        nextToken = operation[i];
    }
    // Como ya se leyeron todos los caracteres de la entrada, ahora hay que vaciar los operandos de la pila de operandos y realizar las operaciones que falten
    while (!isEmpty(operatorStack) && isOperator(peek(operatorStack))) {
        char toProcess = pop(operatorStack);
        processOperator(toProcess, valuesStack, &thereIsError);
    }

    //El resultado final se encuentra en el ultimo valor de la pila de valores
    int result = pop(valuesStack);

    //Si no hubo ningun error en el camino y las pials estan vacias entonces todo termino correctamente, caso contrario se avisa
    if (thereIsError || !isEmpty(operatorStack) || !isEmpty(valuesStack)) {
        printf("Expression error. There was an Error or Operator Stack or Values Stack is not empty\n");
    }

    return result;
}