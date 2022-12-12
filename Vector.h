#ifndef _VECTOR_H
#define _VECTOR_H

class Point;
struct Vector {
   double x;
   double y;

   Vector();
   Vector(double a, double b);
   ~Vector();

   Vector operator * (double scalar);
   Vector operator + (Vector v);   
   void rollRandom();
   void rollReallyRandom();    
   
   void rollRandomY();
   void rollRandomX();   
   void reflectY();
   void reflectX();
   void flip();
   void Angle(Point p, Point p2, double time);
   
};

#endif
