#pragma once

//#include "Node/NodeFlow.hpp"
#include "Singleton.hpp"


class GUI : public Singleton<GUI>
{

  //NodeFlowView node_flow;
  public:
    void Draw();

  private:
    void DrawContent();
};