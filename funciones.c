#include "definiciones.h"
/*
 * En este archivo se encuentran todas las funciones que se utilizan para el correcto funcionamiento
   del programa
 * @author: Valentina Ligueño
 * @version: 21/12/2018
 */
char REGISTROS_NOMBRES[32][6] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7"
								,"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

/*
 * Esta función se encarga de recibir el nombre del archivo de entrada, asegurandose de que
   se entregue un archivo existente.
 * @return No retorna nada, ya que el nombre del archivo queda guardado en una variable global
 */
void recibirNombreArchivo() 
{
	FILE* arch;
	NOMBRE_ARCHIVO_1 = (char*)calloc(25,sizeof(char));//Estas variables globales definidas en las definiciones
	printf("Para comenzar primero se necesita el nombre de su archivo de entrada junto a su formato\n");
	printf("Por ejemplo 'entrada.txt'\n");
	do
	{
		printf("\nIngrese el nombre del archivo solicitado: ");
		scanf("%s",NOMBRE_ARCHIVO_1);
		while(getchar()!='\n');
		arch = fopen(NOMBRE_ARCHIVO_1,"r");
		
		if (arch == NULL) 
			printf("No se encuentra archivo con ese nombre, intente nuevamente\n");
		
	} while (arch == NULL);
	fclose(arch);

}

/*
 * Esta funcion se encarga del proceso de lectura y obtencion de los datos del archivo 
 * @return No retorna nada porque los datos se almacenan en una estructura declarada de manera
 	global
 */
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
	ID = 0;
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
				listaDatos[aux1].etiqueta = 1;
				listaDatos[aux1].escribe = false;
			}
		}
		listaDatos[aux1].id = ID;
		ID++;
		aux1++;	

		fgetc(archivo_instrucciones);
		if (feof(archivo_instrucciones))
			break;	
	}
	fclose(archivo_instrucciones);
}

