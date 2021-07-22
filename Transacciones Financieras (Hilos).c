#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
int i;   //CONTADOR DE TRANSACCIONES
struct timeval  t2, t3; //VARIABLES PARA MEDIR TIEMPO TOTAL
struct timespec t0, t1; //VARIABLES PARA MEDIR TIEMPO INDIVIDUAL
//VARIABLES PARA EL TIEMPO TOTAL
float tiempo_individual;
float tiempo_total;

int cuenta_corriente = 0; //Varibale Global,cuenta inicializada en cero
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER; // controla el acceso a la cuenta corriente

//Hilo2 para crear el deposito de la cuenta deudora 
void *deudor(void *arg){
	//INICIO DE MEDICION DE TIEMPO
	timespec_get(& t1, TIME_UTC); 
	unsigned int ut0 = t0.tv_nsec;

	//Creación/Ejecución del hilo1 para medir el tiempo
	pthread_mutex_lock(&m);
	cuenta_corriente = cuenta_corriente +100;
	pthread_mutex_unlock(&m);
	//FIN DE MEDIDICON DE TIEMPO
	unsigned ut1 = t1.tv_nsec;
	tiempo_individual = (ut1-ut0);

	printf("Número de transación: %d\n",i+1);
	printf("La cuenta deudora no: %d ha depositado 100 dolares\n", rand()%1000+2000);
	printf("La cuenta corriente tiene un total de: %d dolares\n", cuenta_corriente);
	printf("El tiempo de ejecución de la transacción fue de %.1f nanosegundos\n", tiempo_individual);
	printf("\n");
}

//Hilo3 para crear el debito de acreedores 
void *acreedor (void *arg){
	//INICIO DE MEDICION DE TIEMPO
	timespec_get(& t1, TIME_UTC); 
	unsigned int ut0 = t0.tv_nsec;
	
	//contabilización de la cuenta corriente por debitos de acreedores
	pthread_mutex_lock(&m);
	cuenta_corriente = cuenta_corriente -50;
	pthread_mutex_unlock(&m);
	//FIN DE MEDICION DE TIEMPO
	unsigned ut1 = t1.tv_nsec;
	tiempo_individual = (ut1-ut0);
	printf("Numero de transación: %d\n",i+1);
	printf("La cuenta acreedora no: %d ha debitado 50 dolares\n", rand()%2000+3000);
	printf("La cuenta corriente tiene un total de: %d dolares\n", cuenta_corriente);	
	printf("El tiempo de ejecución de la transacción fue de %.1f nanosegundos\n", tiempo_individual);
	printf("\n");
}

int main(){	
	int numero;		
	int j = 0;	
	int k = 0;
	printf("---------------***BANCO CENTRAL***---------------\n");
	printf(">>>Estado de transacciones\n");	
	//INICIO DE MEDICION DE TIEMPO TOTAL
	gettimeofday(& t2, NULL);
	unsigned int ut2 = t2.tv_usec;

	//Creación/Ejecución de hilos(deudor, acreedor)	
	pthread_t	h1, h2;
	pthread_attr_t atrib;
	pthread_attr_init(&atrib);	
	for ( i = 0 ; i < 1000 ; i++){	
	
		numero = rand()%2+1;	

		if (numero == 2 && j < 500 ){	
				pthread_create(&h1, &atrib, deudor, NULL);
				pthread_join(h1, NULL);
				j++;

		}else {
			if (k < 500){
			pthread_create(&h2, &atrib, acreedor, NULL);
			pthread_join(h2, NULL);
			k++;
			}
		}
	}
	//Tiempo total del estado de las transacciones	
	printf("El valor final de la cuenta corriente es: $%d\n ", cuenta_corriente);
	gettimeofday(& t3, NULL);
	unsigned int ut3 = t3.tv_usec;
	tiempo_total = ut3 - ut2;
	printf("El tiempo TOTAL del estado de las transacciones fue de %.1f microsegundos\n", tiempo_total);
}

