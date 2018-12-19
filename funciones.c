#include "definiciones.h"
char REGISTROS_NOMBRES[32][6] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7"
								,"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

void leerArchivosYGuardarDatos()		
{											
	FILE* archivo_instrucciones;		
	int aux1, aux2, contador, i, largo;
	bool etiqueta;	
	char buffer[100],buffer2[100],*funcion,*rd, *rs, *rt, *inmediate;
										
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
	listaDatos = (Datos*)calloc(NINTRUCCIONES,sizeof(Datos));
	pipeline = (Datos*)calloc(NINTRUCCIONES*50,sizeof(Datos));
	REGISTROS_VALOR = (int*)calloc(32,sizeof(int));
	ARREGLO_SP = (int*)calloc(4096,sizeof(int));
	
	archivo_instrucciones = fopen(NOMBRE_ARCHIVO_1,"r");
	
	aux1 = 0;

	while (!feof(archivo_instrucciones))
	{
		memset(buffer,0, 100);
		memset(buffer2,0, 100);
		aux2 = 1;
		etiqueta = false;
		fscanf(archivo_instrucciones," %119[^\n]",buffer);
		largo = strlen(buffer);

		for (i = 0; i < largo; ++i)
		{
			if (buffer[i] == ' ')
				aux2++;

			if (buffer[i] == ':')
				etiqueta = true;
		}

		if (buffer[0] != '\0' && aux2 != largo && buffer[0] != '\n' && buffer[0] != '#')
		{	
			for (i = 0; i < largo; ++i)
				if (buffer[i] != ' ')
					break;			
							
			strncpy(buffer2,buffer+i,largo-i);
			
			if (etiqueta)
				funcion = strtok(buffer2,":");

			else
				funcion = strtok(buffer2," ");
		
			//Tipo R
			if ((strcmp(funcion,"add"))==0 || (strcmp(funcion,"sub"))==0 || (strcmp(funcion,"mul"))==0 || (strcmp(funcion,"div"))==0)
			{
				strcpy(listaDatos[aux1].funcion,funcion);

				rd = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rd,rd);

				rs = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rs,rs);

				rt = strtok(NULL," \f\n\r\t\v");
				strcpy(listaDatos[aux1].rt,rt);

				listaDatos[aux1].escribe = true;

			}

			//Tipo I Aritmético
			else if ((strcmp(funcion,"addi"))==0 || (strcmp(funcion,"addiu"))==0 || (strcmp(funcion,"subi"))==0 )
			{

				strcpy(listaDatos[aux1].funcion,funcion);

				rd = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rd,rd);

				rs = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rs,rs);

				inmediate = strtok(NULL," \f\n\r\t\v");
				strcpy(listaDatos[aux1].inmediate,inmediate);

				listaDatos[aux1].escribe = true;

			}
			//Tipo I Beq

			else if(strcmp(funcion,"beq")==0 || strcmp(funcion,"bne")==0 || strcmp(funcion,"bgt")==0 || strcmp(funcion,"blt")==0)
			{
				strcpy(listaDatos[aux1].funcion,funcion);

				rt = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rt,rt);

				rs = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rs,rs);

				inmediate = strtok(NULL," \f\n\r\t\v");
				strcpy(listaDatos[aux1].inmediate,inmediate);

				listaDatos[aux1].escribe = false;

			}

			//Jumps :D
			else if (strcmp(funcion,"j") == 0 || strcmp(funcion,"jal") == 0)
			{
				strcpy(listaDatos[aux1].funcion,funcion);

				inmediate = strtok(NULL," \f\n\r\t\v");
				strcpy(listaDatos[aux1].inmediate,inmediate);

				listaDatos[aux1].escribe = false;

			}

			else if (strcmp(funcion,"jr")==0)
			{
				strcpy(listaDatos[aux1].funcion,funcion);

				rs = strtok(NULL," \f\n\r\t\v");
				strcpy(listaDatos[aux1].rs,rs);

				listaDatos[aux1].escribe = false;
			}

			//sw
			else if (strcmp(funcion,"sw")==0)
			{
				strcpy(listaDatos[aux1].funcion,funcion);
				
				rt = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rt,rt);

				inmediate = strtok(NULL," (");
				strcpy(listaDatos[aux1].inmediate,inmediate);
				
				rs = strtok(NULL,")");
				strcpy(listaDatos[aux1].rs,rs);

				listaDatos[aux1].escribe = false;
			}

			//lw
			else if (strcmp(funcion,"lw")==0)
			{
				strcpy(listaDatos[aux1].funcion,funcion);
				
				rd = strtok(NULL,", ");
				strcpy(listaDatos[aux1].rd,rd);

				inmediate = strtok(NULL," (");
				strcpy(listaDatos[aux1].inmediate,inmediate);
				
				rs = strtok(NULL,")");
				strcpy(listaDatos[aux1].rs,rs);

				listaDatos[aux1].escribe = true;

			}			
			else
			{
				strcpy(listaDatos[aux1].funcion,funcion);
				listaDatos[aux1].escribe = false;
			}
		}
		aux1++;	

		fgetc(archivo_instrucciones);
		if (feof(archivo_instrucciones))
			break;	
	}
	fclose(archivo_instrucciones);
	/*for (int i = 0; i < NINTRUCCIONES; ++i)
	{
		printf("%s %s %s %s %s\n",listaDatos[i].funcion,listaDatos[i].rd,listaDatos[i].rs,listaDatos[i].rt, listaDatos[i].inmediate);
	}*/

}
void hazardDetenccion(int totIns)
{
	CCTotal = 4;
	for (int i = 0; i < totIns-1; ++i)
	{
		CCTotal++;
		if (!pipeline[i].escribe && strcmp(pipeline[i].funcion,"sw")!=0)
		{
			if(pipeline[i].salta == 1)
			{
				printf("Hazard Control en BRANCH en CC: %i\n",CCTotal-1);
				CCTotal+=3;

			}
			
			else if(pipeline[i].salta == 2)
			{
				printf("Hazard control con JUMP en CC: %i\n",CCTotal-3);
				CCTotal+=1;
			}
		}

		if (i<totIns-2)
		{
			if (pipeline[i].escribe && (strcmp(pipeline[i].rd,pipeline[i+1].rs)==0 || strcmp(pipeline[i].rd,pipeline[i+1].rt)==0))
			{
				//printf("%s %s %s %s %s\n",pipeline[i].funcion,pipeline[i].rd,pipeline[i].rs,pipeline[i].rt, pipeline[i].inmediate);
				//printf("%s %s %s %s %s\n",pipeline[i+1].funcion,pipeline[i+1].rd,pipeline[i+1].rs,pipeline[i+1].rt, pipeline[i+1].inmediate);
				if(strcmp(pipeline[i].funcion,"lw")==0)
				{
					printf("Hay hazzard de datos arreglable con nop y forwarding en CC: %i\n",CCTotal-2);
					CCTotal+=1;

				}

				else
					printf("Hay hazzard de datos arreglable con forwarding en CC: %i\n",CCTotal-2);
			}

			if (pipeline[i].escribe && (strcmp(pipeline[i].rd,pipeline[i+2].rs)==0 || strcmp(pipeline[i].rd,pipeline[i+2].rt)==0))
				printf("Hay hazzard de datos arreglable con forwarding en CC: %i\n",CCTotal-2);

		}
		else
		{
			if (pipeline[i].escribe && (strcmp(pipeline[i].rd,pipeline[i+1].rs)==0 || strcmp(pipeline[i].rd,pipeline[i+1].rt)==0))
			{
				//printf("%s %s %s %s %s\n",pipeline[i].funcion,pipeline[i].rd,pipeline[i].rs,pipeline[i].rt, pipeline[i].inmediate);
				//printf("%s %s %s %s %s\n",pipeline[i+1].funcion,pipeline[i+1].rd,pipeline[i+1].rs,pipeline[i+1].rt, pipeline[i+1].inmediate);
				if(strcmp(pipeline[i].funcion,"lw")==0)
				{
					printf("Hay hazzard de datos arreglable con nop y forwarding en CC: %i\n",CCTotal-2);
					CCTotal+=1;

				}

				else
					printf("Hay hazzard de datos arreglable con forwarding en CC: %i\n",CCTotal-2);
			}
		}	
			
	}
	printf("La cantidad final de ciclos de reloj es %i\n",CCTotal);		
}
void desarrolloDeInstrucciones()
{
	int indice = 0, k, aux1 = 0, total, num2, num3, posicion, temporal;
	char *funcion, *dato1, *dato2, *dato3;

	while (aux1<NINTRUCCIONES) 
	{	
		printf("entra a desarrollar %s\n",listaDatos[aux1].funcion);
		funcion = listaDatos[aux1].funcion;

		//Inmediatas Aritméticas
		if ((strcmp(funcion,"addi"))==0 || (strcmp(funcion,"subi"))==0 || (strcmp(funcion,"addiu"))==0)
		{
			pipeline[indice] = listaDatos[aux1];
			dato1 = listaDatos[aux1].rd;
			dato2 = listaDatos[aux1].rs;
			dato3 = listaDatos[aux1].inmediate;

			num3 = atoi(dato3);

			for (int i = 0; i < 32; ++i)
				if (strcmp(dato2,REGISTROS_NOMBRES[i])==0)
					num2 = REGISTROS_VALOR[i];

			if((strcmp(funcion,"addi"))==0 || (strcmp(funcion,"addiu"))==0 )
				total = num2 + num3;

			else
				total = num2 - num3;

			for (int j = 0; j < 32; ++j)
				if (strcmp(dato1,REGISTROS_NOMBRES[j])==0)
					REGISTROS_VALOR[j] = total;

		}			

		else if ((strcmp(funcion,"add"))==0 || (strcmp(funcion,"sub"))==0 
				|| (strcmp(funcion,"mul"))==0|| (strcmp(funcion,"div"))==0)
		{
			pipeline[indice] = listaDatos[aux1];	   
			dato1 = listaDatos[aux1].rd;
			dato2 = listaDatos[aux1].rs;
			dato3 = listaDatos[aux1].rt;

			for (int i = 0; i < 32; ++i)
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

			for (int j = 0; j < 32; ++j)
				if (strcmp(dato1,REGISTROS_NOMBRES[j])==0)
					REGISTROS_VALOR[j] = total;		
		}	

		else if (strcmp(funcion,"beq")== 0 || strcmp(funcion,"bgt")== 0 || strcmp(funcion,"blt")== 0
				|| strcmp(funcion,"bne")== 0)
		{
			pipeline[indice] = listaDatos[aux1];	   
			dato1 = listaDatos[aux1].rt;
			dato2 = listaDatos[aux1].rs;
			dato3 = listaDatos[aux1].inmediate;
			
			for (int i = 0; i < 32; ++i)
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

				pipeline[indice].salta = 1;		
				aux1 = posicion;
			}
		}

		else if (strcmp(funcion,"j")==0 || strcmp(funcion,"jal")==0 || strcmp(funcion,"jr")==0)
		{
			pipeline[indice] = listaDatos[aux1];	   
			dato1 = listaDatos[aux1].inmediate;

			if(strcmp(funcion,"j")==0 || strcmp(funcion,"jal")==0)
			{	
				if (strcmp(funcion,"jal")==0)
					REGISTROS_VALOR[31] = aux1+1;

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
			pipeline[indice].salta = 2;

		}
		else if (strcmp(funcion,"sw")== 0)
		{	
			pipeline[indice] = listaDatos[aux1];	   
			dato1 = listaDatos[aux1].rt;
			dato2 = listaDatos[aux1].rs;
			dato3 = listaDatos[aux1].inmediate;
			temporal = atoi(dato3);

			if(temporal%4 == 0)
			{	
				for (k = 0; k < 32; ++k)
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
			pipeline[indice] = listaDatos[aux1];	   
			dato1 = listaDatos[aux1].rd;
			dato2 = listaDatos[aux1].rs;
			dato3 = listaDatos[aux1].inmediate;
			temporal = atoi(dato3);

			if(temporal%4 == 0)
			{	
				for (k = 0; k < 32; ++k)
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
		else
			indice--;

		indice++;
		aux1++;
	}

	hazardDetenccion(indice);
	/*for (int j = 0; j < indice; ++j)
	{
		printf("%s \n",pipeline[j].funcion);
	}*/
	for (int j = 0; j < 32; ++j)
	{
		printf("%s: %i\n",REGISTROS_NOMBRES[j],REGISTROS_VALOR[j]);
	}
}
void escribir_archivo(FILE *archivo) 
{ //Escribe los valores de los 32 registros cada vez que se le llama, que es cada vez que se ejecuta una instruccion
	int i; //Estos valores van siendo escritos en el archivo de salida 1
	for (i = 0; i < 32; ++i)
		fprintf(archivo,"%d;",REGISTROS_VALOR[i]);
	
	fputc('\n',archivo);
}
/*void escribir_archivo1()
{ //Busca que funciones si se ejecutaron y las escribe todas en unen el archivo con las lineas de instrucciones
	int i; //que hicieron que el programa se ejecutaron, no aparecen funciones que el programa no recorrió
	char* funcion;//Principalmente por estar en una "etiqueta" a la que nunca se llegaba
	FILE *salida1;
	salida1 = fopen("salida1.txt","wt");

	for (i = 0; i < NINTRUCCIONES; ++i)
	{
		if (listaDatos[i].uso == 1)
		{	
			funcion = listaDatos[i].funcion;
			if ((strcmp(funcion,"addi"))==0 || (strcmp(funcion,"subi"))==0 || (strcmp(funcion,"add"))==0 
				|| (strcmp(funcion,"sub"))==0 || (strcmp(funcion,"mul"))==0|| (strcmp(funcion,"div"))==0 
				|| (strcmp(funcion,"beq"))==0)
			
				fprintf(salida1,"%s %s %s %s\n",listaDatos[i].funcion,listaDatos[i].rd,listaDatos[i].rs,listaDatos[i].rt);

			else if ((strcmp(funcion,"j"))==0)
				fprintf(salida1,"%s %s\n",listaDatos[i].funcion,listaDatos[i].rd);
			
			else if ((strcmp(funcion,"lw"))==0 || (strcmp(funcion,"sw"))==0)
				fprintf(salida1,"%s %s %s\n",listaDatos[i].funcion,listaDatos[i].rd,listaDatos[i].rs);
			
			else
				fprintf(salida1,"%s\n",listaDatos[i].funcion);
		}
	}
}*/
void recibirNombreArchivo() 
{
	FILE* arch;
	NOMBRE_ARCHIVO_1 = (char*)calloc(25,sizeof(char));//Estas variables globales definidas en las definiciones
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