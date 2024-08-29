<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA LISTA

## Alumno: Juan Bautista Fonseca - 111456 - jfonseca@fi.uba.ar

- Para compilar:
```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g <archivo/s_a_compilar> -o <nombre_ejecutable>
```

- Para ejecutar:

```bash
./<nombre_ejecutable>
```

- Para ejecutar con valgrind:
```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes ./<nombre_ejecutable>
```

---

##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.

Incluír **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).

La finalidad del TP era implementar distitas funciones y procedimientos para el correcto funcionamiento y creacion de un TDA lista enlazada simplemente por nodos, estas funciones y procedimiento estas declaradas en el archivo `lista.h` e implementadas en el archivo `lista.c`. A su vez se reutilizaron las funciones implementadas para el TDA lista para los TDA de cola y pila, cuyas funciones estan declaradas en los archivos `cola.h` y `pila.h` e implmentados en `cola.c` y `pila.c`.
Tambien se implementaron dos iteradores, un interno y otro externo, para recorrer las distintas estructuras.

Las estrcuturas desarrolladas en el TP son las listas y los nodos, que tienen esta implementacion (La implementacion puede variar dependiendo de las necesidades del usuario o desarrollador) : 

Implementacion nodo :
```c
void *elemento;
struct nodo *siguiente;
```

Esta implementacion los nodos pueden almacenar un dato de cualquier tipo, por eso ultilizamos `void*`, y almacena un `nodo*` que apunta al siguiente nodo, de esta manera enlazamos un nodo con el siguiente y logramos tener la logica inicial de una lista.

Implementacion lista :
```c
nodo_t *nodo_inicio;
nodo_t *nodo_final;
size_t tamanio;
```

Tiene dos campos uno que hacer referencia a la lista y una referencia al nodo actual

Implementacion iterador interno : 
```c
    lista_t *lista;
    nodo_t *nodo_actual;
```

Esta implementacion de lista tiene dos `nodos_t*` uno que almacena un puntero al primer nodo de lista y otro que almacena un putero al ultimo nodo de lista y un dato `size_t` que nos indica el tamanio de esta lista.
Esta implementacion es util a la hora de insertar nuevos nodos al final y al comienzo ya que tenemos una referencia directa a estas posciones. Tambien al tener el tamanio de lista podes saber si la lista esta esta vacia o la cantidad de elementos que tiene.

No tenemos como tal un estructura de pilas o colas, ya que una lista puede tomar el rol de una pila o cola, lo se tuvo en cuenta a la hora de desarrolar las pilas y colas es que el comportamiento de una lista sea el esperado para una pila o cola.
Por ejemplo si una lista tomo el rol de una pila, solo se van a poder insertar y eliminer elementos que esten en el 
`nodo_final` a su vez si una lista se utiliza como cola solo se van a poder insertar elementos en el `nodo_inicio` y eliminar aquel que este en el `nodo_final`. Por esta razon no estan implementadas las estructuras de pila y cola.

El programa en le proporciona al usuario distintas funciones para que pueda crear y modificar listas, pilas y colas : 

Se cuenta con una funcion `lista_crear` que crea una lista, la cual inicialmente esta vacia, su nodo final e inicial son nulos y el tamanio es 0. El usuario le va a poder insertar nuevos elementos. Se maneja el caso de que ocurre si la a la hora de asignar memoria a lista el alloc falla y es que al usuario se le devuelve una lista nula. Como reutilizamos las estrctura de `lista_t` para las pilas y colas las funciones `cola_crear` y `pila_crear` tienen la misma logica.

El usuario cuenta con dos funciones para insertar nuevos elementos, `lista_insertar` y `lista_insertar_en_posicion`.
La funcion `lista_insertar` hace una llamada a la funcion privada `crear_nodo`, inserta este nodo al final de lista y aumenta el tamanio de la lista, en caso de que la lista no sea nula. Esta funcion se reutiliza para la funcion 
`pila_apilar`, ya que cumple con la logica para apilar elementos.
La funcion `lista_insertar_en_posicon` elimina el elemento en la posicion que el usuario pasa por parametro, en caso de que la posicion sea mas grande a la cantidad de elementos en la lista se elimina el ultimo elemento y se aumenta el tamanio de la lista, en caso de que la lista no sea nula. Esta funcion tambien es reutilizada para la funcion `cola_encolar` ya que podemos insertar siempre al principio de una cola.

