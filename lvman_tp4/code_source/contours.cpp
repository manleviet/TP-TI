//
//
// contours.cpp
//
// L'implémentation de la classe contours.h 
//
// LE Viet Man
// 02/05/10
//
//

#include "contours.h"
#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;

//
// Le constructeur de la classe Contours
// @param :
//		char* fichier : le nom du fichier sorti
//      IplImage *img : un objet de IplImage, une structure stockée l'image
//		int seuil : le seuil de contours
//	
Contours::Contours(char *fichier, IplImage *img, int seuil)
{
    this->image = img;
    this->reference = 0;
    this->seuil = seuil;

	// creer les noms des fichiers sorties
    this->fichier_sobel = this->creerNomFichierSortie(fichier, (char*)"_sobel.pgm");
    this->fichier_prewitt = this->creerNomFichierSortie(fichier, (char*)"_prewitt.pgm");
    this->fichier_laplace = this->creerNomFichierSortie(fichier, (char*)"_laplace.pgm");
    this->fichier_canny = this->creerNomFichierSortie(fichier, (char*)"_canny.pgm");
}

//
// Le constructeur de la classe Contours
// @param :
//		char* fichier : le nom du fichier sorti
//      IplImage *img : un objet de IplImage, une structure stockée l'image
//		IplImage* ref : l'image de la référence tracée à la main
//		int seuil : le seuil de contours
//	
Contours::Contours(char *fichier, IplImage *img, IplImage *ref, int seuil)
{
    this->image = img;
    this->reference = ref;
    this->seuil = seuil;

	// creer les noms des fichiers sorties
    this->fichier_sobel = this->creerNomFichierSortie(fichier, (char*)"_sobel.pgm");
    this->fichier_prewitt = this->creerNomFichierSortie(fichier, (char*)"_prewitt.pgm");
    this->fichier_laplace = this->creerNomFichierSortie(fichier, (char*)"_laplace.pgm");
    this->fichier_canny = this->creerNomFichierSortie(fichier, (char*)"_canny.pgm");
}

