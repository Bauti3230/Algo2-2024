# TDA ABB

## Repositorio de (Juan Bautista Fonseca) - (111456) - (jfonseca@fi.uba.ar)

- Para compilar:

```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g <nombre_archivo> -o <nombre_ejecutable> 
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

El TP consistia en implementar funciones basicas y un iterador interno para un Arbol de Busqueda Binaria (ABB). Las estrcuturas de los TDA que se pusieron a disposicon para los Arboldes de Busquerda Binaria y los Nodos de los arboles binarios tenian las siguoientes estructuras. Cave aclarar que estas estructuras pueden variar en base a las necesidades del usuario y del desarrolador.

Estrctura ABB
```c
nodo_abb_t *nodo_raiz;
abb_comparador comparador;
size_t tamanio;
```
Esta estructura esta definida en la libreria `abb_estructura_privada.h`. Nuestro ABB cuenta con tres campos `nodo_raiz` de tipos `nodo_abb_t*`, `comparador` de tipos `abb_comparador` y un `tamanio` de tipo `size_t`. 
El campo `nodo_raiz` almacena el primer elemento del arbol, ya sea que fue insertado o un elemento del arbol que fue reasignado a esa posicion (Esto ultimo acurre en caso de eliminar el nodo raiz).
El `comparador` es una funcion que devuelve un `int` y esto es la manera que el desarrolador tiene de distribuir los elementos en el arbol. La funcion recibe dos `void*` realiza una operacion y devuleve un resultado de tipo `int` y dependiendo de la implementacion el elemento se ubicaria a la izquierda o derecha (En mi caso decidi que si el resultado era negativo se avanzaba por el lado derecho y si era positivo por el izquierdo).
El `tamanio` no define cuantos elementos hay actualmente en el arbol, ese se ve afectado cada vez que se elimina o se agrega un elemento.

Estrctura Nodo
```c
void *elemento;
struct nodo_abb *izquierda;
struct nodo_abb *derecha;
```  
Esta estructura esta definida en la libreria `abb_estructura_privada.h`. Nuestro `nodo_abb` cuenta con tres campos `elemento` de tipo `void*` e `izquierda` y `derecha` ambos de tipo `nodo_abb*`.
El `elemento` se encarga de almacenar los datos ingresados en el ABB, no nos intersa que tipo de datos son ingresados, cada `nodo_abb` solo puede almacenar un dato en su campo `elemento`.
El campo `izquierda` es un puntero a un `nodo_abb*` que almacena un dato menor (Segun nuestra implementacion) o `NULL` en caso de que no apunte a nada. Este nodo, en caso de no ser `NULL`, cumple con las caracteristicas anterirormente esbosadas.
El campo `derecha` es un puntero a un `nodo_abb*` que almacena un dato mayor o `NULL` en caso de no apuntar a nada. Este nodo, en caso de no ser `NULL`, cumple con las caracteristicas anteriormente esbosadas.

Contando con estas estrcuturas tenemos la logica basica de ub ABB, ya que partimos de un `nodo_raiz` que puede apuntar a dos nodos `izquierda` o `derecha`, en caso de existir y a su vez tanto `izquierda` como `derecha` apuntan a otros dos nodos. Por lo que se cumple una de las caractersticas fundamentales de un ABB que es que un nodo, como maximo, solo puede tener dos hijos.

A continuacion se da una explicacion mas detalla de que es un ABB, sus operaciones basicas y las complejidades de las mismas. A su vez se van a definir tanto los Arboles como los Arboles Binarios.

ABB explicacion, funciones y complejidad de las mismas : 

Un árbol binario de búsqueda es una estructura de datos de árbol en la que cada nodo tiene como máximo dos hijos, denominados izquierdo y derecho. Además, para cada nodo, todos los elementos en el subárbol izquierdo son menores que el nodo raíz, y todos los elementos en el subárbol derecho son mayores que el nodo raíz. Esta propiedad es lo que hace que la búsqueda, inserción y eliminación sean eficientes en un árbol binario de búsqueda.

Búsqueda : Para buscar un elemento en un ABB, se comienza en la raíz y se compara el elemento buscado con el valor del nodo actual. Si el valor buscado es igual al valor del nodo actual, se ha encontrado el elemento. Si el valor buscado es menor que el valor del nodo actual, se busca en el subárbol izquierdo. Si es mayor, se busca en el subárbol derecho. Este proceso se repite recursivamente, que es la manera mas efectiva ya que los ABB son estrcturas con una naturaleza recursica, hasta encontrar el elemento o llegar a un nodo hoja (donde la búsqueda se detiene). La complejidad de tiempo de la búsqueda es `O(n)`.

Inserción : Para insertar un nuevo elemento en un ABB, se realiza una búsqueda similar a la búsqueda mencionada anteriormente. Una vez que se llega a un nodo hoja donde debería ir el nuevo elemento, se inserta como hijo izquierdo o derecho según corresponda para mantener la propiedad de orden. La complejidad de tiempo de la inserción es también `O(n)`.

Eliminacion : Para eliminar un elemento de un ABB implica encontrar el nodo que contiene el elemento a eliminar. Si el nodo tiene cero se puede eliminar directamente. En caso de tener un hijo, este ultimo se asigna a la `izquiera` o `derecha` de su nodo abuelo dependiendo de si su nodo padre era mayor o menos. Si tiene dos hijos, se puede reemplazar el nodo a eliminar con el mínimo elemento del subárbol derecho (o el máximo del subárbol izquierdo) y luego eliminar ese nodo de manera recursiva. 

AB explicacio, funciones y complejidad de las mismas :

Un AB es una estructura de datos en forma de árbol en la que cada nodo tiene como máximo dos hijos, denominados `izquierda` y `derecho`. La principal característica de un árbol binario es que cada nodo puede tener hasta dos hijos, lo que lo distingue de otros tipos de árboles donde los nodos pueden tener más de dos hijos. La diferecnia fundamental con un ABB es que los AB no tiene un orden en particular, es decir, no se cumple la regla que en el subarbol izquierdo estan los elementos menores a la raiz y en el subarbol derecho estan los elementos mayores a la raiz.

Recorrido : Aunque no tenga un orden como los ABB los AB de igual manera estan fuertemente relacionados con la funcion de busqueda binaria por ende las formas de recorrer un AB van a ser fundamentales para las demas funciones (Busqueda, Insercion y Eliminacion). Los recorridos principales son : Inorden, Postorden y Preorden. Cada uno recorre la estructura de una manera particular : 
        * Preorden : Se visita primero el nodo actual, luego todo el subarbol izquierdo y por ultimo el subarbol derecho.
        * Inorden : Se visita primero el subarbol izquierdo, luego el elemento y por ultimo el subarbol derecho.
        * Postorden : Se visita primero el subarbol derecho, luego el elemento y por ultimo el izquierdo.

El programa funciona dandole al usuario funciones basicas de ABB's con las cuales el usuario puede crear y modificar al mismo. Todas las funciones estan declaras en el archivo `abb.h` e implementadas en `abb.c`.
Entre las funciones a las que el usuario tiene acceso son : `abb_crear` la cual le devuleve un puntero a un ABB el cual esta vacio, es decir tiene tamanio 0 y su nodo raiz es `NULL`, `abb_insertar` con la que puede agregarle elementos al ABB haciendo que su tamanio aumente e insertando un nuevo nodo en el lugar correspondiente, `abb_quitar` para remover elementos del ABB, `abb_buscar` funcion que le permite buscar cualquier elemento devolviendole `NULL` en caso de que no este incluido en el ABB o el elemento en si y funciones mas triviales como `abb_tamanio` y `abb_vacio` las cuales le van a indicar el tamanio del arbol ya sea que exista o no y si esta vacio.
Tambien tiene acceso a iteradores que son las funciones `abb_con_cada_elemento` a la cual el usuario va a estar encargado de pasarle un funcion booleana que determina hasta cuando se itera el arbol devolviendole la cantidades de veces que fue invodaca la funcion. Por otro lado esta la funcion `abb_recorrer` la cual va a recorrer el arbol en alguno de los sentido (Inorden, Postorden o Preorden) y va a ir almacenando los datos en un array mientras que el mismo tenga espacio suficiente.

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
Incluír acá las respuestas a las preguntas del enunciado (si aplica).
