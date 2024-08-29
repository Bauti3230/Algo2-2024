<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA HASH

## Repositorio de Juan Bautista Fonseca - 111456 - jfonseca@fi.uba.ar

- Para compilar:

```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g src/*.c pruebas_alumno.c -o pruebas_alumno

gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g src/*.c ejemplo.c -o ejemplo
```

- Para ejecutar:

```bash
./pruebas_alumno

./ejemplo
```

- Para ejecutar con valgrind:
```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes ./pruebas_alumno

valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes ./ejemplo
```
---
El TP apuntaba a la implementacion de un Diccionario mediante un Hash cerrado de direccionamiento. Para poder implementar esto defini una estrucutra que la denomine `par_t`, esta estrcutura tiene una variable 
`void*` donde se almacena la variable (Se utiliza `void*` para poder guardar cualquier tipo de dato), una variable booleana que nos indica si ese elemento fue borrado y un `const char*` donde se almacena un copia de la clave. Esto ultimo, de guardar una copia, se hace para que si se modifica la clave origianl no se vea afectada la clave guardada en la tabla.

Para el `Hash` utilice un vector dinamico de `par_t*`, con esto se emula una tabla y dos variables 
`size_t` donde en una de las variables donde se almacena la capacidad de elementos que puede almacenar la tabla y otra variable que lleva la cuenta de la cantidad de elementos que hay en la tabla.

<div align="center">
<img width="70%" src="img/Par y Heap.png">
</div>

Una observaciones, es que ya viene dado el factor de carga, el cual nos inidca que cuando se supere o iguale el 75% de capacidad en la tabla, indica que se efectua un rehash.

Las funciones que se implementaron en este TP son : `hash_crear`, `hash_insertar`, `hash_quitar`, 
`hash_obtener`, `hash_contiene`, `hash_cantidad`, `hash_destruir`, `hash_destruir_todo` y
`hash_con_cada_clave`.
Paso a explicar el funcionamiento y decisiones de implementacion de las mismas.

En la funcion `hash_crear` se recibe como parametro un capacidad y devuelve un `hash_t*`. Si la capacidad es menor a una capacidad minima (Que en este caso es 3) se crea un hash con la capacidad minima, caso contrario se crea con la capacidad pasada por parametro. 
Primero reservamos memoria para un nuevo Hash, es indistinto es uso de `calloc` o `malloc` en este caso.
Se reserva memoria para la tabla utilizando `calloc`, es importante utilizar `calloc` ya queremons que las variables `void*` y `const char*` se inicialicen en `NULL` y la variable booleana en `false`.
Luego se asigna la capacidad ingresada a la varible `capacidad` y la variable `cantidad` se inicializa en
0. Por ultimo se retorn el hash que acabamos de crear.

En la funcion `hash_insertar` se recibe como parametro un `hash_t*`, una clave de tipo `const char*`,
un elemento `void*` y un anterior `void**`. 
Primero se verifica que tanto el hash como la clave sean validad, se verifica que si el factor de carga superado o igualada, en caso de que ocurra hacemos una llamada a la funcion privada del archvio `hash.c` 
`rehahs`. La funcion de rehash crea un nuevo hash con la capacidad original duplicada recorre la tabla de nuestro hash y va insertando en el nuevo los elementos y por ultimo devolvemos el nuevo hash. Si no se tiene que rehashear insertamos el elemento, si clave que se resive como parametro esta repetida modificamos el valor asociada a esa clave y si la variable anterior no es nula almacenamos la variable que acabamos de sutituir en anterior. En el caso de que no existe la clave en el hash, insertamos el elemento en la posicion, utilizando probing linela, si la variable anterior no es nula la modificamos para que si lo sea.

```c
    hash_t *nuevo_hash = hash_crear(hash->capacidad * 2);
    if (!nuevo_hash)
        return NULL;

    for (size_t i = 0; i < hash->capacidad; i++) {
        hash_insertar(nuevo_hash, hash->tabla[i].key,
                  hash->tabla[i].value, NULL);
    }

    hash_t aux = *hash;
    *hash = *nuevo_hash;
    *nuevo_hash = aux;

    hash_destruir(nuevo_hash);
    return hash;
```

La función `hash_quitar` se utiliza para eliminar una clave y su valor asociado de una tabla hash. La función toma dos argumentos: un puntero a la tabla hash `(hash_t *hash)` y la clave que se desea eliminar `(const char *clave)`. Si se encuentra la clave, la función elimina la entrada correspondiente y devuelve el valor asociado. Si no se encuentra la clave, devuelve NULL. 

