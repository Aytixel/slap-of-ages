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

## Règle du jeu :

Commencez par vous connecter, puis créer votre village.

Chaque bâtiment vous **coûtera de l'or pour le placer.**
Cet or vous seras **remboursé quand vous l'enlèverez.**
Votre **total d'or augmentera** uniquement **si vous perdez ou faites une égalité** avec votre adversaire.

Les maisons que vous placerez feront apparaitre des troupes de défense pendant la phase d'attaque.
La maison la moins cher fera apparaître un *rat*, celle d'après fera apparaître un *démon*, et la plus chère fera apparaître un *géant goblin*.

Une fois votre village prêt appuyez sur le bouton `Attaquer`, vous pouvez aussi annulez si vous le souhaitez avec le bouton `Annuler`.

Après un temps d'attente le temps de trouver un adversaire, vos commercerez à attaquer son village.
Pour cela vous avez différentes troupes avec différentes spécificités à votre disposition :
- Le **rat** attaque en priorité les troupes de défense adverse, puis les bâtiments, il a assez peu de vie mais tape très fort et avance vite.
- Le **démon** n'attaque que les bâtiments, mais peut passer au-dessus des murs, au niveau de sa vie et son attaque et sa vitesse c'est un entre-deux entre le **géant goblin** et le **rat**.
- Le **géant goblin** n'attaque lui aussi que les bâtiments, il est lent et n'attaque pas fort mais à beaucoup de vie.

Pour placer ces troupes vous avez un **nombre d'elixir défini** qui **revient à son maximum à chaque début d'attaque**.
Ce maximum **augmente** à chaque fois que **vous gagnez ou faites une égalité** avec votre adversaire.

*Vous gagnerez si vous arrivez à détruire un maximum de bâtiments dans le village adverse.*
*Si les deux villages ont le même pourcentage de destruction la victoire se joue à la vitesse.*

*Si vous finissez en même temps que votre adversaire avec le même pourcentage de destruction il y a égalité.*

## Compilation :

Si vous compilez le projet pour la première fois faites `make all`. Cette commande compilera la SDL, cela peut donc prendre un peu de temps.

Pour recompiler `make clean build`, `make clean build_test` pour les fichiers de test et `make clean build_release` pour compiler une version définitive.

Si vous voulez juste installer la SDL `make install_sdl`. Cette compilation n'a lieu que sur Linux et MacOs.

## Lancer le jeu :

Pour lancer le jeu il suffit d'exécuter `main_client`, dans le dossier `bin`, puis de vous connecter à un serveur une fois le jeu lancé.

Si vous ne connaissez pas de serveur vous devrez impérativement en lancer un, le jeu n'ayant pas de mode hors ligne ou solo.

Pour lancer un serveur la procédure est la même qu'avec `main_client`, mais avec `main_server`.

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
