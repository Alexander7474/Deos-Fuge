#include "knight.h"

Knight::Knight()
  : Personnage()
{
  //attribut de base 
  //
  speed = 250.f;
  weight = 650.f;
  jump_force = 100.f;

  //attribue du sprite
  //
  setAutoUpdateCollision(true);
  setPosition(300.f,100.f);
  setSize(200.f,200.f); 
  setOrigin(100.f,200.0f);
  getCollisionBox().setOffsetY(Vector2f(100.f,0.f));
  rebuildCollisionBox();
  attack_box.follow(getCollisionBox());
}