// 
// Extraire les contours en utilisant quatre algorithmes Sobel, Prewitt, Laplace et Canny
// Enregister les images de résultats
// Afficher les images sur les fenêtres
// Calculer trois mesures P, TFP et TFN pour chaque algorithmes
//
void Contours::extraireContours()
{
	// extraire les contours en utilisant quatre algorithmes Sobel, Prewitt, Laplace et Canny
    this->filterSobel();
    this->filterPrewitt();
    this->filterLaplace();
    this->filterCanny();
    
	// enregistrer les images de résultats
    this->enregistrerResultat(this->sobel, this->fichier_sobel);
    this->enregistrerResultat(this->prewitt, this->fichier_prewitt);
    this->enregistrerResultat(this->laplace, this->fichier_laplace);
    this->enregistrerResultat(this->canny, this->fichier_canny);
    
	// afficher les images sur les fenêtres
    this->afficheFenetre(this->sobel, (char*)"SOBEL");
    this->afficheFenetre(this->prewitt, (char*)"PREWITT");
    this->afficheFenetre(this->laplace, (char*)"LAPLACE");
    this->afficheFenetre(this->canny, (char*)"CANNY");
    cvWaitKey(0);

    // calculer trois mesures P, TFP et TFN pour chaque algorithmes
    int contours_detectes = 0;
    int contours_reference = 0;
    int contours_corrects = 0;
    int faux_positifs = 0;
    int faux_negatifs = 0;

    double P;
    double TFP;
    double TFN;

    contours_reference = compterContours(this->reference);

    // SOBEL
    cout << "========== SOBEL ==========" << endl;

    contours_detectes = compterContours(this->sobel);
    contours_corrects = compterContoursCorrects(this->reference, this->sobel);

    faux_positifs = abs(contours_corrects - contours_detectes);
    faux_negatifs = abs(contours_corrects - contours_reference);

    P = (double)contours_corrects / (double)(contours_corrects + faux_positifs + faux_negatifs);
    TFP = (double)faux_positifs / (double)(contours_corrects + faux_positifs + faux_negatifs);
    TFN = (double)faux_negatifs / (double)(contours_corrects + faux_positifs + faux_negatifs);

    cout << "Contours_reference : " << contours_reference << endl;
    cout << "Contours_detectes  : " << contours_detectes << endl;
    cout << "Contours_corrects  : " << contours_corrects << endl;
    cout << "Faux_positifs      : " << faux_positifs << endl;
    cout << "Faux_negatifs      : " << faux_negatifs << endl;

    cout << "P                  : " << P << endl;
    cout << "TFP                : " << TFP << endl;
    cout << "TFN                : " << TFN << endl;

    // PREWITT
    cout << "========== PREWITT ==========" << endl;

    contours_detectes = compterContours(this->prewitt);

    contours_corrects = compterContoursCorrects(this->reference, this->prewitt);

    faux_positifs = abs(contours_corrects - contours_detectes);
    faux_negatifs = abs(contours_corrects - contours_reference);

    // Calculer P, TFP, TFN
    P = (double)contours_corrects / (double)(contours_corrects + faux_positifs + faux_negatifs);
    TFP = (double)faux_positifs / (double)(contours_corrects + faux_positifs + faux_negatifs);
    TFN = (double)faux_negatifs / (double)(contours_corrects + faux_positifs + faux_negatifs);

    cout << "Contours_reference : " << contours_reference << endl;
    cout << "Contours_detectes  : " << contours_detectes << endl;
    cout << "Contours_corrects  : " << contours_corrects << endl;
    cout << "Faux_positifs      : " << faux_positifs << endl;
    cout << "Faux_negatifs      : " << faux_negatifs << endl;

    cout << "P                  : " << P << endl;
    cout << "TFP                : " << TFP << endl;
    cout << "TFN                : " << TFN << endl;

    // LAPLACE
    cout << "========== LAPLACE ==========" << endl;

    contours_detectes = compterContours(this->laplace);

    contours_corrects = compterContoursCorrects(this->reference, this->laplace);

    faux_positifs = abs(contours_corrects - contours_detectes);
    faux_negatifs = abs(contours_corrects - contours_reference);

    P = (double)contours_corrects / (double)(contours_corrects + faux_positifs + faux_negatifs);
    TFP = (double)faux_positifs / (double)(contours_corrects + faux_positifs + faux_negatifs);
    TFN = (double)faux_negatifs / (double)(contours_corrects + faux_positifs + faux_negatifs);

    cout << "Contours_reference : " << contours_reference << endl;
    cout << "Contours_detectes  : " << contours_detectes << endl;
    cout << "Contours_corrects  : " << contours_corrects << endl;
    cout << "Faux_positifs      : " << faux_positifs << endl;
    cout << "Faux_negatifs      : " << faux_negatifs << endl;

    cout << "P                  : " << P << endl;
    cout << "TFP                : " << TFP << endl;
    cout << "TFN                : " << TFN << endl;

    // CANNY
    cout << "========== CANNY ==========" << endl;

    contours_detectes = compterContours(this->canny);

    contours_corrects = compterContoursCorrects(this->reference, this->canny);

    faux_positifs = abs(contours_corrects - contours_detectes);
    faux_negatifs = abs(contours_corrects - contours_reference);

    P = (double)contours_corrects / (double)(contours_corrects + faux_positifs + faux_negatifs);
    TFP = (double)faux_positifs / (double)(contours_corrects + faux_positifs + faux_negatifs);
    TFN = (double)faux_negatifs / (double)(contours_corrects + faux_positifs + faux_negatifs);

    cout << "Contours_reference : " << contours_reference << endl;
    cout << "Contours_detectes  : " << contours_detectes << endl;
    cout << "Contours_corrects  : " << contours_corrects << endl;
    cout << "Faux_positifs      : " << faux_positifs << endl;
    cout << "Faux_negatifs      : " << faux_negatifs << endl;

    cout << "P                  : " << P << endl;
    cout << "TFP                : " << TFP << endl;
    cout << "TFN                : " << TFN << endl;
}

