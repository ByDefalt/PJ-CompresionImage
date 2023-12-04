#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "stack2.h"
#include "fonction_zpixel.h"
#include "fonction_pixel_image.h"
#include "fonction_arbre_zpixel.h"


double degradationtaille(GNode *gnode,...){
    return ((zpixel*)gnode->data)->taille;
}
double degradationluminosite(GNode *gnode,...){
    va_list args;
    va_start(args,gnode);
    my_stack_t *mapile=va_arg(args,my_stack_t*);
    if(mapile!=NULL){
        double somme=0;
        int nbelem=stack_mem_used(mapile);
        stack_iterator_t * it=stack_iterator_create(mapile);
        while(nbelem!=0){
            somme+=calculluminositer(((GNode*)stack_iterator_next(it))->data);
            nbelem--;
        }
        stack_iterator_free(it);
        return somme/stack_mem_used(mapile);
    }
    return 0;
}
double degradationdistance(GNode *gnode,...){
    va_list args;
    va_start(args,gnode);
    my_stack_t *mapile=va_arg(args,my_stack_t*);
    if(mapile!=NULL){
        int nbelem=stack_mem_used(mapile);
        double distmax=0;
        double dist=0;
        stack_iterator_t * it=stack_iterator_create(mapile);
        for(int i=0 ; i<nbelem;i++){
            GNode* fils=stack_iterator_next(it);
            stack_iterator_t * it2=stack_iterator_create(mapile);
            for(int j=0 ; j<nbelem;j++){
                GNode* fils2=stack_iterator_next(it2);
                if(fils!=fils2){
                    dist=calculdistance(fils->data,fils2->data);
                    if(dist>distmax){
                        distmax=dist;
                    }
                }
            }
            stack_iterator_free(it2);
        }
        stack_iterator_free(it);
        return distmax;
    }
    return 0;
}

GNode* ConstruireAbreZPixel(int x,int y,int taille,pixelsimages *piximage,FunctionPointer fonction){
    if(x+piximage->largeur<0 || y+piximage->hauteur<0 || x>=piximage->largeur || y>=piximage->hauteur){
        return NULL;
    }
    zpixel *zpix=initialiserzpixelblack(taille,x,y);
    GNode* noeud=g_node_new(zpix);
    if(taille==1){
        zpix->couleur.r=x%256;
        zpix->couleur.g=y%256;
        zpix->couleur.b=(x+y)%256;
    }else{
        my_stack_t * pile=stack_create(4);
        int a=0;
        if((double)taille/2.0!=taille/2){a=1;}
        GNode* filsgh=ConstruireAbreZPixel(x,y,taille/2+a,piximage,fonction);
        if(filsgh!=NULL){
            g_node_prepend(noeud,filsgh);
            stack_push(pile,filsgh);
        }
        GNode* filsdh=ConstruireAbreZPixel(x+taille/2+a,y,taille/2,piximage,fonction);
        if(filsdh!=NULL){
            g_node_prepend(noeud,filsdh);
            stack_push(pile,filsdh);
        }
        GNode* filsgb=ConstruireAbreZPixel(x,y+taille/2+a,taille/2,piximage,fonction);
        if(filsgb!=NULL){
            g_node_prepend(noeud,filsgb);
            stack_push(pile,filsgb);
        }
        GNode* filsdb=ConstruireAbreZPixel(x+taille/2,y+taille/2,taille/2+a,piximage,fonction);
        if(filsdb!=NULL){
            g_node_prepend(noeud,filsdb);
            stack_push(pile,filsdb);
        }
        moyennecouleurzpixel(zpix,pile);
        zpix->factd=fonction(noeud,pile);
        stack_remove(pile);
    }
    return noeud;
}


void projection_degradation(const GNode* gnode,pixelsimages* pixim, double factdeg){
    if(gnode->parent==NULL && ((zpixel*)gnode->data)->factd<factdeg){
        projectionzpixel(gnode->data,pixim);
    }else{
        if(((zpixel*)gnode->data)->factd<factdeg && ((zpixel*)gnode->parent->data)->factd>=factdeg){
            projectionzpixel(gnode->data,pixim);
        }else{
            GNode* child = gnode->children;
            while (child != NULL) {
                projection_degradation(child,pixim,factdeg);
                child = child->next;
            }
        }
    }
}

pixelsimages* copie_tampon_projection_degradation(const GNode* gnode,pixelsimages* pixim, double factdeg){
    pixelsimages* pixim2=(pixelsimages*)malloc(sizeof(pixelsimages));
    copierPixelsImages(pixim2,pixim);
    projection_degradation(gnode,pixim2,factdeg);
    return pixim2;
}
