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
void mostrarPerfilJugador(Map *);
void mostrarInventario(List *);
void agregarItemJugador(Map*, Map*);
Jugador* existeJugador(Map*);
void insertarAccion(Stack*, char*, char*);
void eliminarItemJugador(Map *, Map*);
Item *buscarItem(List *, char *);
void agregarPuntosJugador(Map*);
void insertarAccionPuntaje(Stack*, char*, int);
void mostrarJugadoresItemEspecifico(Map*);
bool existeJugadorLista(List *, char *);
void eliminarJugadorListaItemMapa(Jugador*,char*, Map*);
void eliminarJugadorLista(List *, char *);
void insertarItemMapa(Jugador*, char*, Map*, ItemMapa*);
void deshacerUltimaAccion(Map*);
void exportarDatosJugadores(Map*);
void importarDatosJugadores(Map*, Map*);
void insertarItemMapaArchivo(Map*, char*, Jugador*);

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
  Map *mapaItems = createMap(is_equal_string);

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
        mostrarPerfilJugador(mapaJugadores);
        break;

      case 3:  
        agregarItemJugador(mapaJugadores, mapaItems);
        break;

      case 4:
        eliminarItemJugador(mapaJugadores, mapaItems);
        break;

      case 5:
        agregarPuntosJugador(mapaJugadores);
        break;

      case 6:
        mostrarJugadoresItemEspecifico(mapaItems);
        break;

      case 7:
        deshacerUltimaAccion(mapaJugadores);
        break;

      case 8:
        exportarDatosJugadores(mapaJugadores);
        break;

      case 9:
        importarDatosJugadores(mapaJugadores, mapaItems);
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


/*Para crear el perfil de jugador se verifica de que no exista en el mapa. 
Luego se inicializa la variable puntos de habilidad en 0, se crea la lista del inventario y 
la pila de acciones. Luego se agrega el jugador al mapa de jugadores*/
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

/*Primero se comprueba si existe el jugador dentro del mapa de jugadores. Se muestra el 
nombre y los puntos de habilidad y luego se muestra el inventario a través de la función mostrar inventario*/
void mostrarPerfilJugador(Map *mapaJugadores) {
  Jugador* jugadorBuscado = existeJugador(mapaJugadores);

  if(jugadorBuscado == NULL) return;
  
  printf("\nNombre Jugador: %s\n", jugadorBuscado->nombre);
  printf("Puntos Jugador: %d\n", jugadorBuscado->puntosHabilidad);
  
  mostrarInventario(jugadorBuscado->inventario);
  printf("\n\n");
}

/*Se crea un puntero al princio de la lista del inventario para comenzar la busqueda. 
Si es null significa que la lista está vacía. Si no, muestra la lista de todos los items 
recorriendo la lista.*/
void mostrarInventario(List *inventario){
  Item *itemNodo = firstList(inventario);

  if(itemNodo == NULL ) {
    printf("NO HAY ITEMS INGRESADOS\n");
  } else {
    printf("LISTA DE TODOS LOS ITEMS:\n");
    while (itemNodo != NULL) {
      printf("%s\n", itemNodo->nombre);
      itemNodo = nextList(inventario);
    }
  }
}

//Se comprueba de que el jugador exista en el mapaJugadores mediante la funcion searchMap.
Jugador* existeJugador(Map*mapaJugadores) {
  char nombre[51];

  printf("INGRESE NOMBRE JUGADOR A BUSCAR: ");
  scanf("%50[^\n]s", nombre);
  getchar();

  Jugador* jugadorBuscado = searchMap(mapaJugadores, nombre);
  if(jugadorBuscado == NULL) {
    printf("El jugador %s no existe", nombre);
    return NULL;
  }

  return jugadorBuscado;
}

