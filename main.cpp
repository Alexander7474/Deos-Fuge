#include <BBOP/Graphics.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
  GLFWwindow * window;
  bbopInit(1280, 720, "Bro Melee", window);

  Scene defaultScene;

  while(!glfwWindowShouldClose(window)){
    bbopCleanWindow(window, Vector3i(0,0,0), 1.0);

    defaultScene.Use();

    bbopErrorCheck();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
