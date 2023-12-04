#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glib.h>
#include "stack2.h"
#include "fonction_zpixel.h"
#include "fonction_pixel_image.h"
#include "fonction_arbre_zpixel.h"
#include <gtk/gtk.h>

int test1(){
    double epsilon=0.000001;
    zpixel* zpix=initialiserzpixel(5,0,0,52,63,89);
    if(zpix->couleur.r!=52){
        printf("erreur inizpixel\n");
        return 0;
    }
    double sat=calculsaturation(zpix);
    if(!(fabs(sat - 66.914894) < epsilon)){
        printf("erreur sat\n");
        return 0;
    }
    double lum=calculluminositer(zpix);
    if(lum!=70.5){
        printf("erreur lum\n");
        return 0;
    }
    zpixel *zpixwhite=initialiserzpixelwhite(5,0,0);
    if(zpixwhite->couleur.r!=255){
        printf("erreur inizpixelwhite\n");
        return 0;
    }
    zpixel *zpixblack=initialiserzpixelblack(5,0,0);
    if(zpixblack->couleur.r!=0){
        printf("erreur inizpixelblack\n");
        return 0;
    }
    double dist=calculdistance(zpixwhite,zpixblack);
    if(!(fabs(dist - 441.672956) < epsilon)){
        printf("erreur dist\n");
        return 0;
    }
    zpixel *zpix5=initialiserzpixel(4,-2,-2,77,71,74);
    if(zpix5->taille!=4){
        printf("erreur inizpixel\n");
        return 0;
    }
    pixelsimages *pixim=initialiserpixelsimages(8,8);
    if(pixim->hauteur!=8){
        printf("erreur inipixelimages\n");
        return 0;
    }
    projectionzpixel(zpix5,pixim);
    if(pixim->tab[0]!=77){
        printf("erreur projection\n");
        return 0;
    }
    unsigned char r,g,b;
    getpixel(pixim,1,1,&r,&g,&b);
    if(r!=77){
        printf("erreur getpixel\n");
        return 0;
    }
    setpixel(pixim,3,1,99,0,0);
    if(pixim->tab[zpix5->position.x]==99){
        printf("erreur setpixel\n");
        return 0;
    }
    return 1;
}


int test2(){
    pixelsimages *pixim=initialiserpixelsimages(10,10);
    if(pixim==NULL){
        printf("erreur creation pixelimage\n");
        return 0;
    }
    GNode *gnode1=ConstruireAbreZPixel(0,0,9,pixim,degradationtaille);
    if(gnode1==NULL){
        printf("erreur creation arbre\n");
        return 0;
    }
    GNode *gnode2=ConstruireAbreZPixel(0,0,9,pixim,degradationluminosite);
    if(gnode2==NULL){
        printf("erreur creation arbre\n");
        return 0;
    }
    GNode *gnode3=ConstruireAbreZPixel(0,0,9,pixim,degradationdistance);
    if(gnode3==NULL){
        printf("erreur creation arbre\n");
        return 0;
    }
    unsigned char r,g,b;
    pixelsimages *pixim2=copie_tampon_projection_degradation(gnode1,pixim,3);
    getpixel(pixim2,0,0,&r,&g,&b);
    if(b!=1){
        printf("erreur copie\n");
        return 0;
    }
    pixelsimages *pixim3=copie_tampon_projection_degradation(gnode2,pixim,3);
    getpixel(pixim3,0,0,&r,&g,&b);
    if(b!=4){
        printf("erreur copie\n");
        return 0;
    }
    pixelsimages *pixim4=copie_tampon_projection_degradation(gnode3,pixim,3);
    getpixel(pixim4,0,0,&r,&g,&b);
    if(b!=2){
        printf("erreur copie\n");
        return 0;
    }
    afficherpixelsimages(pixim2);
    afficherpixelsimages(pixim3);
    afficherpixelsimages(pixim4);
    return 0;
}
// Fonction callback pour gérer la fermeture de la fenêtre
#include <gtk/gtk.h>

void on_window_destroy() {
    gtk_main_quit();
}

void fermerInfo(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *window = GTK_WIDGET(data);
    gtk_widget_destroy(window);
}

void afficheInfo(GtkWidget *widget, gpointer data) {
    (void)widget;
    (void)data;
    GtkBuilder *builder = gtk_builder_new();
    GObject *object;

    // Charger l'interface utilisateur à partir du fichier Glade
    if (gtk_builder_add_from_file(builder, "pjinterface2.glade", NULL) == 0) {
        g_error("Impossible de charger le fichier Glade");
    }

    // Récupérer la fenêtre "Apropos" à partir du fichier Glade
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "Apropos"));

    // Récupérer le bouton de fermeture de la fenêtre
    object = gtk_builder_get_object(builder, "finApropos");
    g_signal_connect(object, "clicked", G_CALLBACK(fermerInfo), window);

    // Afficher la fenêtre "Apropos"
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    GObject *object;

    // Initialiser GTK
    gtk_init(&argc, &argv);

    // Créer un nouveau GtkBuilder
    GtkBuilder *builder = gtk_builder_new();

    // Charger l'interface utilisateur à partir du fichier Glade
    if (gtk_builder_add_from_file(builder, "pjinterface2.glade", NULL) == 0) {
        g_error("Impossible de charger le fichier Glade");
        return 1;
    }

    // Récupérer la fenêtre principale à partir du fichier Glade
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));

    // Vérifier si la fenêtre a été trouvée
    if (window == NULL) {
        g_error("La fenêtre principale n'a pas été trouvée dans le fichier Glade");
        return 1;
    }

    // Connecter la fonction callback à l'événement de fermeture de la fenêtre
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    // Récupérer le bouton "aide" pour afficher la fenêtre "Apropos"
    object = gtk_builder_get_object(builder, "aide");
    g_signal_connect(object, "activate", G_CALLBACK(afficheInfo), NULL);

    // Afficher tous les éléments de la fenêtre principale
    gtk_widget_show_all(window);

    // Démarrer la boucle principale GTK
    gtk_main();

    // Libérer la mémoire utilisée par le GtkBuilder
    g_object_unref(builder);

    return 0;
}