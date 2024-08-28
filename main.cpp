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
  leaving=2
};

main_state STATE = in_menu;

void start_menu(Menu *menu, Scene *scene)
{
  scene->Use();
  switch (menu->update()) {
    case 0:
      STATE = in_game;
      break;
    case 1:
      std::cerr << "no local multi yet" << std::endl;
      break;
    case 2:
      std::cerr << "no online multi yet" << std::endl;
      break;
    case 3:
      std::cerr << "no parameters yet" << std::endl;
      break;
    case 4:
      STATE = leaving;
    default:
      break;
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
  Player perso2(window, 0, "Didier", 0);
  
  test.push_back(perso1);
  test.push_back(perso2);

  Game game(window, test);

  Menu m(window);

  glfwSwapInterval(0);

  while(!glfwWindowShouldClose(window)){
    bbopCleanWindow(window, Vector3i(0,0,0), 1.0);

    switch(STATE) {
      case in_menu:
        start_menu(&m, &default_scene);
        break;
      case in_game:
        game.update();
        game.Draw();
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