/*Para agregar el item al jugador se le traspasa el mapa de jugadores y el mapa de items. 
Se verifica de que el jugador exista, se le reserva memoria al nodo del item nuevo y se copia 
el nombreItem al nodo del item nodo. Luego se agrega al final de la lista del inventario del jugador
el nodo del item agregado. Luego guardamos en la pila de acciones "agregar" ya que es la acción que
se acaba de realizar.*/
void agregarItemJugador(Map*mapaJugadores, Map *mapaItems) {
  ItemMapa *itemMNodo;
  Jugador *jugadorBuscado = existeJugador(mapaJugadores);
  if(jugadorBuscado == NULL) return;
  
    char nombreItem[51];

    printf("\nINGRESE ITEM: ");
    scanf("%50[^\n]s", nombreItem);
    getchar();

    Item *itemNodo = malloc(sizeof(Item));
    if(itemNodo == NULL) exit(EXIT_FAILURE);

    strcpy(itemNodo->nombre, nombreItem);
    pushBack(jugadorBuscado->inventario, itemNodo);

    printf("\nITEM INGRESADO CON ÉXITO\n");

    insertarAccion(jugadorBuscado->pilaAcciones, "agregar", nombreItem);
    
    /*busca si existe el item en el mapaItems, si no está crea la lista para ese item. 
    Si no, se traspasa el item buscado a la itemMnodo*/
    ItemMapa *itemBuscado = searchMap(mapaItems, nombreItem);
  
    if(itemBuscado == NULL) {
      itemMNodo = (ItemMapa*) malloc(sizeof(ItemMapa));      
      itemMNodo->listaJugadoresConItem = createList();
      
    } else {
      itemMNodo = itemBuscado;

      //Se verifica que el jugador tenga el item en su inventario
      Item*itemBuscado = buscarItem(jugadorBuscado->inventario, nombreItem);
      
      /*Si existe se verifica de que el jugador no exista en la lista de jugadores del mapa de items. 
      Si ya existe, no se hace nada, si no, se agrega el jugador a la lista de jugadores del mapa del item.*/
      if(itemBuscado != NULL) {
        if(existeJugadorLista(itemMNodo->listaJugadoresConItem, jugadorBuscado->nombre)) {
          return;
        }

        pushBack(itemMNodo->listaJugadoresConItem, jugadorBuscado);
        return;
      }
    }

    strcpy(itemMNodo->nombre, nombreItem);
    //En el caso de que no existiera el item en el mapa de items, se agrega el item al mapa de items.
    insertarItemMapa(jugadorBuscado, nombreItem, mapaItems, itemMNodo);

}

void insertarAccion(Stack*pilaAcciones, char*accion, char*nombreItem) {
  Accion*nodoAccion = malloc(sizeof(Accion));

  strcpy(nodoAccion->nombreAccion, accion);
  strcpy(nodoAccion->item, nombreItem);

  stack_push(pilaAcciones, nodoAccion);
}

/*Para eliminar el item en el perfil del jugador, le pasamos el Mapa de jugadores y el Mapa de items, 
para después preguntar si el item existe en el inventario del jugador buscado o no.*/
void eliminarItemJugador(Map *mapaJugadores, Map* mapaItems) {
  char nombreItem[51];

  Jugador*jugadorBuscado = existeJugador(mapaJugadores);
  if(jugadorBuscado == NULL) return;
  
  printf("\nINGRESE ITEM A ELIMINAR: ");
  scanf("%50[^\n]s", nombreItem);
  getchar();

  Item* itemNodo = buscarItem(jugadorBuscado->inventario, nombreItem);

  /*Si el nodo del Item es igual a null, significa que el item no se 
  encuentra en el inventario, y si existe se inserta la accion en la pila de acciones 
  y se elimina el item de la lista de items del jugador*/
  if (itemNodo == NULL) {
    printf("\nEl item no existe en el inventario del jugador");
  } else {
    insertarAccion(jugadorBuscado->pilaAcciones, "eliminar", nombreItem);
    popCurrent(jugadorBuscado->inventario);
    eliminarJugadorListaItemMapa(jugadorBuscado, nombreItem, mapaItems);
  }
}

