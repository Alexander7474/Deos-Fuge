#include "menu.h"
#include <BBOP/Graphics/spriteClass.h>
#include <GLFW/glfw3.h>
#include <LDtkLoader/DataTypes.hpp>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/Layer.hpp>
#include <string>

Menu::Menu(GLFWwindow *_window)
  : Menu(_window, "assets/menu/")
{}

Menu::Menu(GLFWwindow *_window, std::string _menu_path)
  : window(_window),
    cell_index(0),
    released(true),
    released_t(glfwGetTime())
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
        b.setColor(300,300,0);
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

  cells[0].setRGBFilterState(true);

}

menu_input Menu::handle_input()
{
  if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
    return menu_left;
  }
  if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
    return menu_right;
  }
  if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
    return menu_up;
  }
  if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
    return menu_down;
  }
  if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
    return menu_select;
  }
  return menu_null;
}

int Menu::update()
{
  menu_input user_input = handle_input();

  if(glfwGetTime() - released_t > 0.2)
    released = true;

  if(released && user_input != menu_null){
    int last_cell_index = cell_index;

    switch (user_input) {
      case menu_left:
      case menu_right:
      case menu_up:
        if(cell_index > 0)
          cell_index--;
        else
           cell_index = static_cast<int>(cells.size()) - 1;
        released = false;
        released_t = glfwGetTime();
        break;
      case menu_down:
        if(cell_index < static_cast<int>(cells.size()) - 1)
          cell_index++;
        else
          cell_index = 0;
        released = false; 
        released_t = glfwGetTime();
        break;
      case menu_select:
        return cell_index;
        break;
      default:
        break;
    }
    
    cells[last_cell_index].setRGBFilterState(false);
    cells[cell_index].setRGBFilterState(true);
  }else if(user_input == menu_null){
    released = true;
  }

  return -1;
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