//
// Extraire les contours en utilisant l'algorithme Sobel
//
void Contours::filterSobel()
{
    IplImage* dst_h = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
    IplImage* dst_v = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1);
    sobel = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1);

	// calculer le Sobel horizontal et vertical
    cvSobel(image, dst_h, 1, 0, 3);
    cvSobel(image, dst_v, 0, 1, 3);
	// fusionner les deux Sobel
    cvOr(dst_h, dst_v, sobel, NULL);
	// appliquer le seuillage
    cvThreshold( sobel, sobel, seuil, 255, CV_THRESH_BINARY_INV);

    cvReleaseImage(&dst_h);
    cvReleaseImage(&dst_v);
}

//
// Extraire les contours en utilisant l'algorithme Prewitt
//
void Contours::filterPrewitt()
{
    IplImage* dst_h = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
    IplImage* dst_v = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1);
    prewitt = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1);

	// Calculer le Prewitt horizontal et vertical
    filterPrewitt(image, dst_h, PREWITT_HOR);
    filterPrewitt(image, dst_v, PREWITT_VER);
	// fusionner les deux Prewitt
    cvOr(dst_h, dst_v, prewitt, NULL);
	// appliquer le seuillage
    cvThreshold( prewitt, prewitt, seuil, 255, CV_THRESH_BINARY_INV);

    cvReleaseImage(&dst_h);
    cvReleaseImage(&dst_v);
}

//
// Extraire les contours en utilisant l'algorithme Laplace
//
void Contours::filterLaplace()
{
    IplImage* temp = cvCreateImage( cvGetSize(image), IPL_DEPTH_16S, 1);
    laplace = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	// calculer le Laplace avec le masquage de taille 3
    cvLaplace(image, temp, 3);
	// appliquer le seuillage
    cvThreshold( temp, laplace, seuil, 255, CV_THRESH_BINARY_INV);

    cvReleaseImage(&temp);
}

//
// Extraire les contours en utilisant l'algorithme Canny
//
void Contours::filterCanny()
{
    canny = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1);
	// calculer le Canny avec le masquage de taille 3
    cvCanny(image, canny, 0, 180, 3);
	// appliquer le seuillage
    cvThreshold(canny, canny, 100, 255, CV_THRESH_BINARY_INV);
}