Item *buscarItem(List *inventario, char *nombreItem) {
  Item *item = firstList(inventario);
  
  while (item != NULL) {
    if (strcmp(item->nombre, nombreItem) == 0) {
      return item;
    }
    item = nextList(inventario);
  }
  return NULL;
}

//Para agregar puntos de jugador se inicializa los puntosAIngresar en -1. 
void agregarPuntosJugador(Map*mapaJugadores) {
  char puntosAIngresarString[21];
  int puntosAIngresar = -1;

  Jugador*jugadorBuscado = existeJugador(mapaJugadores);
  if(jugadorBuscado == NULL) return;

  printf("INGRESE PUNTOS DE HABILIDAD JUGADOR: ");
  do {
    scanf("%20[^\n]s", puntosAIngresarString);
    getchar();
    //Se verifica que los puntos a ingresar sea número, sino se transforma a este
    if(isdigit(puntosAIngresarString[0])) {
      puntosAIngresar = atoi(puntosAIngresarString);
    } 
    /*Si se verifica que los puntos a ingresar es negativo o tipo alfa se manda un mensaje 
    para que se ingrese un puntaje válido*/
    if (puntosAIngresar <= 0 || isalpha(puntosAIngresarString[0])) {
      printf("INGRESE UN PUNTAJE VÁLIDO ");
    } 

    //Se repite el ciclo hasta que se ingrese un valor válido
  } while (isalpha(puntosAIngresarString[0]) || puntosAIngresar <=0);

  //Cuando se verifica que los puntos sean válidos, se le suman a los puntos del jugador
  jugadorBuscado->puntosHabilidad += puntosAIngresar;

  //Se traspasa la pila de acciones del jugador, la accion y los puntos ingresados.
  insertarAccionPuntaje(jugadorBuscado->pilaAcciones, "agregar puntaje", puntosAIngresar);
  printf("\nPUNTAJE ASIGNADO CORECTAMENTE\n");
}

//Se traspasa la pila de acciones junto con la accion y el puntaje a guardar como la ultima accion para poder eliminarla en el caso de que el usuario lo requiera. Luego se inserta el nodo accion a la pila de acciones.
void insertarAccionPuntaje(Stack*pilaAcciones, char*accion, int puntaje) {
  Accion*nodoAccion = malloc(sizeof(Accion));

  nodoAccion->puntajeADeshacer = puntaje;
  strcpy(nodoAccion->nombreAccion, accion);
  
  stack_push(pilaAcciones, nodoAccion);
}

//Para mostrar los jugadores con item especificos se le traspasa el mapa de items. Se crea la variable nombreItem y luego se verifica de que el item ingresado exista en el mapa. 
void mostrarJugadoresItemEspecifico(Map*mapaItems) {
  char nombreItem[51];
  
  printf("\nINGRESE ITEM PARA MOSTRAR JUGADORES: ");
  scanf("%50[^\n]s", nombreItem);
  getchar();

  ItemMapa *itemBuscado = searchMap(mapaItems, nombreItem);
  if(itemBuscado == NULL) {
    printf("NO EXISTE EL ITEM\n");
    return;
  }

  //Si existe el item, se crea un puntero al inicio de la lista de jugadores del item para poder recorrerla
  Jugador *jugadorNodo = firstList(itemBuscado->listaJugadoresConItem);

  //Si el primer nodo de la lista es NULL, significa que no existen jugadores con ese item. Si no, se imprimen todos los jugadores con ese item.
  if(jugadorNodo == NULL ) {
    printf("NO HAY JUGADORES CON ESE ITEM\n");
  } else {
    printf("LISTA DE TODOS LOS JUGADORES CON ITEM ESPECIFICO:\n");
    while (jugadorNodo != NULL) {
      printf("%s\n", jugadorNodo->nombre);
      jugadorNodo = nextList(itemBuscado->listaJugadoresConItem);
    }
  }
}

