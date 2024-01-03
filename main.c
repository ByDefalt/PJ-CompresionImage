#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glib.h>
#include "stack2.h"
#include "fonction_zpixel.h"
#include "fonction_pixel_image.h"
#include "fonction_arbre_zpixel.h"
#include "calcul_fichier.h"
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libgen.h>
int test1()
{
    double epsilon = 0.000001;
    zpixel *zpix = initialiserzpixel(5, 0, 0, 52, 63, 89);
    if (zpix->couleur.r != 52)
    {
        printf("erreur inizpixel\n");
        return 0;
    }
    double sat = calculsaturation(zpix);
    if (!(fabs(sat - 66.914894) < epsilon))
    {
        printf("erreur sat\n");
        return 0;
    }
    double lum = calculluminositer(zpix);
    if (lum != 70.5)
    {
        printf("erreur lum\n");
        return 0;
    }
    zpixel *zpixwhite = initialiserzpixelwhite(5, 0, 0);
    if (zpixwhite->couleur.r != 255)
    {
        printf("erreur inizpixelwhite\n");
        return 0;
    }
    zpixel *zpixblack = initialiserzpixelblack(5, 0, 0);
    if (zpixblack->couleur.r != 0)
    {
        printf("erreur inizpixelblack\n");
        return 0;
    }
    double dist = calculdistance(zpixwhite, zpixblack);
    if (!(fabs(dist - 441.672956) < epsilon))
    {
        printf("erreur dist\n");
        return 0;
    }
    zpixel *zpix5 = initialiserzpixel(4, -2, -2, 77, 71, 74);
    if (zpix5->taille != 4)
    {
        printf("erreur inizpixel\n");
        return 0;
    }
    pixelsimages *pixim = initialiserpixelsimages(8, 8);
    if (pixim->hauteur != 8)
    {
        printf("erreur inipixelimages\n");
        return 0;
    }
    projectionzpixel(zpix5, pixim);
    if (pixim->tab[0] != 77)
    {
        printf("erreur projection\n");
        return 0;
    }
    unsigned char r, g, b;
    getpixel(pixim, 1, 1, &r, &g, &b);
    if (r != 77)
    {
        printf("erreur getpixel\n");
        return 0;
    }
    setpixel(pixim, 3, 1, 99, 0, 0);
    if (pixim->tab[zpix5->position.x] == 99)
    {
        printf("erreur setpixel\n");
        return 0;
    }
    return 1;
}

int test2()
{
    pixelsimages *pixim = initialiserpixelsimages(10, 10);
    if (pixim == NULL)
    {
        printf("erreur creation pixelimage\n");
        return 0;
    }
    GNode *gnode1 = ConstruireAbreZPixel(0, 0, 9, pixim, degradationtaille);
    if (gnode1 == NULL)
    {
        printf("erreur creation arbre\n");
        return 0;
    }
    GNode *gnode2 = ConstruireAbreZPixel(0, 0, 9, pixim, degradationluminosite);
    if (gnode2 == NULL)
    {
        printf("erreur creation arbre\n");
        return 0;
    }
    GNode *gnode3 = ConstruireAbreZPixel(0, 0, 9, pixim, degradationdistance);
    if (gnode3 == NULL)
    {
        printf("erreur creation arbre\n");
        return 0;
    }
    unsigned char r, g, b;
    pixelsimages *pixim2 = copie_tampon_projection_degradation(gnode1, pixim, 3);
    getpixel(pixim2, 0, 0, &r, &g, &b);
    if (b != 1)
    {
        printf("erreur copie\n");
        return 0;
    }
    pixelsimages *pixim3 = copie_tampon_projection_degradation(gnode2, pixim, 3);
    getpixel(pixim3, 0, 0, &r, &g, &b);
    if (b != 4)
    {
        printf("erreur copie\n");
        return 0;
    }
    pixelsimages *pixim4 = copie_tampon_projection_degradation(gnode3, pixim, 3);
    getpixel(pixim4, 0, 0, &r, &g, &b);
    if (b != 2)
    {
        printf("erreur copie\n");
        return 0;
    }
    afficherpixelsimages(pixim2);
    afficherpixelsimages(pixim3);
    afficherpixelsimages(pixim4);
    return 0;
}

