#pragma once 

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <BBOP/Graphics/bbopMathClass.h>
#include <BBOP/Graphics/fontsClass.h>
#include <BBOP/Graphics/shapeClass.h>
#include <BBOP/Graphics/spriteClass.h>
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

struct Cellule
{
  Sprite sprite;
  std::string name;
};

class Menu : public BbopDrawable
{
public:
  Menu(GLFWwindow *_window, std::string _menu_path, int _menu_cell_size);
  Menu(GLFWwindow *_window);

  ~Menu();

  /**
   * @brief met a jour le menu est renvoie un pointeur vers la cellule
   *
   * @return cellule
   */
  Cellule *update();

  void Draw(GLint renderModeLoc) const override;

private:
  menu_input handle_input();

  //variable utilisé dans l'interacation avec le menu 
  GLFWwindow *window; //<! pointeur vers la fenêtre de jeu
  Vector2i cell_index; //<! index de la cellule selectionné
  bool released; //<! si la touche à été relaché avant de prendre un autre input
  double released_t; //<! timing depuis le dernière appuis

  std::vector<Sprite> layers; //<! layer du menu
  Cellule *cells[720/8][1280/8]; //<! cellules du menu
};
