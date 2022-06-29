#include "trie.h"
#include "constantes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define max 30

//  Retorna TRUE si el caracter c es una letra 'a', ..., 'z'
int caracterValido(char c) {
    int valido = FALSE;
    if ( 'a' <= c && c <= 'z' )
            valido = TRUE;
    return valido;
}

//  Setea los elementos del string en '\0'.
void limpiarString(char string[max]) {
    int i=0, j=strlen(string);
    while(i<j) string[i++]='\0';
}

//  Lee un string del archivo f. Si el primero es válido entonces lo inserta en el trie.
//  La longitud máxima de una palabra válida es de 30 caracteres.
void procesarString(FILE * f, TTrie trie){
    char c, string[max];
    int i=0, seguir = TRUE;
    limpiarString(string);
    while ( !feof(f) && seguir == TRUE ){
		//	Hasta que lea caracteres válidos, los inserta en el string, que forma una palabra válida.
        c = fgetc(f);
        seguir = caracterValido(c);
        if ( seguir == TRUE ){
            string[i] = c;
            i++;
        }
    }
    if (i!=0) {
        int j=strlen(string);
        if(i<j) string[i]='\0';
        tr_insertar(trie,string);
    }
}

// Llena el trie con las palabras de f.
void llenarTrie(TTrie tr,FILE * f) {
    while ( !feof(f) )
        procesarString(f,tr);
}

//  Busca en la lista ordenada hijos el nodo cuyo rotulo es c.
//  Retorna la posicion de este, si es que lo encuentra, o POS_NULA
//  en caso contrario.
TPosicion estaEnHijos(TListaOrdenada hijos, char c) {
    int elementos = hijos->cantidad_elementos;
    int encontre = FALSE;
    TPosicion retorno = POS_NULA;
    TPosicion movil;
    if ( elementos ) {
        TNodo aux;
        movil = lo_primera(hijos);
        while( encontre == FALSE && movil != POS_NULA ){
			//	Recorre la lista hasta encontrar el nodo correspondiente al caracter pasado por parámetro.
            retorno = movil;
            aux = movil->elemento;
            if ( aux->rotulo == c ) encontre = TRUE;
            movil = lo_siguiente(hijos,movil);
        }
        if ( encontre == FALSE ) retorno = POS_NULA;
    }
    return retorno;
}

//  Busca el nodo del trie con raiz 'raiz' que tenga como rótulo a la última letra de str.
//  Si no lo encuentra, retorna POS_NULA.
TNodo buscarUltimaLetra(TNodo raiz, char * str) {
    TNodo movil;
    int longitud = strlen(str);
    if ( longitud == 0 ) movil = NULL;
    else {
        TPosicion pos;
        movil = raiz;
        TListaOrdenada lo = movil->hijos;
        int seguir = TRUE, indice = 0;
        while ( seguir == TRUE && indice < longitud && lo_size(lo) ) {
			// Recorre la palabra str en el árbol hasta encontrar el último caracter o hasta que se haya terminado el árbol
            pos = estaEnHijos(lo,*(str+indice));
            if (pos != POS_NULA){
                indice++;
                movil = pos->elemento;
                lo = movil->hijos;
            }
            else seguir = FALSE;
        }
		//	Si no se recorrió toda la palabra o la palabra no pertenece al trie.
        if ( seguir == FALSE || indice < longitud ) movil = POS_NULA;
    }
    return movil;
}

//  Recorre el subárbol con raíz n y cuenta la cantidad de palabras que tiene, almacenada en cant.
void recorrer(TNodo n, int * cant) {
    TListaOrdenada lo = n->hijos;
    TPosicion pmovil = lo_primera(lo);
    TNodo nodo;
    while ( pmovil != POS_NULA ) {
        //	Mientras no se hayan visitado todos los hijos de n, se recorre recursivamente cada uno de ellos.
		//	Si el nodo visitado tiene contador distinto de cero, entonces consituye un fin de palabra y se aumenta el contador cant.
        nodo = pmovil->elemento;
        if ( nodo->contador ) (*cant)++;
        recorrer(nodo,cant);
        pmovil = lo_siguiente(lo,pmovil);
    }
}

//  Función que implementa a 2).
int consultar(TTrie tr, char * palabra){
    // si la palabra no pertence al trie tr, retorna -1
    int n = tr_recuperar(tr,palabra);
    if ( n < 0 ) n = 0;
    return n;
}

//  Función que implementa 3)
int comienza_con(TNodo n, char letra) {
    TPosicion hijo = estaEnHijos(n->hijos,letra);
    int cantidad = 0;
    if ( hijo != POS_NULA ) {
        TNodo m = hijo->elemento;
        if ( m->contador ) cantidad = 1;
        recorrer(hijo->elemento,&cantidad);
    }
    return cantidad;
}

//  Función que implementa 5).
float porcentaje_prefijo(TTrie tr, char * palabra) {
    TNodo n = buscarUltimaLetra(tr->raiz,palabra);
    float porcentaje = 0;
    if ( n != POS_NULA && lo_size(n->hijos) ) {
        //  El porcentaje es la cantidad de palabras que empiezan con
        //  la última letra del prefijo, dividido la cantidad total de
        //  palabras en el trie, múltiplicado por cien.
        int hay_palabras = 0;
        recorrer(n,&hay_palabras);
        if (  hay_palabras != 0 )
            porcentaje = 100*((float)hay_palabras/tr_size(tr));
    }
    return porcentaje;
}