//
// Compter les contours corrects dans une image
// @param :
//		IplImage* ref : l'image de la référence tracée à la main
//		IplImage* img : l'image de contours
//
unsigned int Contours::compterContoursCorrects(IplImage* ref, IplImage* img)
{
    unsigned int compte = 0;
    for (int i = 0; i < ref->height; i++)
    {
        for (int j = 0; j < ref->width; j++)
        {
            CvScalar c_ref = cvGet2D(ref, i, j);

            if (c_ref.val[0] == 0)
            {
                CvScalar s;
                s.val[0] = 255;
                CvScalar s11, s12, s13, s21, s22, s23, s31, s32, s33;
                if (i == 0)
                {
                    if (j == 0)
                    {
                        s11.val[0] = 255;
                        s12.val[0] = 255;
                        s13.val[0] = 255;
                        s21.val[0] = 255;
                        s22 = cvGet2D(img, i, j);
                        s23 = cvGet2D(img, i, j+1);
                        s31.val[0] = 255;
                        s32 = cvGet2D(img, i+1, j);
                        s33 = cvGet2D(img, i+1, j+1);
                    }
                    else if (j == ref->width - 1)
                    {
                        s11.val[0] = 255;
                        s12.val[0] = 255;
                        s13.val[0] = 255;
                        s21 = cvGet2D(img, i, j-1);
                        s22 = cvGet2D(img, i, j);
                        s23.val[0] = 255;
                        s31 = cvGet2D(img, i+1, j-1);
                        s32 = cvGet2D(img, i+1, j);
                        s33.val[0] = 255;
                    }
                    else
                    {
                        s11.val[0] = 255;
                        s12.val[0] = 255;
                        s13.val[0] = 255;
                        s21 = cvGet2D(img, i, j-1);
                        s22 = cvGet2D(img, i, j);
                        s23 = cvGet2D(img, i, j+1);
                        s31 = cvGet2D(img, i+1, j-1);
                        s32 = cvGet2D(img, i+1, j);
                        s33 = cvGet2D(img, i+1, j+1);
                    }
                }
                else if (i == ref->height - 1)
                {
                    if (j == 0)
                    {
                        s11.val[0] = 255;
                        s12 = cvGet2D(img, i-1, j);
                        s13 = cvGet2D(img, i-1, j+1);
                        s21.val[0] = 255;
                        s22 = cvGet2D(img, i, j);
                        s23 = cvGet2D(img, i, j+1);
                        s31.val[0] = 255;
                        s32.val[0] = 255;
                        s33.val[0] = 255;
                    }
                    else if (j == ref->width - 1)
                    {
                        s11 = cvGet2D(img, i-1, j-1);
                        s12 = cvGet2D(img, i-1, j);
                        s13.val[0] = 255;
                        s21 = cvGet2D(img, i, j-1);
                        s22 = cvGet2D(img, i, j);
                        s23.val[0] = 255;
                        s31.val[0] = 255;
                        s32.val[0] = 255;
                        s33.val[0] = 255;
                    }
                    else
                    {
                        s11 = cvGet2D(img, i-1, j-1);
                        s12 = cvGet2D(img, i-1, j);
                        s13 = cvGet2D(img, i-1, j+1);
                        s21 = cvGet2D(img, i, j-1);
                        s22 = cvGet2D(img, i, j);
                        s23 = cvGet2D(img, i, j+1);
                        s31.val[0] = 255;
                        s32.val[0] = 255;
                        s33.val[0] = 255;
                    }
                }
                else
                {
                    if (j == 0)
                    {
                        s11.val[0] = 255;
                        s12 = cvGet2D(img, i-1, j);
                        s13 = cvGet2D(img, i-1, j+1);
                        s21.val[0] = 255;
                        s22 = cvGet2D(img, i, j);
                        s23 = cvGet2D(img, i, j+1);
                        s31.val[0] = 255;
                        s32 = cvGet2D(img, i+1, j);
                        s33 = cvGet2D(img, i+1, j+1);
                    }
                    else if (j == ref->width - 1)
                    {
                        s11 = cvGet2D(img, i-1, j-1);
                        s12 = cvGet2D(img, i-1, j);
                        s13.val[0] = 255;
                        s21 = cvGet2D(img, i, j-1);
                        s22 = cvGet2D(img, i, j);
                        s23.val[0] = 255;
                        s31 = cvGet2D(img, i+1, j-1);
                        s32 = cvGet2D(img, i+1, j);
                        s33.val[0] = 255;
                    }
                    else
                    {
                        s11 = cvGet2D(img, i-1, j-1);
                        s12 = cvGet2D(img, i-1, j);
                        s13 = cvGet2D(img, i-1, j+1);
                        s21 = cvGet2D(img, i, j-1);
                        s22 = cvGet2D(img, i, j);
                        s23 = cvGet2D(img, i, j+1);
                        s31 = cvGet2D(img, i+1, j-1);
                        s32 = cvGet2D(img, i+1, j);
                        s33 = cvGet2D(img, i+1, j+1);
                    }
                }

                // kiem tra trung vao xoa
                if (c_ref.val[0] == s22.val[0])
                {
                    compte++;
                    cvSet2D(img, i, j, s);
                }
                else if (c_ref.val[0] == s11.val[0])
                {
                    compte++;
                    cvSet2D(img, i - 1, j - 1, s);
                }
                else if (c_ref.val[0] == s12.val[0])
                {
                    compte++;
                    cvSet2D(img, i - 1, j, s);
                }
                else if (c_ref.val[0] == s13.val[0])
                {
                    compte++;
                    cvSet2D(img, i - 1, j + 1, s);
                }
                else if (c_ref.val[0] == s21.val[0])
                {
                    compte++;
                    cvSet2D(img, i, j - 1, s);
                }
                else if (c_ref.val[0] == s23.val[0])
                {
                    compte++;
                    cvSet2D(img, i, j + 1, s);
                }
                else if (c_ref.val[0] == s31.val[0])
                {
                    compte++;
                    cvSet2D(img, i + 1, j - 1, s);
                }
                else if (c_ref.val[0] == s32.val[0])
                {
                    compte++;
                    cvSet2D(img, i + 1, j, s);
                }
                else if (c_ref.val[0] == s33.val[0])
                    {
                        compte++;
                        cvSet2D(img, i + 1, j + 1, s);
                    }
            }
        }
    }
    return compte;
}

