#pragma once 

////////////////////
/// External headers
////////////////////
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <BBOP/Graphics.h>
#include <vector>

/////////////////
/// Local headers
/////////////////
#include "map.h"
#include "personnage.h"

enum game_state_enum { Start=0, Middle=1, End=2};

/**
 * @class Game
 * @brief class qui gère la partie
 * 
 * @note La class n'hérite pas de BbopDrawable car elle gère sa propre Scene
 */
class Game
{
public:
  /**
  * @brief Constructeur de Game
  *
  * @param[in] map_ Map à utiliser pour la game 
  * @param[in] nPersonnage_ nombre de personnage 
  * @param[in] nBot_ nombre de bot
  *
  * @see Map Personnage
  */
  Game(Map map_, std::vector<Personnage> personnages_, std::vector<Personnage> bots_);

  /**
  * @brief Met a jour tous les attribut de la Game
  *
  * @note À utiliser à chaque frame
  */
  void update();

  /**
  * @brief Dessine tous les attribut de la game 
  *
  * @param[in] window fenêtre glfw initialisé avec bbopInit
  *
  * @note À utiliser à chaque frame avec 
  */
  void Draw(GLFWwindow *&window) const;
private:
  game_state_enum game_state; //<! statut de la game
  Map map; //<! Map où se déroule la game
  int n_bot; //<! nombre de Bot 
  std::vector<Personnage> personnages;
  std::vector<Personnage> bots;
};
