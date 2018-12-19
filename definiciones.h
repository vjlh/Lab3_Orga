#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

struct Datos
{
	char funcion[10];
	char rd[10];
	char rs[10];
	char rt[10];
	char inmediate[40]; //target si es jump
	bool escribe;
};

struct Buffers
{
	char IF[10];
	char ID[10];
	char EX[10];
	char MEM[10];
	char WB[10];
	int cCInicial;
	int cCFinal;
};

typedef struct Datos Datos;

Datos *listaDatos;

int NINTRUCCIONES;
int *REGISTROS_VALOR;
int *ARREGLO_SP;
char *NOMBRE_ARCHIVO_1;
int CCTotal;

void escribir_archivo(FILE *archivo);
void leerArchivosYGuardarDatos();
void hazardDatos();
void desarrolloDeInstrucciones();
void escribir_archivo1();
void recibirNombreArchivo();
void liberarMemoria();
