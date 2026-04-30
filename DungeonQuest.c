/*
 * Lab 6 — Dungeon Quest
 * ============================================================
 * eres un
 * heroe que explora un dungeon, gestiona su inventario,
 * rastrea enemigos y pelea por su vida.
 *
 * Instrucciones generales:
 * - Busca las marcas  TODO:  para saber que debes completar.
 * - Compila con:  gcc -Wall -o exercises exercises.c
 * - Ejecuta con:  ./exercises
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
 * EJERCICIO 1 — El inventario del heroe
 * ============================================================ */

typedef struct {
    char  *name;
    char   type;
    float  weight;
} Item;

void inventory_add(Item **inv, int *count, int *capacity, const char *name, char type, float weight) {
    if (*count == *capacity) {
        *capacity *= 2;
        *inv = realloc(*inv, *capacity * sizeof(Item));
        if (!*inv) { perror("realloc"); exit(1); }
        printf("[Mochila expandida: capacidad -> %d]\n", *capacity);
    }
    
    (*inv)[*count].name = strdup(name);
    (*inv)[*count].type = type;
    (*inv)[*count].weight = weight;
    (*count)++;
}

void inventory_print(Item *inv, int count) {
    for (int i = 0; i < count; i++) {
        const char *tipo_str;
        switch (inv[i].type) {
            case 'W': tipo_str = "Arma"; break;
            case 'A': tipo_str = "Armadura"; break;
            case 'P': tipo_str = "Pocion"; break;
            default:  tipo_str = "Misc"; break;
        }
        printf("    #%d  [%-8s]  %-18s (%.2f kg)\n", i + 1, tipo_str, inv[i].name, inv[i].weight);
    }
}

float inventory_total_weight(Item *inv, int count) {
    float total = 0.0f;
    for (int i = 0; i < count; i++) {
        total += inv[i].weight;
    }
    return total;
}

void exercise1(void) {
    printf("EJERCICIO 1 — El inventario del heroe\n");
    printf("--------------------------------------\n\n");

    int capacity = 2, count = 0;
    Item *inv = malloc(capacity * sizeof(Item));
    if (!inv) { perror("malloc"); return; }

    inventory_add(&inv, &count, &capacity, "Espada larga", 'W', 3.50f);
    inventory_add(&inv, &count, &capacity, "Botas de cuero", 'A', 1.20f);
    inventory_add(&inv, &count, &capacity, "Pocion de vida", 'P', 0.30f);
    inventory_add(&inv, &count, &capacity, "Arco largo", 'W', 2.10f);
    inventory_add(&inv, &count, &capacity, "Casco de hierro", 'A', 2.80f);

    printf("\nInventario del heroe (%d items):\n", count);
    inventory_print(inv, count);
    printf("Peso total: %.2f kg\n", inventory_total_weight(inv, count));

    /* Liberar memoria */
    for (int i = 0; i < count; i++) {
        free(inv[i].name);
    }
    free(inv);

    separator();
}

/* ============================================================
 * EJERCICIO 2 — El bestiario del dungeon
 * ============================================================ */

typedef struct Enemy {
    char         *name;
    int          level;
    int          defeated;
    struct Enemy *next;
} Enemy;

void bestiary_append(Enemy **head, const char *name, int level) {
    Enemy *nuevo = malloc(sizeof(Enemy));
    nuevo->name = strdup(name);
    nuevo->level = level;
    nuevo->defeated = 0;
    nuevo->next = NULL;

    if (*head == NULL) {
        *head = nuevo;
    } else {
        Enemy *cur = *head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = nuevo;
    }
}

void bestiary_print(Enemy *head) {
    Enemy *cur = head;
    int pos = 1;
    while (cur != NULL) {
        printf("  %d. [Nv %d ] %-22s [%s]\n", pos, cur->level, cur->name, cur->defeated ? "derrotado" : "vivo");
        cur = cur->next;
        pos++;
    }
}

void bestiary_insert_at(Enemy **head, const char *name, int level, int pos) {
    Enemy *nuevo = malloc(sizeof(Enemy));
    nuevo->name = strdup(name);
    nuevo->level = level;
    nuevo->defeated = 0;
    nuevo->next = NULL;

    if (pos <= 1 || *head == NULL) {
        nuevo->next = *head;
        *head = nuevo;
    } else {
        Enemy *cur = *head;
        for (int i = 1; i < pos - 1 && cur->next != NULL; i++) {
            cur = cur->next;
        }
        nuevo->next = cur->next;
        cur->next = nuevo;
    }
}

