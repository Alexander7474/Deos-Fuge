#include "src/perso_info.h"
#include "src/personnage.h"
#include "src/game.h"
#include "src/player.h"
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
  perso1.setRGBFilterState(true);
  perso2.setColor(255,255,0);
  perso1.setColor(255,0,255);

  test.push_back(perso1);
  test.push_back(perso2);
  std::vector<Personnage> test2; 

  Game game(window, test, test2);

  glfwSwapInterval(1);

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