//Para buscar si existe el jugador simplemente se agrega un puntero al inicio de la lista, si se encuentra el jugador se retorna true, si no, false.
bool existeJugadorLista(List * listaJugadoresItem, char *nombreJugador){
  Jugador *jugadorBuscado = firstList(listaJugadoresItem);
  
  while (jugadorBuscado != NULL) {
    if (strcmp(jugadorBuscado->nombre, nombreJugador) == 0) {
      return true;
    }
    jugadorBuscado = nextList(listaJugadoresItem);
  }
  
  return false;
}

//Para eliminar un jugador de una lista de jugadores de un mapa de item especifico, primero se busca el mapa del item y se traspasa la lista de jugadores junto con el nombre de jugador a eliminar a la funcion eliminarJugador.
void eliminarJugadorListaItemMapa(Jugador*jugadorBuscado, char*nombreItem, Map*mapaItems) {
  ItemMapa*itemMNodo = searchMap(mapaItems, nombreItem);

  eliminarJugadorLista(itemMNodo->listaJugadoresConItem,jugadorBuscado->nombre);
}


//Para eliminar el jugador de la lista de jugadores de un mapa de item especifico, se recorre la lista hasta encontrarlo y se realiza un popCurrent.
void eliminarJugadorLista(List *listaJugadores, char *nombreJugador) {
  Jugador *jugadorNodo = firstList(listaJugadores);
  
  while (jugadorNodo != NULL) {
    if (strcmp(jugadorNodo->nombre, nombreJugador) == 0) {
      popCurrent(listaJugadores);
    }
    jugadorNodo = nextList(listaJugadores);
  }
}

//Se agrega el jugador a la lista de jugadores del mapa del item correspondiente.
void insertarItemMapa(Jugador*jugadorBuscado, char*nombreItem, Map*mapaItems, ItemMapa* itemM) {
  
  pushBack(itemM->listaJugadoresConItem, jugadorBuscado);
  insertMap(mapaItems, itemM->nombre, itemM);
}

/*Para deshacer la ultima accion se traspasa el mapaJugadores y se verifica que el jugador exista.
 Luego creamos un puntero que apunte al top de la pila de acciones (o sea, la ultima accion realizada).
  Si la ultima accion es null, significa que no se han realizado acciones aún o ya se borraron todas.*/
void deshacerUltimaAccion(Map*mapaJugadores) {
  Jugador*jugadorBuscado = existeJugador(mapaJugadores);
  if(jugadorBuscado == NULL) return;

  Accion *ultimaAccion = stack_top(jugadorBuscado->pilaAcciones);

  if(ultimaAccion == NULL) {
    printf("NO HAY ACCIONES REALIZADAS POR EL JUGADOR\n");
    return;
  }

  //Si la ultima accion fue "agregar" significa que para deshacer necesitamos eliminar el último dato ingresado. Por lo tanto creamos un puntero que apunte al ultimo item agregado.
  if(strcmp(ultimaAccion->nombreAccion, "agregar") == 0) {
    Item* itemNodo = buscarItem(jugadorBuscado->inventario, ultimaAccion->item);

    //Si el nodo es NULL significa que el item no esta en la lista del jugador, sino, eliminamos el último item ingresado del jugador.
    if (itemNodo == NULL) {
      printf("\nEl item no existe en el inventario del jugador");
    } else {
      popCurrent(jugadorBuscado->inventario);
      printf("ACCION DESHECHA CON EXITO\n\n");
    }

    //Finalmente, se elimina la ultima acción que sería "agregar" ya que el usuario la deshizo.
    stack_pop(jugadorBuscado->pilaAcciones);
  }

  //En el caso de que la ultima accion sea eliminar, significa que debemos agregar el ultimo item eliminado. Por lo tanto reservamos memoria para el nodo a agregar.
  if(strcmp(ultimaAccion->nombreAccion, "eliminar") == 0) {
    Item *itemNodo = malloc(sizeof(Item));
    if(itemNodo == NULL) exit(EXIT_FAILURE);

    //Copiamos el item que se eliminó al itemNodo y lo agregamos al final de la lista de inventario del jugador
    strcpy(itemNodo->nombre, ultimaAccion->item);
    pushBack(jugadorBuscado->inventario, itemNodo);
    printf("ACCION DESHECHA CON EXITO\n\n");

    //Se elimina la última accion guardada en la pila de acciones ya que el usuario la deshizo.
    stack_pop(jugadorBuscado->pilaAcciones);
  }

  //Si la ultima accion es agregar puntaje, significa que debemos restar el puntaje agregado al puntaje total. 
  if(strcmp(ultimaAccion->nombreAccion, "agregar puntaje") == 0) {

    jugadorBuscado->puntosHabilidad -= ultimaAccion->puntajeADeshacer;
    printf("ACCION DESHECHA CON EXITO\n\n");
    stack_pop(jugadorBuscado->pilaAcciones);
  }

}


