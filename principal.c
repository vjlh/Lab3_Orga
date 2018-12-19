#include "definiciones.h"

int main(int argc, char** argv)
{	
	recibirNombreArchivo();
	leerArchivosYGuardarDatos();
	for (int i = 0; i < NINTRUCCIONES; ++i)
	{
		printf("%s %s %s %s %s\n",listaDatos[i].funcion,listaDatos[i].rd,listaDatos[i].rs,listaDatos[i].rt, listaDatos[i].inmediate);
	}

	//int x = strcmp(listaDatos[5].funcion,listaDatos[1].inmediate);
	//printf("-%i-\n",x);
	desarrolloDeInstrucciones();


	//escribir_archivo1();
	//liberarMemoria();
	//printf("\nEl programa ha terminado su ejecucion correctamente, revise sus archivos de salida...\n");
}