En el archivo `lista.c` la funcion `crear_nodo`, se le pasa por parametro un elemento, utiliza `calloc` para reservar un bloque de memoria de tamanio `nodo_t`, se verifica si el `calloc` no falla en caso de hacerlo se devulve null,le asigna a 
`nodo->elemento` el valor de elemento pasado por parametro y `nodo->siguiente` se define en null y por ultimo se devulve el nodo ya inicializado.

```c
nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));

if (nuevo_nodo == NULL)
    return NULL;

nuevo_nodo->elemento = dato;
nuevo_nodo->siguiente = NULL;

return nuevo_nodo;
```

El usuario tambien tiene las funciones `lista_quitar` y `lista_quitar_en_posicion`. La funcion `lista_quitar ` elimina el ultimo elemento de la lista, reduce el tamanio de la misma y devuelve el dato del nodo eliminado, hace las mismas verificaciones que `lista_insertar`, es decir si la lista no sea nula ni esta vacia. Esta funcion sirve tanto para la funcion `pila_desapilar` y la funcion `cola_desencolar` ya que cumple con la logica de mabas estructuras.
`lista_quitar_en_posicion` elimina el elemento en la posicion, nuevamente solo lo hace si la lista no es nula o no esta vacia, reduce el tamanio de la lista y devulve el dato del nodo eliminado.

Para efectuar busquedas el usuario tiene las funciones `lista_elemento_en_posicion` y `lista_buscar_elemento`. Con la primera funcion al usuario se le devuelve el elemento del nodo en la posicion que se le pasa por parametro a la funcion, en caso de no existir se le devolvera null. La segunda funcion se le devolvera al usuario el elemento que busca en caso de ser encontrado en la lista, en caso contrario se le devolvera null.

El usuario tambien tiene funciones para acceder al primer y al ultimo elemento de la lista con las funciones `lista_primero` y `lista_ultimo`. En caso de que la lista sea nula o este vacia se le devuelve null.
La funcion `lista_ultimo` se utiliza en las funciones `cola_frente` y `pila_tope` para devolver el elemento ubicado en el frente de una cola o el ultimo elemento insertado en una pila.

Para saber el tamanio y si una lista esta vacia el usuario tiene acceso a las funciones `lista_vacia` y `lista_tamanio`.
`lista_vacia` devuelve verdadero en caso de que el tamanio de la lista sea 0 o sea una lista nula y `lista_tamanio` devuelve 0 si la lista es null o el campo tamanio de la estructura lista.
Estas funciones son reutilizadas en `pila_vacia`, `pila_tamanio`, `cola_vacia` y `cola_tamanio`.

Para liberar la memoria reservada para la lista y los nodos hay dos funciones `lista_destruir` y `lista_destruir_todo`
`lista_destruir` itera por todos los nodos de lista liberando la memoria de los mismo y al final libera la memoria de la lista. `lista_destruir_todo` aparte de liberar la memoria de los nodos tambien le aplica una funcion destrcutora a cada elemento de los nodos, esta funcion destructora es pasada por el usuario y al final libera la memoria de la lista.
Nuevamente las funciones para destruir pilas y colas reutilizan `lista_destruir` y `lista_destruir_todo`.

Hay implementado un iterador externo, que tiene la funciones `lista_iterador_crear`, `lista_iterador_avanzar`, `lista_iterador_tiene_siguiente` y `lista_iterador_elemento_actual`. En general el iterador externo implementados sirve para iterar los elementos de la lista si es que los hay y devolver el elemento actual. 

El iterador interno tiene un funcion `lista_con_cada_elemento` la función recorre la lista y para cada elemento, invoca la función proporcionada por el usuario (funcion) pasándole como argumentos el elemento de la lista y el contexto. Si la función del usuario devuelve false, la iteración se detiene y la función `lista_con_cada_elemento devuelve` la cantidad de elementos iterados hasta ese momento.
La función devuelve la cantidad total de elementos iterados o 0 en caso de error. Los errores pueden incluir errores de memoria, la lista o la función son NULL, entre otro

### Por ejemplo:

