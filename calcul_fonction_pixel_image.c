#include <stdio.h>
#include <stdlib.h>
#include "fonction_zpixel.h"
#include "fonction_pixel_image.h"

pixelsimages* initialiserpixelsimages(int hauteur,int largeur){
    pixelsimages *piximage=malloc(sizeof(pixelsimages));
    if (piximage == NULL) {
        printf("erreur malloc");
        return NULL;
    }
    piximage->hauteur=hauteur;
    piximage->largeur=largeur;
    piximage->rowstride=largeur*3+1;
    piximage->tab = (unsigned char *)malloc(piximage->rowstride*hauteur*sizeof(unsigned char));
    if(piximage->tab==NULL){
        printf("erreur malloc\n");
        free(piximage->tab);
        exit(EXIT_FAILURE);;
    }
    for(int i=1;i<=piximage->rowstride*hauteur;i++){
        if(i%piximage->rowstride==0){
            piximage->tab[i-1]=7;
        }else{
            piximage->tab[i-1]=0;
        }
    }
    return piximage;
}
void setpixel(pixelsimages *piximage,int x,int y,unsigned char r,unsigned char g,unsigned char b){
    if(!(x<0 || y<0 || x>piximage->largeur || y>piximage->hauteur)){  
        int js=x*3+y*piximage->rowstride;
        *(piximage->tab+js)=r;
        *(piximage->tab+js+1)=g;
        *(piximage->tab+js+2)=b;
    }
}
void getpixel(pixelsimages *piximage,int x,int y,unsigned char *r,unsigned char *g,unsigned char *b){
    if(!(x<0 || y<0 || x>piximage->largeur || y>piximage->hauteur)){
        int js=x*3+y*piximage->rowstride;
        *r=*(piximage->tab+js);
        *g=*(piximage->tab+js+1);
        *b=*(piximage->tab+js+2);
    }
}
void afficherpixelsimages(pixelsimages *piximage){
    int j=0;
    int a=0;
    for(int i=1;i<=piximage->hauteur;i++){
        a=0;
        printf("\n(");
        for(;j<piximage->rowstride*i;j++){
            if(a%3==0 && a>0){
                printf("),(");
            }
            if(j==(piximage->rowstride*i)-1){
                printf("%d)",piximage->tab[j]);
            }else{
                if(a%3==2 && a>0){
                    printf("%d",piximage->tab[j]);
                }else{
                    printf("%d,",piximage->tab[j]);
                }
                a++;
            }
        }
    }
    printf("\n\n");
}
void projectionzpixel(zpixel *zpixel,pixelsimages *piximage){
        for(int j=zpixel->position.y;j<zpixel->position.y+zpixel->taille;j++){
            for(int i=zpixel->position.x;i<zpixel->position.x+zpixel->taille;i++){
                if(i<piximage->largeur && j<piximage->hauteur ){
                setpixel(piximage,i,j,zpixel->couleur.r,zpixel->couleur.g,zpixel->couleur.b);
            }
            }
        }
}
void copierPixelsImages(pixelsimages* dest, const pixelsimages* src) {
    dest->largeur = src->largeur;
    dest->hauteur = src->hauteur;
    dest->rowstride = src->rowstride;
    dest->tab = (unsigned char*)malloc(src->rowstride * src->hauteur);
    if (dest->tab == NULL) {
        free(dest->tab);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < src->rowstride * src->hauteur; i++) {
        dest->tab[i] = src->tab[i];
    }
}