#pragma once 
#include "map.h"
#include "personnage.h"
#include "perso_info.h"
#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <GLFW/glfw3.h>
#include <iostream>

/**
* @brief class qui gère les caractèristique du joueur et sa manette pour manipuler un Personnage
*/
class Player : public Personnage
{
public:

  /**
  * @brief Contructeur de Player
  *
  * @param[in] joystick_ GLFW_JOYSTICK_N joystick du player 
  * @param[in] name_ nom du player 
  * @param[in] personnage_info_ struct d'information du personnage du player 
  */ 
  Player(GLFWwindow*& window_, int joystick_, std::string name_, perso_info personnage_info_);

  /**
  * @brief Met a jour le personnage du player en fonction des touches de la manettes
  *
  * @param[in] delta_time temps ecoulé depuis la dernière frame 
  * @param[in] map_ map de jeu
  */ 
  void update(double delta_time_, Map *map_);

private:
  std::string name; //<! nom du Player
  int joystick; //<! manette du Player

  //attribut system
  GLFWwindow *window;//<! pour récupérer les touches du clavier
};