/*
 * Lab 6 — Dungeon Quest
 * ============================================================
 * eres un
 * heroe que explora un dungeon, gestiona su inventario,
 * rastrea enemigos y pelea por su vida.
 *
 * Instrucciones generales:
 *   - Busca las marcas  TODO:  para saber que debes completar.
 *   - Compila con:  gcc -Wall -o exercises exercises.c
 *   - Ejecuta con:  ./exercises
 *

 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void separator(void) {
    printf("\n==================================================\n\n");
}


/* ============================================================
 *
 *  EJERCICIO 1 — El inventario del heroe
 *  Temas: tipos de datos, structs, array dinamico, malloc/realloc
 *
 * ============================================================
 *
 * Contexto:
 *   Tu heroe acaba de entrar al dungeon y necesita un inventario
 *   para guardar los objetos que encuentre. El inventario empieza
 *   con capacidad para 2 items y se duplica automaticamente
 *   cuando se llena (como una mochila magica que crece).
 *
 *   Cada item tiene:
 *     - nombre   (puntero char* — cadena en heap)
 *     - tipo     (caracter: 'W'=weapon, 'A'=armor, 'P'=potion)
 *     - peso     (numero decimal, en kg)
 *
 *   Como `name` es un puntero (char*), al agregar un item debes
 *   duplicar la cadena con strdup para que el inventario tenga
 *   su propia copia independiente. Al liberar, primero liberas
 *   cada name y luego el array.
 *
 * Lo que debes hacer:
 *   1. Completa `inventory_add`:
 *      - Si el inventario esta lleno (count == capacity), usa
 *        realloc para duplicar la capacidad e imprime:
 *        "[Mochila expandida: capacidad -> N]\n"
 *      - Copia el item en la siguiente posicion:
 *          (*inv)[*count] = item;
 *        Pero name es un puntero — debes duplicar la cadena:
 *          (*inv)[*count].name = strdup(item.name);
 *      - Incrementa *count.
 *
 *   2. Completa `inventory_print`:
 *      - Recorre el array con un for e imprime cada item:
 *        "  #N  [tipo]  nombre  (peso kg)\n"
 *      - Para el tipo usa switch: 'W'->"Arma", 'A'->"Armadura",
 *        'P'->"Pocion", default->"Misc".
 *
 *   3. Completa `inventory_total_weight`:
 *      - Suma los pesos con un for y retorna el total.
 *
 *   4. En `exercise1` los items ya estan definidos. Agrega los 5,
 *      imprime el inventario y el peso total.
 *      Al final libera cada name con free() y luego el array.
 *
 * Ejemplo de salida esperada:
 *   [Mochila expandida: capacidad -> 4]
 *   [Mochila expandida: capacidad -> 8]
 *
 *   Inventario del heroe (5 items):
 *     #1  [Arma    ]  Espada larga        (3.50 kg)
 *     #2  [Armadura]  Botas de cuero      (1.20 kg)
 *     #3  [Pocion  ]  Pocion de vida      (0.30 kg)
 *     #4  [Arma    ]  Arco largo          (2.10 kg)
 *     #5  [Armadura]  Casco de hierro     (2.80 kg)
 *   Peso total: 9.90 kg
 *
 * ---------------------------------------------------------- */

typedef struct {
    char  *name;    /* puntero — la cadena vive en heap, usa strdup al asignar */
    char   type;    /* 'W', 'A', 'P' */
    float  weight;
} Item;

void inventory_add(Item **inv, int *count, int *capacity, const char *name,
                   char type, float weight) {
    /* TODO:
     * 1. Si *count == *capacity:
     *      duplica *capacity,
     *      usa realloc sobre *inv para el nuevo tamano,
     *      imprime "[Mochila expandida: capacidad -> N]\n".
     * 2. Asigna los campos en (*inv)[*count]:
     *      .name   = strdup(name)   <- copia independiente en heap
     *      .type   = type
     *      .weight = weight
     * 3. Incrementa *count.                                          */
}

void inventory_print(Item *inv, int count) {
    /* TODO: recorre el array con un for.
     * Para cada item usa switch sobre inv[i].type para obtener
     * la cadena del tipo ("Arma", "Armadura", "Pocion", "Misc")
     * e imprime con el formato del ejemplo.                          */
}

float inventory_total_weight(Item *inv, int count) {
    /* TODO: suma los pesos con un for y retorna el total. */
    return 0.0f;
}

