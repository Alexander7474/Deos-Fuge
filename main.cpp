#include "include/perso_info.h"
#include "include/personnage.h"
#include "include/game.h"
#include "include/menu.h"
#include <BBOP/Graphics.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
  GLFWwindow * window;
  bbopInit(1280, 720, "Bro Melee", window);

  std::vector<Player> test; 
  Player perso1(window, GLFW_JOYSTICK_1, "roger", DEFAULT_PERSO);
  Player perso2(window, -1, "roger", DEFAULT_PERSO);

  test.push_back(perso1);
  test.push_back(perso2);
  std::vector<Personnage> test2; 
  Map map;
  Menu menu;

  Game game(window, map, test, test2);

  while(!glfwWindowShouldClose(window)){
    bbopCleanWindow(window, Vector3i(0,0,0), 1.0);

    game.update();
    game.Draw();
    menu.Update();
    //bbopErrorCheck();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
