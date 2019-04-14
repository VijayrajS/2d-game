#include "enemies.h"
#include "main.h"

// Zapper
Zapper::Zapper(float x, float y, float height, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = rand()%180;
    this->speed.x = 0.01;

    const float width = 0.1;

    speed = glm::vec3(0, 0, 0);
    GLfloat line_data[] = {

      width/2, height/2, 0.0f,
      width/2, -height/2, 0.0f,
      -width/2, height/2, 0.0f,
      -width/2, -height/2, 0.0f,
      -width/2, height/2, 0.0f,
      width/2, -height/2, 0.0f,
    };

    GLfloat pads_data[] = {
        0.2, height/2, 0,
        0.2, float(height/2 + 0.2), 0,
        -0.2, height/2, 0,
        -0.2, height/2, 0,
        0.2, float(height/2 + 0.2), 0,
        -0.2, float(height/2 + 0.2), 0,
        0.2, -height/2, 0,
        -0.2, -height/2, 0,
        -0.2, -float(height/2 + 0.2), 0,
        -0.2, -float(height/2 + 0.2), 0,
        0.2, -float(height/2 + 0.2), 0,
        0.2, -height/2, 0,
    }; 

    this->line = create3DObject(GL_TRIANGLES, 6, line_data, color, GL_FILL);
    this->pads = create3DObject(GL_TRIANGLES, 12, pads_data, {78, 90, 101}, GL_FILL);
}

void Zapper::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->pads);
}

void Zapper::drawfire(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->line);
}

void Zapper::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Zapper::tick()
{
    this->position.x -= speed.x;
}

bounding_box_t Zapper::bounding_box()
{
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = {x, y, 0.3, 0.3};
    return bbox;
}

// Laser
Laser::Laser(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    const float width = 0.1;
    
    speed = glm::vec3(0, 0, 0);
    GLfloat line_data[] = {
        3.8, 0.1, 0,
        -3.8, 0.1, 0,
        -3.8,-0.1,0,
        3.8, 0.1, 0,
        -3.8,-0.1,0,
        3.8, -0.1,0,
    };

    GLfloat pads_data[] = {
        -4, 0.3, 0,
        -4,-0.3, 0,
        -3.7, 0.3, 0,
        -3.7, 0.3, 0,
        -4,-0.3, 0,
        -3.7, -0.3, 0,
        3.7, 0.3, 0,
        4,-0.3, 0,
        4, 0.3, 0,
        3.7, -0.3, 0,
        4,-0.3, 0,
        3.7, 0.3, 0,
    }; 

    this->line = create3DObject(GL_TRIANGLES, 6, line_data, color, GL_FILL);
    this->pads = create3DObject(GL_TRIANGLES, 12, pads_data, {78, 90, 101}, GL_FILL);
}

void Laser::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->pads);
}

void Laser::drawfire(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->line);
}

void Laser::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Laser::tick()
{
    this->position.x -= speed.x;
    this->position.y -= speed.y;
}

bounding_box_t Laser::bounding_box()
{
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = {x, y, 7.2, 0.2};
    return bbox;
}

// Boomerang
Brang::Brang(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = glm::vec3(0, 0, 0);
    static const GLfloat vertex_buffer_data[] = {
        0, 0, 0, // vertex 1
        0.2, 0.2, 0,  // vertex 2
        -0.2, 0, 0,   // vertex 3

        0, 0, 0,  // vertex 3
        -0.2, 0, 0, // vertex 4
        0.2, -0.2, 0 // vertex 1
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Brang::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Brang::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Brang::tick()
{
    this->position.x -= speed.x;
    this->position.y -= speed.y;
}

bounding_box_t Brang::bounding_box()
{
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = {x, y, 0.2, 0.2};
    return bbox;
}

// Dragon
Dragon::Dragon(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = glm::vec3(0, 0, 0);
    GLfloat g_vertex_buffer_data[100000]; // Eye
    GLfloat g_vertex_buffer_data2[100000]; // Body

    float phi = M_PI / 180;
    float r = 1.0f;
    int angle = 0;

    for (int i = 0; i < 3240; i += 9)
    {
        g_vertex_buffer_data2[i] = r * cos(phi * angle);
        g_vertex_buffer_data2[i + 1] = r * sin(phi * angle);
        g_vertex_buffer_data2[i + 2] = 0;

        g_vertex_buffer_data2[i + 3] = r * cos(phi * (angle + 1));
        g_vertex_buffer_data2[i + 4] = r * sin(phi * (angle + 1));
        g_vertex_buffer_data2[i + 5] = 0;

        g_vertex_buffer_data2[i + 6] = 0;
        g_vertex_buffer_data2[i + 7] = 0;
        g_vertex_buffer_data2[i + 8] = 0;

        angle += 1;
    }

    r = 0.2;
    angle = 0;

    for (int i = 0; i < 3240; i += 9)
    {
        g_vertex_buffer_data[i] = r * cos(phi * angle);
        g_vertex_buffer_data[i + 1] = r * sin(phi * angle);
        g_vertex_buffer_data[i + 2] = 0;

        g_vertex_buffer_data[i + 3] = r * cos(phi * (angle + 1));
        g_vertex_buffer_data[i + 4] = r * sin(phi * (angle + 1));
        g_vertex_buffer_data[i + 5] = 0;

        g_vertex_buffer_data[i + 6] = 0;
        g_vertex_buffer_data[i + 7] = 0;
        g_vertex_buffer_data[i + 8] = 0;

        angle += 1;
    }

    GLfloat teeth[] = {
        -0.2, -0.5, 0,
        0, -0.8, 0,
        0.2,-0.5, 0,

        0.2, -0.5, 0,
        0.4, -0.8, 0,
        0.4, -0.5, 0,

        -0.2, -0,5, 0,
        -0.4, -0.5, 0,
        -0.4, -0.8, 0,
    };

    GLfloat horns[] = {
        -1, 1.2, 0,
        -1, 0, 0,
        0, 0, 0,

        1, 0, 0,
        1, 1.2, 0,
        0, 0, 0,
    };

    this->eye = create3DObject(GL_TRIANGLES, 360 * 3, g_vertex_buffer_data, {150, 239, 84}, GL_FILL);
    this->body = create3DObject(GL_TRIANGLES, 360 * 3, g_vertex_buffer_data2, {114, 43, 47}, GL_FILL);
    this->teeth = create3DObject(GL_TRIANGLES, 9, teeth, {220, 131, 50}, GL_FILL);
    this->horns = create3DObject(GL_TRIANGLES, 6, horns, {103, 100, 87}, GL_FILL);

}


void Dragon::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->horns);
    draw3DObject(this->body);
    draw3DObject(this->teeth);
    draw3DObject(this->eye);
}

void Dragon::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Dragon::tick()
{
    this->position.x -= speed.x;
    this->position.y -= speed.y;
}

bounding_box_t Dragon::bounding_box()
{
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = {x, y, 0.2, 0.2};
    return bbox;
}