El programa funciona abriendo el archivo pasado como parámetro y leyendolo línea por línea. Por cada línea crea un registro e intenta agregarlo al vector. La función de lectura intenta leer todo el archivo o hasta encontrar el primer error. Devuelve un vector con todos los registros creados.

<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

En el archivo `sarasa.c` la función `funcion1` utiliza `realloc` para agrandar la zona de memoria utilizada para conquistar el mundo. El resultado de `realloc` lo guardo en una variable auxiliar para no perder el puntero original en caso de error:

```c
int *vector = realloc(vector_original, (n+1)*sizeof(int));

if(vector == NULL)
    return -1;
vector_original = vector;
```


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas

- ¿Qué es una lista/pila/cola? Explicar con diagramas.

<div align="center">
<img width="70%" src="img/lista.svg">
</div>

<div align="center">
<img width="70%" src="img/Cola.svg">
</div>

<div align="center">
<img width="70%" src="img/pilas.svg">
</div>

 - Explica y analiza las diferencias de complejidad entre las implementaciones de lista simplemente enlazada, doblemente enlazada y vector dinámico para las operaciones:
   - Insertar/obtener/eliminar al inicio

      Lista Simplemente Enlazada:
        Insertar: O(1)
        Obtener: O(1)
        Eliminar: O(1)

      Lista Doblemente Enlazada:
        Insertar: O(1)
        Obtener: O(1)
        Eliminar: O(1)

      Vector Dinámico:
        Insertar: O(n) ya que se requiere desplazar todos los elementos hacia la derecha.
        Obtener: O(1)
        Eliminar: O(n) ya que se requiere desplazar todos los elementos hacia la izquierda.

   - Insertar/obtener/eliminar al final

      Lista Simplemente Enlazada:
        Insertar: O(n), por que debemos recorrer la lista hasta el final para agregar el nuevo elemento.
        Obtener: O(n), ya que también se debe recorrer la lista hasta el final para obtener el último elemento.
        Eliminar: O(n), similar a la inserción, se debe recorrer la lista hasta el penúltimo elemento para eliminar el último.

      Lista Doblemente Enlazada:
        Insertar: O(1), ya que el puntero al último nodo está disponible y no se requiere recorrer la lista.
        Obtener: O(1), se puede acceder directamente al último elemento.
        Eliminar: O(1), ya que el puntero al último nodo está disponible y no se requiere recorrer la lista.

      Vector Dinámico:
        Insertar: O(1) O(n) ya que vamos a tener que redimensionar el tamnio del vector.
        Obtener: O(1)
        Eliminar: O(1) O(n) ya que vamos a tener que redimensionar el tamnio del vector.

   - Insertar/obtener/eliminar al medio

      Lista Simplemente Enlazada:
        Insertar: O(n), ya que se debe recorrer la lista hasta la posición deseada.
        Obtener: O(n), similar a la inserción, se debe recorrer la lista hasta la posición deseada.
        Eliminar: O(n), similar a la inserción, se debe recorrer la lista hasta la posición anterior al elemento a eliminar.

      Lista Doblemente Enlazada:
        Insertar: O(n), similar a la lista simplemente enlazada, aunque se requiere recorrer hasta la mitad de la lista en promedio.
        Obtener: O(n), similar a la inserción, se debe recorrer la lista hasta la posición deseada.
        Eliminar: O(n), similar a la inserción, se debe recorrer la lista hasta la posición deseada.

      Vector Dinámico:
        Insertar: O(n), ya que se deben desplazar todos los elementos a la derecha para hacer espacio.
        Obtener: O(1)
        Eliminar: O(n), similar a la inserción, se deben desplazar todos los elementos a la izquierda después de eliminar un elemento.

 - Explica la complejidad de las operaciones implementadas en tu trabajo para la pila y la cola.
    Tanto para pilas como para colas todas las operaciones implementadas tienen una complejidad computacional O(1) a excepcion de las funciones desapilar y desencolar que son O(n) por como esta implementada la funcion quitar en la lista.
    En mi trabajo hay un caso particular que es la funcion encolar, ya que la funcion que inserta en una posicion en particular en la lista es O(n) pero tiene una condicion (cuando se inserta un elemento en posicion 0) que la vuelve O(1). 