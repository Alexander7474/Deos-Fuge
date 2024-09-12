#include "menu.h"
#include <BBOP/Graphics/bbopMathClass.h>
#include <BBOP/Graphics/spriteClass.h>
#include <GLFW/glfw3.h>
#include <LDtkLoader/DataTypes.hpp>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/Layer.hpp>
#include <iostream>
#include <iterator>
#include <string>

Menu::Menu(GLFWwindow *_window)
  : Menu(_window, "assets/menu/", 8)
{}

Menu::Menu(GLFWwindow *_window, std::string _menu_path, int _menu_cell_size)
  : window(_window),
    cell_index(Vector2i(0,0)),
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
  for(int i = 0; i < 720/8; i++){
    for(int y = 0; y < 1280/8; y++){
      cells[i][y] = nullptr;
    }
  }
  for(const auto& layer : level.allLayers() ){
    if(layer.getType() == ldtk::LayerType::Entities){
      for(const auto& cell : layer.allEntities()){
        std::string texture_path = folder_path + cell.getTexturePath();
        int tabX = cell.getPosition().x / _menu_cell_size;
        int tabY = cell.getPosition().y / _menu_cell_size;
        const auto& n =  cell.getField<ldtk::FieldType::String>("name");
        cells[tabY][tabX] = new Cellule({Sprite(texture_path.c_str()), n.value()});
        cells[tabY][tabX]->sprite.setPosition(cell.getPosition().x,cell.getPosition().y);
        cells[tabY][tabX]->sprite.setSize(cell.getSize().x, cell.getSize().y);
        cells[tabY][tabX]->sprite.setColor(300,300,0);
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

  //on cherche la première case du tableau pour la colorer
  bool found = false;
  for(int i = 0; i < 720/8; i++){
    for(int y = 0; y < 1280/8; y++){
      if(cells[i][y] != nullptr){
        cells[i][y]->sprite.setRGBFilterState(true);
        cell_index = Vector2i(y,i);
        found = true;
      }
      if(found)
        break;
    }
    if(found)
      break;
  }
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

Cellule *Menu::update()
{
  menu_input user_input = handle_input();

  if(glfwGetTime() - released_t > 0.2)
    released = true;

  if(released && user_input != menu_null){
    Vector2i last_cell_index(cell_index);
        
    bool found = false;

    switch (user_input) {
      case menu_left: 
        while(!found){
          cell_index.x--;
          if(cell_index.x < 0)
            cell_index.x = 1280/8-1;
          if(cells[cell_index.y][cell_index.x] != nullptr)
            found = true;
        }
        released = false;
        released_t = glfwGetTime();
        break;
      case menu_right:
        while(!found){
          cell_index.x++;
          if(cell_index.x >= 1280/8)
            cell_index.x = 0;
          if(cells[cell_index.y][cell_index.x] != nullptr)
            found = true;
        }
        released = false;
        released_t = glfwGetTime();
        break;
      case menu_up:
        while(!found){
          cell_index.y--;
          if(cell_index.y < 0)
            cell_index.y = 720/8-1;
          if(cells[cell_index.y][cell_index.x] != nullptr)
            found = true;
        }
        released = false;
        released_t = glfwGetTime();
        break;
      case menu_down: 
        while(!found){
          cell_index.y++;
          if(cell_index.y >= 720/8)
            cell_index.y = 0;
          if(cells[cell_index.y][cell_index.x] != nullptr)
            found = true;
        }
        released = false; 
        released_t = glfwGetTime();
        break;
      case menu_select:
        released = false; 
        released_t = glfwGetTime();
        return cells[cell_index.y][cell_index.x];
        break;
      default:
        break;
    }
    
    cells[last_cell_index.y][last_cell_index.x]->sprite.setRGBFilterState(false);
    cells[cell_index.y][cell_index.x]->sprite.setRGBFilterState(true);
  }else if(user_input == menu_null){
    released = true;
  }

  if(false){
    std::cerr << cell_index.x << "   " << cell_index.y << "  "<< glfwGetTime() <<std::endl;
  }

  return nullptr;
}

void Menu::Draw(GLint renderModeLoc) const
{
  for(int y = layers.size()-1; y >= 0; y--){
    layers[y].Draw(renderModeLoc);
  }
  for(int i = 0; i < 720/8; i++){
    for(int y = 0; y < 1280/8; y++){
      if(cells[i][y] != nullptr){
        cells[i][y]->sprite.Draw(renderModeLoc);
      }
    }
  }
}

Menu::~Menu()
{
  for(int i = 0; i < 720/8; i++){
    for(int y = 0; y < 1280/8; y++){
      if(cells[i][y] != nullptr)
        delete cells[i][y];
    }
  }
}
