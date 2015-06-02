//
//
//  point.cpp
//
//  L'implémentation des fonctions de la classe point.h
//
//  LE Viet Man
//  30/03/2010
//
//

#include "point.h"

//
// Le constructeur de la classe Point sans les paramètres
//	
Point::Point()
{
    this->x = 0;
    this->y = 0;
}

//
// Le constructeur de la classe Point
// @param :
//      int valeurX : la coordonnée x
//		int valeurY : la coordonnée y
//
Point::Point(int valeurX, int valeurY)
{
    this->x = valeurX;
    this->y = valeurY;
}

//
// Mettre la coordonnée x
// @param :
//		int valeurX : la coordonnée x
//
void Point::setX(int valeurX)
{
    this->x = valeurX;
}

//
// Mettre la coordonnée y
// @param :
//		int valeurY : la coordonnée y
//
void Point::setY(int valeurY)
{
    this->y = valeurY;
}

//
// Mettre deux coordonnées x et y en même temps
// @param :
//		Point *point : un point avec les coordonnées
//
void Point::setPoint(Point *point)
{
    this->x = point->getX();
    this->y = point->getY();
}
