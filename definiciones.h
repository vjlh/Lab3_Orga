#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Datos
{
	char funcion[10];
	char dato1[10];
	char dato2[10];
	char dato3[10];
	char estado;
	int uso;
};

typedef struct Datos Datos;

Datos *listaDatos;

int NINTRUCCIONES;
int *REGISTROS_VALOR;
int *ARREGLO_SP;
char *NOMBRE_ARCHIVO_1;

void escribir_archivo(FILE *archivo);
void leerArchivosYGuardarDatos();
void desarrolloDeInstrucciones();
void escribir_archivo1();
void recibirNombreArchivo();
void liberarMemoria();
