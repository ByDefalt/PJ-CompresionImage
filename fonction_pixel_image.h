#ifndef _IMAGE_H_
#define _IMAGE_H_
typedef struct {
    int largeur;
    int hauteur;
    int rowstride;
    unsigned char *tab;
}pixelsimages ;


pixelsimages* initialiserpixelsimages(int hauteur,int largeur);
void getpixel(pixelsimages *piximage,int x,int y,unsigned char *r,unsigned char *g,unsigned char *b);
void setpixel(pixelsimages *piximage,int x,int y,unsigned char r,unsigned char g,unsigned char b);
void afficherpixelsimages(pixelsimages *piximage);
void projectionzpixel(zpixel *zpixel,pixelsimages *piximage);
void copierPixelsImages(pixelsimages* dest, const pixelsimages* src);
void pixelsImagesToBitmap(const pixelsimages *image, const char *nomFichier);
pixelsimages bitmapToPixelsImages(const char *nomFichier);
#endif