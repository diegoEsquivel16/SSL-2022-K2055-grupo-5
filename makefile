flags= -std=c99 -Wno-missing-prototypes
rm = rm -rf
CC = gcc

.PHONY: clean run

Main: Main.o CharToIntConverter.o FileInputReader.o ResolveOperation.o RecognizeDigits.o
	$(CC) Main.o CharToIntConverter.o FileInputReader.o ResolveOperation.o RecognizeDigits.o -o Main $(flags)

Main.o: CharToIntConverter.o FileInputReader.o ResolveOperation.o RecognizeDigits.o
	$(CC) -c Main.c -o Main.o $(flags)

ResolveOperation.o: ResolveOperation.h CharToIntConverter.o
	$(CC) -c ResolveOperation.c -o ResolveOperation.o $(flags)

RecognizeDigits.o: RecognizeDigits.h
	$(CC) -c RecognizeDigits.c -o RecognizeDigits.o $(flags)

CharToIntConverter.o: CharToIntConverter.h
	$(CC) -c CharToIntConverter.c -o CharToIntConverter.o $(flags)

FileInputReader.o: FileInputReader.h
	$(CC) -c FileInputReader.c -o FileInputReader.o $(flags)

clean: 
	$(rm) *.o
	$(rm) $(TARGET)