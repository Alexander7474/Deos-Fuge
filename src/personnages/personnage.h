#pragma once
#include <BBOP/Graphics.h>
#include <BBOP/Graphics/collisionBoxClass.h>
#include <vector>
#include <iostream>
#include "../map.h"
#include "perso_info.h"

//enumeration des états possible du personnage
enum perso_state:int{
  stationary=0,
  run=1,
  dash=2,
  jump=3,
  fall=4,
  light_attack=5,
  attack=6,
  hit=7
};

//enumeration des direction possible du personnage
enum perso_direction:int{
  left=-1,
  right=1
};

/**
 * @class Personnage
 *
 * @brief gère un personnage, a ne pas utiliser directement mais avec un Bot ou un Personnage
 * @todo utiliser une list de int pour stocker le compteur de frame des états
 */
class Personnage : public Sprite
{
protected:
  
  // caractèristique du personnage
  float percentage; //<! pourcentage de dégats 
  float speed; //<! vitesse
  float weight; //<! poid
  float jump_force; //<! force de saut

  //stockage des animations
  std::vector<Texture> animation[9]; //<! stockage des anims
  int anim_frame_n[8]; //<! nombre de frame en fonction de chaque anim
  double anim_t[8]; //<! durée total de l'anim
  double anim_frame_t[8]; //<! temps entre chaque frame en fonction de chaque anim
  // stockage dynamique des infos nécessaire a l'animation
  double anim_start_t[8]; //<! timing de départ e l'anim
  double anim_last_frame_t[8]; //<! timing de la dernière frame de l'anim
  int anim_frame_cpt[8]; //<! compteur de frame de chaque anim
  int frame_cpt; //<! compteur de frame par default

  // gestion des états du personnage
  perso_state state; //<! etat général
  perso_state calling_state; //<! état demandé par les inputs donné à personnage
  perso_direction direction; //<! direction 
  Vector2f mouvement; //<! mouvement du personnages
  double fall_start_t; //<! timing du debut de la chute
  bool call_hit; // gére les appelle sur l'etat hit car différent du reste

  //information sur les boite de collision
  CollisionBox attack_box; //<! box de collision pour les attaques du personnage
public:

  friend class Player;
 
  /**
  * @brief Constructeur de personnage
  */  
  Personnage();

  /**
   * @brief Destructeur de personnages
   */

  /**
  * @brief met à jour le personnage à partir de ces attribut et de ce de la map
  */
  void updatePersonnage(double delta_time_, Map *map_);

  /**
  * @brief Va a gauche
  *
  * @param[in] value -1.0 vitesse max - 0.0 immobile
  */
  void goLeft(double delta_time_, float value);

  /**
  * @brief Va a droite
  *
  * @param[in] value 1.0 vitesse max - 0.0 immobile
  */
  void goRight(double delta_time_, float value);

   /**
   * @brief le personnage est touché
   */ 
  void doHit(int dir, float percentage_);

  /**
   * @brief renvoie l'état du personnage
   *
   * @return state état du personnage
   */
  int getState();

  /**
   * @brief renvoie la direction 
   *
   * @return direction 
   */
  int getDirection();

  /**
   * @brief permet de recalculer la boite de collision
   */
  virtual void rebuildCollisionBox();

  /**
   * @brief renvoie la box d'attaque du personnage 
   *
   * @return attackBox box d'attaque
   */
  const CollisionBox &getAttackBox() const;

  /**
   * @brief construit les tableau contenant toutes les informations sur les animations 
   */
  void buildAnimCache(perso_info info_);

  /**
   * @détermine si le personnage est dans un etat d'attaque
   */
  bool isAttacking();

  /**
   * @brief retourne le nombre de pourcentage à appliquer a l'adversaire lors d'une attaque
   */
  virtual float getPercentageToApply();

  /**
   * @brief Methode purement virtuelle qui définissent comment le personnage agit selon son etat
   */
  //////////////////////////////////////
  virtual void Dash(double delta_time_) = 0;
  virtual void Jump(double delta_time_) = 0;
  virtual void Attack(double delta_time_) = 0;
  virtual void Light_attack(double delta_time_) = 0;
  virtual void Hit(double delta_time_) = 0;
  //////////////////////////////////////

};
