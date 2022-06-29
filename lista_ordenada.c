#include "lista_ordenada.h"
#include "constantes.h"

int (*sortf)(void *, void *);

TListaOrdenada crear_lista_ordenada(int (*f)(void *, void *)) {
    TLista l = crear_lista();
    TListaOrdenada lo = (TListaOrdenada) malloc(sizeof(struct lista_ordenada));
    lo->cantidad_elementos = 0;
    lo->lista = l;
    sortf = f;
    return lo;
}

int lo_insertar(TListaOrdenada lista, TElemento elem) {
    int retorno = FALSE;
    if ( lista == NULL ) exit(LST_NO_INI);
    else {
        TLista l = lista->lista;
        TPosicion movil;
        TPosicion sig;
        if ( (lista->cantidad_elementos) > 0 ){
            movil = l_primera(l);
            int encontre = FALSE;
            int orden = 0;
            int listaRecorridaTotalmente = FALSE;
			//	Recorrer la lista hasta encontrar el elemento que sea mayor a elem.
            while( encontre == FALSE ){
                sig = l_siguiente(l,movil);
                orden = sortf(elem,movil->elemento);
                if ( orden == 1 && sig != POS_NULA) {
					//	Hay que seguir recorriendo la lista.
                    movil = sig;
				}
                else {
                    if ( sig == POS_NULA && orden == 1 ) {
						//	Se recorrió toda lista pero todavía no se encontro la posición correcta (hay que insertarlo al final).
						listaRecorridaTotalmente = TRUE;
					}
                    encontre = TRUE;
                }
            }
			//	Con l_insertar, se inserta el nuevo elemento en la posición correcta (movil).
            retorno = l_insertar(l, movil, elem);
            if ( retorno == TRUE && encontre == TRUE && listaRecorridaTotalmente == TRUE ) {
				//	Si el elemento hay que insertarlo al final, entonces hay que intercambiarlo con el actual último de la lista:
				//	se insertó en la ante-última, se elimina el último y se lo inserta nuevamente en la posición anterior al nuevo.
                TElemento e = l_recuperar(l,movil);
                retorno = l_eliminar(l,movil);
                if ( retorno == TRUE )
                    retorno = l_insertar(l,l_ultima(l),e);
            }
        }
        else {
			// Hay que insertarlo en la primera posición.
			retorno = l_insertar(l,POS_NULA,elem);
		}
    }
    if ( retorno == TRUE ) lista->cantidad_elementos++;
    return retorno;
}

int lo_eliminar(TListaOrdenada lista, TPosicion pos) {
    int resultado = FALSE;
    if ( lista == NULL ) exit(LST_NO_INI);
    else {
        if (lista->cantidad_elementos!=0)
            resultado = l_eliminar(lista->lista,pos);
        if ( resultado == TRUE ) lista->cantidad_elementos--;
    }
    return resultado;
}

int lo_size(TListaOrdenada lista) {
    int cantidad = 0;
    if (lista == NULL) exit(LST_NO_INI);
    else cantidad = lista->cantidad_elementos;
    return cantidad;
}

TPosicion lo_primera(TListaOrdenada lista) {
    TPosicion primera;
    if ( lista == NULL ) exit(LST_NO_INI);
    else primera = l_primera(lista->lista);
    return primera;
}

TPosicion lo_ultima(TListaOrdenada lista) {
    TPosicion ultima;
    if (lista == NULL) exit(LST_NO_INI);
    else  ultima = l_ultima(lista->lista);
    return ultima;
}

TPosicion lo_siguiente(TListaOrdenada lista, TPosicion pos){
    TPosicion sig;
    if (lista == NULL) exit(LST_NO_INI);
    else sig = l_siguiente(lista->lista,pos);
    return sig;
}
