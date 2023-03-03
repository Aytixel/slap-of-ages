#include <stdio.h>

typedef struct carte_s
{
  int x;
  int y;
  int batiment;
} carte_t;

void stockerDonnee(int nb_or, int nb_victoir, carte_t *carte);
void recupererDonnee(int *nb_or, int *nb_victoire, carte_t **carte);
