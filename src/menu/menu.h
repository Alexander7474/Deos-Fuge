#pragma once 

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <BBOP/Graphics/fontsClass.h>
#include <BBOP/Graphics/shapeClass.h>
#include <string>
#include <vector>

class Menu : public BbopDrawable
{
public:
  Menu(std::string _menu_path);
  Menu();

  /**
   * @brief met a jour le menu est renvoie le numero de cellule selectionné pour faire d'autre action
   *
   * @return Index de la cellule selectionné
   */
  int update();

  void Draw(GLint renderModeLoc) const override;

private:
  std::vector<Sprite> layers; //<! layer du menu
  std::vector<Sprite> cells; //cellule du menu 
};
