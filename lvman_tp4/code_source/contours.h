//
//
// contours.h
//
// La classe permet de détecter les contours dans une image en utilisant quatre algorithmes :
// Sobel, Prewitt, Laplace et Canny. 
// Les fonctionnalités :
// - Prendre une image en argument d'entrée
// - Extraire les contours en utilisant quatre algorithmes
// - Calculer trois mesures P, TFP et TFN
// - Enregistrer les images de résultats
//
// LE Viet Man
// 02/05/10
//
//

#ifndef CONTOURS_H
#define CONTOURS_H

#include <cv.h>

int const PREWITT_HOR = 0;
int const PREWITT_VER = 1;

// deux masques de Prewitt
int const prewitt1[3][3] = {{-1,-1,-1},{0,0,0},{1,1,1}};
int const prewitt2[3][3] = {{-1,0,1},{-1,0,1},{-1,0,1}};

class Contours
{
public:
	
	//
    // Le constructeur de la classe Contours
    // @param :
	//		char* fichier : le nom du fichier sorti
    //      IplImage *img : un objet de IplImage, une structure stockée l'image
	//		int seuil : le seuil de contours
    //	
    Contours(char* fichier, IplImage* img, int seuil);

	//
    // Le constructeur de la classe Contours
    // @param :
	//		char* fichier : le nom du fichier sorti
    //      IplImage *img : un objet de IplImage, une structure stockée l'image
	//		IplImage* ref : l'image de la référence tracée à la main
	//		int seuil : le seuil de contours
    //	
    Contours(char* fichier, IplImage* img, IplImage* ref, int seuil);

	// 
	// Extraire les contours en utilisant quatre algorithmes Sobel, Prewitt, Laplace et Canny
	// Enregister les images de résultats
	// Afficher les images sur les fenêtres
	// Calculer trois mesures P, TFP et TFN pour chaque algorithmes
	//
    void extraireContours();

private:
    IplImage* image; // l'image originale
    IplImage* reference; // l'image de la référence tracée à la main

    IplImage* sobel; // l'image de contours en utilisant l'algorithme Sobel
    IplImage* prewitt; // l'image de contours en utilisant l'algorithme Prewitt
    IplImage* laplace; // l'image de contours en utilisant l'algorithme Laplace
    IplImage* canny; // l'image de contours en utilisant l'algorithme Canny

    int seuil; // le seuil

	// les noms du fichier sorties
    char* fichier_sobel;
    char* fichier_prewitt;
    char* fichier_laplace;
    char* fichier_canny;

	//
    // Créer les nom du fichier sortie
    // @param :
    //      char *fichier : le nom du fichier sortie
    //      char *ext : la partie de l'extension
    //
    char* creerNomFichierSortie(const char *fichier, const char *ext);
	
	//
	// Extraire les contours en utilisant l'algorithme Prewitt
	// @param :
	//		IplImage* src : l'image de source
	//		IplImage* dst : l'image de destination
	//		int type :	le type de contours (PREWITT_HOR ou 0 : contours horizontaux)
	//										(PREWITT_VER ou 1 : contours verticaux)
	//
	void filterPrewitt(IplImage* src, IplImage* dst, int type);
	
	//
    // Afficher la fenêtre du résultat
    // @param :
    //      IplImage *img : l'image du résultat
    //      char *titre : le titre de la fenêtre
    //	
    void afficheFenetre(IplImage *img, const char *titre);
	
	//
    // Enrefistrer une image de résultat dans un fichier avec le nom donné
    // @ param :
    //      IplImage *img : une image
    //      const char *fichier : le nom de fichier enregistré
    //
    void enregistrerResultat(IplImage *img, const char *fichier);

	//
	// Extraire les contours en utilisant l'algorithme Sobel
	//
    void filterSobel();
	
	//
	// Extraire les contours en utilisant l'algorithme Prewitt
	//
    void filterPrewitt();
	
	//
	// Extraire les contours en utilisant l'algorithme Laplace
	//
    void filterLaplace();
	
	//
	// Extraire les contours en utilisant l'algorithme Canny
	//
    void filterCanny();

	//
	// Compter les contours corrects dans une image
	// @param :
	//		IplImage* ref : l'image de la référence tracée à la main
	//		IplImage* img : l'image de contours
	//
    unsigned int compterContoursCorrects(IplImage* ref, IplImage* img);
	
	//
	// Compter les contours dans une image
	// @param :
	//		IplImage* img : l'image de contours
	//
    unsigned int compterContours(IplImage* img);
};

#endif // CONTOURS_H
