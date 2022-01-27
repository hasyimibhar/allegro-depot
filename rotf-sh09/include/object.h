#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

/*
 * HEADER FILES
 */

#include "Vector2D.h"

/*
 * VARIABLES
 */

/*
 * CLASS INTERFACE
 */

class Object
{
public:
   Object(const Vector2D& pos, int w, int h): pos_(pos), w_(w), h_(h) {}

   virtual void Update() = 0;

   virtual void HandleEvents() = 0;

   virtual void Render() = 0;

   virtual ~Object() {}

   virtual Vector2D IsColliding(Object* object)
   {
      Vector2D separations[2];

      double object1Points[2] = { pos_.x, pos_.x + w_ };
      double object2Points[2] = { object->GetPosition().x, object->GetPosition().x + object->GetWidth() };

      if (object2Points[0] > object1Points[0] && object2Points[0] < object1Points[1])
         separations[0] = Vector2D(object2Points[0] - object1Points[1], 0.0);

      else if (object2Points[1] > object1Points[0] && object2Points[1] < object1Points[1])
         separations[0] = Vector2D(object2Points[1] - object1Points[0], 0.0);

      else
         return Vector2D(0.0, 0.0);

      object1Points[0] = pos_.y;
      object1Points[1] = pos_.y + h_;

      object2Points[0] = object->GetPosition().y;
      object2Points[1] = object->GetPosition().y + object->GetHeight();

      if (object2Points[0] > object1Points[0] && object2Points[0] < object1Points[1])
         separations[1] = Vector2D(0.0, object2Points[0] - object1Points[1]);

      else if (object2Points[1] > object1Points[0] && object2Points[1] < object1Points[1])
         separations[1] = Vector2D(0.0, object2Points[1] - object1Points[0]);

      else
         return Vector2D(0.0, 0.0);

      if (separations[0].GetMagnitudeSquared() > separations[1].GetMagnitudeSquared())
         return separations[1];
      else
         return separations[0];
   }

   int GetWidth()
   {
      return w_;
   }

   int GetHeight()
   {
      return h_;
   }

   Vector2D GetPosition()
   {
      return pos_;
   }

   virtual Vector2D GetVelocity()
   {
      return Vector2D();
   }

   virtual void OnCollision(Vector2D, Object*) = 0;

protected:
   Vector2D  pos_;
   int       w_, h_;
};

/*
 * FUNCTION PROTOTYPES
 */

#endif // OBJECT_H_INCLUDED
