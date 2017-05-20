#include <stdio.h> //standard input/output
#include <pthread.h> //para usar threads
#include <unistd.h> 	// para hacer sleep
#include <stdlib.h> 	// para libreria de numeros random: srand, rand
#include <time.h>   	// para tomar el tiempo


int variableCompartidaSuma = 0;

int elementosPitagoricos[4] = {1, 2, 3, 4};

#define NUM_THREADS 4

pthread_mutex_t mi_mutex;

void* sumarValor (void* parametro){

   //tomar los datos del thread, imprimirlos, hacer la suma y terminar el thread

   int posArray;

   posArray = (intptr_t) parametro;

   printf("Sumando posicion = %d \n", posArray);

   pthread_mutex_lock(&mi_mutex);

   int aux = variableCompartidaSuma;

   /********************************
   * simular un tiempo para que ocurra una condición de carrera
   *********************************/

   //inicializar la semilla del generador random:

   int microseconds; srand (time(NULL));

   //generar un numer random entre 1 y 1000:

   microseconds = rand() % 1000 + 1;

   //dormir el thread, simula que esta haciendo alguna tarea

   usleep(microseconds);

   /****************************
   * fin simular un tiempo
   ******************************/

   aux = aux + elementosPitagoricos[posArray];

   variableCompartidaSuma=aux;

   pthread_mutex_unlock(&mi_mutex);

   pthread_exit(NULL);

}

int main (){

   pthread_t threads[NUM_THREADS]; //una variable de tipo pthread_t sirve para identificar cada hilo que se cree

   //la variable threads es una array de pthread_t

   //comparar con char data[100], un array de char

   pthread_mutex_init ( &mi_mutex, NULL);

   int rc;

   int i;

   for( i=0; i < NUM_THREADS; i++ ){
  	printf("main() : creando thread %d \n", i);
  	rc = pthread_create(&threads[i],	//identificador unico
                      	NULL,    	//atributos del thread
                      	sumarValor,	//funcion a ejecutar
                      	(void *)(intptr_t) i);	//parametros de la funcion a ejecutar, pasado por referencia
    if (rc){
     	printf("Error:unable to create thread, %d \n", rc);

     	exit(-1);
  	}

   }
   //join the threads

	for(i = 0 ; i < NUM_THREADS ; i++){
      pthread_join(threads[i] , NULL);
  }

	printf("El numero de la perfeccion es : %d \n", variableCompartidaSuma);

  pthread_mutex_destroy( &mi_mutex);
	pthread_exit(NULL);
}

//Para compilar:   gcc seccionCritica.c -o seccionCritica -lpthread
//Para ejecutar:   ./seccionCritica
