#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * En este archivo se encuentran las definicion de la estructura a utilizar, las variables globales y los encabezados
   de cada función
 * @author: Valentina Ligueño
 * @version: 21/12/2018
 */

//Estructura
struct Datos
{
	char funcion[10];
	char rd[10];
	char rs[10];
	char rt[10];
	char inmediate[10]; //target si es jump
	int salta;
	int etiqueta;
	int id;
	bool escribe;
};

typedef struct Datos Datos;

//Definicion de variables globales
Datos *listaDatos;
Datos *pipeline;
int NINTRUCCIONES;
int *REGISTROS_VALOR;
int *ARREGLO_SP;
int ID;
int CCTotal;
char *NOMBRE_ARCHIVO_1;

//Encabezados de funcion
void recibirNombreArchivo();
void leerArchivosYGuardarDatos();
void desarrolloDeInstrucciones();
void hazardDeteccion(int);
int indiceInstruccion(int);
void archivos_salida();
void escribir_registros(char*);
char* recibirNombreArchivoSalida(char*);
void liberarMemoria();