/*Al momento de querer exportar los datos ya creados, a la función se le pasa el Mapa de Jugadores,
 se verifica primeramente si este es NULL, si lo es, este sale de la función sin hacer unn archivo vacio.
  Si esto no sucede, la función procede normalmente, creando una variable FILE a la cual se le otorga el
   nombre del archivo preguntado anteriormente, posteriormente se crea una variable tipo Jugador a la 
   cual se le otorga el primer Mapa del mapaJugadores y se van printeando cada una de las cosas dentro 
   del perfil de los jugadores, hasta que el nodo sea NULO, lo que termina el ciclo while, después se 
   cierra el archivo y se hace un print diciendo que todos los datos se exportaron correctamente*/
void exportarDatosJugadores(Map*mapaJugadores) {
  if(mapaJugadores == NULL) exit(EXIT_FAILURE);
  char nombreArchivo[101];

  printf("\nINGRESE NOMBRE DEL ARCHIVO A EXPORTAR LOS JUGADORES:\n");
  scanf("%100[^\n]s", nombreArchivo);

  FILE* archivoJugadores = fopen(nombreArchivo, "w");
  
  if (archivoJugadores == NULL) {
    printf("Error al abrir el archivo\n");
    return;
  }

  Jugador *jugadorNodo = firstMap(mapaJugadores);

  fprintf(archivoJugadores, "Nombre,Puntos de habilidad,Items\n");
  
  while(jugadorNodo != NULL) {
    fprintf(archivoJugadores, "%s, %d", jugadorNodo->nombre, jugadorNodo->puntosHabilidad);

    Item*itemNodo = firstList(jugadorNodo->inventario);

    while(itemNodo != NULL) {
      fprintf(archivoJugadores, " ,%s", itemNodo->nombre);
      itemNodo = nextList(jugadorNodo->inventario);
    }
    fprintf(archivoJugadores, "\n");
    jugadorNodo = nextMap(mapaJugadores);
  }

  fclose(archivoJugadores);
  printf("\nLOS DATOS SE HAN EXPORTADO CORRECTAMENTE AL ARCHIVO\n: %s\n", nombreArchivo);
}

