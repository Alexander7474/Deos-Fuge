
#include "src/personnages/personnage.h"
#include "src/game.h"
#include "src/player.h"

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopFunc.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <LDtkLoader/Project.hpp>
#include <iostream>

int main()
{


  GLFWwindow * window;
  bbopInit(1280, 720, "Bro Melee", window);

  std::vector<Player> test; 
  Player perso1(window, GLFW_JOYSTICK_1, "Didier", 0);
  Player perso2(window, GLFW_JOYSTICK_2, "Roger", 1);
  
  test.push_back(perso1);
  test.push_back(perso2);

  Game game(window, test);

  glfwSwapInterval(0);

  while(!glfwWindowShouldClose(window)){
    bbopCleanWindow(window, Vector3i(0,0,0), 1.0);

    game.update();
    game.Draw();

    bbopErrorCheck();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
}