La función `hash_obtener` busca y devuelve el valor asociado a una clave dada en una tabla hash. Si la clave no se encuentra en la tabla, la función retorna `NULL`.
La función verifica si los punteros hash y clave son válidos. Si alguno de ellos es `NULL`, la función retorna `NULL`. Se calcula la posición inicial en la tabla hash utilizando una función de hash aplicada a la clave, y luego se toma el módulo con la capacidad de la tabla `(hash->capacidad)`. La función recorre la tabla hash comenzando desde la posición calculada y manejando posibles colisiones mediante sondeo lineal. 
Entrada Vacía no Marcada como Borrada: Si la clave en la posición actual es `NULL` y el campo borrado es false, significa que la clave no existe en la tabla, y se retorna `NULL`. 
Clave Encontrada: Si la clave en la posición actual coincide con la clave buscada, se retorna el valor asociado a esa clave.
Entrada Vacía Marcada como Borrada: Si la clave en la posición actual es NULL pero el campo borrado es true, se sigue buscando en la siguiente posición.

La función `hash_contiene` verifica si una clave dada existe en la tabla hash. Retorna `true` si la clave se encuentra en la tabla y `false` si no.
La función verifica si los punteros hash y clave son válidos. Si alguno de ellos es `NULL`, la función retorna `false`.
Se calcula la posición inicial en la tabla hash utilizando una función de hash aplicada a la clave, y luego se toma el módulo con la capacidad de la tabla `(hash->capacidad)`.
La función recorre la tabla hash comenzando desde la posición calculada y manejando posibles colisiones mediante sondeo lineal. 
Entrada Vacía no Marcada como Borrada: Si la clave en la posición actual es `NULL` y el campo borrado es false, significa que la clave no existe en la tabla, y se retorna `false`.
Clave Encontrada: Si la clave en la posición actual coincide con la clave buscada, se retorna true.
Entrada Vacía Marcada como Borrada: Si la clave en la posición actual es `NULL` pero el campo borrado es 
`true`, se sigue buscando en la siguiente posición.

La función hash_cantidad se utiliza para obtener el número de elementos almacenados actualmente en una tabla hash. Esta función es útil para conocer la cantidad de pares clave-valor presentes en la tabla, lo que puede ser necesario para diversas operaciones como análisis de carga, estadísticas, o simplemente para controlar el tamaño de la estructura.
La función primero verifica si el puntero hash es válido (no `NULL`). Si el puntero es `NULL`, la función retorna 0 indicando que no hay elementos en la tabla hash. 
Si el puntero hash es válido, la función retorna el valor del campo cantidad de la estructura `hash_t`. Este campo cantidad representa el número de elementos actualmente almacenados en la tabla hash.

La función hash_destruir libera la memoria utilizada por una tabla hash y sus elementos. Esta función es esencial para evitar fugas de memoria al finalizar el uso de la tabla hash.
La función verifica si el puntero hash es válido (no NULL). Si el puntero es NULL, la función retorna inmediatamente sin realizar ninguna operación.
La función recorre cada entrada en la tabla hash (hash->tabla) y libera la memoria asignada a las claves (key). Si key es NULL, se omite esa entrada.
Se libera la memoria utilizada por la tabla hash (hash->tabla) y finalmente la estructura hash en sí misma.

La función hash_destruir_todo no solo libera la memoria utilizada por la tabla hash y sus claves, sino que también permite la liberación de la memoria asociada a los valores mediante un destructor personalizado. Esta función es útil cuando los valores almacenados en la tabla hash requieren una gestión de memoria específica.
Similar a hash_destruir, la función verifica si el puntero hash es válido. Si es NULL, retorna inmediatamente.
Si key no es NULL, se verifica si hay un destructor proporcionado.
Si hay un destructor, se llama a la función destructora pasando el valor (value) como argumento para liberar cualquier recurso asociado al valor.
Luego, se libera la memoria asignada a la clave (key).
Finalmente, se libera la memoria utilizada por la tabla hash (hash->tabla) y la estructura hash en sí misma.

La función hash_con_cada_clave itera sobre cada clave-valor almacenado en una tabla hash y aplica una función pasada como argumento a cada par clave-valor. La función permite realizar operaciones específicas en todos los elementos de la tabla hash y puede interrumpirse si la función aplicada a un par clave-valor retorna false.
La función verifica si los punteros hash y f (función a aplicar) son válidos. Si alguno de ellos es NULL, la función retorna 0, indicando que no se realizaron llamadas a la función aplicada.
La función recorre cada entrada en la tabla hash (hash->capacidad veces). 
Clave No Nula: Si la clave en la posición actual no es NULL, se incrementa el contador de llamadas a la función (llamadas_a_funcion).
Aplicación de la Función: Se llama a la función f, pasando la clave, el valor asociado y el puntero auxiliar aux como argumentos.
Interrupción por Retorno Falso: Si la función f retorna false, se interrumpe la iteración y la función retorna el número de llamadas realizadas hasta ese punto.
Si la iteración completa se realiza sin interrupción, la función retorna el número total de llamadas realizadas a f.

