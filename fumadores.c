/*
Universidad LAMAR
Ingenieria en Computacion
Sistemas Operativos Avanzados
Examen: Problema de los 3 fumadores
Maria Ines Calderon Zetter, 7o sem.
*/


/*
El problema de los 3 fumadores dice:
"Hay tres fumadores empedernidos y un agente que, de tiempo en tiempo, consigue ciertos insumos. Los ingredientes necesarios para fumar son tabaco, papel y cerillos. Cada uno de los fumadores tiene una cantidad infinita de alguno de los ingredientes, pero no les gusta compartir. Afortunadamente, del mismo modo que no comparten, no son acaparadores.
De tiempo en tiempo, el agente consigue una dosis de dos de los ingredientes — Por ejemplo, si deja en la mesa un papel y tabaco, el que trae los cerillos educadamente tomará los ingredientes, se hara un cigarro, y lo fumara."


El problema de los 3 fumadores fue inventado en 1971 por Suhas Patil
quien buscaba demostrar que hay problemas que no se pueden resolver
por medio de semaforos. El problema original de Patil planteaba que
no se podia utilizar semaforos ni ciclos de control en el flujo del 
programa, haciendo efectivamente irresoluble su planteamiento.
*/



#include <stdio.h>
#include <pthread.h>

/*
Para este problema necesitamos tres procesos fumadores,
tres elementos en un arreglo que representan los cerillos,
el papel y el tabaco, y finalmente un agente-candado que les libere.
El manejo de punto critico del programa se realiza por medio 
de candados mutex, en este caso contamos con un candado global
que es el agente, y tres candados en representacion de los elementos
que se van a bloquear o desbloquear para que los hilos les utilicen.
*/ 

pthread_t fumador[3];
pthread_mutex_t agente[1], elementos[3];

//Creamos la tarea que realizara el proceso fumador
void *procfumador(void *num)
{
//Ciclo for repetitivo libre de parametros
  for(;;) 
  {

//Esperamos a recibir el elemento (distinto al que tengo)
    if ((int)num!=2)
    pthread_mutex_lock(elementos+(int)num+1);

    else
//Si no se bloquean los elementos presentes
    pthread_mutex_lock(elementos);

//Y mientras yo fumo nadie mas lo puede hacer (tengo los 3 elementos)
    pthread_mutex_lock(agente);

    printf("Soy el hilo %d y estoy fumando. ", (int)num);
    fflush(stdout); //Para mostrar el flujo de datos 
    sleep(1);	    //Incluyendo el sleep
    printf("Ya termine.\n");

//Liberamos el agente para continuar pasandolo entre los fumadores
    pthread_mutex_unlock(agente);

//Se liberan los elementos
    pthread_mutex_unlock(elementos+(int)num);

  }

}



int main(int c, char *v[])
{
int i;

//Inicializacion de candados mutex
  for(i=0;i<3;i++)
    pthread_mutex_init(elementos+i, NULL);
    pthread_mutex_init(agente, NULL);

//Bloqueo los elementos disponibles
  for(i=0;i<3;i++)
    pthread_mutex_lock(elementos+i);

//Creacion de los tres hilos fumadores
  for(i=0;i<3;i++)
    pthread_create(fumador+i, NULL, procfumador, (int *)i);

//Se libera uno de los tres elementos con modulo
    pthread_mutex_unlock(elementos + random()%3);

//Esperamos
    sleep(40);

//Finalmente terminamos los procesos
  for(i=0;i<3;i++)
    pthread_mutex_destroy(elementos+i);
}
