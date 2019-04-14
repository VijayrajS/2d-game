#include "main.h"

#ifndef SEMIC_H
#define SEMIC_H

class Semic
{
  public:
    Semic() {}
    Semic(float x, float y, color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    float rotation;
    float length;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();

  private:
    VAO *object;
    VAO *object2;
};

#endif // SEMIC_H
