# TP: Carrera de obstáculos

## Repositorio de Juan Bautista Fonseca - 111456 - jfonseca@fi.uba.ar

- Para compilar:

```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g src/*.c pruebas_alumno.c -o pruebas_alumno

gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g src/*.c juego.c -o juego
```

- Para ejecutar:

```bash
./ pruebas_alumno

./ juego
```

- Para ejecutar con valgrind:
```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes ./pruebas_alumno

valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes ./juego
```
---
##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.

Incluír **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).

La finalidad del TP era la implementacion de un juego de carreas de pokemones y su logica. Se implementaron 2 TDA'S : `TP`  y `pokemon_info`.

El TDA `TP` comprendi la inicializacion del juego, es decir : pokemones disponibles,la pista de ambos jugadores y los datos de los jugadores. Para almacenar los pokemones se utilizo un `abb` ya que, el recorrido `INORDEN` facilitaba mucho el recorrido e insercion de los nombres de los pokemones, en orden alfabetico, en un string (Como era pedido) y aparte de esto como se podemos adoptar un regla de ordenamiento nos ahorramos la implementacion de un sort para el vector. Aunque un Heap hubiera sido mas rapido a la hora de encontrar elementos, la logica se asemejaba mas a la de un `abb`. 
Respecto a los datos de los jugadores se creo, de manera privada, un `struct datos_juador_t` que tenia un variable `struct pokemon_info *` donde se almacenaba el pokemon seleccionado y una variable booleana que indicaba si el jugador habia o no seleccionado un pokemon.
Para las pistas se utilizaron listas, ya que la logica que se pedia para las pistas era la misma que para las listas. Tambien al haber implementado un iterador externo nos permitida recorrer toda la lista y acceder a cada elemento.
```c
    struct pokemon_info *pokemon;
    bool seleccionado;
```
```c
    abb_t *roster_pokemon;
    lista_t *pista_1;
    lista_t *pista_2;
    datos_jugador_t *jugador1;
    datos_jugador_t *jugador2;
```
El `struct pokemon_info` ya estba defindio en el `tp.h`, tiene 4 campos : un `char*` destinado a almacenar el nombre del pokemon y 3 datos `int` que comprenden a las estadisitcas del pokemon 
(FUERZA,INTELIGENCIA Y DESTREZA). Los campos de las estadisticas estan definidad como constantes de la siguiente forma `enum TP_OBSTACULOS {FUERZA,INTELIGENCIA,DESTREZA}`.

Para este tp se implementaron las siguientes funciones : `tp_crear`, `tp_cantidad`, `tp_pokemon_buscar`
`tp_nombres_disponibles`, `tp_seleccionar_pokemon`, `tp_pokemon_seleccionado`, `tp_agregar_obstaculo`,
`tp_quitar_obstaculo`, `tp_obstaculos_pista`, `tp_limpiar_pista`,`tp_calcular_tiempo_pista`, 
`tp_tiempo_por_obstaculos` y `tp_destruir`.

