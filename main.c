#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-svg.h>
#include <gtk/gtk.h>

#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include <stdlib.h>
#include <stdio.h>

#define PI 3.14159

#define TAILLE 50

unsigned int xPacman, yPacman, xFantome, yFantome, pastis = 0, score = 0;

typedef struct
 {
  char*        cases;
  unsigned int nbLignes;
  unsigned int nbColonnes;
 }
 Labyrinthe;

Labyrinthe* labyrinthe;

cairo_t* canvas;

void lireLabyrinthe (char* nomFichier)
 {
  labyrinthe = (Labyrinthe*) calloc (sizeof(Labyrinthe), 1);

  int numLigne=0;
  int numColonne=0;

  if (labyrinthe)
        {
         FILE* fichier = fopen (nomFichier, "r");

         if (fichier)
               {
                int caractere;

                printf ("Lecture du fichier %s\n", nomFichier);


                do
                 {
                  caractere = fgetc (fichier);

                  if (caractere == '\n' || caractere == EOF)
                        {
                         labyrinthe->nbLignes   += 1;

                         if (numColonne > labyrinthe->nbColonnes)
                          labyrinthe->nbColonnes = numColonne;

                         numLigne++;
                         numColonne=0;
                        }
                   else if (caractere >= 32)
                         {

                          if (caractere == '.'){
                            pastis++;
                          }

                          printf ("%u %u %c : realloc avec %p, %u\n", numLigne, numColonne, caractere,  labyrinthe->cases, labyrinthe->nbLignes * labyrinthe->nbColonnes + 1);

                          labyrinthe->cases = (char*) realloc (labyrinthe->cases,
                                                numLigne * labyrinthe->nbColonnes + numColonne + 1);

                          if (labyrinthe->cases != NULL)
                                 {
                                  labyrinthe->cases [numLigne * labyrinthe->nbColonnes + numColonne]
                                                                        = (char) caractere;
                                  numColonne++;
                                 }
                            else {
                                  fprintf (stderr, "Probleme de redimensionnement de cases\n");
                                 }
                         }
                 }
                 while (caractere != EOF);

                fclose (fichier);
               }
          else {
                fprintf (stderr, "On ne peut pas ouvrir le fichier %s\n", nomFichier);
               }
        }
   else {
         fprintf (stderr, "On ne peut pas creer la structure Labyrinthe\n");
        }
 }

void dessinerMur (int x, int y)
 {
  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_rectangle       (canvas, x*TAILLE,  y*TAILLE, TAILLE, TAILLE);
  cairo_stroke_preserve (canvas);

  cairo_set_source_rgb  (canvas, 0.5, 0.5, 0.5);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);
 }

void dessinerSol (int x, int y)
 {
  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_rectangle       (canvas, x*TAILLE,  y*TAILLE, TAILLE, TAILLE);
  cairo_stroke_preserve (canvas);

  cairo_set_source_rgb  (canvas, 1, 1, 1);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);
 }

void dessinerPastille (int x, int y)
 {
  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_rectangle       (canvas, (x+0.3)*TAILLE,  (y+0.45)*TAILLE, TAILLE*0.4, TAILLE*0.1);
  cairo_stroke_preserve (canvas);

  cairo_set_source_rgb  (canvas, 1, 1, 0);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);
 }

void dessinerPacgomme (int x, int y)
 {
  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_arc             (canvas, (x+0.5)*TAILLE, (y+0.5)*TAILLE, TAILLE*0.2, 0, 2*PI);
  cairo_stroke_preserve (canvas);

  cairo_set_source_rgb  (canvas, 1, 1, 0);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);
 }

void dessinerPacman (int x, int y)
 {
  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_arc             (canvas, (x+0.5)*TAILLE, (y+0.5)*TAILLE, TAILLE*0.4, 20.0 / 360.0 * 2 * PI, 340.0 / 360.0 * 2 * PI );
  cairo_line_to         (canvas, (x+0.5)*TAILLE, (y+0.5)*TAILLE);
  cairo_close_path      (canvas);
  cairo_stroke_preserve (canvas);

  cairo_set_source_rgb  (canvas, 1, 1, 0);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);

  cairo_set_source_rgb  (canvas, 0, 0, 1);
  cairo_arc             (canvas, (x+0.6)*TAILLE, (y+0.3)*TAILLE, TAILLE*0.05,   0, 2*PI);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);
 }