void bestiary_remove_defeated(Enemy **head) {
    Enemy *cur = *head;
    Enemy *prev = NULL;

    while (cur != NULL) {
        if (cur->defeated == 1) {
            Enemy *siguiente = cur->next;
            if (prev != NULL) {
                prev->next = siguiente;
            } else {
                *head = siguiente;
            }
            free(cur->name);
            free(cur);
            cur = siguiente;
        } else {
            prev = cur;
            cur = cur->next;
        }
    }
}

void exercise2(void) {
    printf("EJERCICIO 2 — El bestiario del dungeon\n");
    printf("---------------------------------------\n\n");

    Enemy *head = NULL;

    bestiary_append(&head, "Rata gigante", 3);
    bestiary_append(&head, "Esqueleto guerrero", 5);
    bestiary_append(&head, "Slime verde", 2);
    bestiary_append(&head, "Vampiro menor", 7);

    printf("Bestiario inicial:\n");
    bestiary_print(head);

    bestiary_insert_at(&head, "Golem de Piedra", 8, 2);
    printf("\n  >> Golem de Piedra avistado! Insertado en posicion 2.\n\n");

    /* Marcar como derrotados al primer y tercer enemigo actual */
    if (head) head->defeated = 1;
    if (head && head->next && head->next->next) head->next->next->defeated = 1;

    bestiary_remove_defeated(&head);

    printf("Tras purgar derrotados:\n");
    bestiary_print(head);

    /* Liberar memoria restante */
    Enemy *cur = head;
    while (cur != NULL) {
        Enemy *siguiente = cur->next;
        free(cur->name);
        free(cur);
        cur = siguiente;
    }

    separator();
}

/* ============================================================
 * EJERCICIO 3 — El combate  [BOSS FIGHT]
 * ============================================================ */

typedef struct {
    char name[32];
    int  atk;
    int  def;
    int  hp;
} Fighter;

int calc_damage(int atk, int def) {
    int damage = atk - def;
    if (damage < 1) {
        damage = 1;
    }
    return damage;
}

int is_alive(Fighter *f) {
    if (f->hp > 0) {
        return 1;
    }
    return 0;
}

void combat_round(Fighter *hero, Fighter *boss, int turn) {
    printf("--- Turno %d ---\n", turn);

    int hero_dmg = calc_damage(hero->atk, boss->def);
    boss->hp -= hero_dmg;
    printf("    [Heroe] %s ataca por %d de danio!\n", hero->name, hero_dmg);

    if (is_alive(boss)) {
        int boss_dmg = calc_damage(boss->atk, hero->def);
        hero->hp -= boss_dmg;
        printf("    [Jefe ] %s contraataca por %d de danio!\n", boss->name, boss_dmg);
    }

    printf("    HP -> %s: %d  |  %s: %d\n\n", hero->name, hero->hp, boss->name, boss->hp);
}

void exercise3(void) {
    printf("EJERCICIO 3 — El combate  [BOSS FIGHT]\n");
    printf("---------------------------------------\n\n");

    Fighter hero = {"Kael",                  18, 10, 50};
    Fighter boss = {"Senor de las Sombras",  15,  7, 50};

    printf("Heroe: %s  (ATK:%d DEF:%d HP:%d)\n", hero.name, hero.atk, hero.def, hero.hp);
    printf("Jefe:  %s  (ATK:%d DEF:%d HP:%d)\n\n", boss.name, boss.atk, boss.def, boss.hp);

    int turn = 1;

    while (is_alive(&hero) && is_alive(&boss)) {
        combat_round(&hero, &boss, turn);
        turn++;
    }

    if (is_alive(&hero)) {
        printf("VICTORIA! %s derroto a %s!\n", hero.name, boss.name);
    } else {
        printf("DERROTA... %s fue vencido por %s.\n", hero.name, boss.name);
    }

    separator();
}

/* ============================================================
 * MAIN — menu con switch para elegir ejercicio
 * ============================================================ */
int main(void) {
    printf("\n");
    printf("  ╔══════════════════════════════════╗\n");
    printf("  ║       DUNGEON QUEST  — Lab 6     ║\n");
    printf("  ╚══════════════════════════════════╝\n\n");
    printf("  Selecciona un ejercicio:\n");
    printf("  [1] El inventario del heroe\n");
    printf("  [2] El bestiario del dungeon\n");
    printf("  [3] El combate (Boss Fight)\n");
    printf("  [0] Ejecutar todos\n\n");
    printf("  Opcion: ");

    int opcion;
    if (scanf("%d", &opcion) != 1) opcion = -1;
    printf("\n");

    switch (opcion) {
        case 1:
            exercise1();
            break;
        case 2:
            exercise2();
            break;
        case 3:
            exercise3();
            break;
        case 0:
            exercise1();
            exercise2();
            exercise3();
            break;
        default:
            printf("Opcion invalida.\n");
            break;
    }

    printf("  Fin del laboratorio. Buen trabajo!\n\n");
    return 0;
}

