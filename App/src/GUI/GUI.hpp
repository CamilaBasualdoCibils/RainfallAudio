#pragma once

#include "misc/Singleton.hpp"
#include "Node/NodeEditor.hpp"

class GUI : public Singleton<GUI>
{
  NodeEditor node_editor;
  public:
    void Draw();

  private:
    void DrawContent();
};