#ifndef _FICHIER_H_
#define _FICHIER_H_
#include <stdint.h>
// Structure pour l'en-tête du fichier BMP
typedef struct {
    uint16_t signature;       // Signature du fichier ('BM' pour les fichiers BMP)
    uint32_t taille_fichier;  // Taille totale du fichier en octets
    uint16_t reserve1;        // Réservé, doit être 0
    uint16_t reserve2;        // Réservé, doit être 0
    uint32_t offset_donnees;  // Offset, en octets, où commencent les données de l'image
} EnTeteFichierBMP;

// Structure pour l'en-tête du bitmap
typedef struct {
    uint32_t taille_entete;       // Taille de cet en-tête en octets
    int32_t largeur;              // Largeur de l'image en pixels (peut être négatif)
    int32_t hauteur;              // Hauteur de l'image en pixels (peut être négatif)
    uint16_t nombre_plans;        // Nombre de plans de couleur, doit être 1
    uint16_t bits_par_pixel;      // Nombre de bits par pixel (1, 4, 8, 16, 24 ou 32)
    uint32_t compression;         // Méthode de compression utilisée (0 pour pas de compression)
    uint32_t taille_donnees_image; // Taille totale des données de l'image en octets
    int32_t resolution_horizontale; // Résolution horizontale en pixels par mètre
    int32_t resolution_verticale;   // Résolution verticale en pixels par mètre
    uint32_t nombre_couleurs_palette; // Nombre de couleurs de la palette (0 signifie utiliser toutes les couleurs possibles)
    uint32_t nombre_couleurs_importantes; // Nombre de couleurs importantes (0 signifie toutes)
} EnTeteBitmapBMP;

EnTeteBitmapBMP getfichierinfo(char* filename);
#endif