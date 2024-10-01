#pragma once 

#include "personnages/knight.h"
#include "personnages/white_knight.h"
#include "personnages/personnage.h"

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <GLFW/glfw3.h>
#include <iostream>

/**
* @brief class qui gère les caractèristique du joueur et sa manette pour manipuler un Personnage
*/
class Player
{
public:

  friend class Game;

  /**
  * @brief Contructeur de Player
  *
  * @param[in] joystick_ GLFW_JOYSTICK_N joystick du player 
  * @param[in] name_ nom du player 
  * @param[in] personnage_info_ struct d'information du personnage du player 
  */ 
  Player(GLFWwindow*& window_, int joystick_, std::string name_, int perso_select);

  ~Player();


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
  bool joystick_connected; //<! Uniquement pour gérer le message d'erreur
  Personnage *perso; //<! personnage du joueur

  //attribut system
  GLFWwindow *window;//<! pour récupérer les touches du clavier
};