void exercise1(void) {
    printf("EJERCICIO 1 — El inventario del heroe\n");
    printf("--------------------------------------\n\n");

    int capacity = 2, count = 0;
    Item *inv = malloc(capacity * sizeof(Item));
    if (!inv) { perror("malloc"); return; }

    /* TODO: agrega los 5 items llamando a inventory_add con estos datos:
     *   "Espada larga",    'W', 3.50f
     *   "Botas de cuero",  'A', 1.20f
     *   "Pocion de vida",  'P', 0.30f
     *   "Arco largo",      'W', 2.10f
     *   "Casco de hierro", 'A', 2.80f                                */

    printf("\nInventario del heroe (%d items):\n", count);
    inventory_print(inv, count);
    printf("Peso total: %.2f kg\n", inventory_total_weight(inv, count));

    /* TODO: libera cada name con free() y luego el array.
     * Orden importa: primero for(i) free(inv[i].name), luego free(inv).
     * Si liberas el array primero pierdes acceso a los name.         */

    separator();
}


/* ============================================================
 *
 *  EJERCICIO 2 — El bestiario del dungeon
 *  Temas: lista enlazada, malloc/free, structs, loops
 *
 * ============================================================
 *
 * Contexto:
 *   El dungeon tiene criaturas registradas en un bestiario:
 *   una lista enlazada donde cada nodo es un enemigo.
 *   El explorador puede agregar nuevos avistamientos, insertar
 *   un enemigo en una posicion especifica (segun prioridad de
 *   peligro) y eliminar enemigos que ya fueron derrotados.
 *
 *   Cada enemigo tiene:
 *     - nombre      (puntero char* — cadena en heap)
 *     - nivel       (entero 1-10, indica peligrosidad)
 *     - derrotado   (entero: 0 = vivo, 1 = derrotado)
 *
 *   Como `name` es char*, al crear cada nodo debes duplicar la
 *   cadena con strdup. Al hacer free de un nodo, primero liberas
 *   el name y luego el nodo mismo.
 *
 * Lo que debes hacer:
 *   1. Completa `bestiary_append`: crea un nodo con malloc,
 *      asigna name = strdup(name) y enlaza al final.
 *
 *   2. Completa `bestiary_print`: recorre con while e imprime
 *      cada enemigo con posicion, nivel, nombre y estado
 *      (usa if para mostrar "[vivo]" o "[derrotado]").
 *
 *   3. Completa `bestiary_insert_at`: inserta un enemigo en la
 *      posicion indicada (base 1). Si la posicion supera el largo
 *      de la lista, inserta al final.
 *      Pista: avanza un puntero `cur` (pos-1) pasos y enlaza
 *      el nuevo nodo entre cur y cur->next.
 *
 *      Diagrama para insertar en posicion 2 en [A]->[B]->[C]:
 *
 *        antes:  head -> [A] -> [B] -> [C] -> NULL
 *                         ^
 *                        cur  (avanzo 1 paso)
 *
 *        nuevo->next = cur->next   =>  [NUEVO] -> [B]
 *        cur->next   = nuevo       =>  [A] -> [NUEVO]
 *
 *        despues: head -> [A] -> [NUEVO] -> [B] -> [C] -> NULL
 *
 *   4. Completa `bestiary_remove_defeated`: recorre la lista y
 *      elimina TODOS los nodos con derrotado == 1.
 *      Por cada nodo eliminado: free(cur->name) primero, luego free(cur).
 *
 *   5. En `exercise2`:
 *      a) Agrega los 4 enemigos iniciales con bestiary_append.
 *      b) Imprime el bestiario inicial.
 *      c) Inserta "Golem de Piedra" (nivel 8) en la posicion 2.
 *      d) Marca como derrotados al primer y tercer enemigo
 *         (accede directo: head->defeated = 1, etc.).
 *      e) Llama a bestiary_remove_defeated e imprime el resultado.
 *      f) Libera todos los nodos restantes (free name, luego free nodo).
 *
 * Ejemplo de salida esperada:
 *   Bestiario inicial:
 *   1. [Nv 3 ] Rata gigante           [vivo]
 *   2. [Nv 5 ] Esqueleto guerrero     [vivo]
 *   3. [Nv 2 ] Slime verde            [vivo]
 *   4. [Nv 7 ] Vampiro menor          [vivo]
 *
 *   >> Golem de Piedra avistado! Insertado en posicion 2.
 *
 *   Tras purgar derrotados:
 *   1. [Nv 8 ] Golem de Piedra        [vivo]
 *   2. [Nv 2 ] Slime verde            [vivo]
 *   3. [Nv 7 ] Vampiro menor          [vivo]
 *
 * ---------------------------------------------------------- */

typedef struct Enemy {
    char        *name;      /* puntero — duplicar con strdup al crear el nodo */
    int          level;
    int          defeated;  /* 0 = vivo, 1 = derrotado */
    struct Enemy *next;
} Enemy;