/*En esta función se recibe el mapa con todos los jugadores y el mapa con todos los items.
Se crea una variable tipo char donde se guardará el nombre del archivo que el usuario desea 
ingresar, luego se abre el archivo mediante el uso de la funcion "fopen", en modo de lectura,
"r", y se verifica que el archivo solicitado se abrio sin problemas, en caso contrario se imprime
el mensaje correspondiente y se termina la ejecucion de la funcion, si no, se crea otra variable 
tipo char donde se guardaran las lineas del archivo a importar, entrando a un ciclo while condicionado
por la funcion fgets, la cual por los parametros lineaArchivo, sizeof(lineaArchivo) y el 
archivo correspondiente, se encarga de que se lea linea por linea hasta que se llegue al 
final del archivo. Se crea un auxiliar tipo Jugador que se le reserva memoria. 
Mediante el uso de la funcion "strtok", se divide la linea de archivo leida en campos separados 
por comas (","), siendo su primera llamada cuando se quiere ingresar el nombre del jugador respectivo.
En un principio para poder ingresar el nombre, se lee la linea completa hasta la coma, 
(donde su primer argumento es la lineaArchivo), pero para los siguientes datos el primer 
argumento cambia a NULL ya que de esta manera se sigue dividiendo la cadena desde la llamada 
anterior de la función. Para el nombre, este se guarda gracias a la funcion strcopy, 
luego para los puntos de habilidad se guardan en un auxiliar como string, se transforma a
entero por el uso de la funcion atoi y se termina guardando en el juadorNodo, se crea la 
pila para el jugadorNodo, se guarda la cantidad de items como tipo char yse crea la lista 
para el inventario de items del jugador. Para los items, se lee el primer item del jugador
del archivo, y se entra a un ciclo while el cual este termina en caso de que el item leido 
sea NULL (ya no quedan mas items del jugador respectivo). Dentro del ciclo se crea una variable 
tipo Item como un nodo auxiliar la cual se le reserva memoria, el item que se leyo anteriormente s
e guarda dentro del nodo auxiliar, y se ingresa el nodo dentro de la lista de items del jugador 
mediante pushFront y se llama a la funcion insertarItemMapaArchivo, para añadir el item al mapa de items.*/
void importarDatosJugadores(Map*mapaJugadores, Map*mapaItems) {
  char nombreArchivo[101];

  printf("\nINGRESE NOMBRE DEL ARCHIVO A IMPORTAR LOS JUGADORES:\n");
  scanf("%100[^\n]s", nombreArchivo);

  FILE* archivoJugadores = fopen(nombreArchivo, "r");
  
  if (archivoJugadores == NULL) {
    printf("Error al abrir el archivo\n");
    return;
  }

  char lineaArchivo[200];
  while (fgets(lineaArchivo, sizeof(lineaArchivo), archivoJugadores)) {
    Jugador *jugadorNodo = malloc(sizeof(Jugador));

    char *item;
    
    strcpy(jugadorNodo->nombre, strtok(lineaArchivo, ","));

    char *puntosHabilidadString = strtok(NULL, ",");
    int puntosHabilidad = atoi(puntosHabilidadString);

    jugadorNodo->puntosHabilidad = puntosHabilidad;

    jugadorNodo->pilaAcciones = stack_create();
    
    char *cantidadItems = strtok(NULL, ",");

    item = strtok(NULL, ",");

    jugadorNodo->inventario = createList();
      
    while(item != NULL) {
      Item* ItemNodo = (Item*) malloc(sizeof(Item));
      if(ItemNodo == NULL) exit(EXIT_FAILURE);
        
      strcpy(ItemNodo->nombre, item);

      pushBack(jugadorNodo->inventario, ItemNodo);

      insertarItemMapaArchivo(mapaItems, item, jugadorNodo);
        
      item = strtok(NULL, ",");
    }

    insertMap(mapaJugadores, jugadorNodo->nombre, jugadorNodo);
  }

  fclose(archivoJugadores);

}

// Se usa el mismo metodo para agregar el item en en el mapaItems de la funcion AgregarItem. 
void insertarItemMapaArchivo(Map* mapaItems, char*nombreItem, Jugador*jugadorBuscado) {
  ItemMapa*itemMNodo;
  
  ItemMapa *itemBuscado = searchMap(mapaItems, nombreItem);
  
  if(itemBuscado == NULL) {
    itemMNodo = (ItemMapa*)malloc(sizeof(ItemMapa));      
    itemMNodo->listaJugadoresConItem = createList();
      
  } else {
    itemMNodo = itemBuscado;

    Item*itemBuscado = buscarItem(jugadorBuscado->inventario, nombreItem);
    if(itemBuscado != NULL) {
        
      if(existeJugadorLista(itemMNodo->listaJugadoresConItem, jugadorBuscado->nombre)) {
        return;
      }

      pushBack(itemMNodo->listaJugadoresConItem, jugadorBuscado);
      return; 
      } 
    }

  strcpy(itemMNodo->nombre, nombreItem);
  insertarItemMapa(jugadorBuscado, nombreItem, mapaItems, itemMNodo);
}