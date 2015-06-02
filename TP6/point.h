//
//
//  point.h
//
//  La classe stocke les points de la fonction de changement de contraste
//
//  LE Viet Man
//  30/03/2010
//
//

#ifndef POINT_H
#define POINT_H

class Point
{
public:
	
    //
    // Le constructeur de la classe Point sans les paramètres
    //	
    Point();
	
    //
    // Le constructeur de la classe Point
    // @param :
    //      int valeurX : la coordonnée x
    //      int valeurY : la coordonnée y
    //	
    Point(int valeurX, int valeurY);

    //
    // Recevoir la coordonnée x
    //	
    int getX() {return x;}
	
    //
    // Mettre la coordonnée x
    // @param :
    //		int valeurX : la coordonnée x
    //
    void setX(int valeurX);

    //
    // Recevoir la coordonnée y
    //
    int getY() {return y;}
	
    //
    // Mettre la coordonnée y
    // @param :
    //		int valeurY : la coordonnée y
    //
    void setY(int valeurY);

    //
    // Mettre deux coordonnée x et y en même temps
    // @param :
    //		Point *point : un point avec les coordonnées
    //
    void setPoint(Point *point);

private:
    int x; // la coordonnée x
    int y; // la coordonnée y
};

#endif // POINT_H
