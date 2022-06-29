#include "lista.h"

//  Definición del TDA Lista Ordenada:

// Modela una lista ordenada.
typedef struct lista_ordenada{
    unsigned int cantidad_elementos;
    TLista lista;
} * TListaOrdenada;

//  Declaración de funciones del TDA Lista Ordenada:

//  Crea y retorna la lista ordenada vacía. El orden de los elementos estará dado
//  por la función de comparación f, pasada por parámetro. La función f devuelve -1
//  si el orden del primer argumento es menor que el orden del segundo, 0 si es el
//  mismo, y 1 en caso contrario.
TListaOrdenada crear_lista_ordenada(int (*f)(void *, void *));

//  Agrega el elemento en la posición correspondiente de la lista, de modo que quede
//  ordenada de forma ascendente. Retorna verdadero si procede con éxito y falso en caso
//  contrario.
int lo_insertar(TListaOrdenada lista, TElemento elem);

//  Elimina el elemento en la posición pos y reacomoda la lista adecuadamente. Retorna
//  verdadero si procede con éxito y falso en caso contrario.
int lo_eliminar(TListaOrdenada, TPosicion pos);

//  Retorna la cantidad de elementos de la lista.
int lo_size(TListaOrdenada lista);

//  Retorna la primer posición de la lista.
TPosicion lo_primera(TListaOrdenada lista);

//  Retorna la última posición de la lista.
TPosicion lo_ultima(TListaOrdenada lista);

// Retorna la posicion siguiente a pos en la lista.
TPosicion lo_siguiente(TListaOrdenada lista, TPosicion pos);

//  En los casos anteriores, excepto crear_lista_ordenada(), si la lista no está inicializada
//  se aborta con LST_NO_INI. Además, estas funciones trabajan con posiciones válidas, es decir
//  que pertenecen a la lista.
