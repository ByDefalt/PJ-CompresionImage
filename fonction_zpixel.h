#ifndef _ZPIXEL_H_
#define _ZPIXEL_H_
#include "stack2.h"
typedef struct {
    int taille;
    double factd;
    struct{
      int x;
      int y;
    }position;
    struct{
      unsigned char r;
      unsigned char g;
      unsigned char b;
    }couleur;
}zpixel;

zpixel* initialiserzpixel(int taille, int x,int y,unsigned char r,unsigned char g,unsigned char b);
zpixel* initialiserzpixelwhite(int taille, int x,int y);
zpixel* initialiserzpixelblack(int taille, int x,int y);
void afficherzpixel(zpixel *zpix);
double calculluminositer(zpixel *zpix);
double calculsaturation(zpixel *zpix);
double calculdistance(zpixel *zpix1,zpixel *zpix2);
void moyennecouleurzpixel(zpixel* zpix,my_stack_t* mapile);

#endif