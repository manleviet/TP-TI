//
//
//  main.cpp
//
//  Détection de contours
//
//  LE Viet Man
//  02/05/2010
//
//

#include <unistd.h>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "contours.h"

using namespace std;

char *Usage = (char*)"Usage: contours [option] <fichier>\n\
Extraire des contours et faire l'évaluation avec la référence tracée à la main.\n\
Arguments:\n\
    -a : aide\n\
    -s <int> : le seuil\n\
    -r <fichier> : l'image de la reference tracee a la main\n";

//
// Prototypes des fonctions
//
void afficheAide(bool);
char *prendreNomFichier(char *fichier);

//
// La fonction main
//
int main(int argc, char *argv[])
{
    if (argc < 2) // manque les paramètres
    {
        afficheAide(true); // afficher les informations d'aide
    }

    bool drapeau_ref = false; // true : faire la comparaison avec la reference
    char* fichier_ref = 0; // le nom du fichier de la reference
    char *fichier_sortie = 0; // le nom du fichier sorti
    int seuil = 100; // le seuil

    // cette boucle lit les arguments passés
    int c;
    while ((c = getopt(argc, argv, "as:r:")) != -1)
    {
        switch (c)
        {
            case 'a':
            {
                afficheAide(false);
                break;
            }
            case 's':
            {
                extern char *optarg;
                seuil = atoi(optarg);
                break;
            }
            case 'r':
            {
                drapeau_ref = true;
                extern char *optarg;
                fichier_ref = optarg;
                break;
            }
            default:
                break;
        }
    }

    // lire le nom de fichier a la fin de la ligne de commande
    extern int optind;
    char *fichier_entre = NULL;
    if (optind>=argc)
    {
        afficheAide(true);
    }
    fichier_entre = argv[optind];

    // lire le contenu de l'image
    IplImage *img = 0;
    img = cvLoadImage(fichier_entre, 0); // avec la valeur 0 : transformer l'image en niveaux de gris
    if (!img)
    {
        // il n'existe pas de cette image
        cerr << "Ne pas pouvoir charger l'image " << fichier_entre << endl;
        exit(1);
    }

	// lire le contenu de l'image de la référence
    IplImage* ref = 0;
    if (drapeau_ref)
    {
        ref = cvLoadImage(fichier_ref, 0);
    }

    // si on ne passe pas le nom du fichier sorti
    if (fichier_sortie == 0)
    {
        fichier_sortie = prendreNomFichier(fichier_entre); // le nom du fichier entré sera pris
    }

    // créer un objet de la clasee Contours
    Contours* contours;
    if (drapeau_ref)
        contours = new Contours(fichier_sortie, img, ref, seuil);
    else
        contours = new Contours(fichier_sortie, img, seuil);

	// appeller la méthode extraireContours
    contours->extraireContours();

    delete contours;
    cvReleaseImage(&img);
    cvReleaseImage(&ref);
    return 0;
}

//
// La fonction sert à afficher les informations d'aide de ce programme
// @param :
//	bool erreur : true, cad il y a une erreur
//                          la fonction exécutera la commande exit(1)
//                    false, cad il n'y a pas d'erreur
//                           la fonction exécutera la commande exit(0)
//
void afficheAide(bool erreur)
{
    cerr << Usage << endl;
    if (erreur)
    {
        exit(1);
    }
    else
    {
        exit(0);
    }
}

//
// La fonction sert à prendre le nom de fichier sorti de le fichier entré
// @param :
//	char *fichier : le fichier entré
//
char *prendreNomFichier(char *fichier) // ex : /home/manleviet/12003.jpg
{
    // effacer le chemin dans le fichier entré
    char *nomFichier; // doit contenir 12003
    nomFichier = strrchr(fichier, '/');
    if (nomFichier == 0)
        nomFichier = fichier;
    else
        nomFichier++;
    // ici, nomFichier doit contenir 12003.jpg

    // effacer l'extension dans le fichier entré
    char *ext;
    ext = strrchr(fichier, '.');
    ext[0] = '\0';
    // ici, nomFichier doit contenir 12003

    return nomFichier;
}
