#include "semic.h"
#include "main.h"

Semic::Semic(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = glm::vec3(0, 0, 0);

    GLfloat g_vertex_buffer_data[100000];  // Inner circle
    GLfloat g_vertex_buffer_data2[100000]; // Outer circle

    float r = 0.9f;

    float phi = M_PI/180;
    int angle = 0;

    for(int i = 0; i < 1620; i += 9)
    {
        g_vertex_buffer_data[i] = -r*cos(phi*angle);
        g_vertex_buffer_data[i + 1] = -r*sin(phi*angle);
        g_vertex_buffer_data[i + 2] = 0;

        g_vertex_buffer_data[i + 3] = -r*cos(phi*(angle+1));
        g_vertex_buffer_data[i + 4] = -r*sin(phi*(angle+1));
        g_vertex_buffer_data[i + 5] = 0;

        g_vertex_buffer_data[i + 6] = 0;
        g_vertex_buffer_data[i + 7] = 0;
        g_vertex_buffer_data[i + 8] = 0;

        if (angle > 178)
            break;

        angle++;
    }

    r = 1.0f;
    angle = 0;

    for(int i = 0; i < 1620; i += 9)
    {
        g_vertex_buffer_data2[i] = -r*cos(phi*angle);
        g_vertex_buffer_data2[i + 1] = -r*sin(phi * angle);
        g_vertex_buffer_data2[i + 2] = 0;

        g_vertex_buffer_data2[i + 3] = -r*cos(phi*(angle+1));
        g_vertex_buffer_data2[i + 4] = -r*sin(phi*(angle+1));
        g_vertex_buffer_data2[i + 5] = 0;

        g_vertex_buffer_data2[i + 6] = 0;
        g_vertex_buffer_data2[i + 7] = 0;
        g_vertex_buffer_data2[i + 8] = 0;

        if(angle > 178) 
            break;

        angle+=1;
    }

    this->object = create3DObject(GL_TRIANGLES, 180*3, g_vertex_buffer_data, COLOR_BACKGROUND, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 180*3, g_vertex_buffer_data2, color, GL_FILL);

}

void Semic::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object2);
    draw3DObject(this->object);
}

void Semic::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Semic::tick()
{
    this->position.x -= speed.x;
    this->position.y -= speed.y;
}

bounding_box_t Semic::bounding_box()
{
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = {x, y, 0.2, 0.2};
    return bbox;
}
