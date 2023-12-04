#ifndef _ARBRE_H_
#define _ARBRE_H_
typedef double (*FunctionPointer)(GNode*,...);

double degradationtaille(GNode *gnode,...);
double degradationluminosite(GNode *gnode,...);
double degradationdistance(GNode *gnode,...);
GNode* ConstruireAbreZPixel(int x,int y,int taille,pixelsimages *piximage,FunctionPointer fonction);
void projection_degradation(const GNode* gnode,pixelsimages* pixim,double factd);
pixelsimages* copie_tampon_projection_degradation(const GNode* gnode,pixelsimages* pixim, double factdeg);
#endif