void dessinerFantome (int x, int y)
 {
  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_arc             (canvas, (x+0.5)*TAILLE, (y+0.5)*TAILLE, TAILLE*0.4, PI, 0 );
  cairo_line_to         (canvas, (x+0.9)*TAILLE, (y+0.9)*TAILLE);
  cairo_line_to         (canvas, (x+0.8)*TAILLE, (y+0.85)*TAILLE);
  cairo_line_to         (canvas, (x+0.7)*TAILLE, (y+0.95)*TAILLE);
  cairo_line_to         (canvas, (x+0.6)*TAILLE, (y+0.85)*TAILLE);
  cairo_line_to         (canvas, (x+0.5)*TAILLE, (y+0.95)*TAILLE);
  cairo_line_to         (canvas, (x+0.4)*TAILLE, (y+0.85)*TAILLE);
  cairo_line_to         (canvas, (x+0.3)*TAILLE, (y+0.95)*TAILLE);
  cairo_line_to         (canvas, (x+0.2)*TAILLE, (y+0.85)*TAILLE);
  cairo_line_to         (canvas, (x+0.1)*TAILLE, (y+0.9)*TAILLE);
  cairo_close_path      (canvas);
  cairo_stroke_preserve (canvas);

  cairo_set_source_rgb  (canvas, 0.8, 0.8, 0.8);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);


  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_arc             (canvas, (x+0.65)*TAILLE, (y+0.35)*TAILLE, TAILLE*0.1,   0, 2*PI);
  cairo_stroke_preserve (canvas);

  cairo_set_source_rgb  (canvas, 1, 1, 1);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);

  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_arc             (canvas, (x+0.65)*TAILLE, (y+0.35)*TAILLE, TAILLE*0.05,   0, 2*PI);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);


  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_arc             (canvas, (x+0.35)*TAILLE, (y+0.35)*TAILLE, TAILLE*0.1,   0, 2*PI);
  cairo_stroke_preserve (canvas);

  cairo_set_source_rgb  (canvas, 1, 1, 1);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);

  cairo_set_source_rgb  (canvas, 0, 0, 0);
  cairo_arc             (canvas, (x+0.35)*TAILLE, (y+0.35)*TAILLE, TAILLE*0.05,   0, 2*PI);
  cairo_fill            (canvas);
  cairo_stroke          (canvas);
 }

void dessiner ()
 {
  int colonne, ligne;

  if (labyrinthe)
   {
    for (ligne=0; ligne<labyrinthe->nbLignes; ligne++)
     {
      for (colonne=0; colonne<labyrinthe->nbColonnes; colonne++)
       {
        switch (labyrinthe->cases [ligne * labyrinthe->nbColonnes + colonne])
         {
          case '#' : dessinerMur (colonne, ligne);
//                     printf ("#");
                     break;

//          case ' ' : dessinerSol     (colonne, ligne);
//                     printf (" ");
//                     break;

          case '.' : dessinerSol      (colonne, ligne);
                     dessinerPastille (colonne, ligne);
//                     printf (".");
//                     pastis++;
                     break;

          case 'o' : dessinerSol      (colonne, ligne);
                     dessinerPacgomme (colonne, ligne);
//                     printf ("o");
                     break;

          case 'P' : dessinerSol    (colonne, ligne);
                     if (xPacman == 0 && yPacman == 0){
                        xPacman = colonne; yPacman = ligne;
                     }
                     dessinerPacman (xPacman, yPacman);
//                     printf ("P");
                     break;

          case 'F' : dessinerSol     (colonne, ligne);
                     if (xFantome == 0 && yFantome == 0){
                        xFantome = colonne; yFantome = ligne;
                     }
                     dessinerFantome (xFantome, yFantome);
//                     printf ("F");
                     break;
         }
       }
     }
   }
 }

void deplacerPacman (GtkWidget* fenetre, unsigned int direction);

gboolean on_draw_event (GtkWidget* widget, cairo_t* contexte, gpointer user_data)
{
    canvas = contexte;

    dessiner ();

    return FALSE;
}

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer pData)
{
    unsigned int direction = 0;

    switch (event->keyval)
    {
        case GDK_KEY_Escape : exit(1); break;
        case GDK_KEY_Left   : direction = 1; break;
        case GDK_KEY_Up     : direction = 2; break;
        case GDK_KEY_Right  : direction = 3; break;
        case GDK_KEY_Down   : direction = 4; break;
        default             : direction = 0; break;
    }

    if (direction != 0){
        deplacerPacman(pData, direction);
//           g_signal_connect (G_OBJECT (widget), "key_press_event", G_CALLBACK (deplacerPacman), *direction);
    }

    return FALSE;
}

gboolean time_handler (GtkWidget *widget)
{
    int x = xFantome, y = yFantome;

    switch(rand() % 4){
        case 0: xFantome--; break;
        case 1: yFantome--; break;
        case 2: xFantome++; break;
        case 3: yFantome++; break;
    }

    if (labyrinthe->cases[yFantome * labyrinthe->nbColonnes + xFantome] == '#'){
        xFantome = x; yFantome = y;
    }

    return TRUE;
}

void refresh(GtkWidget *widget)
{
    gtk_widget_queue_draw (widget);

    if (pastis == 0){
        if (gtk_widget_get_has_window(widget) == TRUE){
            quick_message("You won !", widget);
            gtk_widget_set_has_window (widget, FALSE);
        }
     }
     else{
         if(xPacman == xFantome && yPacman == yFantome){
            if (gtk_widget_get_has_window(widget) == TRUE){
                quick_message("You lost !", widget);
                gtk_widget_set_has_window (widget, FALSE);
            }
         }
     }
}

