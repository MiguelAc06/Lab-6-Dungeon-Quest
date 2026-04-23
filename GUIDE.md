# Guia de C — Lab 6: Dungeon Quest
> Referencia para el instructor. Cubre todos los conceptos usados en los ejercicios.
> Ideal para preparar laminas de clase.
>
> Los tres ejercicios del lab forman un mismo universo:
> **Ejercicio 1** — inventario del heroe (array dinamico, malloc/realloc, switch)
> **Ejercicio 2** — bestiario del dungeon (lista enlazada, strdup, free)
> **Ejercicio 3** — combate contra el jefe (if/else, operaciones aritmeticas)

---

## Tabla de contenidos

1. [Tipos de datos primitivos](#1-tipos-de-datos-primitivos)
2. [Variables y declaracion](#2-variables-y-declaracion)
3. [Entrada y salida: printf y scanf](#3-entrada-y-salida-printf-y-scanf)
4. [Operadores](#4-operadores)
5. [Condicionales: if / else if / else](#5-condicionales-if--else-if--else)
6. [Switch](#6-switch)
7. [Loops: for, while, do-while](#7-loops-for-while-do-while)
8. [Arrays](#8-arrays)
9. [Punteros](#9-punteros)
10. [Memoria dinamica: malloc, realloc, free](#10-memoria-dinamica-malloc-realloc-free)
11. [Cadenas de texto (strings)](#11-cadenas-de-texto-strings)
12. [Estructuras (struct)](#12-estructuras-struct)
13. [Listas enlazadas](#13-listas-enlazadas)
14. [Copia vs. referencia: char* en structs](#14-copia-vs-referencia-char-en-structs)

---

## 1. Tipos de datos primitivos

C es un lenguaje de tipado estatico: cada variable tiene un tipo fijo que se declara antes de usarla.

| Tipo     | Tamano tipico | Rango / Uso                       | Ejemplo de literal |
|----------|---------------|-----------------------------------|--------------------|
| `char`   | 1 byte        | Caracter ASCII o entero -128..127 | `'A'`, `'W'`       |
| `int`    | 4 bytes       | Entero                            | `42`, `-7`         |
| `float`  | 4 bytes       | Decimal de precision simple       | `3.14f`            |
| `double` | 8 bytes       | Decimal de doble precision        | `3.14159`          |

> **Nota sobre `char`:** un `char` es un numero entero de 8 bits. Se puede usar
> como caracter (`'W'`, `'A'`, `'P'`) o como entero pequeno. Los caracteres se
> escriben entre comillas simples; las cadenas de texto entre comillas dobles.

### Conversion de tipos (casting)

Cuando mezclas `int` y `float` en una division, C hace division entera si
ambos operandos son `int`. Para obtener el resultado decimal hay que convertir:

```c
int a = 10, b = 3;
int   entero  = a / b;          /* resultado: 3  (pierde el decimal) */
float exacto  = (float)a / b;   /* resultado: 3.333... */
```

El operador `(tipo)` delante de una variable fuerza la conversion antes de operar.

---

## 2. Variables y declaracion

```c
int    nivel   = 5;          /* entero */
float  peso    = 3.50f;      /* decimal — la f indica float, no double */
char   tipo    = 'W';        /* caracter — comillas simples */
char   nombre[32] = "Kael";  /* array de chars = cadena de texto */
```

- Las variables locales (dentro de una funcion) viven en el **stack** y
  desaparecen cuando la funcion termina.
- La memoria pedida con `malloc` vive en el **heap** y persiste hasta
  que se llama a `free`.

---

## 3. Entrada y salida: printf y scanf

### printf — imprimir en pantalla

```c
printf("Texto plano\n");
printf("Entero:    %d\n",  nivel);
printf("Decimal:   %.2f\n", peso);   /* .2 = dos decimales */
printf("Caracter:  %c\n",  tipo);
printf("Cadena:    %s\n",  nombre);
```

### Especificadores de formato

| Especificador | Tipo     | Ejemplo                    | Imprime              |
|---------------|----------|----------------------------|----------------------|
| `%d`          | `int`    | `printf("%d", 42)`         | `42`                 |
| `%f`          | `float`  | `printf("%f", 3.5f)`       | `3.500000`           |
| `%.2f`        | `float`  | `printf("%.2f", 3.5f)`     | `3.50`               |
| `%c`          | `char`   | `printf("%c", 'A')`        | `A`                  |
| `%s`          | `char *` | `printf("%s", "Hola")`     | `Hola`               |
| `%-10s`       | `char *` | `printf("%-10s", "Hola")`  | `Hola      ` (izq)   |
| `%10s`        | `char *` | `printf("%10s",  "Hola")`  | `      Hola` (der)   |

### scanf — leer del teclado

```c
int opcion;
scanf("%d", &opcion);   /* lee un entero — nota el & */

float peso;
scanf("%f", &peso);     /* lee un decimal */
```

> `scanf` necesita la **direccion** de la variable (`&variable`) para escribir
> el valor leido en ella. Sin el `&` el programa falla o produce resultados erroneos.

---

## 4. Operadores

### Aritmeticos

| Operador | Significado     | Ejemplo          | Resultado |
|----------|-----------------|------------------|-----------|
| `+`      | Suma            | `10 + 3`         | `13`      |
| `-`      | Resta           | `10 - 3`         | `7`       |
| `*`      | Multiplicacion  | `10 * 3`         | `30`      |
| `/`      | Division        | `10 / 3`         | `3` (entero) |
| `%`      | Modulo (resto)  | `10 % 3`         | `1`       |

### Comparacion

| Operador | Significado   |
|----------|---------------|
| `==`     | igual a       |
| `!=`     | diferente de  |
| `<`      | menor que     |
| `>`      | mayor que     |
| `<=`     | menor o igual |
| `>=`     | mayor o igual |

### Logicos

| Operador | Significado                          | Ejemplo                   |
|----------|--------------------------------------|---------------------------|
| `&&`     | AND — ambas condiciones verdaderas   | `hp > 0 && turno < 10`    |
| `\|\|`   | OR — al menos una condicion verdadera| `vivo == 1 \|\| hp > 0`   |
| `!`      | NOT — invierte el valor booleano     | `!is_alive(enemy)`        |

---

## 5. Condicionales: if / else if / else

Ejecuta codigo segun una condicion. En C, `0` es falso y cualquier otro valor es verdadero.

```c
if (condicion) {
    /* se ejecuta si condicion es verdadera */
} else if (otra_condicion) {
    /* se ejecuta si la primera falla y esta es verdadera */
} else {
    /* se ejecuta si ninguna condicion anterior fue verdadera */
}
```

> **Regla para rangos:** evalua de mayor a menor. Una vez que una condicion
> es verdadera, las siguientes no se evaluan.

### Patron: verificar antes de operar

```c
if (b == 0) {
    printf("Error: division entre cero\n");
    return;   /* salir sin continuar */
}
float resultado = (float)a / b;  /* seguro: b != 0 aqui */
```

### Patron: minimo garantizado (util en combate)

```c
int dmg = atk - def;
if (dmg < 1) {
    dmg = 1;   /* siempre hay al menos 1 de danio */
}
```

---

## 6. Switch

`switch` compara una expresion entera o `char` contra valores constantes exactos.
Es mas claro que una cadena de `if/else if` cuando los casos son valores discretos.

```c
switch (expresion) {
    case VALOR_A:
        /* codigo */
        break;      /* sin break la ejecucion "cae" al caso siguiente */
    case VALOR_B:
        /* codigo */
        break;
    default:
        /* se ejecuta si ningun caso coincide */
        break;
}
```

> **`break` es obligatorio** al final de cada caso para detener la ejecucion.

### Usar switch con char — clasificar un item

```c
char tipo = 'W';
switch (tipo) {
    case 'W': printf("Arma");     break;
    case 'A': printf("Armadura"); break;
    case 'P': printf("Pocion");   break;
    default:  printf("Misc");     break;
}
```

### Usar switch como menu interactivo

```c
int opcion;
scanf("%d", &opcion);

switch (opcion) {
    case 1: exercise1(); break;
    case 2: exercise2(); break;
    case 3: exercise3(); break;
    case 0:
        exercise1();
        exercise2();
        exercise3();
        break;
    default:
        printf("Opcion invalida.\n");
        break;
}
```

### Cuando usar switch vs. if/else

| Situacion                                       | Usar         |
|-------------------------------------------------|--------------|
| Valor exacto contra constantes (`'W'`, `1`, `2`)| `switch`     |
| Rangos numericos (`hp >= 50`, `score < 60`)     | `if/else if` |
| Condiciones con `&&` o `\|\|`                   | `if/else`    |

---

## 7. Loops: for, while, do-while

### for — cuando sabes cuantas veces iterar

```c
for (inicializacion; condicion; incremento) {
    /* cuerpo */
}

/* recorrer un array de n elementos */
for (int i = 0; i < n; i++) {
    printf("%d\n", arr[i]);
}
```

### while — mientras se cumpla una condicion

```c
while (condicion) {
    /* cuerpo */
}

/* recorrer una lista enlazada */
Enemy *cur = head;
while (cur != NULL) {
    printf("%s\n", cur->name);
    cur = cur->next;
}
```

### while con prev y cur — eliminar nodos mientras se recorre

Este patron es necesario cuando se quieren eliminar multiples nodos de una
lista enlazada sin detener el recorrido:

```c
Enemy *cur = *head, *prev = NULL;
while (cur != NULL) {
    if (cur->defeated == 1) {
        Enemy *siguiente = cur->next;   /* guardar antes del free */
        if (prev != NULL)
            prev->next = siguiente;     /* saltar el nodo */
        else
            *head = siguiente;          /* era el primero */
        free(cur->name);
        free(cur);
        cur = siguiente;                /* avanzar cur, prev NO se mueve */
    } else {
        prev = cur;
        cur  = cur->next;               /* avanzar ambos */
    }
}
```

> La clave es que `prev` solo avanza cuando NO se elimina el nodo actual.
> Asi se pueden eliminar nodos consecutivos correctamente.

### do-while — ejecuta al menos una vez

```c
do {
    /* cuerpo — se ejecuta antes de comprobar la condicion */
} while (condicion);
```

---

## 8. Arrays

Un array es una coleccion de elementos del mismo tipo en posiciones contiguas de memoria.

```c
int arr[5] = {10, 20, 30, 40, 50};
/*             ^                ^
 *          indice 0        indice 4  (siempre 0..n-1) */

arr[2] = 99;            /* modificar el tercer elemento */
printf("%d\n", arr[2]); /* leer el tercer elemento */
```

### Tamanio en tiempo de compilacion

```c
int n = sizeof(arr) / sizeof(arr[0]); /* numero de elementos */
```

### Limitacion

Los arrays estaticos no cambian de tamano en ejecucion.
Para un array que crece se usa `malloc` + `realloc` (ver seccion 10).

---

## 9. Punteros

Un puntero es una variable que almacena una **direccion de memoria**, no un valor directamente.

```c
int  hp  = 50;
int *ptr = &hp;   /* ptr guarda la direccion de hp */

printf("%d\n", *ptr); /* 50 — * desreferencia: lee el valor en esa direccion */
*ptr = 30;            /* escribe a traves del puntero — ahora hp vale 30 */
```

### Operadores de punteros

| Operador | Nombre        | Significado                                       |
|----------|---------------|---------------------------------------------------|
| `&x`     | Direccion de  | Obtiene la direccion de memoria de `x`            |
| `*p`     | Desreferencia | Lee o escribe el valor en la direccion `p`        |
| `p->f`   | Flecha        | Accede al campo `f` de la struct apuntada por `p` |

### El ejemplo del chismoso

Imagina tres personas: **Domingo** cuenta un cuento, **Jesus** lo repite,
**Leonardo** lo escucha. Podemos modelar esto con punteros en C.

**Version con copia** — cada quien tiene su propio texto:

```c
typedef struct { char name[32]; char *message; } Person;

Person teller   = {"Domingo",  NULL};
Person gossip   = {"Jesus",    NULL};
Person receiver = {"Leonardo", NULL};

teller.message  = strdup("Habia una vez un dragon...");
gossip.message  = strdup(teller.message);    /* copia independiente */
receiver.message = strdup(gossip.message);   /* otra copia */

/* Domingo cambia el cuento */
free(teller.message);
teller.message = strdup("Ahora el dragon come galletas...");

printf("%s\n", teller.message);   /* Ahora el dragon come galletas... */
printf("%s\n", receiver.message); /* Habia una vez un dragon...  <- version antigua */
```

```
teller.message   ──> [ Ahora el dragon come galletas...\0 ]
gossip.message   ──> [ Habia una vez un dragon...\0 ]
receiver.message ──> [ Habia una vez un dragon...\0 ]
```

Cada `strdup` creo un bloque independiente. Cambiar el de `teller` no afecta a los demas.

---

**Version compartida** — todos apuntan al mismo texto:

```c
char *shared = malloc(128);
strcpy(shared, "Habia una vez un dragon...");

Person teller   = {"Domingo",  shared};
Person gossip   = {"Jesus",    shared};   /* misma direccion */
Person receiver = {"Leonardo", shared};   /* misma direccion */

/* Modificar el buffer compartido */
strcpy(shared, "El dragon ahora es amigo de las galletas");

printf("%s\n", teller.message);   /* El dragon ahora es amigo... */
printf("%s\n", gossip.message);   /* El dragon ahora es amigo... */
printf("%s\n", receiver.message); /* El dragon ahora es amigo... */

free(shared); /* un solo free — todos apuntaban al mismo bloque */
```

```
teller.message   ──┐
gossip.message   ──┼──> [ El dragon ahora es amigo de las galletas\0 ]
receiver.message ──┘
```

> Este es el concepto de **aliasing**: varios punteros apuntan a la misma
> zona de memoria. Cambiar el contenido lo ven todos. Solo se hace `free` una vez.

### Puntero a puntero (`**`)

Se usa cuando una funcion necesita **modificar el puntero del llamante**.
Sin `**`, la funcion solo recibe una copia del puntero y los cambios no se
propagan hacia afuera:

```c
/* head es Enemy* en el llamante.
 * Para que la funcion pueda cambiar a donde apunta head,
 * se pasa como Enemy** */
void agregar(Enemy **head, const char *name, int level) {
    Enemy *nuevo = malloc(sizeof(Enemy));
    nuevo->name  = strdup(name);
    nuevo->level = level;
    nuevo->next  = *head;   /* *head lee el valor actual del puntero */
    *head = nuevo;          /* *head escribe — cambia el head del llamante */
}
```

Lo mismo aplica para arrays dinamicos en heap cuando la funcion puede hacer
`realloc` (que puede cambiar la direccion):

```c
void agregar_item(Item **inv, int *count, int *cap, ...) {
    if (*count == *cap) {
        *cap *= 2;
        *inv = realloc(*inv, *cap * sizeof(Item)); /* nueva direccion posible */
    }
    (*inv)[*count].weight = peso;   /* (*inv) desreferencia para acceder al array */
    (*count)++;
}
```

---

## 10. Memoria dinamica: malloc, realloc, free

La memoria del **heap** se gestiona manualmente. El programador la pide y la libera.

```
Stack (automatico)          Heap (manual)
┌──────────────┐           ┌──────────────────────┐
│  Item *inv   │ ────────> │  [Item][Item][Item]  │
│  int count   │           └──────────────────────┘
└──────────────┘
```

### malloc — reservar memoria

```c
#include <stdlib.h>

Item *inv = malloc(capacity * sizeof(Item));
if (!inv) {           /* malloc retorna NULL si falla — siempre verificar */
    perror("malloc");
    exit(EXIT_FAILURE);
}
```

### realloc — cambiar el tamano de un bloque

```c
capacity *= 2;
inv = realloc(inv, capacity * sizeof(Item));
/* Puede devolver una NUEVA direccion.
 * Si la funcion recibe inv como Item**, usa *inv = realloc(...) */
```

> **Importante:** despues de un `realloc` cualquier puntero que apuntaba a
> elementos del bloque anterior puede quedar invalido. Actualiza siempre los
> punteros derivados tras un realloc.

### free — liberar memoria

```c
free(inv);
inv = NULL;  /* buena practica: evita usar el puntero despues */
```

> Regla: **por cada `malloc`/`strdup` debe haber exactamente un `free`.**

### Ciclo completo: array que crece

```c
int cap = 2, n = 0;
Item *inv = malloc(cap * sizeof(Item));   /* 1. pedir */

/* ... agregar items ... */

/* lleno — duplicar */
cap *= 2;
inv = realloc(inv, cap * sizeof(Item));   /* 2. crecer */

/* ... agregar mas ... */

for (int i = 0; i < n; i++) free(inv[i].name); /* 3. liberar campos */
free(inv);                                       /* 4. liberar array */
```

---

## 11. Cadenas de texto (strings)

En C una cadena es un **array de `char`** terminado en el caracter nulo `'\0'`.

```c
char nombre[32] = "Kael";
/*  nombre[0]='K', nombre[1]='a', nombre[2]='e', nombre[3]='l', nombre[4]='\0' */
```

### Funciones de `<string.h>`

| Funcion               | Uso                                                        |
|-----------------------|------------------------------------------------------------|
| `strlen(s)`           | Numero de caracteres (sin contar el `'\0'`)                |
| `strcpy(dst, src)`    | Copia `src` en `dst` — `dst` debe tener suficiente espacio |
| `strncpy(dst, src, n)`| Copia hasta `n` caracteres — mas seguro que `strcpy`       |
| `strcmp(a, b)`        | Compara: 0 si iguales, != 0 si diferentes                  |
| `strdup(s)`           | Crea una copia en heap — requiere `free` al terminar       |

### strdup — la funcion clave de los ejercicios

```c
char *copia = strdup("Espada larga");
/* equivale a:
 *   char *copia = malloc(strlen("Espada larga") + 1);
 *   strcpy(copia, "Espada larga");
 */

free(copia);   /* obligatorio al terminar */
```

En los ejercicios los campos de nombre son `char *`. Hay que usar `strdup`
para que cada struct tenga su propia copia. Ver seccion 14 para el detalle.

### printf con strings

```c
printf("%-10s", nombre);   /* alinear a la izquierda en 10 chars */
printf("%10s",  nombre);   /* alinear a la derecha en 10 chars   */
```

---

## 12. Estructuras (struct)

Una `struct` agrupa campos de distintos tipos bajo un mismo nombre.
Se define despues de haber visto punteros porque frecuentemente sus
campos son punteros (`char *`, `struct Node *`).

```c
typedef struct {
    char  *name;     /* puntero a cadena en heap */
    char   type;     /* 'W', 'A', 'P' */
    float  weight;
} Item;
```

- `typedef struct { ... } Nombre;` permite usar `Nombre` como tipo directamente.
- Inicializacion con literales: `Item it = {strdup("Espada"), 'W', 3.50f};`

### Acceso a campos

```c
Item  it = ...;
Item *p  = &it;

it.weight    /* con variable directa: punto */
p->weight    /* con puntero: flecha  (equivale a (*p).weight) */
```

### Dos formas de campo de texto — con y sin puntero

```c
/* Array fijo — el texto vive dentro de la struct */
typedef struct { char name[32]; float weight; } ItemFijo;
ItemFijo a = {"Espada", 3.5f};
ItemFijo b = a;           /* copia completa — nombre incluido */

/* Puntero — el texto vive en heap, la struct solo guarda la direccion */
typedef struct { char *name; float weight; } ItemPtr;
ItemPtr c;
c.name   = strdup("Espada");   /* reserva y copia en heap */
c.weight = 3.5f;
ItemPtr d = c;    /* copia solo el puntero, NO el texto — ambos apuntan al mismo buffer */
```

> En los ejercicios los nombres son `char *`. Usar `=` entre structs copia
> solo la direccion. Para una copia independiente del texto se necesita `strdup`.
> Ver seccion 14.

### Struct que se apunta a si misma — nodo de lista

```c
typedef struct Enemy {
    char         *name;
    int           level;
    int           defeated;
    struct Enemy *next;    /* puntero al siguiente Enemy */
} Enemy;
```

El campo `next` usa `struct Enemy *` y no `Enemy *` porque el `typedef`
aun no esta completo en ese punto de la definicion.

---

## 13. Listas enlazadas

Una lista enlazada es una cadena de **nodos en heap** donde cada nodo
apunta al siguiente. No hay indices: para llegar al nodo N hay que
recorrer desde `head`.

```
head
 |
 v
[Rata | •] ──> [Esqueleto | •] ──> [Slime | NULL]
```

Cada nodo se crea con `malloc` y se elimina con `free`. Si el nodo tiene
campos `char *`, esos campos tambien necesitan su propio `free`.

### Crear un nodo

```c
Enemy *nuevo = malloc(sizeof(Enemy));
nuevo->name     = strdup(name);   /* copia independiente */
nuevo->level    = level;
nuevo->defeated = 0;
nuevo->next     = NULL;
```

### Agregar al final (append)

```c
if (*head == NULL) {
    *head = nuevo;
    return;
}
Enemy *cur = *head;
while (cur->next != NULL) {
    cur = cur->next;      /* avanzar hasta el ultimo */
}
cur->next = nuevo;
```

### Recorrer la lista

```c
int pos = 1;
Enemy *cur = head;
while (cur != NULL) {
    printf("%d. [Nv %d] %s\n", pos, cur->level, cur->name);
    pos++;
    cur = cur->next;
}
```

### Insertar en posicion N (base 1)

```c
/* Caso especial: insertar al principio */
if (pos <= 1 || *head == NULL) {
    nuevo->next = *head;
    *head = nuevo;
    return;
}
/* Caso general: avanzar (pos-1) pasos */
Enemy *cur = *head;
for (int i = 1; i < pos - 1 && cur->next != NULL; i++) {
    cur = cur->next;
}
nuevo->next = cur->next;
cur->next   = nuevo;
```

Diagrama para insertar en posicion 2 en `[A] -> [B] -> [C]`:

```
antes:  head -> [A] -> [B] -> [C] -> NULL
                 ^
                cur  (avanzo 1 paso)

nuevo->next = cur->next   =>  [NUEVO] -> [B]
cur->next   = nuevo       =>  [A]    -> [NUEVO]

despues: head -> [A] -> [NUEVO] -> [B] -> [C] -> NULL
```

### Liberar toda la lista

```c
while (head != NULL) {
    Enemy *siguiente = head->next;
    free(head->name);   /* primero el campo puntero */
    free(head);         /* luego el nodo */
    head = siguiente;
}
```

### Arrays vs. listas enlazadas

| Caracteristica          | Array estatico | Array dinamico | Lista enlazada |
|-------------------------|----------------|----------------|----------------|
| Acceso por indice       | O(1)           | O(1)           | O(n)           |
| Insertar al principio   | O(n)           | O(n)           | O(1)           |
| Insertar al final       | no posible     | O(n) / O(1)*   | O(n)           |
| Eliminar en medio       | O(n)           | O(n)           | O(n)           |
| Tamano variable         | No             | Si (realloc)   | Siempre        |
| Memoria contigua        | Si             | Si             | No             |

---

## 14. Copia vs. referencia: char* en structs

Este es el concepto que une los ejercicios 1 y 2. Cuando un campo de una
struct es `char *`, copiar la struct con `=` copia solo el **puntero**
(la direccion), no el texto.

### El problema: dos punteros al mismo texto

```c
typedef struct { char *name; float weight; } Item;

Item a;
a.name   = strdup("Espada");
a.weight = 3.5f;

Item b = a;   /* copia el puntero — b.name y a.name apuntan al mismo texto */
```

```
a.name ──┐
          v
b.name ──> [ E s p a d a \0 ]   <- un solo bloque en heap
```

Si alguien llama `free(a.name)`, el bloque desaparece y `b.name` queda
apuntando a memoria liberada (error grave).

### La solucion: strdup crea una copia independiente

```c
Item b;
b.name   = strdup(a.name);   /* nuevo bloque en heap — copia independiente */
b.weight = a.weight;
```

```
a.name ──> [ E s p a d a \0 ]   <- bloque A
b.name ──> [ E s p a d a \0 ]   <- bloque B (independiente)
```

Ahora `free(a.name)` no afecta a `b.name`.

### Regla de free: uno por cada strdup

```c
/* Con strdup — dos bloques — dos free */
free(a.name);
free(b.name);   /* correcto: bloque B es independiente */

/* Sin strdup — un bloque — un free */
free(a.name);
/* NO llamar free(b.name): apuntaban al mismo bloque */
```

### Orden correcto al liberar

Primero los campos puntero, luego la struct o el nodo:

```c
/* Liberar items del inventario */
for (int i = 0; i < count; i++) {
    free(inv[i].name);   /* campo puntero primero */
}
free(inv);               /* array al final */

/* Liberar un nodo de la lista */
free(enemy->name);   /* campo puntero primero */
free(enemy);         /* nodo al final */
```

> Si liberas el nodo primero, `->name` ya no es accesible y no puedes
> liberarlo: quedaria una fuga de memoria.

### Resumen

| Campo en struct | Asignar con `=`    | Para copia independiente | Free necesario         |
|-----------------|--------------------|--------------------------|------------------------|
| `char name[32]` | Copia el texto     | `=` es suficiente        | Solo `free(nodo)`      |
| `char *name`    | Copia el puntero   | `strdup(original)`       | `free(name)` + `free(nodo)` |

---

*Guia preparada para Lab 6 — Algoritmos y Programacion.*
