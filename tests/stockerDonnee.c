#include <stdio.h>
#include <stdlib.h>
#include "stockerDonnee.h"

void stockerDonnee(int nb_or, int nb_victoire, carte_t *carte)
{
  FILE *fichier;
  fichier = fopen("stockageDonnee.txt", "w");

  if (fichier == NULL)
  {
    printf("Echec d'ouverture!");
    exit(1);
  }

  else
  {

    fprintf(fichier, " %d\n", nb_or);

    fprintf(fichier, "%d\n", nb_victoire);

    fprintf(fichier, "%d\t", carte->x);
    fprintf(fichier, "%d\t", carte->y);
    fprintf(fichier, "%d", carte->batiment);

    fclose(fichier);
  }
}

//@main
int main()
{

  int nbre_or, nbre_victoir;
  carte_t *carte;

  stockerDonnee(nbre_or, nbre_victoir, carte);

  return 0;
}