void bestiary_append(Enemy **head, const char *name, int level) {
    /* TODO: crea un nodo con malloc(sizeof(Enemy)).
     * Asigna: node->name = strdup(name), level, defeated=0, next=NULL.
     * Si la lista esta vacia el nuevo nodo es el head.
     * Si no, recorre hasta el ultimo y enlaza ahi.                   */
}

void bestiary_print(Enemy *head) {
    /* TODO: usa un while y una variable de posicion.
     * Imprime numero, nivel, nombre y estado de cada enemigo.
     * Usa if/else para el estado: defeated==1 -> "[derrotado]".      */
}

void bestiary_insert_at(Enemy **head, const char *name, int level, int pos) {
    /* TODO: crea un nodo con malloc, asigna name=strdup(name), level,
     * defeated=0, next=NULL.
     *
     * Casos para insertar:
     *   a) pos <= 1 o lista vacia:
     *        nuevo->next = *head;
     *        *head = nuevo;
     *
     *   b) pos > 1:
     *        Avanza `cur` exactamente (pos-1) pasos o hasta que
     *        cur->next sea NULL (lo que ocurra primero).
     *        nuevo->next = cur->next;
     *        cur->next   = nuevo;                                     */
}

void bestiary_remove_defeated(Enemy **head) {
    /* TODO: recorre la lista con cur y prev.
     * Si cur->defeated == 1:
     *   - guarda siguiente = cur->next
     *   - si hay prev: prev->next = siguiente
     *   - si no (es el head): *head = siguiente
     *   - free(cur->name) primero, luego free(cur)
     *   - cur = siguiente  (prev NO avanza)
     * Si cur->defeated == 0: prev = cur, cur = cur->next.            */
}

void exercise2(void) {
    printf("EJERCICIO 2 — El bestiario del dungeon\n");
    printf("---------------------------------------\n\n");

    Enemy *head = NULL;

    /* TODO: agrega estos 4 enemigos con bestiary_append:
     *   "Rata gigante",       nivel 3
     *   "Esqueleto guerrero", nivel 5
     *   "Slime verde",        nivel 2
     *   "Vampiro menor",      nivel 7                                 */

    printf("Bestiario inicial:\n");
    bestiary_print(head);

    /* TODO: inserta "Golem de Piedra" (nivel 8) en la posicion 2.
     * Imprime: ">> Golem de Piedra avistado! Insertado en posicion 2.\n" */

    /* TODO: marca como derrotados el primer y tercer enemigo.
     * El primer enemigo es head->defeated = 1.
     * El tercero es head->next->next->defeated = 1.                  */

    bestiary_remove_defeated(&head);

    printf("\nTras purgar derrotados:\n");
    bestiary_print(head);

    /* TODO: libera todos los nodos restantes.
     * Por cada nodo: free(cur->name) primero, luego free(cur).       */

    separator();
}


/* ============================================================
 *
 *  EJERCICIO 3 — El combate  [BOSS FIGHT]
 *  Temas: if/else, operaciones aritmeticas, structs, tipos de datos
 *
 * ============================================================
 *
 * Contexto:
 *   El heroe encuentra al jefe del dungeon. El combate se
 *   resuelve por turnos: primero ataca el heroe, luego el jefe.
 *   El combate continua hasta que alguien llega a 0 HP o menos.
 *
 *   Estadisticas del combate:
 *     - ataque  (int): danio base que inflige el atacante.
 *     - defensa (int): reduce el danio recibido.
 *     - hp      (int): puntos de vida actuales.
 *     - nombre  (cadena)
 *
 *   Formula de danio:
 *     danio = ataque_atacante - defensa_defensor
 *     Si danio < 1, se aplica un minimo de 1 (siempre hay danio).
 *
 * Lo que debes hacer:
 *   1. Completa `calc_damage`:
 *      - Calcula danio = atk - def.
 *      - Usa if para garantizar el minimo de 1.
 *      - Retorna el danio final.
 *
 *   2. Completa `is_alive`: retorna 1 si hp > 0, 0 si no.
 *
 *   3. Completa `combat_round`:
 *      - Calcula el danio del heroe al jefe y reduce el hp del jefe.
 *      - Imprime: "  [Heroe] <nombre> ataca por N de danio!\n"
 *      - Si el jefe sigue vivo (usa is_alive), calcula el danio del
 *        jefe al heroe y reduce el hp del heroe.
 *      - Imprime: "  [Jefe ] <nombre> contraataca por N de danio!\n"
 *      - Al final del turno imprime el HP restante de ambos.
 *
 *   4. En `exercise3`:
 *      - Las stats ya estan definidas. Ejecuta el bucle de combate:
 *        mientras ambos esten vivos, llama a combat_round.
 *      - Al salir del bucle usa if/else para determinar quien gano
 *        e imprime el mensaje de victoria o derrota.
 *
 * Ejemplo de salida esperada (puede variar segun stats):
 *
 *   --- Turno 1 ---
 *     [Heroe] Kael ataca por 8 de danio!
 *     [Jefe ] Senor de las Sombras contraataca por 5 de danio!
 *     HP -> Kael: 45  |  Senor de las Sombras: 42
 *
 *   --- Turno 2 ---
 *   ...
 *
 *   VICTORIA! Kael derroto al Senor de las Sombras!
 *   -- o --
 *   DERROTA... El Senor de las Sombras venció a Kael.
 *
 * ---------------------------------------------------------- */

