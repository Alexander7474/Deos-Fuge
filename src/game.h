#pragma once 

////////////////////
/// External headers
////////////////////
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/fontsClass.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <BBOP/Graphics.h>
#include <vector>

/////////////////
/// Local headers
/////////////////
#include "player.h"

enum game_state_enum{ 
  start=0, 
  middle=1, 
  end=2
};

struct player_hud {
  TexteBox name;
  TexteBox percent;
  Sprite player_pp;
};

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
  Game(GLFWwindow*& window_, std::vector<Player> &players_);

  ~Game();

  /**
  * @brief Met a jour tous les attribut de la Game
  *
  * @note À utiliser à chaque frame
  */
  int update();

  /**
  * @brief Dessine tous les attribut de la game 
  *
  * @note À utiliser à chaque frame avec 
  */
  void Draw();
private:
  //attribut fonctionelle
  Scene scene; //<! Scene avec paramètre de rendue de la game
  Camera players_camera; //<! camera pour afficher les joueur
  float cam_scale_goal; //<! scale objectif de la cam
  float cam_scale_last; //<! dernière scale de la camera
  game_state_enum game_state; //<! statut de la game
  Map map; //<! Map où se déroule la game
  std::vector<Player> players; //<! liste des personnages
  double last_time; //<! timing de la dernière frame
  
  //Stockage de la fenêtre 
  GLFWwindow *window; //<! pointeur vers la fenêtre glfw
  
  //Attribut pour le hud 
  Font font; //<! font pour le texte
  TexteBox fps_hud; //<! texte box affichage des fps
  double last_time_fps; //<! last time pour les fps
  int fps_counter; //<! conpteur de frame
  player_hud** players_hud;
  float hud_padding;
  
  /**
  * @brief Met à jour le hud
  */
  void updateHUD();
};
