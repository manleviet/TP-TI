//
//
//  main.cpp
//
//  Une application complete de traitement d'image,
//  centree sur la segmentation
//
//  LE Viet Man
//  23/05/2010
//
//

#include <cv.h>
#include <highgui.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "contraste.h"
#include "point.h"

using namespace std;

char *Usage = (char*)"Usage: segment <filename>\n\
Une application complete de traitement d\'image centree sur la segmentation.\n\
Arguments:\n\
    -a : aide\n";

//
// Prototypes des fonctions
//
void showUsage(bool);
char* concatString(const char *st1, const char *st2);
char *getFilename(char *path);
void showImage(const char *title, const IplImage *img);
void saveImage(const char *filename, const char *ext, const IplImage *img);
IplImage *rehausserContours(IplImage *img);
IplImage *removeNoise(IplImage *img);
void foodfill(IplImage *img, int color, int new_color);
int labelling(IplImage *img);
IplImage *createMask(IplImage *img, int index);
IplImage *takeObject(IplImage *img, IplImage *mask);

//
// La fonction main
//
int main(int argc, char *argv[])
{
    if (argc < 2) // manque les paramètres
    {
        showUsage(true); // afficher les informations d'aide
    }

    char* filename = (char*)""; // l'image

    // lire le nom de fichier a la fin de la ligne de commande
    extern int optind;
    if (optind>=argc)
    {
        showUsage(true);
    }
    filename = argv[optind];
    // prendre seulement le nom du fichier
    char *name = (char*)malloc(sizeof(char)*strlen(filename));
    strcpy(name, filename);
    name = getFilename(name);

    // lire le contenu de l'image
    IplImage *img = 0;
    img = cvLoadImage(filename, -1);
    if (!img)
    {
        // il n'existe pas de cette image
        cerr << "Ne pas pouvoir charger l'image " << filename << endl;
        showUsage(false);
    }

    cout << "Pre-traitement...";
    // PRE-TRAITEMENT
    // convertir en niveaux de gris
    IplImage *gray = cvCreateImage(cvGetSize(img), img->depth, 1);
    cvConvertImage(img, gray, CV_BGR2GRAY);

    //saveImage(name, (char*)"_gray.png", gray);

    // Augmenter le contraste de l'image
    // en utilisant mon algorithme
    Contraste *contraste = new Contraste();
    gray = contraste->modifierContraste(gray);

    //saveImage(name, (char*)"_contraste.png", gray);

    // Lisser l'image en utilisant le filtre Gaussien
    cvSmooth(gray, gray, CV_GAUSSIAN, 7, 7);

    // Rehausser les contours dans l'image
    IplImage *rehaussee = rehausserContours(gray);

    //saveImage(name, (char*)"_rehaussee.png", rehaussee);

    cout << "Done" << endl;

    cout << "Segmentation...";
    // SEGMENTATION
    // detecter les contours
    int lowerThreshold = 0;// lower canny edge threshold initial setting
    int upperThreshold = 160; // upper canny edge threshold initial setting
    int matrixSize = 3; // la taille de matrix Canny

    IplImage *contour = cvCreateImage(cvGetSize(gray), gray->depth, gray->nChannels);
    cvCanny(rehaussee, contour, lowerThreshold, upperThreshold, matrixSize);

    //saveImage(name, (char*)"_contour.png", contour);

    cout << "Done" << endl;

    cout << "Post-traitement...";
    // POST-TRAITEMENT
    // effacer les bruits
    contour = removeNoise(contour);

    //saveImage(name, (char*)"_remnoise.png", contour);

    // effacer le fond
    foodfill(contour, 0, 255);

    //saveImage(name, (char*)"_foodfill1.png", contour);

    // convertir le fond en blanc en le fond en noire
    cvThreshold(contour, contour, 254, 255, CV_THRESH_BINARY_INV);

    //saveImage(name, (char*)"_threshold.png", contour);

    // effacer les bruits au coin de l'image
    foodfill(contour, 255, 0);

    //saveImage(name, (char*)"_foodfill2.png", contour);

    // effacer encore les bruits
    cvErode(contour, contour, NULL, 3);

    //saveImage(name, (char*)"_erode1.png", contour);

    // lier les composants inconnexes
    IplImage *dilate = cvCreateImage(cvGetSize(gray), gray->depth, gray->nChannels);
    cvDilate(contour, dilate, NULL, 6);

    //saveImage(name, (char*)"_dilate.png", dilate);

    // eroder les objects
    IplImage *erode = cvCreateImage(cvGetSize(gray), gray->depth, gray->nChannels);
    cvErode(dilate, erode, NULL, 2); // tham so 1 co the thay doi

    //saveImage(name, (char*)"_erode2.png", erode);

    cout << "Done" << endl;

    cout << "Etiquetage...";
    // l'etiquetage
    IplImage *label = cvCloneImage(erode);

    int num_seg = labelling(label);
    cout << "Done" << endl;
    cout << "Nombre des objets : " << num_seg << endl;

    cout << "Creer les masques...";
    // creer les masques
    IplImage *masks[num_seg];
    for (int i = 1; i <= num_seg; i++)
    {
        masks[i - 1] = createMask(label, i);
    }
    cout << "Done" << endl;

    //saveImage(name, (char*)"_mask.png", masks[3]);

    cout << "Prendre les objects...";
    // prendre les objects
    IplImage *objects[num_seg];
    for (int i = 0; i < num_seg; i++)
    {
        objects[i] = takeObject(img, masks[i]);
    }
    cout << "Done" << endl;

    // afficher l'image originale et les objects segmentes
    showImage((char*)"image originale", img);
    for (int i = 0; i < num_seg; i++)
    {
        char buffer[10];
        sprintf(buffer, "%d", i + 1);
        char *title = concatString((char*)"object", buffer);
        showImage(title, objects[i]);
    }
    cvWaitKey(0);

    // enregistrer les objects sur les fichier separees
    for (int i = 0; i < num_seg; i++)
    {
        char buffer[20];
        sprintf(buffer, "_obj%d.png", i + 1);
        saveImage(name, buffer, objects[i]);
    }

    delete contraste;
    cvReleaseImage(&img);
    cvReleaseImage(&gray);
    cvReleaseImage(&rehaussee);
    cvReleaseImage(&contour);
    cvReleaseImage(&dilate);
    cvReleaseImage(&erode);
    cvReleaseImage(&label);
    for (int i = 0; i < num_seg; i++)
    {
        cvReleaseImage(&masks[i]);
        cvReleaseImage(&objects[i]);
    }

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
void showUsage(bool erreur)
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
char *getFilename(char *path) // ex : /home/manleviet/12003.jpg
{
    // effacer le chemin dans le fichier entré
    char *filename; // doit contenir 12003
    filename = strrchr(path, '/');
    if (filename == 0)
        filename = path;
    else
        filename++;
    // ici, nomFichier doit contenir 12003.jpg

    // effacer l'extension dans le fichier entré
    char *ext;
    ext = strrchr(path, '.');
    ext[0] = '\0';
    // ici, nomFichier doit contenir 12003

    return filename;
}

//
// Joindre deux strings en un strings
// @param :
//      const char *st1 : premier string
//      const char *st2 : deuxieme string
// @result : un string
//
char* concatString(const char *st1, const char *st2)
{
    char *st;
    st = (char *)malloc(sizeof(char)
                         * (strlen(st1) + strlen(st2) + 1));
    strcpy(st, st1);
    strcat(st, st2);
    return st;
}

//
// Afficher les resultats sur les fenetres
// @param :
//      const char *title : le nom de la fenetre
//      const IplImage *img : une image
//
void showImage(const char *title, const IplImage *img)
{
    cvNamedWindow(title, 0);
    cvShowImage(title, img);
}

//
// Enregistrer une image sur le fichier
// @param :
//      const char *filename : le chemin et le nom du fichier sortie
//      const IplImage *img : l'image
//
void saveImage(const char *filename, const char *ext, const IplImage *img)
{
    char* fileout;
    fileout = concatString(filename, ext); // le nom du fichier sortie finit avec _wat.png

    cvSaveImage(fileout, img);
}

//
// Rehausser les contours dans une image
// code-source : http://www.siteduzero.com/tutoriel-3-80697-les-filtres-de-convolution.html#ss_part_4
// @param :
//      IplImage *img : une image
// @result : une image autant les contours rehausses
//
IplImage *rehausserContours(IplImage *img)
{
    // Résultat du filtre gaussien
    IplImage* lissee    = cvCloneImage( img );

    // Résultat du rehaussement de contours
    IplImage* rehaussee = cvCloneImage( img );

    // Masques de convolution
    CvMat* rehaussement = cvCreateMat( 3, 3, CV_32FC1);
    CvMat* gaussien     = cvCloneMat( rehaussement );

    // Remplissage du masque du rehaussement de contours :
    //
    //	|   0   -1    0   |
    //	|  -1    5   -1   |
    //	|   0   -1    0   |

    // On initialise la matrice avec des 0 partout...
    cvSet( rehaussement, cvScalar( 0 ) );

    // ... puis on fixe les autres valeurs.
    cvmSet( rehaussement, 0, 1, -1 );
    cvmSet( rehaussement, 1, 0, -1 );
    cvmSet( rehaussement, 1, 2, -1 );
    cvmSet( rehaussement, 2, 1, -1 );
    cvmSet( rehaussement, 1, 1,  5 );


    // Remplissage du masque du filtre gaussien « approximatif » :
    //
    // |   1   2   1   |
    // |   2   4   2   |
    // |   1   2   1   |
    // somme = 16

    cvSet( gaussien, cvScalar( 1.0 / 16.0 ) );

    cvmSet( gaussien, 0, 1, 2.0 / 16.0 );
    cvmSet( gaussien, 1, 0, 2.0 / 16.0 );
    cvmSet( gaussien, 1, 2, 2.0 / 16.0 );
    cvmSet( gaussien, 2, 1, 2.0 / 16.0 );
    cvmSet( gaussien, 1, 1, 4.0 / 16.0 );

    // Convolution
    //cvFilter2D( img, lissee, gaussien );
    for (int i = 0; i < 10; i++)
    {
        cvFilter2D( img, rehaussee, rehaussement );
    }

    cvReleaseImage( &lissee );

    cvReleaseMat( &gaussien );
    cvReleaseMat( &rehaussement );

    return rehaussee;
}

//
// Supprimer les bruits
// code-source : http://stackoverflow.com/questions/2243646/object-detection-in-opencv
// @param :
//      IplImage *img : une image
// @result : une image modifiee
//
IplImage *removeNoise(IplImage *img)
{
    // supprimer les bruits
    IplImage *output = cvCloneImage(img);
    CvSize sz = cvSize( img->width & -2, img->height & -2 );
    IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 1 );

    cvPyrDown( output, pyr, 7 );
    cvPyrUp( pyr, output, 7 );

    return output;
}

