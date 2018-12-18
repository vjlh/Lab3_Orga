#include "definiciones.h"
char REGISTROS_NOMBRES[31][6] = {"$zero","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7"
								,"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

void leerArchivosYGuardarDatos()		
{											
	FILE* archivo_instrucciones;		
	int aux1, aux2, contador, i, largo;	
	char buffer[100],buffer2[100],*valor1,*valor2, *valor3, *valor4;
										
	contador = 0;						
	archivo_instrucciones = fopen(NOMBRE_ARCHIVO_1,"r"); 

	while (!feof(archivo_instrucciones))
	{
		memset(buffer,0, 100);
		aux2 = 1;
		fscanf(archivo_instrucciones, "%119[^\n]", buffer);

		for (i = 0; i < strlen(buffer); ++i)
			if (buffer[i] == ' ')
				aux2++;

		if (buffer[0] != '\0' && aux2 != strlen(buffer))
			contador++;

		fgetc(archivo_instrucciones);
	}
	fclose(archivo_instrucciones);

	NINTRUCCIONES = contador;
	listaDatos = (Datos*)malloc(sizeof(Datos)*NINTRUCCIONES);
	REGISTROS_VALOR = (int*)malloc(sizeof(int)*31);
	ARREGLO_SP = (int*)malloc(sizeof(int)*4096);

	for (i = 0; i < 31; ++i)
		REGISTROS_VALOR[i] = 0;
	for (i = 0; i < 4096; ++i)
		ARREGLO_SP[i] = 0;
	
	archivo_instrucciones = fopen(NOMBRE_ARCHIVO_1,"r");
	
	aux1 = 0;

	while (!feof(archivo_instrucciones))
	{
		memset(buffer,0, 100);
		memset(buffer2,0, 100);
		aux2 = 1;
		fscanf(archivo_instrucciones," %119[^\n]",buffer);
		largo = strlen(buffer);

		for (i = 0; i < largo; ++i)
			if (buffer[i] == ' ')
				aux2++;

		if (buffer[0] != '\0' && aux2 != largo && buffer[0] != '\n' && buffer[0] != '#')
		{	
			for (i = 0; i < largo; ++i)
				if (buffer[i] != ' ')
					break;

			strncpy(buffer2,buffer+i,largo-i);
			
			if (aux2==1)
				valor1 = strtok(buffer2,":");

			else
				valor1 = strtok(buffer2," ");

			if ((strcmp(valor1,"addi"))==0 || (strcmp(valor1,"addiu"))==0 || (strcmp(valor1,"subi"))==0 || (strcmp(valor1,"add"))==0 
				|| (strcmp(valor1,"sub"))==0 || (strcmp(valor1,"mul"))==0|| (strcmp(valor1,"div"))==0 || (strcmp(valor1,"beq"))==0 
				|| (strcmp(valor1,"bne"))==0|| (strcmp(valor1,"bgt"))==0|| (strcmp(valor1,"blt"))==0|| (strcmp(valor1,"bnt"))==0)
			{
				strcpy(listaDatos[aux1].funcion,valor1);
				valor2 = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rd,valor2);
				valor3 = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rs,valor3);
				valor4 = strtok(NULL," ");
				strcpy(listaDatos[aux1].rt,valor4);
			}	
			else if (strcmp(valor1,"j") ==0 || strcmp(valor1,"jr") == 0 || strcmp(valor1,"jal")==0)
			{
				strcpy(listaDatos[aux1].funcion,valor1);
				valor2 = strtok(NULL," ");
				strcpy(listaDatos[aux1].rd,valor2);

			}
			else if (strcmp(valor1,"lw")==0 || strcmp(valor1,"sw")==0)
			{
				strcpy(listaDatos[aux1].funcion,valor1);
				valor2 = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rd,valor2);
				valor3 = strtok(NULL," ");
				strcpy(listaDatos[aux1].rs,valor3);
			}
			else
				strcpy(listaDatos[aux1].funcion,valor1);
		}
		aux1++;	

		fgetc(archivo_instrucciones);
		if (feof(archivo_instrucciones))
			break;	
	}
	fclose(archivo_instrucciones);
	for (int i = 0; i < NINTRUCCIONES; ++i)
	{
		printf("%s %s %s %s\n",listaDatos[i].funcion,listaDatos[i].rd,listaDatos[i].rs,listaDatos[i].rt);
	}

}
void algo()
{
	for (int i = 0; i < NINTRUCCIONES; ++i)
	{
		
	}
}
void desarrolloDeInstrucciones()
{
	//esta funcion es la encargada de leer las instrucciones almacenadas por la funcion anterior una a una
	//Haciendo lo que cada instrucción requiere, por ejemplo si es un add, sumará, un sub restará,etc
	
	//Declaracion de variables a utilizar	
	int i, k, aux1, total, num2, num3, posicion, temporal;
	char *funcion, *dato1, *dato2, *dato3, *dato2temp, temp[10];
	
	for (i = 0; i < 256; ++i)
		ARREGLO_SP[i] = 0;

	aux1 = 0;
	while (aux1<NINTRUCCIONES) //Con este while se irá recorriendo la estructura, donde aux irá desde cero
	{						   //Hasta el numero total de instrucciones que fue contado al comienzo de esta función
		funcion = listaDatos[aux1].funcion;

		if ((strcmp(funcion,"addi"))==0 || (strcmp(funcion,"subi"))==0 || (strcmp(funcion,"addiu"))==0)
		{
			dato1 = listaDatos[aux1].rd;
			dato2 = listaDatos[aux1].rs;
			dato3 = listaDatos[aux1].rt;

			num3 = *dato3 - 48;
			for (int i = 0; i < 31; ++i)
				if (strcmp(dato2,REGISTROS_NOMBRES[i])==0)
					num2 = REGISTROS_VALOR[i];

			if((strcmp(funcion,"addi"))==0 || (strcmp(funcion,"addiu"))==0 )
				total = num2 + num3;

			else
				total = num2 - num3;

			for (int j = 0; j < 31; ++j)
				if (strcmp(dato1,REGISTROS_NOMBRES[j])==0)
					REGISTROS_VALOR[j] = total;
		}			

		else if ((strcmp(funcion,"add"))==0 || (strcmp(funcion,"sub"))==0 
				|| (strcmp(funcion,"mul"))==0|| (strcmp(funcion,"div"))==0)
		{
			dato1 = listaDatos[aux1].rd;
			dato2 = listaDatos[aux1].rs;
			dato3 = listaDatos[aux1].rt;

			for (int i = 0; i < 31; ++i)
			{
				if (strcmp(dato2,REGISTROS_NOMBRES[i])==0)
					num2 = REGISTROS_VALOR[i];

				if (strcmp(dato3,REGISTROS_NOMBRES[i])==0)
					num3 = REGISTROS_VALOR[i];
			}
			if ((strcmp(funcion,"add"))==0)
				total = num2 + num3;

			else if ((strcmp(funcion,"sub"))==0)
				total = num2 - num3;

			else if ((strcmp(funcion,"mul"))==0)
				total = num2 * num3;

			else if ((strcmp(funcion,"div"))==0)
				total = num2 / num3;

			for (int j = 0; j < 31; ++j)
				if (strcmp(dato1,REGISTROS_NOMBRES[j])==0)
					REGISTROS_VALOR[j] = total;		
				
		}	

		else if (strcmp(funcion,"beq")== 0 || strcmp(funcion,"bgt")== 0 || strcmp(funcion,"blt")== 0
			|| strcmp(funcion,"bne")== 0)
		{
			dato1 = listaDatos[aux1].rd;
			dato2 = listaDatos[aux1].rs;
			dato3 = listaDatos[aux1].rt;
			
			for (int i = 0; i < 31; ++i)
			{
				if (strcmp(dato1,REGISTROS_NOMBRES[i])==0)
					num2 = REGISTROS_VALOR[i];
				if (strcmp(dato2,REGISTROS_NOMBRES[i])==0)
					num3 = REGISTROS_VALOR[i];
			}

			if (  (strcmp(funcion,"beq")== 0 && num2 == num3)
				||(strcmp(funcion,"bgt")== 0 && num2 > num3)
				||(strcmp(funcion,"blt")== 0 && num2 < num3)
				||(strcmp(funcion,"bne")== 0 && num2 != num3))
			{
				for (int k = 0; k < NINTRUCCIONES; ++k)
					if (strcmp(dato3,listaDatos[k].funcion)==0)
					{	
						posicion = k;
						break;	
					}	
				aux1 = posicion;
			}
		}

		else if (strcmp(funcion,"j")==0 || strcmp(funcion,"jal")==0 || strcmp(funcion,"jr")==0)
		{
			dato1 = listaDatos[aux1].rd;

			if(strcmp(funcion,"j")==0 || strcmp(funcion,"jal")==0)
			{	
				if (strcmp(funcion,"jal")==0)
					REGISTROS_VALOR[30] = aux1+1;

				for (int k = 0; k < NINTRUCCIONES; ++k)
					if (strcmp(dato1,listaDatos[k].funcion)==0)
					{	
						posicion = k;
						break;
					}
				aux1 = posicion;
			}

			else if(strcmp(funcion,"jr")==0)
			{	
				for (int k = 0; k < NINTRUCCIONES; ++k)
					if (strcmp(dato1,REGISTROS_NOMBRES[k])==0)
					{	
						posicion = REGISTROS_VALOR[k]-1;
						break;
					}
				aux1 = posicion;
			}
		}
		else if (strcmp(funcion,"sw")== 0)
		{	
			dato1 = listaDatos[aux1].rd;
			dato2 = listaDatos[aux1].rs;
			strcpy(temp,dato2);

			dato2temp = strtok(temp,"(");
			temporal = *dato2temp - 48;

			if(temporal%4 == 0)
			{	
				for (k = 0; k < 31; ++k)
					if (strcmp(dato1,REGISTROS_NOMBRES[k])==0)
						ARREGLO_SP[(temporal/4)]= REGISTROS_VALOR[k];
			}		
			else if(temporal%4 != 0)
			{
				printf("No se puede ejecutar el programa, ya que StoreWord presenta un error en uno de sus parametros\n");
				exit(0);
			}		

		}		
			
		else if (strcmp(funcion,"lw")== 0)
		{	
			dato1 = listaDatos[aux1].rd;
			dato2 = listaDatos[aux1].rs;
			strcpy(temp,dato2);

			dato2temp = strtok(temp,"(");
			temporal = *dato2temp - 48;

			if(temporal%4 == 0)
			{	
				for (k = 0; k < 31; ++k)
					if (strcmp(dato1,REGISTROS_NOMBRES[k])==0)
					{	
						REGISTROS_VALOR[k] = ARREGLO_SP[(temporal/4)];
						break;
					}
			}
			else if(temporal%4 != 0)
			{
				printf("No se puede ejecutar el programa, ya que LoadWord presenta un error en uno de sus parametros\n");
				exit(0);
			}			
		}
		aux1++;
	}
}
void escribir_archivo(FILE *archivo) 
{ //Escribe los valores de los 32 registros cada vez que se le llama, que es cada vez que se ejecuta una instruccion
	int i; //Estos valores van siendo escritos en el archivo de salida 1
	for (i = 0; i < 31; ++i)
		fprintf(archivo,"%d;",REGISTROS_VALOR[i]);
	
	fputc('\n',archivo);
}
/*void escribir_archivo1()
{ //Busca que funciones si se ejecutaron y las escribe todas en unen el archivo con las lineas de instrucciones
	int i; //que hicieron que el programa se ejecutaron, no aparecen funciones que el programa no recorrió
	char* valor1;//Principalmente por estar en una "etiqueta" a la que nunca se llegaba
	FILE *salida1;
	salida1 = fopen("salida1.txt","wt");

	for (i = 0; i < NINTRUCCIONES; ++i)
	{
		if (listaDatos[i].uso == 1)
		{	
			valor1 = listaDatos[i].funcion;
			if ((strcmp(valor1,"addi"))==0 || (strcmp(valor1,"subi"))==0 || (strcmp(valor1,"add"))==0 
				|| (strcmp(valor1,"sub"))==0 || (strcmp(valor1,"mul"))==0|| (strcmp(valor1,"div"))==0 
				|| (strcmp(valor1,"beq"))==0)
			
				fprintf(salida1,"%s %s %s %s\n",listaDatos[i].funcion,listaDatos[i].rd,listaDatos[i].rs,listaDatos[i].rt);

			else if ((strcmp(valor1,"j"))==0)
				fprintf(salida1,"%s %s\n",listaDatos[i].funcion,listaDatos[i].rd);
			
			else if ((strcmp(valor1,"lw"))==0 || (strcmp(valor1,"sw"))==0)
				fprintf(salida1,"%s %s %s\n",listaDatos[i].funcion,listaDatos[i].rd,listaDatos[i].rs);
			
			else
				fprintf(salida1,"%s\n",listaDatos[i].funcion);
		}
	}
}*/
void recibirNombreArchivo() 
{
	FILE* arch;
	NOMBRE_ARCHIVO_1 = (char*)malloc(sizeof(char)*25);//Estas variables globales definidas en las definiciones
	printf("Para comenzar primero se necesita el nombre de sus dos archivos de entrada junto a su formato\n");
	printf("Por ejemplo 'entrada1.txt' o prueba1.txt\n\nRecuerde que el primero es el que contiene las instrucciones y el segundo las lineas de control\n");
	do
	{
		printf("\nIngrese el nombre del primer archivo solicitado: ");
		scanf("%s",NOMBRE_ARCHIVO_1);
		while(getchar()!='\n');
		arch = fopen(NOMBRE_ARCHIVO_1,"r");
		
		if (arch == NULL) 
			printf("No se encuentra archivo con ese nombre, intente nuevamente\n");
		
	} while (arch == NULL);
	fclose(arch);

}
//Función encargada de liberar la memoria que se ocupó durante el procedimiento
void liberarMemoria()
{
	free(listaDatos);
	free(REGISTROS_VALOR);
	free(ARREGLO_SP);
	free(NOMBRE_ARCHIVO_1);
}