char *selected_filename = "images.jpg";
char *selected_degradation = "degradationtaille";
double selected_degradation_valeur = 0;

void on_window_destroy(GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;
    gtk_main_quit();
}

void fermerInfo(GtkWidget *widget, gpointer data)
{
    (void)widget;
    GtkWidget *window = GTK_WIDGET(data);
    gtk_widget_destroy(window);
}

void afficheInfo(GtkWidget *widget, gpointer data)
{
    (void)data;
    (void)widget;
    GtkBuilder *builder = gtk_builder_new();
    GObject *object;

    if (gtk_builder_add_from_file(builder, "pjinterface2.glade", NULL) == 0)
    {
        g_error("Impossible de charger le fichier Glade");
        return;
    }

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "Apropos"));
    object = gtk_builder_get_object(builder, "finApropos");
    g_signal_connect(object, "clicked", G_CALLBACK(fermerInfo), window);

    gtk_widget_show_all(window);

    // Libérer la mémoire utilisée par le GtkBuilder après utilisation
    g_object_unref(builder);
}

void changerImage(GtkBuilder *builder, const char *image_id, const char *nouvelle_image_path)
{
    g_return_if_fail(GTK_IS_BUILDER(builder));

    g_message("Changement de l'image avec l'ID %s vers le chemin %s", image_id, nouvelle_image_path);
    // Charger l'image
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(nouvelle_image_path, NULL);

    GtkImage *image_widget = GTK_IMAGE(gtk_builder_get_object(builder, image_id));

    if (image_widget == NULL)
    {
        g_error("Impossible de récupérer l'objet GtkImage avec l'ID %s du fichier Glade", image_id);
        return;
    }

    gtk_image_set_from_file(image_widget, nouvelle_image_path);
    // Récupérer l'objet GtkLabel
    GtkLabel *label_widget = GTK_LABEL(gtk_builder_get_object(builder, "label1"));

    if (label_widget == NULL)
    {
        g_error("Impossible de récupérer l'objet GtkLabel avec l'ID %s du fichier Glade", "label1");
        return;
    }

    const char *nomFichier = basename((char *)nouvelle_image_path);
    // Changer le texte du label
    gtk_label_set_text(label_widget, nomFichier);
    // Récupérer les dimensions maximales souhaitées
    int max_width = 400;  // Remplacez par la largeur maximale souhaitée
    int max_height = 300; // Remplacez par la hauteur maximale souhaitée

    // Vérifier et ajuster les dimensions si nécessaire
    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);

    if (width > max_width || height > max_height)
    {
        double ratio = MIN((double)max_width / width, (double)max_height / height);
        width = ratio * width;
        height = ratio * height;
    }

    // Redimensionner le pixbuf à la nouvelle taille
    GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);
    // Mettre à jour l'image avec le nouveau pixbuf
    gtk_image_set_from_pixbuf(image_widget, resized_pixbuf);

    // Libérer la mémoire utilisée par les pixbuf
    g_object_unref(pixbuf);
    g_object_unref(resized_pixbuf);
}

void on_import_button_clicked(GtkButton *button, GtkBuilder *builder)
{
    (void)button;
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Sélectionnez un fichier",
                                         GTK_WINDOW(gtk_builder_get_object(builder, "main")),
                                         action,
                                         "_Annuler",
                                         GTK_RESPONSE_CANCEL,
                                         "_Importer",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        if (filename != NULL)
        {
            selected_filename = g_strdup(filename);
            g_print("Fichier sélectionné : %s\n", selected_filename);

            // Changer l'image après avoir sélectionné un fichier
            changerImage(builder, "image1", selected_filename);

            g_free(filename);
        }
    }

    gtk_widget_destroy(dialog);
}

// Fonction de rappel pour l'événement "value-changed" du GtkVScale
void on_vscale_value_changed(GtkRange *range, gpointer user_data)
{
    (void)user_data;
    // Obtenez la valeur actuelle du GtkVScale
    double value = gtk_range_get_value(range);

    // Stockez la valeur dans la variable globale
    selected_degradation_valeur = value;
}

