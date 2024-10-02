#include "src/menu/menu.h"
#include "src/personnages/personnage.h"
#include "src/game.h"
#include "src/player.h"

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopFunc.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <LDtkLoader/Project.hpp>
#include <cstdlib>
#include <iostream>

enum main_state {
  in_game=0,
  in_menu=1,
  in_player_selection=2,
  leaving=3
};

main_state STATE = in_menu;

void start_menu(Menu *menu, Scene *scene)
{
  scene->Use();
  Cellule *menu_cell = menu->update();
  if(menu_cell != nullptr){
    if(menu_cell->name == "solo"){
      STATE = in_player_selection;
    }else if(menu_cell->name == "leave"){
      STATE = leaving;
    }else if(menu_cell->name == "local_multiplayer"){
      STATE = in_player_selection;
    }
  }
  scene->Draw(*menu);
}

void player_select_menu(Menu *menu, Scene *scene)
{
   scene->Use();
  Cellule *menu_cell = menu->update();
  if(menu_cell != nullptr){
    if(menu_cell->name == "local_multiplayer"){
      STATE = in_game;
    }
  }
  scene->Draw(*menu);
}

int main()
{
  GLFWwindow * window;
  bbopInit(1280, 720, "Bro Melee", window);

  //scene par default pour tous les élément sans scene comme le menu d'intro
  Scene default_scene;

  std::vector<Player> test; 
  Player perso1(window, -1, "Didier", 0);
  
  test.push_back(perso1);

  Game* game;

  Menu m(window);
  Menu player_selection(window, "assets/menu/player_selection/", 8);

  glfwSwapInterval(0);

  while(!glfwWindowShouldClose(window)){
    bbopCleanWindow(window, Vector3i(0,0,0), 1.0);

    switch(STATE) {
      case in_menu:
        start_menu(&m, &default_scene);
        break;
      case in_player_selection:
        player_select_menu(&player_selection, &default_scene);
        if(STATE == in_game)
          game = new Game(window, test);
        break;
      case in_game:
        if(game->update() == -1){
          STATE = in_menu;
          delete game;
          break;
        }
        game->Draw();
        break;
      case leaving:
        return 0;
      default:
        std::cerr << "ERROR: default state rrrraaaaaaah impossible" << std::endl;
        return 1;
        break;
    }

    bbopErrorCheck();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

