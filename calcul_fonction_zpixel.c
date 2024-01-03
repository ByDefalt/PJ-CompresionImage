#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glib.h>
#include "stack2.h"
#include "fonction_zpixel.h"

#define MINN(i, j) (((i) < (j)) ? (i) : (j))
#define MAXX(i, j) (((i) > (j)) ? (i) : (j))
#define carre(a)  (a)*(a)


zpixel* initialiserzpixel(int taille, int x,int y,unsigned char r,unsigned char g,unsigned char b){
    zpixel *zpix=malloc(sizeof(zpixel));
    if (zpix == NULL) {
        printf("erreur malloc");
        return NULL;
    }
    zpix->taille=taille;
    zpix->position.x=x;
    zpix->position.y=y;
    zpix->couleur.r=r;
    zpix->couleur.g=g;
    zpix->couleur.b=b;
    return zpix;
}

zpixel* initialiserzpixelwhite(int taille, int x,int y){
    zpixel *zpix=malloc(sizeof(zpixel));
    if (zpix == NULL) {
        printf("erreur malloc");
        return NULL;
    }
    zpix->taille=taille;
    zpix->position.x=x;
    zpix->position.y=y;
    zpix->couleur.r=255;
    zpix->couleur.g=255;
    zpix->couleur.b=255;
    return zpix;
}

zpixel* initialiserzpixelblack(int taille, int x,int y){
    zpixel *zpix=malloc(sizeof(zpixel));
    if (zpix == NULL) {
        printf("erreur malloc");
        return NULL;
    }
    zpix->taille=taille;
    zpix->position.x=x;
    zpix->position.y=y;
    zpix->couleur.r=0;
    zpix->couleur.g=0;
    zpix->couleur.b=0;
    return zpix;
}

void afficherzpixel(zpixel *zpix){
    printf("taille=%d\nfactd=%f\nposition=(%d,%d)\ncouleur RGB=(%d,%d,%d)\n",
    zpix->taille,
    zpix->factd,
    zpix->position.x,
    zpix->position.y,
    zpix->couleur.r,
    zpix->couleur.g,
    zpix->couleur.b);
}

double calculluminositer(zpixel *zpix){
    double max=MAXX(MAXX(zpix->couleur.r,zpix->couleur.g),zpix->couleur.b);
    double min=MINN(MINN(zpix->couleur.r,zpix->couleur.g),zpix->couleur.b);
    return (min+max)/2;
}

double calculsaturation(zpixel *zpix){
    double max=MAXX(MAXX(zpix->couleur.r,zpix->couleur.g),zpix->couleur.b);
    double min=MINN(MINN(zpix->couleur.r,zpix->couleur.g),zpix->couleur.b);
    double light=(min+max)/2;
    if(max+min==0){
        return 0;
    }
    if(light>=128){
        return 255*((max-min)/511-(max+min));
    }else{
        return 255*((max-min)/(max+min));
    }
}
double calculdistance(zpixel *zpix1,zpixel *zpix2){
    return sqrt(
        carre(zpix1->couleur.r-zpix2->couleur.r)
        +carre(zpix1->couleur.g-zpix2->couleur.g)
        +carre(zpix1->couleur.b-zpix2->couleur.b)
    );
}
void moyennecouleurzpixel(zpixel* zpix,my_stack_t * mapile){
    int nbarg=stack_mem_used(mapile);
  if(nbarg!=0){
  int r=0,g=0,b=0;
  stack_iterator_t * it=stack_iterator_create(mapile);
  while(nbarg!=0){
    zpixel* zpix2=(zpixel*)(((GNode*)stack_iterator_next(it))->data);
    r+=zpix2->couleur.r;
    g+=zpix2->couleur.g;
    b+=zpix2->couleur.b;
    nbarg--;
  }
  stack_iterator_free(it);
  r/=stack_mem_used(mapile);
  g/=stack_mem_used(mapile);
  b/=stack_mem_used(mapile);
  zpix->couleur.r=r;
  zpix->couleur.g=g;
  zpix->couleur.b=b;
  }else{
    
  zpix->couleur.r=0;
  zpix->couleur.g=0;
  zpix->couleur.b=0;
  }
}
