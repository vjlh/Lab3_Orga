#Este es el makefile para compilar el interprete de mips
ARCHIVOS = principal.o funciones.o
CFLAGS = -g -Wall

all: programa imprimir

programa: $(ARCHIVOS)
		gcc -o programa $(ARCHIVOS)

%.o: %.c
		gcc $(CFLAGS) -c $< -o $@

imprimir:
		@echo "\nEl makefile se ha realizado con exito..."			
clean:
	rm -f programa *.o