typedef struct {
    char name[32];
    int  atk;
    int  def;
    int  hp;
} Fighter;

int calc_damage(int atk, int def) {
    /* TODO: calcula atk - def.
     * Si el resultado es menor que 1, retorna 1 (minimo de danio).
     * Si no, retorna el resultado normal.                            */
    return 0;
}

int is_alive(Fighter *f) {
    /* TODO: retorna 1 si f->hp > 0, retorna 0 si no. */
    return 0;
}

void combat_round(Fighter *hero, Fighter *boss, int turn) {
    printf("--- Turno %d ---\n", turn);

    /* TODO:
     * 1. Calcula el danio del heroe al jefe con calc_damage.
     *    Resta el danio al hp del jefe.
     *    Imprime: "  [Heroe] <nombre> ataca por N de danio!\n"
     *
     * 2. Usa is_alive para verificar si el jefe sigue vivo.
     *    Solo si sigue vivo:
     *      Calcula el danio del jefe al heroe con calc_damage.
     *      Resta el danio al hp del heroe.
     *      Imprime: "  [Jefe ] <nombre> contraataca por N de danio!\n"
     *
     * 3. Imprime el HP restante de ambos:
     *    "  HP -> <heroe>: N  |  <jefe>: N\n"                        */
}

void exercise3(void) {
    printf("EJERCICIO 3 — El combate  [BOSS FIGHT]\n");
    printf("---------------------------------------\n\n");

    Fighter hero = {"Kael",                  18, 10, 50};
    Fighter boss = {"Senor de las Sombras",  15,  7, 50};

    printf("Heroe: %s  (ATK:%d DEF:%d HP:%d)\n",
           hero.name, hero.atk, hero.def, hero.hp);
    printf("Jefe:  %s  (ATK:%d DEF:%d HP:%d)\n\n",
           boss.name, boss.atk, boss.def, boss.hp);

    int turn = 1;

    /* TODO: ejecuta el bucle de combate.
     * Mientras ambos esten vivos (is_alive(&hero) && is_alive(&boss)):
     *   llama a combat_round(&hero, &boss, turn) e incrementa turn.  */

    /* TODO: al terminar el bucle, usa if/else para determinar
     * quien gano segun quien sigue vivo (is_alive) e imprime:
     *
     * Si el heroe gano:
     *   "\nVICTORIA! %s derroto a %s!\n"
     * Si el jefe gano:
     *   "\nDERROTA... %s fue vencido por %s.\n"                       */

    separator();
}


/* ============================================================
 *  MAIN — menu con switch para elegir ejercicio
 * ============================================================ */
int main(void) {
    printf("\n");
    printf("  ╔══════════════════════════════════╗\n");
    printf("  ║      DUNGEON QUEST  — Lab 6      ║\n");
    printf("  ╚══════════════════════════════════╝\n\n");
    printf("  Selecciona un ejercicio:\n");
    printf("  [1] El inventario del heroe\n");
    printf("  [2] El bestiario del dungeon\n");
    printf("  [3] El combate (Boss Fight)\n");
    printf("  [0] Ejecutar todos\n\n");
    printf("  Opcion: ");

    int opcion;
    if (scanf("%d", &opcion) != 1) opcion = 0;
    printf("\n");

    /* TODO: implementa el switch sobre `opcion`:
     *   case 1: llama a exercise1(); break;
     *   case 2: llama a exercise2(); break;
     *   case 3: llama a exercise3(); break;
     *   case 0: llama a los tres en orden; break;
     *   default: imprime "Opcion invalida.\n";
     *
     * Nota: el switch es la herramienta ideal aqui porque
     * comparamos un entero contra valores constantes exactos.        */

    printf("  Fin del laboratorio. Buen trabajo!\n\n");
    return 0;
}
