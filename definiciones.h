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
	char inmediate[10]; //target si es jump
	bool escribe;
	int salta;
};


typedef struct Datos Datos;

Datos *listaDatos;
Datos *pipeline;

int NINTRUCCIONES;
int *REGISTROS_VALOR;
int *ARREGLO_SP;
char *NOMBRE_ARCHIVO_1;
int CCTotal;

void escribir_archivo(FILE *archivo);
void leerArchivosYGuardarDatos();
void hazardDatos();
void desarrolloDeInstrucciones();
void archivos_salida();
void escribir_registros();
void recibirNombreArchivo();
void liberarMemoria();