//
// Compter les contours dans une image
// @param :
//		IplImage* img : l'image de contours
//
unsigned int Contours::compterContours(IplImage* img)
{
    unsigned int compte = 0;
    for (int i = 0; i < img->height; i++)
    {
        for (int j = 0; j < img->width; j++)
        {
            CvScalar c = cvGet2D(img, i, j);

            if (c.val[0] == 0) compte++;
        }
    }
    return compte;
}

//
// Créer les nom du fichier sortie
// @param :
//	char *fichier : le nom du fichier sortie
//	char *ext : la partie de l'extension
//
char* Contours::creerNomFichierSortie(const char *fichier, const char *ext) // fichier : abc, ext : _out.ppm
{
    char *nom;
    nom = (char *)malloc(sizeof(char)
                         * (strlen(fichier) + strlen(ext) + 1));
    strcpy(nom, fichier); // nom : abc

    strcat(nom, ext); // nom : abc_out.ppm

    return nom;
}

//
// Extraire les contours en utilisant l'algorithme Prewitt
// @param :
//		IplImage* src : l'image de source
//		IplImage* dst : l'image de destination
//		int type :	le type de contours (PREWITT_HOR ou 0 : contours horizontaux)
//										(PREWITT_VER ou 1 : contours verticaux)
//
void Contours::filterPrewitt(IplImage* src, IplImage* dst, int type)
{
    for (int i = 1; i < src->height - 1; i++)
    {
        for (int j = 1; j < src->width - 1; j++)
        {
            CvScalar center = cvGet2D(src, i, j);

            CvScalar s11, s12, s13, s21, s22, s23, s31, s32, s33;
            s11 = cvGet2D(src, i-1, j-1);
            s12 = cvGet2D(src, i-1, j);
            s13 = cvGet2D(src, i-1, j+1);
            s21 = cvGet2D(src, i, j-1);
            s22 = cvGet2D(src, i, j);
            s23 = cvGet2D(src, i, j+1);
            s31 = cvGet2D(src, i+1, j-1);
            s32 = cvGet2D(src, i+1, j);
            s33 = cvGet2D(src, i+1, j+1);

            // tinh gia tri
            int sum;
            if (type == PREWITT_HOR)
            {
                sum = s11.val[0] * prewitt1[0][0] + s12.val[0] * prewitt1[0][1] + s13.val[0] * prewitt1[0][2] +
                      s21.val[0] * prewitt1[1][0] + s22.val[0] * prewitt1[1][1] + s23.val[0] * prewitt1[1][2] +
                      s31.val[0] * prewitt1[2][0] + s32.val[0] * prewitt1[2][1] + s33.val[0] * prewitt1[2][2];
            }
            else
            {
                sum = s11.val[0] * prewitt2[0][0] + s12.val[0] * prewitt2[0][1] + s13.val[0] * prewitt2[0][2] +
                      s21.val[0] * prewitt2[1][0] + s22.val[0] * prewitt2[1][1] + s23.val[0] * prewitt2[1][2] +
                      s31.val[0] * prewitt2[2][0] + s32.val[0] * prewitt2[2][1] + s33.val[0] * prewitt2[2][2];
            }

            center.val[0] = sum;
            cvSet2D(dst, i, j, center);
        }
    }
}

//
// Afficher la fenêtre du résultat
// @param :
//	IplImage *img : l'image du résultat
//	char *titre : le titre de la fenêtre
//
void Contours::afficheFenetre(IplImage *img, const char *titre)
{
    cvNamedWindow(titre, CV_WINDOW_AUTOSIZE);
    cvShowImage(titre, img);
}

//
// Enrefistrer une image de résultat dans un fichier avec le nom donné
// @ param :
//	IplImage *img : une image
//	const char *fichier : le nom de fichier enregistré
//
void Contours::enregistrerResultat(IplImage *img, const char *fichier)
{
    cvSaveImage(fichier, img);
}
