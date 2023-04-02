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

Pour recompiler `make clean build`, `make clean build_test` pour les fichiers de test et `make clean build_release` pour compiler une version définitive.

Si vous voulez juste installer la SDL `make install_sdl`. Cette compilation n'a lieu que sur Linux et MacOs.

## Lancer le jeu :

Pour lancer le jeu il suffit d'exécuter `main_client`, dans le dossier `bin`.

Et si vous souhaitez lancer un serveur faite de même avec `main_server`.

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
