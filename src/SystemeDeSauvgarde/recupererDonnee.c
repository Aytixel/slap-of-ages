#include <stdio.h>
#include <stdlib.h>
#include "stockerDonnee.h"
#define NB 300

void recupererDonnee(int *nb_or, int *nb_victoire, carte_t **carte){
    int i;
  FILE* fichier;
  fichier=fopen("stockageDonnee.txt","r");
  
  if(fichier == NULL){
    printf("Echec de lecture des données!");
    exit(1);
  }
  else{
    
    for(i=0;i<NB;i++){
      fscanf(fichier,"%d",&(nb_or[i]));
      fscanf(fichier,"%d",&(nb_victoire[i]));

      fscanf(fichier,"%d", &carte[i]->x);
      fscanf(fichier,"%d", &carte[i]->y);
      fscanf(fichier,"%d",&carte[i]->batiment);
      fclose(fichier);

    }
  }
  
}

int main(){

  int nbre_or,nbre_victoire;
   carte_t *carte;
   recupererDonnee(nbre_or,nbre_victoire, carte);

  return 0;
}
