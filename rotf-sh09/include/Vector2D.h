#ifndef VECTOR2D_H_INCLUDED
#define VECTOR2D_H_INCLUDED

/*
 * HEADER FILES
 */

#include <cmath>

/*
 * CONSTANTS
 */

/*
 * VARIABLES
 */

/*
 * CLASS INTERFACE
 */

class Vector2D
{
public:
   double x, y;

   Vector2D(): x(0.0), y(0.0) {}
   Vector2D(const double& c): x(c), y(c) {}
   Vector2D(const double& _x, const double& _y): x(_x), y(_y) {}

   Vector2D operator+(const Vector2D& v)
   {
      return Vector2D(x + v.x, y + v.y);
   }

   Vector2D operator+(const double& c)
   {
      return Vector2D(x + c, y + c);
   }

   Vector2D operator-(const Vector2D& v)
   {
      return Vector2D(x - v.x, y - v.y);
   }

   Vector2D operator-(const double& c)
   {
      return Vector2D(x - c, y - c);
   }

   Vector2D operator*(const double& c)
   {
      return Vector2D(x * c, y * c);
   }

   double operator*(const Vector2D& v)
   {
      return x * v.x + y * v.y;
   }

   Vector2D operator/(const double& c)
   {
      return Vector2D(x / c, y / c);
   }

   double Dot(const Vector2D& v)
   {
      return x * v.x + y * v.y;
   }

   double GetMagnitude()
   {
      return sqrtf(x * x + y * y);
   }

   double GetMagnitudeSquared()
   {
      return x * x + y * y;
   }
};

/*
 * FUNCTION PROTOTYPES
 */

#endif // VECTOR2D_H_INCLUDED
