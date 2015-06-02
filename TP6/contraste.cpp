//
//
//  contraste.cpp
//
//  L'implémentation la classe Contraste
//
//  LE Viet Man
//  30/03/2010
//  modifie 29/05/2010
//
//

#include "contraste.h"
#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;

//
// Le constructeur de la classe Contraste
// @param :
//      IplImage *img : un objet de IplImage, une structure stockée l'image
//
Contraste::Contraste()
{
}

//
// Le destructeur
//
Contraste::~Contraste()
{
}

//
// Faire la modification du contraste de l'image entrée
// @param :
//      IplImage *img : une image
// @result : une image modifiee
//
IplImage* Contraste::modifierContraste(IplImage *img)
{
    float min_value, max_value;
    // calculer l'histogramme de l'image originale
    CvHistogram *hist = calcHistogram(img, min_value, max_value);

    int max_pos = getPosMaxHist(hist, max_value);

    // calculer deux points
    int a = max_pos - 60;
    int b = max_pos + 10;

    // creer la fonction de transformation
    int taillePoints = 4;
    Point *points = new Point[taillePoints];
    points[0].setX(0);
    points[0].setY(0);
    points[1].setX(a);
    points[1].setY(a);
    points[2].setX(b);
    points[2].setY(250);
    points[3].setX(255);
    points[3].setY(255);

    // ve fonction
    //img_fonction = dessinerFonction(points, taillePoints);
    // calculer le tableau LUT
    calculerLUT(points, taillePoints);
    // faire la modification du contraste de l'image originale
    IplImage *img_contraste = faireContraste(img);

    return img_contraste;
}

//
// Calculer l'histogramme d'une image
// @param :
//      IplImage *img : une image
//      float &min_value : une variable qui contenira la plus moins valeur
//      float &max_value : une variable qui contenire la plus haute valeur
// @result : CvHistogram
//
CvHistogram* Contraste::calcHistogram(IplImage *img, float &min_value, float &max_value)
{
    // la taille de l'histogramme
    int bins = 256;
    int hsize[] = {bins};

    //ranges - grayscale 0 to 256
    float xranges[] = { 0, 256 };
    float* ranges[] = { xranges };

    //planes to obtain the histogram, in this case just one
    IplImage* planes[] = { img };

    //get the histogram and some info about it
    CvHistogram *hist = cvCreateHist( 1, hsize, CV_HIST_ARRAY, ranges,1);
    cvCalcHist( planes, hist, 0, NULL); // calculer l'histogramme de l'image
    cvGetMinMaxHistValue( hist, &min_value, &max_value); // recevoir max et min

    return hist;
}

//
// Prendre la position de la plus haute valeur dans l'histogramme
// @param :
//      CvHistogram *hist : un histogramme
//      float max_value : la plus haute valeur
// @result : la position dans l'histogramme
//
int Contraste::getPosMaxHist(CvHistogram *hist, float max_value)
{
    int i;
    for (i = 0; i < 256; i++)
    {
        float value = cvQueryHistValue_1D(hist, i);
        if (value == max_value) break;
    }

    if (i == 256) return 256;
    return i;
}

//
// Dessiner l'histogramme de l'image passée
// @param :
//		IplImage *img : l'image passée
//		IplImage *&img_his : l'image de l'histogramme de l'image passée
//
IplImage* Contraste::dessinerHistogramme(IplImage *img)
{
    float min_value, max_value;

    CvHistogram *hist = calcHistogram(img, min_value, max_value);

    //value and normalized value
    float value;
    int normalized;

    //create an 8 bits single channel image to hold the histogram
    //paint it white
    IplImage *img_his = cvCreateImage(cvSize(256, 300), 8, 3);
    cvRectangle(img_his, cvPoint(0,0), cvPoint(256,300), cvScalar(255,255,255), -1);

    //draw the histogram
    for(int i=0; i < 256; i++){
        value = cvQueryHistValue_1D( hist, i);
        normalized = cvRound(value*300/max_value);
        cvLine(img_his, cvPoint(i,300), cvPoint(i,300-normalized), cvScalar(0,0,0));
    }

    return img_his;
}

//
// Calculer le tableau LUT
// @param :
//      Point *points : un array des Points de la fonction de transformation
//      int taillePoints : la longeur du array
//
void Contraste::calculerLUT(Point *points, int taillePoints)
{
    for (int i = 0; i < taillePoints - 1; i++)
    {
        // La coordonnée du premier point
        int ax = points[i].getX();
        int ay = points[i].getY();
        // La coordonnée du deuxième point
        int bx = points[i+1].getX();
        int by = points[i+1].getY();

        for (int j = ax; j < bx; j++)
        {
            if (j == 0) // si a est la racine
            {
                LUT[j] = ay;
                continue;
            }
            if (ax == bx) // la ligne est horizontale
            {
                LUT[j] = ay;
                continue;
            }
            if (ay == by) // la ligne est verticale
            {
                LUT[j] = ay;
                continue;
            }
            if (by > ay) // la ligne est inclinée à droite
                LUT[j] = ((by - ay) * (j - ax))/(bx - ax) + ay;
            else // by < ay // la ligne est inclinée à gauche
                LUT[j] = ((ay - by) * (bx - j))/(bx - ax) + by;
        }
    }
    LUT[255] = points[taillePoints - 1].getY();
}

//
// Faire la modification du contraste de l'image passée
// @param:
//      IplImage *img : une image
// @result : une image modifiee
//
IplImage* Contraste::faireContraste(IplImage *img)
{
    int height = img->height;
    int width = img->width;
    // créer l'image qui contenira l'image modifiée
    IplImage *img_contraste = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

    // faire la modification du contraste de l'image entrée
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j< width; j++)
        {
            CvScalar v = cvGet2D(img, i, j); // recevoir la valeur du pixel (i,j)
            int val = v.val[0];
            v.val[0] = LUT[val];
            cvSet2D(img_contraste, i, j, v); // mettre la nouvelle valeur au pixel (i,j)
        }
    }

    return img_contraste;
}

//
// Dessiner la fonction de changement de contraste
// @param :
//      Point *points : un array des Point de la fonction de transfromation
//      int taillePoints : la longeur du array
// @result : une image de la fonction de transformation
//
IplImage* Contraste::dessinerFonction(Point *points, int taillePoints)
{
    // créer l'image qui contenira la fonction de changement de contraste
    IplImage *img_fonction = cvCreateImage(cvSize(256,256), 8, 3);
    // la peindre en blanc
    cvRectangle(img_fonction, cvPoint(0,0), cvPoint(256,256), cvScalar(255,255,255), -1);

    // dessiner les lignes entres chaque paire de deux points
    for (int i = 0; i < taillePoints-1; i++)
    {
        cvLine(img_fonction,
                cvPoint(points[i].getX(), 256 - points[i].getY()),
                cvPoint(points[i+1].getX(), 256 - points[i+1].getY()),
                cvScalar(0,0,0),
                1);
    }

    return img_fonction;
}
