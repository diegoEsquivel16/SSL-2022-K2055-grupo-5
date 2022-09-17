#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileInputReader.h"

int getFileInputs(char *strs[]) {
    //Inicializo variables
    FILE * inputsFile;
    char * line = NULL;
    size_t len = 0;
    int i = 0;

    //Intento abrir el archivo en modo lectura, si falla, salgo del programa
    inputsFile = fopen("input.txt", "r");
    if (inputsFile == NULL) {
        printf("Error while reading input file, exit program");
        exit(EXIT_FAILURE);
    }    

    while (getline(&line, &len, inputsFile) != -1) {
        
        strs[i] = malloc(strlen(line) + 1); //asigno memoria para el elemento del array
        strcpy(strs[i], line); //copio lo que leí de la linea del archivo al elemento del array
        i++;
    }

    //Cierro el archivo leido y libero la memoria del line
    fclose(inputsFile);
    if (line)
        free(line);
    return 0;    
}