La función tp_crear toma como parámetro el nombre de un archivo `(nombre_archivo)`, lee su contenido, y crea una estructura `TP` inicializada con los datos del archivo. La estructura `TP` incluye un árbol binario de búsqueda `(ABB)` para almacenar información de Pokémon, dos listas y datos de dos jugadores. La función retorna un puntero a la estructura `TP` creada, o `NULL` si ocurre algún error.
Se declaran un puntero a `FILE` y un arreglo de caracteres para leer líneas del archivo. Se intenta abrir el archivo en modo lectura. Si `fopen` falla (el archivo no existe o no se puede leer), la función retorna 
`NULL`. Se asigna memoria para una nueva estructura `TP`. Si `calloc` falla, se llama a 
`finalizacion_abrupta` que maneja la limpieza necesaria y retorna NULL.
La funcion `finalizacion_abrupta`, es un funcion privada declarada e implementada en el archivo `tp.c`. 
En caso de un error, en la asignacion de memoria, libera todo los bloques pedido hasta el momento para una estructura `TP` y devuelve `NULL`.
```c
    if (tp->pista_1)
        free(tp->pista_1);

    if (tp->pista_2)
        free(tp->pista_2);

    if (tp->roster_pokemon)
        free(tp->roster_pokemon);

    if (tp->jugador1)
        free(tp->jugador1);

    if (tp->jugador2)
        free(tp->jugador2);

    if (archivo)
        fclose(archivo);

    return NULL;
```
Se crea un árbol binario de búsqueda para almacenar la información de los Pokémon. Si falla, se maneja el error adecuadamente. Las funciones para la logica del abb estan definidas en el archivo `abb.h`. Para la creacion del abb la funcion `abb_crear` necesita como parametro un funcion comparador que en este caso pertenece a las funciones privadas del archivo `tp.c` y esta definida como `comparador_pokemones`.
```c
    struct pokemon_info *poke_1 = (struct pokemon_info *)dato_1;
    struct pokemon_info *poke_2 = (struct pokemon_info *)dato_2;

    return strcmp(poke_1->nombre, poke_2->nombre);
```
Se crean dos listas que forman parte de la estructura TP. Nuevamente, si ocurre algún error, se maneja con finalizacion_abrupta. Las funciones para la logica de las listas estan definidas en el archivo 
`lista.h`.
Se asigna memoria para dos estructuras de datos de jugadores. Cualquier falla se maneja apropiadamente. 
Se lee el archivo línea por línea. Para cada línea:
Se verifica si tiene el formato correcto con `tiene_el_formato_correcto`, se elimina el carácter de nueva línea, se divide la línea en componentes con `split`, se crea una estructura `pokemon_info` y se inserta en el ABB y se libera la memoria utilizada por `split`.
La funcion `tiene_el_formato_correcto` cuenta la cantidad de `','` la linea que se esta leyendo, en caso de encontrar una cantidad distina a la desperada devuelve `false` indicando que el archivo no tiene el formato correct. Recibe un `char*` que es la linea del archivo.
```c
    int contador = 0;
    while (*linea_de_archivo) {
        if (*linea_de_archivo == DELIMITADOR)
            contador++;
        linea_de_archivo++;
    }
    return contador == CANTIDAD_ESPERADA;
```
Para destruir el retorno de la funcion `split` se utiliza la funcion privada del archivo `tp.c`, 
`split_destruir` que se encarga de liberar la memoria reservada por la funcion `split`.
```c
    int i = 0;
    while (split[i] != NULL) {
        free(split[i]);
        i++;
    }
    free(split);
```
Para crear nuevas estrcturas de tipo `pokemon_info` se utiliza la funcion privada del archivo 
`crear_pokemon` que reserva memoria y asigna los valores de la linea de archivo que recibe.
```c
    struct pokemon_info *nuevo_pokemon =
        malloc(sizeof(struct pokemon_info));
    if (!nuevo_pokemon)
        return NULL;

    for (int i = 0; linea_de_archivo[i]; i++) {
        switch (i) {
        case POSICION_NOMBRE:
            to_lower(linea_de_archivo[i]);
            linea_de_archivo[i][strcspn(linea_de_archivo[i], "\n")] =
                '\0';
            nuevo_pokemon->nombre = strdup2(linea_de_archivo[i]);
            if (!nuevo_pokemon->nombre) {
                free(nuevo_pokemon);
                return NULL;
            }
            break;
        case POSICION_FUERZA:
            nuevo_pokemon->fuerza = atoi(linea_de_archivo[i]);
            break;
        case POSICION_DESTREZA:
            nuevo_pokemon->destreza = atoi(linea_de_archivo[i]);
            break;
        case POSICION_INTELIGENCIA:
            nuevo_pokemon->inteligencia = atoi(linea_de_archivo[i]);
            break;
        }
    }
    return nuevo_pokemon;
```

La función `tp_cantidad_pokemon` recibe un puntero a una estructura `TP` y devuelve la cantidad de Pokémon en su árbol binario de búsqueda (ABB). Si el puntero `TP` es `NULL`, la función retorna 0, indicando que no hay Pokémon.
La función comienza verificando si el puntero tp es `NULL`. Si tp es `NULL`, se retorna 0, indicando que no hay Pokémon disponibles.
Si tp no es `NULL`, la función llama a `abb_tamanio`, pasando el puntero al árbol binario de búsqueda 
`(roster_pokemon)` de la estructura `TP`. La función `abb_tamanio` devuelve el tamaño del ABB, es decir, la cantidad de elementos que contiene. Este valor se convierte explícitamente a un entero y se retorna.


