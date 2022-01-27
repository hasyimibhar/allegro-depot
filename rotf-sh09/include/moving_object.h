#ifndef MOVINGOBJECT_H_INCLUDED
#define MOVINGOBJECT_H_INCLUDED

/*
 * HEADER FILES
 */

#include "Vector2D.h"
#include "object.h"

/*
 * CONSTANTS
 */

/*
 * VARIABLES
 */

/*
 * CLASS INTERFACE
 */

class MovingObject : public Object
{
public:
   MovingObject(Vector2D pos, int w, int h, Vector2D vel): Object(pos, w, h), vel_(vel) {}

   virtual void Update()
   {
      pos_ = pos_ + vel_;
   }

   virtual void Render() = 0;
   virtual ~MovingObject() {}

   Vector2D GetVelocity()
   {
      return vel_;
   }

protected:
   Vector2D vel_;
};

/*
 * FUNCTION PROTOTYPES
 */

#endif // MOVINGOBJECT_H_INCLUDED