void on_radio_button_toggled(GtkToggleButton *button, gpointer user_data)
{
    (void)button;
    GtkBuilder *builder = GTK_BUILDER(user_data);

    // Vérifier si l'ID est correct
    const gchar *vscale_id = "vscale1";
    GtkWidget *vscale = GTK_WIDGET(gtk_builder_get_object(builder, vscale_id));

    if (vscale == NULL)
    {
        g_error("Impossible de récupérer l'objet %s depuis Glade", vscale_id);
        return;
    }

    GtkAdjustment *adjustment = gtk_range_get_adjustment(GTK_RANGE(vscale));

    if (GTK_IS_ADJUSTMENT(adjustment))
    {
        // Modifier 'upper' en fonction de l'option sélectionnée
        if (g_strcmp0(selected_degradation, "Taille") == 0)
        {
            gtk_adjustment_set_upper(adjustment, 100.0); // Remplacez 100.0 par la valeur souhaitée
        }
        else if (g_strcmp0(selected_degradation, "Luminosité") == 0)
        {
            gtk_adjustment_set_upper(adjustment, 200.0); // Remplacez 200.0 par la valeur souhaitée
        }
        else if (g_strcmp0(selected_degradation, "Distance") == 0)
        {
            gtk_adjustment_set_upper(adjustment, 300.0); // Remplacez 300.0 par la valeur souhaitée
        }
        else
        {
            g_warning("Option de dégradation inconnue : %s", selected_degradation);
        }
    }
    else
    {
        g_error("Erreur lors de la récupération de GtkAdjustment depuis Glade");
    }
}

void on_button_clicked(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;
    printf("lol\n");
    pixelsimages pixim=bitmapToPixelsImages(selected_filename);
    printf("lol\n");
    FunctionPointer fonction=(FunctionPointer)selected_degradation;
    printf("lol\n");
    EnTeteBitmapBMP bit=getfichierinfo(selected_filename);
    printf("lol\n");
    GNode *gnode = ConstruireAbreZPixel(0, 0, bit.hauteur>bit.largeur ? bit.hauteur : bit.largeur, &pixim, fonction);
    printf("lol\n");
    projection_degradation(gnode, &pixim,selected_degradation_valeur);
    printf("lol\n");
    bitmapToPixelsImages("jsp.jpg");
    printf("lol\n");
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new();
    GObject *object;
    GObject *object2;
    GObject *object3;
    GObject *object4;
    GObject *object5;

    if (gtk_builder_add_from_file(builder, "pjinterface2.glade", NULL) == 0)
    {
        g_error("Impossible de charger le fichier Glade");
        return 1;
    }

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));
    if (window == NULL)
    {
        g_error("La fenêtre principale n'a pas été trouvée dans le fichier Glade");
        return 1;
    }

    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    object2 = gtk_builder_get_object(builder, "imagemenuitem2");
    g_signal_connect(object2, "activate", G_CALLBACK(on_import_button_clicked), builder);

    object = gtk_builder_get_object(builder, "aide");
    g_signal_connect(object, "activate", G_CALLBACK(afficheInfo), NULL);

    object3 = gtk_builder_get_object(builder, "taille");
    g_signal_connect(object3, "toggled", G_CALLBACK(on_radio_button_toggled), "degradationtaille");

    object4 = gtk_builder_get_object(builder, "luminositer");
    g_signal_connect(object4, "toggled", G_CALLBACK(on_radio_button_toggled), "degradationluminosite");

    object5 = gtk_builder_get_object(builder, "distance");
    g_signal_connect(object5, "toggled", G_CALLBACK(on_radio_button_toggled), "degradationdistance");

    GtkWidget *vscale = GTK_WIDGET(gtk_builder_get_object(builder, "vscale1"));

    // Connectez la fonction de rappel à l'événement "value-changed" du GtkVScale
    g_signal_connect(vscale, "value-changed", G_CALLBACK(on_vscale_value_changed), NULL);

    GtkWidget *button = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));

    if (button == NULL || !GTK_IS_BUTTON(button)) {
        g_error("Impossible de récupérer le bouton depuis Glade");
        return 1;
    }

    // Connectez la fonction de rappel à l'événement "clicked" du bouton
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);


    gtk_widget_show_all(window);
    gtk_main();

    g_object_unref(builder);
    printf("\n%s\n", selected_filename);
    printf("\n%s\n", selected_degradation);
    printf("\n%f\n", selected_degradation_valeur);
    return 0;
}