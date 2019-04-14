#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->shieldon = false;
    this->isbarry = false;

    speed = glm::vec3(0,0,0);
    static const GLfloat vertex_buffer_data[] = {
        -0.2, -0.2, 0, // vertex 1
        0.2, -0.2, 0,  // vertex 2
        0.2, 0.2, 0,   // vertex 3

        0.2, 0.2, 0,  // vertex 3
        -0.2, 0.2, 0, // vertex 4
        -0.2, -0.2, 0 // vertex 1
    };

    static const GLfloat vertex_buffer_data2[] = {
        -0.1, -0.2, 0, // vertex 1
        -0.2, -0.2, 0,   // vertex 3
        -0.15, -0.5, 0,  // vertex 2
    };

    static const GLfloat vertex_buffer_data3[] = {
        -0.1, -0.2, 0, // vertex 1
        0.2, -0.2, 0,  // vertex 2
        0.2, 0.2, 0,   // vertex 3

        0.2, 0.2, 0,  // vertex 3
        -0.1, 0.2, 0, // vertex 4
        -0.1, -0.2, 0 // vertex 1
    };

    static const GLfloat vertex_buffer_data4[] = {
        -0.1, -0.2, 0, // vertex 1
        -0.1, 0.2, 0,  // vertex 2
        -0.2, 0.2, 0,   // vertex 3

        -0.1, -0.2, 0, // vertex 1
        -0.2, 0.2, 0,   // vertex 3
        -0.2, -0.2, 0, // vertex 4
    };

    static const GLfloat vertex_buffer_data5[] = {
        0.075, 0.2, 0,   // vertex 1
        0.075, 0.3, 0,  // vertex 2
        -0.075, 0.3, 0, // vertex 3

        -0.075, 0.3, 0, // vertex 3
        -0.075, 0.2, 0,  // vertex 3
        0.075, 0.2, 0,   // vertex 1
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
    this->flame = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data2, {255, 165, 0}, GL_FILL);
    this->shield = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data3, {0, 255, 0}, GL_FILL);
    this->coat = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data3, {59, 73, 144}, GL_FILL);
    this->jetpack = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data4, {108, 108, 71}, GL_FILL);
    this->head = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data5, {1233, 169, 103}, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);

    if(isbarry)
    {
        draw3DObject(this->head);
        draw3DObject(this->coat);
        draw3DObject(this->jetpack);
    }
}

void Ball::drawflame(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->flame);
}

void Ball::drawshield(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->shield);
}

void Ball::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick(int sign) {
    this->position.x -= speed.x;

    if (this->position.y >= -3 && this->position.y < 3)
        this->position.y -= speed.y;

    if (this->position.y >= 3)
        this->position.y = 2.9;

    if (this->position.x >= 3.9 && this->worth == 0)
        this->position.x = 3.9;

    if (this->position.x <= -3.9 && this->worth == 0)
        this->position.x = -3.9;

    if (this->position.y <= -2.7 && speed.y > 0)
    {
        speed.y = 0;
        this->position.y = -3;
    }
}

bounding_box_t Ball::bounding_box()
{
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = {x, y, 0.3, 0.3};
    return bbox;
}
