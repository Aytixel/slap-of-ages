# Slap of Ages

***Slap of Ages*** est un projet mené par *4 étudiants en L2 Informatique* de l'université du Mans sur une durée de 3 mois, le jeu consiste en la création d'un village et d'une armée pour combattre les villages des autres joueurs. Ce jeu est inspiré du jeu Clash of Clans.

[Lien vers le jeu](https://play.google.com/store/apps/details?id=com.supercell.clashofclans&hl=fr&gl=US)

## Membres du groupe :
- **Lucas Dureau** alias `Aytixel`
- **Hôa Le Luet** alias `ABTonniere`
- **Arthur Dureau** alias `MasterPNJ`
- **Chadli Basma** alias `Basmachadli`

## Documents liés :

Gantt : https://docs.google.com/spreadsheets/d/1tzo2DsVGko6vCwdnjP4RdRnp5ozh0s1jijUkGqu_1Vs/edit?usp=sharing

Documentation *(prise sur la branche `dev`)* : https://aytixel.github.io/slap-of-ages/doc/html/

## Compilation :

Si vous compilez le projet pour la première fois faite `make all`. Cette commande compilera la SDL, cela peut donc prendre un peu de temps.

Si vous voulez recompiler `make clean build` et pour recompiler les fichiers de test `make clean build_test`.

Si vous voulez juste compiler la SDL `make install_sdl`. Cette compilation n'a lieu que sur Linux et MacOs.

## Convention de nommage :

**Code en anglais.**

Variable en snake case :
```c
int foo_bar = 0;
```

Définition de type en snake case, et finissant par `t` :
```c
typedef unsigned long foo_t;
```

Définition de structure en snake case, et finissant par `s` :
```c
typedef struct foo_s {
    int bar;
    struct foo_s *foo;
} foo_t;
```
```c
struct foo_s {
    int bar;
};
```

Définition d'enum en snake case, et finissant par `e` :
```c
typedef enum {
    FOO = 12,
    BAR,
} bar_e;
```
```c
enum bar_e {
    FOO,
    BAR = 12,
};
```

Définition de fonction en camel case, sans majuscule au début :
```c
int fooBar() {
    return 0;
}
```

Définition de constante en snake case, majuscule :
```c
const int FOO_BAR = 0;
```
