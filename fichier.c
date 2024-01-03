#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "calcul_fichier.h"

EnTeteBitmapBMP getfichierinfo(char* filename) {
    FILE *fichier = fopen(filename, "rb");

    if (!fichier) {
        fprintf(stderr, "Impossible d'ouvrir le fichier BMP.\n");
        exit(1);
    }

    // Lire l'en-tête du fichier BMP
    EnTeteFichierBMP en_tete_fichier;
    fread(&en_tete_fichier, sizeof(EnTeteFichierBMP), 1, fichier);

    // Vérifier la signature BMP
    if (en_tete_fichier.signature != 0x4D42) {
        fprintf(stderr, "Le fichier n'est pas un fichier BMP.\n");
        fclose(fichier);
        exit(1);
    }

    // Lire l'en-tête du bitmap
    EnTeteBitmapBMP en_tete_bitmap;
    fread(&en_tete_bitmap, sizeof(EnTeteBitmapBMP), 1, fichier);

    // Afficher la largeur et la hauteur de l'image
    printf("Largeur de l'image : %d pixels\n", en_tete_bitmap.largeur);
    printf("Hauteur de l'image : %d pixels\n", en_tete_bitmap.hauteur);

    fclose(fichier);
    return en_tete_bitmap;
}
