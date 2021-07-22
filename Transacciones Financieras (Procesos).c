#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/time.h>
//NUMERO DE TRANSACCION ACTUAL
int i;

//Definicion de 3 valores constantes que se usaran con frecuencia a lo largo del programa.
#define NUM_PROC 1000
#define DEPOSITO 100
#define RETIRO 50

//Declaraciones necesarias para poder medir el tiempo al inicio y fin de la ejecucion. 
struct timeval t0, t1, t2, t3;
//Declaracion de metodos y variables a ser usadas de manera global.
void clientesDeudores(int numero);
void clientesAcreedores(int numero);
int cuenta_corriente = 0;//Variable que inicializa la cuenta en cero
float tiempo_individual, tiempo_total;;

/*Metodo principal donde se iran creando procesos hijos que estaran divididos a traves de un ciclo for 
como pares e impares que a su vez, ejecutaran las clases de acreedores y deudores respectivamente*/
int main()
{
	printf("------------------***BANCO CENTRAL***------------------\n");
	printf(">>> Estado de transacciones\n");
	printf("\n");
	int numero, pid;
	int j = 0; //CONTADOR DE TRANSACCIONES DE ACREEDORES
	int k = 0; //CONTADOR DE TRANSACCIONES DE DEUDORES		
	//INICIO DE CONTABILIZACION DE TIEMPO TOTAL 
	gettimeofday(& t2, NULL); 
	unsigned int ut2 = t2.tv_sec;

	//INICIO DE LAS TRANSACCIONES
	for( i=0; i<=NUM_PROC; i++)
	{
		if(cuenta_corriente == 0){
			numero = 2;
		}else{
			numero = rand()%2+1;
		}
		if(numero == 1 && j < 500){
			pid = fork();
				switch(pid)
				{
					case -1:
							printf(stderr, "Error al crear el proceso.\n");
							break;
					case 0:		
							//Inicializacion para contabilizar el tiempo de ejecucion por cada proceso
							gettimeofday(& t0, NULL);							      
							clientesAcreedores(numero);
							j++;
							break;
							
					default:
							wait(0);
							exit(0);
				}
		}else
		{
			if(k<500){
			pid = fork();
				switch(pid)
				{
					case -1:
							printf(stderr, "Error al crear el proceso.\n");						
							break;
					case 0:
							//Inicializacion para contabilizar el tiempo de ejecucion por cada proceso
							gettimeofday(& t0, NULL);							
							clientesDeudores(numero);
							k++;
							break;
					
					default:
							wait(0);
							exit(0);
				}
			}
		}
	}
	printf("El saldo total de la cuenta es de: $%d\n",cuenta_corriente);
	//Obtencion del tiempo total de ejecucion de todos los procesos
	gettimeofday(& t3, NULL);
	unsigned int ut3 = t3.tv_sec;
	tiempo_total = ut3 - ut2;
	printf("EL tiempo total de la ejecucion es: %f segundos\n", tiempo_total);
}

//Metodo que ejecutaran los procesos hijo impares
//Crear un deposito por parte de una cuenta deudora
void clientesDeudores(int numero)
{
	//Contabilizacion de la cuenta corriente por depositos de deudores
	cuenta_corriente = cuenta_corriente + DEPOSITO;
	printf("Número de transacción: %d\n", i+1);
	printf("La cuenta deudora No: %d ha depositado 100 dolares.\n", rand()%1000+2000);
	printf("La cuenta corriente tiene un total de: %d dolares.\n", cuenta_corriente);
	//Obtencion del tiempo total de ejecucion de cada proceso
	gettimeofday(& t1, NULL);
	unsigned int ut0 = t0.tv_usec;
	unsigned int ut1 = t1.tv_usec;
	tiempo_individual = (ut1-ut0);
	printf("EL tiempo de ejecucion de la transaccion fue de %f microsegundos.\n", tiempo_individual);
	printf("\n");
}

//Metodo que ejecutaran los procesos hijo pares
//Crear un debito por parte de una cuenta acreedora
void clientesAcreedores(int numero)
{
	//Contabilizacion de la cuenta corriente por debitos de acreedores		
	printf("Número de transacción: %d\n", i+1);
	cuenta_corriente = cuenta_corriente - RETIRO;
	printf("La cuenta acreedora No: %d ha retirado 50 dolares.\n", rand()%1000+2000);
	printf("La cuenta corriente tiene un total de: %d dolares\n", cuenta_corriente);
	//Obtencion del tiempo total de ejecucion de cada proceso
	gettimeofday(& t1, NULL);
	unsigned int ut0 = t0.tv_usec;
	unsigned int ut1 = t1.tv_usec;
	tiempo_individual = (ut1-ut0);
	printf("EL tiempo de ejecucion de la transaccion fue de %f microsegundos.\n", tiempo_individual);
	printf("\n");
}
