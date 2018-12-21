#include "definiciones.h"
/*
 * En este archivo se encuentra el llamado a las funciones para ejecutar el programa
   del programa
 * @author: Valentina Ligueño
 * @version: 21/12/2018
 */

int main(int argc, char** argv)
{	
	recibirNombreArchivo();
	leerArchivosYGuardarDatos();
	desarrolloDeInstrucciones();
	//liberarMemoria();
	printf("\nEl programa ha terminado su ejecucion correctamente, revise sus archivos de salida...\n");
}