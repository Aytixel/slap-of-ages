# Slap of Ages

## Membres du groupe :
- **Lucas Dureau** alias `Aytixel`
- **Hôa Le Luet** alias `ABTonniere`
- **Arthur Dureau** alias `MasterPNJ`
- **Chadli Basma** alias `Basmachadli`

## Compilation

Attention pour compiler sur **Windows** vous deverez utilisez le fichier `Makefile-windows`.

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
struct foo_s {
    int bar;
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
