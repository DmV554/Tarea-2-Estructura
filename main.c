#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> 
#include "Map.h"
#include "list.h"
#include "stack.h"

typedef struct{
    char nombre[51];
    int puntosHabilidad;
    List* inventario;
    Stack*pilaAcciones;
}Jugador;

typedef struct{
  char nombre[51];
  List* listaJugadoresConItem;
}ItemMapa;

typedef struct {
  char nombre[51];
} Item;

typedef struct {
  char nombreAccion[51];
  char item[51];
  int puntajeADeshacer;
}Accion;

void crearPerfil(Map *, Jugador*);

/*
  función para comparar claves de tipo string
  retorna 1 si son iguales
*/
int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

/*
  función para comparar claves de tipo string
  retorna 1 si son key1<key2
*/
int lower_than_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2) < 0) return 1;
    return 0;
}

/*
  función para comparar claves de tipo int
  retorna 1 si son iguales
*/
int is_equal_int(void * key1, void * key2) {
    if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

/*
  función para comparar claves de tipo int
  retorna 1 si son key1<key2
*/
int lower_than_int(void * key1, void * key2) {
    if(*(int*)key1 < *(int*)key2) return 1;
    return 0;
}


int main() {
  char opcion[11];
  bool ejecucion = true;

   Map *mapaJugadores = createMap(is_equal_string);
   Jugador *jugador;

  while (ejecucion) {
    printf("\n1.-CREAR PERFIL JUGADOR\n");
    printf("2.-MOSTRAR PERFIL JUGADOR\n");
    printf("3.-AGREGAR ITEM A JUGADOR\n");
    printf("4.-ELIMINAR ITEM DE JUGADOR\n");
    printf("5.-AGREGAR PUNTOS DE HABILIDAD A JUGADOR\n");
    printf("6.-MOSTRAR JUGADORES CON ITEM ESPECÍFICO\n");
    printf("7.-DESHACER ULTIMA ACCION DE UN JUGADOR\n");
    printf("8.-EXPORTAR DATOS DE JUGADORES A ARCHIVO DE TEXTO\n");
    printf("9.-CARGAR DATOS DE JUGADORES DESDE UN ARCHIVO DE TEXTO\n");
    printf("PRESIONE 0 PARA SALIR\n\n");

    scanf("%10s[^\n]", opcion);
    getchar();

    //Se comprueba que la opcion ingresada sea un número
    if (isdigit(opcion[0])) {
      int opcionNumero = atoi(opcion);

      switch (opcionNumero) {
      case 1:
        jugador = (Jugador*)malloc(sizeof(Jugador));
        crearPerfil(mapaJugadores, jugador);
        
        break;
        
      case 2:
        
        break;

      case 3:  
        
        break;

      case 4:
        
        break;

      case 5:
     
        break;

      case 6:

        break;

      case 7:
        
        break;

      case 8:

        break;

      case 9:
    
        break;

      case 0:
        printf("QUE TENGA BUEN DÍA, ADIÓS\n");
        ejecucion = false;
        break;

      default:
        printf("\nSELECCIONE UNA OPCIÓN VÁLIDA\n\n");
        break;
      }
    } else {
      if (isalpha(opcion[0]))
        printf("\nSELECCIONE UNA OPCIÓN VÁLIDA\n\n");
    }
  }
    return 0;
}

void crearPerfil(Map *mapaJugadores, Jugador* jugador) {
  printf("INGRESE NOMBRE JUGADOR A CREAR: ");
  scanf("%50[^\n]s", jugador->nombre);
  getchar();

 Jugador* jugadorBuscado = searchMap(mapaJugadores, jugador->nombre);
  if(jugadorBuscado != NULL) {
    printf("\nEl jugador %s ya existe\n\n", jugador->nombre);
    return;
  }
  

  jugador->puntosHabilidad = 0;
  
  jugador->inventario = createList();
  jugador->pilaAcciones = stack_create();
  
  insertMap(mapaJugadores, jugador->nombre, jugador);

  printf("\nJUGADOR CREADO CON ÉXITO\n");
}