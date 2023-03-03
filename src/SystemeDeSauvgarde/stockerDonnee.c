#include <stdio.h>
#include <stdlib.h>
#include "stockerDonnee.h"

void stockerDonnee(int nb_or, int nb_victoire, carte_t *carte){
  int victoir=0;
  FILE* fichier;
  fichier=fopen("stockageDonnee.txt","w");
 
  if(fichier == NULL){
        printf("Echec d'ouverture!");
        exit(1);
  }
 
  else{
    
      printf("le nombre d'or: ");
      scanf("%d",&nb_or);

      printf("le nombre de victoir:  ");
      scanf("%d",&nb_victoire);

      printf("La carte du joueur,repere x: ");
      scanf("%d", &carte->x);
      printf("La carte du joueur,repere y: ");
      scanf("%d",&carte->y);
      printf("La carte du joueur,batiment: ");
      scanf("%d", &carte->batiment);

      fprintf(fichier," %d\n",nb_or);

      fprintf(fichier,"%d\n",nb_victoire);

      fprintf(fichier,"%d\t", carte->x);
      fprintf(fichier,"%d\t", carte->y);
      fprintf(fichier,"%d",carte->batiment);


    fclose(fichier);
  }
  
}

int main(){

  int nbre_or, nbre_victoir;
  carte_t *carte;

  stockerDonnee(nbre_or,nbre_victoir,carte);


return 0;

}
