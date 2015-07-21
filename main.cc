#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include "ui/canvas.h"
#include "ui/layout.h"
#include "ui/text.h"
#include "ui/math.h"

#include <memory>

#include <dlfcn.h>

int main(int argc, char **argv)
{
  if (!glfwInit())
    return 1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  auto window = glfwCreateWindow(1280, 720, "Lab", NULL, NULL);
  glfwMakeContextCurrent(window);
  gl3wInit();

  auto vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  auto lib = dlopen("build/spring.dylib", 0);
  auto create = (Layout* (*)()) dlsym(lib, "create");

  auto c = new Canvas(vg);
  auto ex = create();
  auto fm = new FontManager;
  auto font = fm
    ->createFace("/Library/Fonts/TeX Gyre/texgyrepagella-regular.otf")
    .createFont(16);

  c->setParagraphFont(std::move(font));

  auto mathface = std::make_unique<Face>(fm->createFace("/Library/Fonts/TeX Gyre/texgyrepagella-math.otf"));
  c->mathFace = std::move(mathface);

  int columnWidth = 500;
  Position start = {(width-columnWidth)/2, 0};

  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  float devPixelRatio = width / (float)windowWidth;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, width, height, devPixelRatio);

    ex->draw(*c, start, columnWidth);

    nvgEndFrame(vg);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}

