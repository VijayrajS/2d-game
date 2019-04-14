#include "main.h"

#ifndef ENEMIES_H
#define ENEMIES_H

class Zapper
{
  public:
    Zapper() {}
    Zapper(float x, float y, float length, color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    float rotation;
    float length;
    bool flameon = true;
    void draw(glm::mat4 VP);
    void drawfire(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();

  private:
    VAO *line;
    VAO *pads;
};

class Laser
{
  public:
    Laser() {}
    Laser(float x, float y, color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    float rotation;
    float length;
    int start_time;
    bool flameon = false;
    void draw(glm::mat4 VP);
    void drawfire(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();

  private:
    VAO *line;
    VAO *pads;
};

class Brang
{
  public:
    Brang() {}
    Brang(float x, float y, color_t color);
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
};

class Dragon
{
  public:
    Dragon() {}
    Dragon(float x, float y);
    glm::vec3 position;
    glm::vec3 speed;
    float rotation;
    float length;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();
    int lives;

  private:
    VAO *eye;
    VAO *body;
    VAO *horns;
    VAO *teeth;
};

#endif // ENEMIES_H

