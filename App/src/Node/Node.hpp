#pragma once

#include "GL.hpp"
#include "glm/fwd.hpp"
#include "imgui.h"
#include <boost/static_string.hpp>
#include <string>
#include <string_view>
class Node
{
  public:
    template <typename T> class Pin
    {
    };
    template <typename T> class Input : public Pin<T>
    {
    };
    template <typename T> class Output : public Pin<T>
    {
    };

  protected:
    template <typename T> Input<T> &AddInput(const std::string_view name);
    template <typename T> Output<T> &AddOutput(const std::string_view name);

    virtual void DrawPreview() = 0;
    virtual std::string GetTitle() = 0;

  public:
    Node() = default;
    virtual ~Node() = default;
    virtual glm::vec2 GetSize() const { return {5, 5}; }
    void DrawNode(ImDrawList* drawlist,glm::mat3 World2Screen);
    constexpr glm::vec2 GetPosition() const {return Position;}
    constexpr void SetPosition(glm::vec2 p) {Position = p;}
    virtual bool IsHovered() const;

  private:
    glm::vec2 Position = {0,0};
    const float HeaderHeight = 0.4;
};