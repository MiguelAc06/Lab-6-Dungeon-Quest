# Lab 6 — Dungeon Quest
> Un laboratorio de introduccion a C con tema de videojuego.
> Tres ejercicios conectados bajo el mismo universo: inventario, bestiario y combate.

---



## Los ejercicios

### Ejercicio 1 — El inventario del heroe
**Temas:** tipos de datos, structs, array dinamico, `malloc`/`realloc`, `switch`

El heroe guarda items (armas, armaduras, pociones) en una mochila que crece
automaticamente con `realloc`. Implementa `inventory_add`, `inventory_print`
(con `switch` para el tipo de item) e `inventory_total_weight`.

### Ejercicio 2 — El bestiario del dungeon
**Temas:** lista enlazada, `malloc`/`free`, structs, loops, `if`/`else`

Un registro de enemigos en el dungeon. Implementa `bestiary_append`,
`bestiary_print`, `bestiary_insert_at` (insertar en posicion N) y
`bestiary_remove_defeated` (eliminar todos los nodos marcados como derrotados).

### Ejercicio 3 — El combate  *(Boss Fight)*
**Temas:** `if`/`else`, operaciones aritmeticas, structs, bucle de juego

El heroe enfrenta al jefe del dungeon en un combate por turnos.
Implementa `calc_damage`, `is_alive` y `combat_round`, luego conecta
el bucle de combate y determina el ganador con `if`/`else`.

---

## El menu principal (switch)

El `main` usa `switch` para dejar al usuario elegir que ejercicio ejecutar:

```
[1] El inventario del heroe
[2] El bestiario del dungeon
[3] El combate (Boss Fight)
[0] Ejecutar todos
```

El `switch` es parte del ejercicio: esta marcado con `TODO` para que el
estudiante lo complete.

---

## Como compilar y ejecutar

```bash
# Compilar los ejercicios
gcc -Wall -o exercises exercises.c

# Ejecutar
./exercises

```
---

## Guia de referencia

Consulta [GUIDE.md](GUIDE.md) para encontrar explicaciones y ejemplos de:

- Tipos de datos primitivos y casting
- Structs y acceso con `->` y `.`
- `if / else if / else` — rangos y condiciones
- `switch` — menu interactivo y clasificacion por tipo
- Loops: `for`, `while`, `do-while`
- Punteros y aritmetica de punteros
- `malloc`, `realloc`, `free` con diagramas
- Arrays vs. listas enlazadas (tabla comparativa)
- Insercion en posicion N en lista enlazada (con diagrama)
- Copia vs. referencia en memoria (aliasing)

---