//
// Traverser les pixels qui sont au bord de l'image
// si sa couleur est color, mettre la nouvelle valeur new_color.
// @param:
//      IplImage *img : une image
//      int color : la couleur (0-255)
//      int new_color : la nouvelle couleur (0-255)
//
void foodfill(IplImage *img, int color, int new_color)
{
    CvScalar new_val;
    new_val.val[0] = new_color;

    // traverser les pixels sont a la ligne verticale
    for (int i = 0; i < img->height - 1; i++)
    {
        CvScalar s1 = cvGet2D(img, i, 0);
        CvScalar s2 = cvGet2D(img, i, img->width - 1);

        if (s1.val[0] == color)
        {
            CvPoint seed1;
            seed1.x = 0;
            seed1.y = i;
            cvFloodFill(img, seed1, new_val);
        }

        if (s2.val[0] == color)
        {
            CvPoint seed2;
            seed2.x = img->width - 1;
            seed2.y = i;
            cvFloodFill(img, seed2, new_val);
        }
    }

    // traverser les pixels sont a la ligne horizontale
    for (int i = 0; i < img->width - 1; i++)
    {
        CvScalar s1 = cvGet2D(img, 0, i);
        CvScalar s2 = cvGet2D(img, img->height - 1, i);

        if (s1.val[0] == color)
        {
            CvPoint seed1;
            seed1.x = i;
            seed1.y = 0;
            cvFloodFill(img, seed1, new_val);
        }

        if (s2.val[0] == color)
        {
            CvPoint seed2;
            seed2.x = i;
            seed2.y = img->height - 1;
            cvFloodFill(img, seed2, new_val);
        }
    }
}