La función busca un Pokémon en el ABB (árbol binario de búsqueda) de una estructura `TP` usando el nombre proporcionado. Si encuentra el Pokémon, devuelve un puntero a la información del mismo. Si no lo encuentra o si los argumentos son inválidos, retorna `NULL`.
La función primero verifica si tp o nombre son `NULL`. Si cualquiera de ellos es `NULL`, la función retorna `NULL`. La función duplica el string nombre usando `strdup2`. Esto crea una copia del nombre para manipularlo sin alterar el original. Si `strdup2` falla y retorna `NULL` (por ejemplo, por falta de memoria), la función también retorna `NULL`.
```c
    size_t len = strlen(str) + 1;
    char *dst = malloc(len * sizeof(char));
    if (!dst)
        return NULL;
    strcpy(dst, str);
    return dst;
``` 
Convierte el string duplicado aux a minúsculas. Asegurando que la comparación no dependa de mayúsculas o minúsculas. Se crea una estructura `pokemon_info` temporal `pokemon_aux` con el nombre convertido a minúsculas. Los demás campos `(fuerza, destreza, inteligencia)` se inicializan a 0, ya que solo se utiliza el nombre para la búsqueda. Se realiza la búsqueda en el ABB `roster_pokemon` de la estructura 
`TP`. La función `abb_buscar` busca un nodo que coincida con `pokemon_aux` y devuelve un puntero a la información del Pokémon encontrado. Si no se encuentra ningún Pokémon, devuelve `NULL`. Se libera la memoria asignada para aux ya que ya no se necesita. Esto evita fugas de memoria. La función retorna el puntero pokemon_buscado, que apunta a la información del Pokémon encontrado o `NULL` si no se encontró.

La función `tp_nombres_disponibles` devuelve una cadena con los nombres de todos los Pokémon disponibles en una estructura `TP`. Esta cadena contiene los nombres de los Pokémon, separados por comas. Si el puntero `TP` es `NULL`, la función retorna `NULL`.
La función comienza verificando si el puntero tp es `NULL`. Esta verificación es importante para evitar desreferencias de punteros nulos. Si tp es `NULL`, la función retorna `NULL`. Se llama a la función 
`tp_cantidad_pokemon` para obtener la cantidad de Pokémon en el ABB de la estructura `TP`. El valor se almacena en la variable tamanio.
Se declara un array `todos_los_nombres` de tamaño tamanio para almacenar los nombres de los Pokémon. Luego, se llama a la función `abb_recorrer` para recorrer el ABB en orden `(INORDEN)` y llenar el array 
`todos_los_nombres` con los nombres de los Pokémon. El número de elementos recorridos se almacena en cantidad_elementos.
Se convierte el array `todos_los_nombres` a un array de punteros a `struct pokemon_info`. Esto se hace para facilitar el uso posterior en la función `crear_string_nombres`.
Se llama a la función `crear_string_nombres` pasando el array de punteros a struct `pokemon_info`, la cantidad de elementos y la estructura TP. Esta función devuelve una cadena con los nombres de los Pokémon separados por comas, que es lo que retorna `tp_nombres_disponibles`.

`crear_sring_nombres` crea una cadena con los nombres de los Pokémon disponibles. Los Pokémon disponibles son aquellos que no fueron seleccionados por el jugador 1 o el jugador 2. La cadena resultante contiene los nombres de los Pokémon separados por comas. Si no hay Pokémon disponibles, la cadena está vacía. La función devuelve la cadena resultante o `NULL` en caso de error.
Se define el tamaño inicial de la cadena resultante `(TAMANIO_INICIAL)`, se asigna memoria para la cadena resultante y si `malloc` falla y devuelve `NULL`, la función retorna `NULL`.
Se inicializa la cadena resultante con el carácter nulo `(CARACTER_NULO)`, indicando que la cadena está vacía.
Se itera sobre cada Pokémon en array_pokemones. Para cada Pokémon, se verifica si está disponible usando la función `esta_disponible`, que comprueba si el nombre del Pokémon esta seleccionado o no.
Si el Pokémon está disponible:
Se incrementa el tamaño necesario de la cadena resultante para incluir el nombre del Pokémon y una coma, se realoca memoria para resultado con el nuevo tamaño. Si `realloc` falla, la función retorna `NULL` y 
se añade el nombre del Pokémon y una coma a la cadena resultado usando `strcat`.
Después de la iteración, si hay al menos un Pokémon disponible, se reemplaza la última coma por el carácter nulo `('\0')` para terminar correctamente la cadena.
La función devuelve la cadena resultante con los nombres de los Pokémon disponibles, separados por comas.
```c
    size_t tamanio = TAMANIO_INICIAL;
    char *resultado = malloc(tamanio * sizeof(char));
    if (!resultado)
        return NULL;

    resultado[0] = CARACTER_NULO;

    for (size_t i = 0; i < cantidad_pokemones; i++) {
        if (esta_disponible(tp->jugador1->pokemon,
                    tp->jugador2->pokemon,
                    array_pokemones[i]->nombre)) {
            struct pokemon_info *pokemon = array_pokemones[i];
            tamanio += strlen(pokemon->nombre) + 1;
            resultado = realloc(resultado, tamanio * sizeof(char));
            if (!resultado)
                return NULL;
            strcat(resultado, pokemon->nombre);
            strcat(resultado, ",");
        }
    }

    if (cantidad_pokemones > 0) {
        resultado[tamanio - 2] = '\0';
    }

    return resultado;
```