## Respuestas a las preguntas teóricas
- Qué es un diccionario
    - Explicá 3 formas diferentes de implementar un diccionario (tabla de hash cuenta como 1)

    Un diccionario en programación es una estructura de datos que permite almacenar pares de clave-valor, donde cada clave es única y se asocia a un valor. Es muy útil para realizar búsquedas rápidas, ya que la clave se puede usar para acceder al valor correspondiente de manera eficiente.

    Una de las formas para implementar un diccionario podria ser utilizando vectores, ya sean dinamicos o estaticos. Para encarar esta forma hay varios metodos uno podria ser utilizando vectores paralelos, donde en uno se almecenen los datos y en el otro las claves, de esta manera emulariamos el formato de un diccionario donde cada clave esta asociada exclusivamente a un dato en el vector.

    Otro enfoque podria ser utilizando listas enlazadas, donde cada nodo alacene el dato y su clave asociada. Una forma de este metodo es la que utiliza los hash de direccionamiento cerrado, pero el metodo que planteo solo utilizaria una lista para almacenar los datos.

    Por ultimo y lo que fue implementado en el TP es utilizar una table de hash. Esto se logra teniendo un vector de pares o un vector de punteros a pares. La estrctura pares esta compuesta inicialmente por : un variable donde almacenar el dato y una copia de la clave en un string. Estos pares pueden tener flags, dependiendo de como se quiera implementar.

   - Qué es una función de hash y qué características debe tener

    Una función de hash es un algoritmo que toma una entrada (o "clave") y produce una salida fija, generalmente un número entero. Este valor se usa para indexar una estructura de datos, como una tabla hash, que permite un acceso rápido a los datos.
    La función de hash debe distribuir las entradas de manera uniforme a través del rango de salidas posibles. Esto minimiza las colisiones (cuando dos entradas diferentes producen el mismo hash) y asegura que las estructuras de datos hash, como las tablas hash, se utilicen de manera eficiente.
    La función de hash debe ser rápida de calcular. Dado que se llama con frecuencia durante operaciones de búsqueda, inserción y eliminación, su eficiencia es crucial para el rendimiento global de la estructura de datos.
    Aunque es imposible evitar las colisiones completamente, una buena función de hash debe minimizar su frecuencia. Esto se logra generalmente mediante una buena distribución de los resultados.
    La función de hash debe ser capaz de manejar cualquier entrada posible dentro del dominio de los datos. Esto incluye ser capaz de producir un hash válido para cualquier posible clave.

   - Qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos (encadenamiento, probing, etc)

    Cuando dos o más claves generan el mismo valor de hash, se produce una colisión. Existen varios métodos para manejar estas colisiones en una tabla hash.

    En el encadenamiento, cada entrada en la tabla hash apunta a una lista enlazada de todos los elementos que tienen el mismo valor de hash. Cuando ocurre una colisión, se añade el nuevo elemento al final de la lista correspondiente. Un metodo facil de implementar pero que podria ser costoso en termino de unidades de tiempo.

    En la dirección abierta, todas las entradas se almacenan en la propia tabla hash, y cuando ocurre una colisión, se busca otra posición libre en la tabla siguiendo una secuencia de sondeo. Los métodos de sondeo más comunes son:

    Probing Lineal donde se  busca la siguiente posición libre en la tabla de manera secuencial. Nuevamente es facil de implementar pero podria causar agrupamientos que nos llevaria a tener mas colisiones. Esto ultimo se podria solventar con una buena funcion de hash.

    Proging Cuadratico se busca la siguiente posición libre utilizando una secuencia cuadrática.

    Hashing Doble, se usa una segunda función de hash para determinar la secuencia de sondeo.
    Reduce los agrupamientos pero requiere una segunda funcion de hash independiente de la primera y como ya de por si es dificl encontrar una funcion de hash, esto lo complica aun mas.

    Tambien podrias calificar a el Rehash como un metodo para evitar colisiones ya que si la tabla hash se llena demasiado o si las colisiones se vuelven demasiado frecuentes, se puede crear una nueva tabla hash con el doble de tamaño (u otro tamaño apropiado) y se reinsertan todas las entradas en la nueva tabla. Esto generalmente se hace como parte de la gestión de capacidad dinámica.