#include "src/menu/menu.h"
#include "src/personnages/personnage.h"
#include "src/game.h"
#include "src/player.h"

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopFunc.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <LDtkLoader/Project.hpp>
#include <iostream>

enum main_state {
  in_game=0,
  start_menu=1,
  leaving=2
};

int main()
{
  GLFWwindow * window;
  bbopInit(1280, 720, "Bro Melee", window);

  main_state STATE = start_menu;

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
      case start_menu:
        default_scene.Use();
        if(m.update() == 0)
          STATE = in_game;
        default_scene.Draw(m);
        break;
      case in_game:
        game.update();
        game.Draw();
        break;
      default:
        std::cerr << "ERROR: default state rrrraaaaaaah impossible" << std::endl;
        break;
    }


    bbopErrorCheck();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
}
