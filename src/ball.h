#include "main.h"

#ifndef BALL_H
#define BALL_H

#define GLM_ENABLE_EXPERIMENTAL

class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    bool shieldon;
    bool isbarry;

    float rotation;
    void draw(glm::mat4 VP);
    void drawflame(glm::mat4 VP);
    void drawshield(glm::mat4 VP);

    void set_position(float x, float y);
    void tick(int sign);
    bounding_box_t bounding_box();
    int worth = 0;

  private:
    VAO *object;
    VAO *flame;
    VAO *shield;
    VAO *jetpack;
    VAO *coat;
    VAO *head;
};

#endif // BALL_H