void deplacerPacman (GtkWidget* P_score, unsigned int direction)
{
    int x = xPacman, y = yPacman;
    char display_score[3];

    GtkWidget *label = (GtkWidget*) P_score;

    switch (direction){
        case 1: xPacman--; break;
        case 2: yPacman--; break;
        case 3: xPacman++; break;
        case 4: yPacman++; break;
    }

    switch(labyrinthe->cases[yPacman * labyrinthe->nbColonnes + xPacman]){
        case '#': xPacman = x; yPacman = y;
                  break;
        case '.': score++; pastis--;
//                  sprintf(display_score, "%d", score);
                  itoa(score, display_score, 10);
                  gtk_label_set_text (label, display_score);
                  labyrinthe->cases[yPacman * labyrinthe->nbColonnes + xPacman] = ' ';
                  break;
    }
 }

void quick_message (gchar *message, GtkWidget* fenetre)
{
    GtkWidget *dialog, *label;

    dialog = gtk_message_dialog_new (fenetre,
                                     GTK_DIALOG_MODAL,
                                     GTK_MESSAGE_OTHER,
                                     GTK_BUTTONS_CLOSE,
                                     message);

    g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), fenetre);

    gtk_widget_show (dialog);
}

void OnOuvrir (GtkWidget *pwidget, gpointer pData)
 {
    xPacman = -1;   //    gdk_window_freeze_updates (pData);

    GtkWidget *dialog;
    gint res;

    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.txt");

    dialog = gtk_file_chooser_dialog_new ("Map select",
                                      pData,
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      ("Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

    gtk_file_chooser_set_filter (dialog, filter);

    res = gtk_dialog_run (GTK_DIALOG (dialog));

    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        printf("%s", filename);
//        open_file (filename);
//        g_free (filename);

        xPacman = 0; yPacman = 0;
        xFantome = 0; yFantome = 0;
        score = 0; pastis = 0;
        lireLabyrinthe(filename);
        dessiner();
        gtk_widget_show_all (pData);
    }

    gtk_widget_destroy (dialog);
 }


int main (int argc, char *argv[])
{ srand(time(NULL));

  GtkWindow* fenetre;
  GtkWidget* canvas;
  GtkWidget* Game;
  GtkWidget* Upper_box;
  GtkWidget* Player_Score;
  GtkWidget* pVBox;
  GtkWidget* barreDeMenu;
  GtkWidget* menu;
  GtkWidget* itemMenu;
  GtkWidget* itemOuvrir;

  gtk_init (&argc, &argv);

  fenetre       = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  canvas        = gtk_drawing_area_new ();
  Game          = gtk_vbox_new (FALSE, 0);
  Upper_box     = gtk_hbox_new (FALSE, 0);
  Player_Score  = gtk_label_new("0");
  pVBox         = gtk_vbox_new(FALSE, 0);
  barreDeMenu   = gtk_menu_bar_new();
  menu          = gtk_menu_new();
  itemMenu      = gtk_menu_item_new_with_label("Menu");
  itemOuvrir    = gtk_menu_item_new_with_label("Ouvrir");

  lireLabyrinthe ("laby.txt");

  gtk_window_set_position      (GTK_WINDOW (fenetre), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size  (GTK_WINDOW (fenetre), (TAILLE*labyrinthe->nbColonnes)+1, (TAILLE*labyrinthe->nbLignes)+61);
  gtk_window_set_title         (GTK_WINDOW (fenetre), "Pacman GTK Cairo");

  gtk_container_add (GTK_CONTAINER (fenetre), Game);

  gtk_box_set_homogeneous   (Game, FALSE);
  gtk_box_pack_start        (Upper_box, gtk_label_new("Score"), FALSE, FALSE, 10);
  gtk_box_pack_start        (Upper_box, Player_Score, FALSE, FALSE, 10);
  gtk_box_pack_start        (Game, pVBox, FALSE, FALSE, 0);
  gtk_box_pack_start        (Game, Upper_box, FALSE, FALSE, 10);
  gtk_box_pack_end          (Game, canvas, TRUE, TRUE, 0);
  gtk_box_pack_start        (GTK_BOX(pVBox), barreDeMenu, FALSE, FALSE, 0);

  g_signal_connect (G_OBJECT (fenetre),     "destroy", G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect (G_OBJECT (fenetre),     "key_press_event", G_CALLBACK (on_key_press), Player_Score);
  g_signal_connect (G_OBJECT (canvas) ,     "draw", G_CALLBACK (on_draw_event), NULL);
  g_signal_connect (G_OBJECT (itemOuvrir),  "activate", G_CALLBACK(OnOuvrir), fenetre);

  gtk_menu_shell_append     (GTK_MENU_SHELL(menu), itemOuvrir);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM(itemMenu), menu);
  gtk_menu_shell_append     (GTK_MENU_SHELL(barreDeMenu), itemMenu);

  g_timeout_add (1000, (GSourceFunc) time_handler, (gpointer) fenetre);

  gtk_widget_show_all (fenetre);

  g_timeout_add (10, (GSourceFunc) refresh, (gpointer) fenetre);

  gtk_main();

}
