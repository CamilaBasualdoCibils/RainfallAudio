
#pragma once
#include "AppWindow.hpp"
#include "GL.hpp"
#include "GLFW/glfw3.h"
#include "misc/Singleton.hpp"
class RainfallEditor :public AppWindow, public Singleton<RainfallEditor> {
double DeltaTime;
public:
  RainfallEditor();

  void Run();
  double GetDeltaTime() const {return DeltaTime;}
  double Getime() const {return glfwGetTime();}
};