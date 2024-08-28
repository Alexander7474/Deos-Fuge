#include "menu.h"
#include <BBOP/Graphics/spriteClass.h>
#include <LDtkLoader/DataTypes.hpp>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/Layer.hpp>
#include <string>

Menu::Menu()
  : Menu("assets/menu/")
{}

Menu::Menu(std::string _menu_path)
{
  // on charge la map
  ldtk::Project ldtk_project;

  std::string ldtk_file_path = _menu_path + "menu.ldtk";
  std::string folder_path = _menu_path;

  ldtk_project.loadFromFile(ldtk_file_path);

  const auto& world = ldtk_project.getWorld();
  const auto& level = world.getLevel("Level");

  for(const auto& layer : level.allLayers() ){
    if(layer.getType() == ldtk::LayerType::Entities){
      for(const auto& cell : layer.allEntities()){
        std::string texture_path = folder_path + cell.getTexturePath();
        Sprite b(texture_path.c_str());
        b.setPosition(cell.getPosition().x,cell.getPosition().y);
        b.setSize(cell.getSize().x, cell.getSize().y);
        cells.push_back(b);
      }
    }else{
      std::string texture_path = folder_path + "menu/png/Level_" + layer.getName() + ".png";
      Sprite l(texture_path.c_str());
      l.setSize(1280,720);
      l.setPosition(0,0);
      layers.push_back(l);
    }
  }

  std::string background_texture_path = folder_path + "menu/png/Level_bg.png";
  Sprite background(background_texture_path.c_str());
  background.setSize(1280,720);
  background.setPosition(0,0);
  layers.push_back(background);

}

int Menu::update()
{
  return 0;
}

void Menu::Draw(GLint renderModeLoc) const
{
  for(int y = layers.size()-1; y >= 0; y--){
    layers[y].Draw(renderModeLoc);
  }
  for(Sprite cell : cells){
    cell.Draw(renderModeLoc);
  }
}
