#include "include/personnage.h"
#include "include/game.h"
#include <BBOP/Graphics.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
  GLFWwindow * window;
  bbopInit(1280, 720, "Bro Melee", window);

  std::vector<Personnage> test; 
  Personnage perso(GLFW_JOYSTICK_1, "img/personnages/default/", 100, 5, 75);
  test.push_back(perso);
  std::vector<Personnage> test2; 
  Map map;

  Game game(map, test, test2);

  while(!glfwWindowShouldClose(window)){
    bbopCleanWindow(window, Vector3i(0,0,0), 1.0);

    game.update();
    game.Draw();

    //bbopErrorCheck();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