La función `tp_seleccionar_pokemon` permite a un jugador (especificado por el parámetro jugador) seleccionar un Pokémon (especificado por el parámetro nombre) dentro de una estructura `TP`. La función verifica si el Pokémon existe y está disponible antes de asignarlo al jugador. Devuelve `true` si el Pokémon fue exitosamente seleccionado, y `false` en caso contrario.
Se verifica si los punteros tp y nombre son válidos. Si alguno de ellos es `NULL`, la función retorna 
`false`, evitando desreferencias de punteros nulos.
Se llama a la función `tp_buscar_pokemon` para buscar un Pokémon en la estructura `TP` con el nombre proporcionado y si no se encuentra el Pokémon `(tp_buscar_pokemon devuelve NULL)`, la función retorna 
`false`.
Se llama a la función `esta_disponible` para verificar si el Pokémon está disponible, es decir, si no ha sido seleccionado por ninguno de los jugadores y si el Pokémon no está disponible, la función retorna 
`false`. 
Se verifica el valor del parámetro jugador. Si es `JUGADOR_1`, se llama a pokemon_elegido para asignar el Pokémon al jugador 1. Si la asignación es exitosa `(pokemon_elegido no devuelve NULL)`, la función retorna `true`. Si jugador es `JUGADOR_2`, se llama a `pokemon_elegido` para asignar el Pokémon al jugador 2. Si la asignación es exitosa, la función retorna `true`. Si jugador no es `JUGADOR_1` ni 
`JUGADOR_2`, la función retorna `false`.

La función `pokemon_elegido` asigna un Pokémon a un jugador, marcándolo como seleccionado, y devuelve un puntero a la información del Pokémon seleccionado.
Se asigna el puntero pokemon al miembro pokemon de la estructura jugador.
El cast a `(struct pokemon_info *)` se utiliza para eliminar la constancia del puntero pokemon para permitir la asignación. 
Se establece el miembro seleccionado de la estructura jugador a `true`, indicando que el jugador ha seleccionado un Pokémon. 
La función retorna el puntero al Pokémon seleccionado, que ahora es parte de la estructura jugador.
```c
    jugador->pokemon = (struct pokemon_info *)pokemon;
    jugador->seleccionado = true;

    return jugador->pokemon;
```

La función `tp_pokemon_seleccionado` devuelve un puntero a la estructura `pokemon_info` del Pokémon seleccionado por el jugador especificado en una estructura `TP`. Si el puntero `TP` es `NULL` o si el valor de jugador no es válido, la función retorna `NULL`.
La función comienza verificando si el puntero tp es `NULL`. Esta verificación es importante para evitar desreferencias de punteros nulos. Si tp es `NULL`, la función retorna `NULL`.
La función verifica el valor del parámetro jugador:
Si jugador es `JUGADOR_1`, retorna el puntero al Pokémon seleccionado por el jugador 1 
`(tp->jugador1->pokemon)`. Si jugador es `JUGADOR_2`, retorna el puntero al Pokémon seleccionado por el jugador 2 `(tp->jugador2->pokemon)`. Si jugador no es ni `JUGADOR_1` ni `JUGADOR_2`, la función retorna 
`NULL`.

La función `tp_agregar_obstaculo` permite agregar un obstáculo a la pista de un jugador (ya sea jugador 1 o jugador 2) en una estructura `TP`. Retorna el tamaño de la lista de obstáculos del jugador correspondiente después de agregar el nuevo obstáculo. Si la estructura `TP` no está inicializada (tp es 
`NULL`), la función retorna 0.
La función comienza verificando si el puntero tp es `NULL`. Si lo es, significa que la estructura `TP` no está inicializada correctamente, por lo tanto, la función retorna 0 indicando un error o condición no válida.
Se reserva memoria dinámica para un entero `(int)` a través de `malloc`. Si la asignación de memoria falla (cuando `malloc` devuelve `NULL`), la función retorna 0, indicando que no se pudo agregar el obstáculo debido a falta de memoria.
Se asigna el valor del obstáculo (de tipo `enum TP_OBSTACULO`) al puntero `obstaculo_ptr`. Esto permite manejar dinámicamente el obstáculo dentro de la estructura `TP`.
Si jugador es `JUGADOR_1`, se llama a `agregar_obstaculo_segun_tipo` para agregar el obstáculo al jugador 1 en la posición especificada. Luego, la función retorna el tamaño de la lista de obstáculos de la pista del jugador 1. Si jugador es `JUGADOR_2`, se hace lo mismo pero para el jugador 2. Si jugador no es ni 
`JUGADOR_1` ni `JUGADOR_2`, la función retorna 0.
Si la función llega a este punto (por ejemplo, si jugador no tiene un valor válido), retorna 0 como un valor por defecto o indicador de que no se realizó ninguna acción válida.

