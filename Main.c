#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CharToIntConverter.h"
#include "FileInputReader.h"
#include "ResolveOperation.h"
#include "RecognizeDigits.h"

int main(int argc, char *argv[]) {
    printf("--------SETUP--------\n");
    printf("Obtengo los valores del archivo de inputs \n");
    char *inputs[3]; 
    getFileInputs(inputs);

    //TODO solo lo imprimo para ver si leyó bien, se puede borrar
    printf("Leo los inputs obtenidos\n");
    printf("Linea 0 = %s", inputs[0]);
    printf("Linea 1 = %s", inputs[1]);
    printf("Linea 2 = %s", inputs[2]);

    //PUNTO 1
    printf("\n--------PUNTO 1--------\n");
    //Para facilidad de lectura de codigo copio el input en la posicion 0 en una variable que
    //indica a qué hace referencia
    char *stringDigits = malloc(strlen(inputs[0]) + 1);
    strcpy(stringDigits, inputs[0]);
    recognizeDigitsFromString(stringDigits);

    //PUNTO 2
    printf("\n--------PUNTO 2--------\n");
    //Para facilidad de lectura de codigo copio el input en la posicion 1 en una variable que
    //indica a qué hace referencia
    char *numberToConvert = malloc(strlen(inputs[1]) + 1);
    strcpy(numberToConvert, inputs[1]);
    //Asingo el resultado de la conversion a una variable int
    int numberConverted = convertToInt(numberToConvert);

    printf("Numero a convertir de String a Entero = %s \n", numberToConvert);
    printf("Numero convertido a Entero = %d \n", numberConverted);

    //PUNTO 3
    printf("\n--------PUNTO 3--------\n");
    //Para facilidad de lectura de codigo copio el input en la posicion 2 en una variable que
    //indica a qué hace referencia
    char *operation = malloc(strlen(inputs[2]) + 1);
    strcpy(operation, inputs[2]);
    int operationResult = resolveOperation(operation);

    printf("punto 3 resultado %d \n", operationResult);

    return EXIT_SUCCESS;
}