/*
 * Se hace el desarrollo de las instrucciones, se pasan una a una las intrucciones que fueron almacenadas
   en la funcion anterior, y se realiza la ejecución de esta, se guarda en otro arreglo declarado de manera
   global las intrucciones que han sido ejecutadas y se le pasa este arreglo a la función de hazardDeteccion
 * @return No retorna nada ya que de de aquí se llama a hazadDeteccion y esta última se encarga de finalizar 
   la ejecucion del programa
*/
void desarrolloDeInstrucciones()
{
	int indice = 0, k, aux1 = 0, total, num2, num3, posicion, temporal;
	char *funcion, *dato1, *dato2, *dato3;
	REGISTROS_VALOR[29] = 4096;
	inicioSP = 4096;
	finSP = -1;

	while (aux1<NINTRUCCIONES) 
	{	
		funcion = listaDatos[aux1].funcion;

		//Inmediatas Aritméticas
		if ((strcmp(funcion,"addi"))==0 || (strcmp(funcion,"subi"))==0 || (strcmp(funcion,"addiu"))==0)
		{
			double totaliu;
			pipeline[indice] = listaDatos[aux1];
			dato1 = listaDatos[aux1].rd;
			dato2 = listaDatos[aux1].rs;
			dato3 = listaDatos[aux1].inmediate;

			num3 = atoi(dato3);

			if(strcmp(dato1,"$sp") == 0)
			{
				if(num3%4 == 0)
					num3 = num3/4;

				else
				{
					printf("No se puede ejecutar el programa, ya que addi presenta un error en uno de sus parametros con respecto al stack pointer\n");
					exit(0);
				}

			}
			

			for (int i = 0; i < 32; ++i)
				if (strcmp(dato2,REGISTROS_NOMBRES[i])==0)
					num2 = REGISTROS_VALOR[i];

			if((strcmp(funcion,"addi"))==0)
			{
				total = num2 + num3;
			}

			else if ((strcmp(funcion,"addiu"))==0)
			{
				double imm = atoi(dato3);
				totaliu = num2 + imm;
			}

			else
				total = num2 - num3;

			for (int j = 0; j < 32; ++j)
			{
				if (strcmp(dato1,REGISTROS_NOMBRES[j])==0 && strcmp(funcion,"addiu")==0)
					REGISTROS_VALOR[j] = totaliu;
				else if (strcmp(dato1,REGISTROS_NOMBRES[j])==0)
					REGISTROS_VALOR[j] = total;
			}
			if (total<inicioSP && strcmp(dato1,"$sp") == 0)
				inicioSP = total;
			if (total>finSP && strcmp(dato1,"$sp") == 0)
				finSP = total;

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
						ARREGLO_SP[REGISTROS_VALOR[29]+temporal]= REGISTROS_VALOR[k];
					
				
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
						REGISTROS_VALOR[k] = ARREGLO_SP[(temporal/4)+REGISTROS_VALOR[29]];
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
	printf("Inicio: %i\n",inicioSP);
	printf("Final: %i\n",REGISTROS_VALOR[29]);


	hazardDeteccion(indice);
}

/*
 * Esta funcion se encarga de tomar el arreglo post ejecución de la función anterior, es decir el listado completo
   de las instrucciones que el programa ejecutó (No se ocupa simplemente el arreglo que se genera al inicio del programa
   al leer los datos del archivo porque al ejecutar es distinto, hay, saltos, bifurcaciones,etc en donde muchas
   instrucciones tienden a repetirse)
 * @param totIns: Es un entero que almacena la cantidad final de instrucciones que se generaron  
 * @return No retorna nada porque mientras se ejecuta se escriben los archivos de salida 2 y 3, y en cuanto recorre todo
 		   genera el archivo con los valores de los registros
*/
void hazardDeteccion(int totIns)
{
	FILE *hazard,*solucion;
	char *arch1, *arch2, *arch3;
	int indice=0,instruccion;

	printf("\nIngrese el nombre de su primer archivo de salida que contendrá los registros y sus valores, este será guardado por defecto en formato .txt\n");
	arch1 = recibirNombreArchivoSalida(".txt");
	printf("\nIngrese el nombre de su segundo archivo de salida que contendrá los hazard detectados, este será guardado por defecto en formato .csv\n");
	arch2 = recibirNombreArchivoSalida(".csv");	
	printf("\nIngrese el nombre de su primer archivo de salida que contendrá la solucion a los hazard detectados, este será guardado por defecto en formato .txt\n");
	arch3 = recibirNombreArchivoSalida(".txt");	


	hazard = fopen(arch2,"wt");
	solucion = fopen(arch3,"wt");

	fprintf(hazard,"Hazard;Tipo;Instruccion;CC\n");
	fprintf(solucion,"Hazard|Solucion\n");

	CCTotal = 4;
	for (int i = 0; i < totIns-1; ++i)
	{
		CCTotal++;
		if (!pipeline[i].escribe && strcmp(pipeline[i].funcion,"sw")!=0)
		{
			instruccion = indiceInstruccion(pipeline[i].id);

			if(pipeline[i].salta == 1)
			{
				indice++;
				fprintf(hazard,"%i\t;Control;%i\t;%i\t\n",indice,instruccion,CCTotal-1);
				fprintf(solucion,"  %i   |Solucionable con: FLUSH/NOP en IF/ID, ID/EX y EX/MEM\n",indice);
				CCTotal+=3;

			}
			
			else if(pipeline[i].salta == 2)
			{
				indice++;
				fprintf(hazard,"%i\t;Control;%i\t;%i\t\n",indice,instruccion,CCTotal-3);
				fprintf(solucion,"  %i   |Solucionable con: FLUSH/NOP en IF/ID\n",indice);
				CCTotal+=1;
			}
		}

		if (i<totIns-2)
		{
			if (pipeline[i].escribe && (strcmp(pipeline[i].rd,pipeline[i+1].rs)==0 || strcmp(pipeline[i].rd,pipeline[i+1].rt)==0))
			{
				instruccion = indiceInstruccion(pipeline[i+1].id);
				if(strcmp(pipeline[i].funcion,"lw")==0)
				{
					indice++;
					fprintf(hazard,"%i\t;Datos;%i\t;%i\t\n",indice,instruccion,CCTotal-2);
					fprintf(solucion, "  %i   |Solucionable con: NOP y forwarding MEM/WB a ID/EX\n",indice);
					CCTotal+=1;

				}

				else
				{
					indice++;
					fprintf(hazard,"%i\t;Datos;%i\t;%i\t\n",indice,instruccion,CCTotal-2);
					fprintf(solucion,"  %i   |Solucionable con: Forwarding en EX/MEM a ID/EX\n",indice);
				}

			}

			if (pipeline[i].escribe && (strcmp(pipeline[i].rd,pipeline[i+2].rs)==0 || strcmp(pipeline[i].rd,pipeline[i+2].rt)==0))
			{
				instruccion = indiceInstruccion(pipeline[i+2].id);
				indice++;
				fprintf(hazard,"%i\t;Datos;%i\t;%i\t\n",indice,instruccion,CCTotal-2);
				fprintf(solucion,"  %i   |Solucionable con: Forwarding en EX/MEM a ID/EX\n",indice);
			}
		}
		else
		{
			if (pipeline[i].escribe && (strcmp(pipeline[i].rd,pipeline[i+1].rs)==0 || strcmp(pipeline[i].rd,pipeline[i+1].rt)==0))
			{
				instruccion = indiceInstruccion(pipeline[i+1].id);
				if(strcmp(pipeline[i].funcion,"lw")==0)
				{
					indice++;
					fprintf(hazard,"%i\t;Datos;%i\t;%i\t\n",indice,instruccion,CCTotal-2);
					fprintf(solucion, "  %i   |Solucionable con: NOP y forwarding MEM/WB a ID/EX\n",indice);
					CCTotal+=1;
				}

				else
				{
					indice++;
					fprintf(hazard,"%i\t;Datos;%i\t;%i\t\n",indice,instruccion,CCTotal-2);
					fprintf(solucion,"  %i   |Solucionable con: Forwarding en EX/MEM a ID/EX\n",indice);
				}
			}
		}	
			
	}
	fclose(hazard);
	fclose(solucion);
	//printf("%i \n",&REGISTROS_VALOR[31]);
	escribir_registros(arch1);
	free(arch1);
	free(arch2);
	free(arch3);
}

/*
 * Esta funcion se encarga de encontrar el número de una instrucción especifica a través de un id único que 
   posee cada instrucción, con número se refiere a por ejemplo esta es la instruccion número 5, según el orden en que
   están escritas archivo de entrada depende su número, para estas cuentas la etiquetas no cuentan
   @param id: El id único que se le asocia a una instrucción
 * @return Retorna un entero que corresponde a que número de instruccion corresponda la instrucción con el id especificado
*/
int indiceInstruccion(int id)
{
	int indice = 1;
	for (int i = 0; i < NINTRUCCIONES; ++i)
	{
		if(listaDatos[i].etiqueta == 1)
			indice--;
		if(listaDatos[i].id == id)
			break;
		indice++;
	}
	return indice;
}

/*
 * Esta funcion se encarga de escribir en un archivo de texto los 32 registros
   @param nombre: Es el nombre con que se llamará el archivo que se creará
 * @return No retorna ningun dato
*/
void escribir_registros(char *nombre) 
{ 
	int i; FILE *archivo;
	archivo = fopen(nombre,"wt");
	for (i = 0; i < 32; ++i)
		fprintf(archivo,"%s: %d\n",REGISTROS_NOMBRES[i],REGISTROS_VALOR[i]);

	fprintf(archivo,"\nContenido Stack Pointer: [ ");
	for (int i = inicioSP; i <= finSP; ++i)
	{
		fprintf(archivo,"%i ",ARREGLO_SP[i]);
	}
	fprintf(archivo,"]");
	
	fclose(archivo);
}

/*
 * Esta funcion se encarga de pedir por consola al usuario, en nombre para un archivo de salida x
   @param formato: Se refiere a la extensión que tendrá el archivo
 * @return retorna el nombre recibido por el usuario
*/
char* recibirNombreArchivoSalida(char* formato) 
{
	char* fileOut;
	bool validar;
	fileOut = (char*)malloc(sizeof(char)*30);
	do
	{
		validar = false;
		printf("\nIngrese el nombre para el archivo: ");
		scanf("%s",fileOut);	
		for (int i = 0; i < strlen(fileOut); ++i)
		{
			if (fileOut[i] == '\\' || fileOut[i] == '/' || fileOut[i] == ':' || fileOut[i] == '*' || fileOut[i] == '?' || fileOut[i] == '<' || fileOut[i] == '>' || fileOut[i] == '|')
				validar = true;
		}

		if (!validar) 
			validar = false;

		else
			printf("\nEl nombre escogido no debe poseer ninguno de los siguientes carácteres: \\/ : * ? < > | intente nuevamente\n");

		
	} while (validar);
	while(getchar()!='\n');
	strcat(fileOut,formato);
	return fileOut;

}

/*
 * Esta funcion se encarga de liberar de la memoria algunos elementos utilizados en el código
 * @return No retorna ningun dato
 */
void liberarMemoria()
{
	free(listaDatos);
	free(REGISTROS_VALOR);
	free(ARREGLO_SP);
	free(NOMBRE_ARCHIVO_1);
}