//
// Etiquetage
// @param :
//      IplImage *img : une image, apres l'etiquetage, chaque composante a une chiffre (1-nombre des composantes)
// @result : nombre des composantes connexes
//
int labelling(IplImage *img)
{
   int num_seg = 0;
   for (int i = 0; i < img->height; i++)
   {
       for (int j = 0; j < img->width; j++)
       {
           CvScalar s = cvGet2D(img, i, j);
           if (s.val[0] == 255)
           {
               num_seg++;
               CvPoint seed;
               seed.x = j;
               seed.y = i;
               s.val[0] = num_seg;
               cvFloodFill(img, seed, s);
           }
       }
   }

   return num_seg;
}

//
// Creer un masque d'un objet
// @param :
//      IplImage *img : une image qui a ete fait l'etiquetage
//      int index : l'indice du composante
// @result : le masque de l'objet index eme
//
IplImage *createMask(IplImage *img, int index)
{
    IplImage *mask = cvCloneImage(img);
    cvZero(mask);

    for (int i = 0; i < img->height; i++)
    {
        for (int j = 0; j < img->width; j++)
        {
            CvScalar s = cvGet2D(img, i, j);
            if (s.val[0] == index)
            {
                s.val[0] = 255;
                cvSet2D(mask, i, j, s);
            }
        }
    }
    cvThreshold(mask, mask, 200, 255, CV_THRESH_BINARY_INV);

    return mask;
}

//
// Prendre un objet
// @param :
//      IplImage *img : l'image originale
//      IplImage *mask : un masque
// @result : l'image de l'objet
//
IplImage *takeObject(IplImage *img, IplImage *mask)
{
    IplImage *object = cvCloneImage(img);
    cvZero(object);

    for (int i = 0; i < img->height; i++)
    {
        for (int j = 0; j < img->width; j++)
        {
            CvScalar s = cvGet2D(mask, i, j);
            if (s.val[0] == 0)
            {
                CvScalar t = cvGet2D(img, i, j);
                cvSet2D(object, i, j, t);
            }
        }
    }

    return object;
}