La función `tp_quitar_obstaculo` tiene como objetivo quitar un obstáculo de la pista de un jugador especificado `(JUGADOR_1 o JUGADOR_2)` en una estructura `TP`. Retorna la cantidad de obstáculos restantes en la pista del jugador después de quitar el obstáculo en la posición especificada. Si la estructura `TP` no está inicializada (tp es `NULL`), la función retorna 0.
La función comienza verificando si el puntero tp es `NULL`. Si es así, la función retorna 0. Esto evita problemas de desreferencia de punteros nulos y asegura que la estructura `TP` esté correctamente inicializada antes de continuar.
Si jugador es `JUGADOR_1`, la función llama a obstaculos_restantes_en_pista pasando como argumentos la pista del jugador 1 `(tp->pista_1)` y la posición del obstáculo a quitar. Si jugador es `JUGADOR_2`, hace lo mismo pero para el jugador 2.
La posición del obstáculo a quitar, y devuelve la cantidad de obstáculos restantes en la pista después de quitar el obstáculo en esa posición. 
Si jugador no es ni `JUGADOR_1` ni `JUGADOR_2`, la función retorna 0. Esto podría ser útil como un valor por defecto o para indicar que la operación no se realizó correctamente debido a un valor de jugador no válido.

La función `tp_obstaculos_pista` devuelve una representación en forma de cadena de caracteres de los obstáculos presentes en la pista de un jugador específico `(JUGADOR_1 o JUGADOR_2)` dentro de una estructura `TP`. Retorna `NULL` si la estructura `TP` no está inicializada `(tp es NULL)` o si el jugador especificado no es válido.
La función comienza verificando si el puntero tp es `NULL`. Si es así, la función retorna `NULL`, indicando que la estructura `TP` no está inicializada correctamente.
Si jugador es `JUGADOR_1`, la función llama a `string_obstaculos` pasando como argumento `tp->pista_1`, que representa la pista de obstáculos del jugador 1 en la estructura `TP`.
Si jugador es `JUGADOR_2`, se llama a `string_obstaculos` con `tp->pista_2`, que representa la pista de obstáculos del jugador 2 en la estructura `TP`.
Ambas llamadas delegan la tarea de convertir los obstáculos en cadena de caracteres a la función 
`string_obstaculos`.

La función `string_obstaculos` recibe una lista pista que contiene punteros a obstáculos (representados como enteros) y crea una cadena de caracteres `(char *)` que representa estos obstáculos en formato de cadena. Cada tipo de obstáculo (definido por los valores de enumeración `OBSTACULO_FUERZA`,
`OBSTACULO_DESTREZA`, `OBSTACULO_INTELIGENCIA`) se convierte en un carácter específico (`FUERZA`, 
`DESTREZA`, `INTELIGENCIA`). Si la lista pista está vacía, la función retorna `NULL`.
```c
    if (lista_vacia(pista))
        return NULL;

    size_t cantidad_obstaculos = lista_tamanio(pista);
    char *str_obstaculos = malloc((cantidad_obstaculos + 1) * sizeof(char));
    if (!str_obstaculos)
        return NULL;

    int i = 0;
    lista_iterador_t *iterador_externo = lista_iterador_crear(pista);

    for (; lista_iterador_tiene_siguiente(iterador_externo);
         lista_iterador_avanzar(iterador_externo)) {
        int *obstaculo_ptr =
            lista_iterador_elemento_actual(iterador_externo);

        if (*obstaculo_ptr == OBSTACULO_FUERZA) {
            str_obstaculos[i] = FUERZA;
        } else if (*obstaculo_ptr == OBSTACULO_DESTREZA) {
            str_obstaculos[i] = DESTREZA;
        } else if (*obstaculo_ptr == OBSTACULO_INTELIGENCIA) {
            str_obstaculos[i] = INTELIGENCIA;
        }
        i++;
    }
    str_obstaculos[i] = CARACTER_NULO;

    lista_iterador_destruir(iterador_externo);
    return str_obstaculos;
```








 
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
