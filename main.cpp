#include <BBOP/Graphics.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
  GLFWwindow * window;
  bbopInit(1500, 1500, "water", window);

  Scene defaultScene;
  RectangleShape defaultRectangle(Vector2f(100.0f,100.0f),Vector2f(750.0f,750.0f),Vector3i(0,0,255),Vector2f(25.0f,25.0f));

  while(!glfwWindowShouldClose(window)){
    bbopCleanWindow(window, Vector3i(0,0,0), 1.0);

    defaultScene.Use();
    defaultScene.Draw(defaultRectangle);

    bbopErrorCheck();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
