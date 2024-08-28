#pragma once 

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <BBOP/Graphics/fontsClass.h>
#include <BBOP/Graphics/shapeClass.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

//action possible dans un menu
enum menu_input {
  menu_left=0,
  menu_right=1,
  menu_up=2,
  menu_down=3,
  menu_select=4,
  menu_back=5,
  menu_null=6
};

class Menu : public BbopDrawable
{
public:
  Menu(GLFWwindow *_window, std::string _menu_path);
  Menu(GLFWwindow *_window);

  /**
   * @brief met a jour le menu est renvoie le numero de cellule selectionné pour faire d'autre action
   *
   * @return Index de la cellule selectionné
   */
  int update();

  void Draw(GLint renderModeLoc) const override;

private:
  menu_input handle_input();

  //variable utilisé dans l'interacation avec le menu 
  GLFWwindow *window; //<! pointeur vers la fenêtre de jeu
  int cell_index; //<! index de la cellule selectionné
  bool released; //<! si la touche à été relaché avant de prendre un autre input
  double released_t; //<! timing depuis le dernière appuis

  std::vector<Sprite> layers; //<! layer du menu
  std::vector<Sprite> cells; //cellule du menu 
};
