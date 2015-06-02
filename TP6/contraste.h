//
//
//  contraste.h
//
//  La classe permet d'modifier le contraste d'une image
//  bang cach tinh gia tri mau co histogram lon nhat, roi dua vao do de tinh hai diem moc de thay doi contraste
//  Les fonctionnalités :
//	- Dessiner l'histogramme de l'image entrée et de l'image transformée
//      - Dessiner la fonction de changement de contraste
//      - Afficher les résultats dans les fenêtres
//      - Modifier le contraste de l'image entrée
//	- Enregistrer les résultats dans des fichiers sous forme ppm
//
//  LE Viet Man
//  30/03/2010
//  modifie : 29/05/2010
//
//

#ifndef CONTRASTE_H
#define CONTRASTE_H

#include <cv.h>
#include "point.h"

int const MAXNIVEAUDEGRIS = 256;

class Contraste
{
public:
	
    //
    // Le constructeur de la classe Contraste
    //
    Contraste();

    //
    // Le destructeur
    //
    ~Contraste();

    //
    // Faire la modification du contraste de l'image entrée
    // @param :
    //      IplImage *img : une image
    // @result : une image modifiee
    //
    IplImage* modifierContraste(IplImage *img);

    //
    // Dessiner l'histogramme de l'image passée
    // @param :
    //		IplImage *img : l'image passée
    //		IplImage *&img_his : l'image de l'histogramme de l'image passée
    //
    IplImage* dessinerHistogramme(IplImage *img);

private:
    int LUT[MAXNIVEAUDEGRIS]; // le tableau LUT

    //
    // Calculer l'histogramme d'une image
    // @param :
    //      IplImage *img : une image
    //      float &min_value : une variable qui contenira la plus moins valeur
    //      float &max_value : une variable qui contenire la plus haute valeur
    // @result : CvHistogram
    //
    CvHistogram* calcHistogram(IplImage *img, float &min_value, float &max_value);

    //
    // Prendre la position de la plus haute valeur dans l'histogramme
    // @param :
    //      CvHistogram *hist : un histogramme
    //      float max_value : la plus haute valeur
    // @result : la position dans l'histogramme
    //
    int getPosMaxHist(CvHistogram *hist, float max_value);

    //
    // Calculer le tableau LUT
    // @param :
    //      Point *points : un array des Points de la fonction de transformation
    //      int taillePoints : la longeur du array
    //
    void calculerLUT(Point *points, int taillePoints);

    //
    // Faire la modification du contraste de l'image passée
    // @param:
    //      IplImage *img : une image
    // @result : une image modifiee
    //
    IplImage* faireContraste(IplImage *img);

    //
    // Dessiner la fonction de changement de contraste
    // @param :
    //      Point *points : un array des Point de la fonction de transfromation
    //      int taillePoints : la longeur du array
    // @result : une image de la fonction de transformation
    //
    IplImage* dessinerFonction(Point *points, int taillePoints);
};

#endif // CONTRASTE_H
