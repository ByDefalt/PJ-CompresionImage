#include <stdio.h>
#include <stdlib.h>
#include "fonction_zpixel.h"
#include "fonction_pixel_image.h"
#include <string.h>

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

// Convertit une image bitmap en pixelsimages
pixelsimages bitmapToPixelsImages(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "rb");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    fseek(fichier, 18, SEEK_SET); // Ignorer l'en-tête BMP fixe de 18 octets

    pixelsimages image;
    fread(&image.largeur, sizeof(int), 1, fichier);
    fread(&image.hauteur, sizeof(int), 1, fichier);
    fread(&image.rowstride, sizeof(int), 1, fichier);

    // Allouer de la mémoire pour le tableau de pixels
    image.tab = (unsigned char *)malloc(image.rowstride * image.hauteur*sizeof(unsigned char));
    if (image.tab == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    // Lire les données des pixels
    fseek(fichier, 54, SEEK_SET); // Aller au début des données des pixels
    fread(image.tab, sizeof(unsigned char), image.rowstride * image.hauteur, fichier);

    fclose(fichier);
    return image;
}

// Convertit une structure pixelsimages en image bitmap
void pixelsImagesToBitmap(const pixelsimages *image, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "wb");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Écrire l'en-tête BMP fixe
    unsigned char enTete[54] = {66, 77, 18, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
    fwrite(enTete, sizeof(unsigned char), 54, fichier);

    // Écrire la largeur, la hauteur et la rowstride
    fwrite(&image->largeur, sizeof(int), 1, fichier);
    fwrite(&image->hauteur, sizeof(int), 1, fichier);
    fwrite(&image->rowstride, sizeof(int), 1, fichier);

    // Écrire les données des pixels
    fwrite(image->tab, sizeof(unsigned char), image->rowstride * image->hauteur, fichier);

    fclose(fichier);
}