//  Función que implementa 4). Una palabra no es prefijo de si misma.
int es_prefijo(TTrie tr, char * palabra) {
    int retorno=FALSE;
    TNodo n = buscarUltimaLetra(tr->raiz,palabra);
    if ( n!=POS_NULA && n->hijos->cantidad_elementos )
        retorno = TRUE;
    return retorno;
}

//  Imprime las palabras del trie con raíz n.
void imprimirPalabras(int nivel, TNodo n, char * palabra){
	//	Recorre el trie en "preorden" e inserta los rótulos en el string en la posición dictada por nivel.
	TListaOrdenada hijos = n->hijos;
	TPosicion p = lo_primera(hijos);
	if ( nivel != 0 ) {
		// La raíz no tiene rótulo.
		palabra [nivel-1] = n->rotulo;
	}
	while(p!=POS_NULA) {
	//	Llama recursivamente con el cada uno de los hijos de n.
		imprimirPalabras(nivel+1,p->elemento,palabra);
		p=lo_siguiente(hijos,p);
	}
	//	Vuelve de la recursión e imprime la palabra completa y elimina el último caracter (rótulo de n).
	if(n->contador) printf("· %s, %i \n", palabra, n->contador);
	if ( nivel != 0 ) palabra [nivel-1] = '\0';
}

//  Función que implementa 1).
void mostrar_palabras(TTrie tr){
    char palabra[max];
    limpiarString(palabra);
    imprimirPalabras(0,tr->raiz,palabra);
}

int main(int n, char ** archivo) {
    printf("\n");
    char modo[2] = "r";
    if ( !n ) printf("Error al procesar el archivo. Ejecución finalizada.\n");
    else {
        FILE* f = fopen(*(archivo+1),modo);
        if ( f == NULL ) printf("Error al procesar el archivo. Ejecución finalizada.\n");
        else {
            int op, seguir = TRUE;
            TTrie tr = crear_trie();
            llenarTrie(tr,f);
            fclose(f);
            char string[max], c;
            printf("Las operaciones son las siguientes:\n");
            printf("1) Mostrar palabras: permite visualizar el listado de todas las palabras junto con la cantidad de apariciones de la misma.\n");
            printf("2) Consultar: permite determinar si una palabra ingresada pertenece o no al archivo, y en consecuencia, cuántas veces se repite en el archivo.\n");
            printf("3) Comienzan con: permite consultar cuántas palabras comienzan con una letra dada.\n");
            printf("4) Es prefijo: permite consultar si una palabra ingresada es prefijo de otras almacenadas en el Trie.\n");
            printf("5) Porcentaje prefijo: dado un prefijo, indica el porcentaje de palabras del trie que comienzan con él.\n");
            printf("6) Salir: permite salir del programa, liberando toda la memoria utilizada por el Trie.\n");
            printf("Ingrese un número correspondiente a una de las operaciones previamente mencionadas: \n");
            scanf("%i",&op);
            while (seguir==TRUE){
                if ( op == 1 ) mostrar_palabras(tr);
                if ( op == 2 ) {
                    limpiarString(string);
                    printf("Ingrese una palabra: ");
                    scanf("%s",string);
                    int cons = consultar(tr,string);
                    if ( cons ) printf("La palabra %s está en el archivo %i vez/veces.\n",string,cons);
                    else printf("La palabra %s no está en el archivo.\n",string);
                }
                if ( op == 3 ) {
                    printf("Ingrese una letra:\n");
                    scanf("\n%c",&c);
                    int cant = comienza_con(tr->raiz,c);
                    if (cant) printf("Hay %i palabra/s distinta/s en el archivo que empiezan con '%c'.\n", cant,c);
                    else printf("No hay palabras que empiecen con '%c'. \n",c);
                }
                if ( op == 4 ){
                    limpiarString(string);
                    printf("Ingrese una palabra: ");
                    scanf("%s",string);
                    int hay_palabra = es_prefijo(tr,string);
                    if ( hay_palabra == TRUE ) printf("Hay por lo menos una palabra en el archivo que empiezan con ese prefijo.\n");
                    else printf("No hay palabras que empiecen con ese prefijo.\n");
                }
                if ( op == 5 ){
                    limpiarString(string);
                    printf("Ingrese una palabra: ");
                    scanf("%s",string);
                    printf("El %f de palabras del archivo empiezan con ese prefijo.\n", porcentaje_prefijo(tr,string));
                }
                if ( op == 6 ) seguir = FALSE;
                //	Si el usuario ingresa un número de operación inválido o si ingresa uno válido distinto de 6, se puede volver ingresar otro.
                // 	Si ingresa el nro de operación 6, finaliza el programa.
                if ( op < 1 || 6 < op ) printf("Número de operación incorrecta.\n");
                if ( op != 6) {
                    printf("Ingrese el número de una nueva operación, de las mencionadas previamente: ");
                    scanf("%i",&op);
                }
            }
        }
    }
    printf("Fin del programa.");
    return 0;
}
