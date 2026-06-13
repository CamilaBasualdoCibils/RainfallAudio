
#pragma once
#include "Singleton.hpp"
class RainfallEditor : public Singleton<RainfallEditor> {
double DeltaTime;
public:
  RainfallEditor();

  int Run(int& argc,char**argv);
};