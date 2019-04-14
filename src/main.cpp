#include "main.h"
#include "ball.h"
#include "timer.h"
#include "semic.h"
#include "magnet.h"
#include "enemies.h"
#include "sevenseg.h"
#include "rectangle.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

// Game objects
Ball ball1;
Ball coins[10];
Ball powerup[2];
vector <Ball> balloons;
vector <Ball> fire;

Rectangle top;
Rectangle ground;

Laser las;
Magnet mag;
Brang boomer;
Semic ring;
vector <Zapper> zap1;
Dragon demon;

Sevenseg seg1[6];

// flag variables
bool semicOn = false;
bool onRing = false;

bool laserOn = false;
bool magnetOn = false;
bool boomerOn = false;
bool isDemon = false;

bool isPower[2] = {false, false};
bool Powerf[2] = {false, false};

// coin score
int score = 0;
int demon_time = 0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

// float random number generator
float rand_FloatRange(float a, float b)
{
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

void draw() {
    // Don't change
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram (programID);

    glm::vec3 eye (0, 0, 3);
    glm::vec3 target (0, 0, 0);
    glm::vec3 up (0, 1, 0);
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    glm::mat4 VP = Matrices.projection * Matrices.view;

    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    if (semicOn)
        ring.draw(VP);

    ball1.draw(VP);
    ground.draw(VP);
    top.draw(VP);
    
    // seg1.draw(VP, 5);
    int temp = score;
    for(int i = 4; i >= 0; i--)
    {
        seg1[i].draw(VP, temp%10);
        temp /= 10;
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE))
        ball1.drawflame(VP);

    if(ball1.shieldon)
        ball1.drawshield(VP);

    for (int i = 0; i < 10; i++)
        coins[i].draw(VP);

    if (zap1.size())
        for (int ptr = 0; ptr < zap1.size(); ptr++)
        {
            zap1[ptr].draw(VP);
            if(zap1[ptr].flameon)
                zap1[ptr].drawfire(VP);
        }
    
    if(magnetOn)
        mag.draw(VP);
    
    if(boomerOn)
        boomer.draw(VP);
    
    if(laserOn)
    {
        las.draw(VP);
        if (las.flameon)
            las.drawfire(VP);
    }
 
    for(int i = 0; i < balloons.size(); i++)
    {   
        if(balloons[i].position.y >-3 && balloons[i].position.x <= 3.9)
            balloons[i].draw(VP);

        else if(balloons[i].position.y <= -3)
        {
            balloons[i].draw(VP);
            balloons[i].tick(1);
            balloons.clear();
        }

        else
            balloons.clear();
    }

    for (int i = 0; i < fire.size(); i++)
    {
        if (fire[i].position.y > -3 && fire[i].position.x <= 3.9)
            fire[i].draw(VP);

        else if (fire[i].position.y <= -3)
        {
            fire[i].draw(VP);
            fire[i].tick(1);
            fire.clear();
        }

        else
            fire.clear();
    }

    for(int i = 0; i < 2; i++)
    {
        if (powerup[i].position.y > -3 && powerup[i].position.x >= -3.9)
            powerup[i].draw(VP);

        else if (powerup[i].position.y <= -3)
        {
            powerup[i].draw(VP);
            powerup[i].tick(1);
            if(i == 0)
                powerup[i] = Ball(5, 3, {86, 255, 0});
            if(i == 1)
                powerup[1] = Ball(5, 3, {227, 0, 255});

            isPower[i] = false;
        }

        else
        {
            if(i == 0)
                powerup[i] = Ball(5, 3, {86, 255, 0});
            if(i == 1)
                powerup[1] = Ball(5, 3, {227, 0, 255});

            isPower[i] = false;
        }
    }
    if(isDemon)
        demon.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int left = glfwGetKey(window, GLFW_KEY_LEFT);

    int space = glfwGetKey(window, GLFW_KEY_SPACE);

    int b = glfwGetKey(window, GLFW_KEY_B);

    if(!onRing)
    {
        if(b && balloons.size() < 1)
        {   
            balloons.push_back(Ball(ball1.position.x+0.2, ball1.position.y+0.2, {0, 159, 255}));
            balloons[balloons.size()-1].speed.x = -0.1;
            balloons[balloons.size()-1].speed.y = -0.1;
        }

        if(space && ball1.position[1] < 3)
            ball1.speed.y = -0.1;

        if(!space && ball1.position[1] > -3)
        {
            if(ball1.speed.y < 0)
                ball1.speed.y = 0;

            ball1.speed.y += 0.01;
        }

        if(left)
            ball1.position.x -= 0.1;

        if (right)
            ball1.position.x += 0.1;
    }
}

void tick_elements() {
    ball1.tick(1);

    if(isDemon)
        demon.tick();

    if(zap1.size())
        for(int ptr = 0; ptr < zap1.size(); ptr++)
            zap1[ptr].tick();

    if(magnetOn)
        mag.tick();

    if(semicOn)
        ring.tick();

    if(boomerOn)
        boomer.tick();

    if (laserOn)
        las.tick();

    for(int i = 0; i < 2; i++)
        if(isPower[i])
            powerup[i].tick(1);

    for (int i = 0; i < 10; i++)
        coins[i].tick(1);
    
    for(int i = 0; i < balloons.size(); i++)
        balloons[i].tick(1);

    for (int i = 0; i < fire.size(); i++)
        fire[i].tick(1);

    for (int i = 0; i < 5; i++)
        seg1[i].tick();
}

void initGL(GLFWwindow *window, int width, int height) {

    ball1 = Ball(-3, -3, COLOR_RED);
    ball1.isbarry = true;

    ground = Rectangle(0, -4.6, 10, 4, COLOR_LGREY);
    top = Rectangle(0, 5, 10, 2.5, COLOR_LGREY);
    mag = Magnet(1, 1, COLOR_GREY);
    las = Laser(0, 3, {255, 103, 0});

    powerup[0] = Ball(5, 3, {86, 255, 0});
    powerup[1] = Ball(5, 3, {227, 0, 255});

    boomer = Brang(4, 2.5, {255, 0, 178});
    boomer.speed.x = 0.05;

    ring = Semic(0, 0, {154, 42, 193});
    ring.speed.x = 0.01;

    demon = Dragon(5, 2);
    demon.lives = 5;
    demon.speed.x = 0.02;

    float u = -0.46;
    for(int i = 0; i < 5; i++)
    {
        seg1[i] = Sevenseg(u, 3.5);
        u += 0.23;
    }

    for(int i = 0; i < 10; i++)
    {
        int worth = i%2 + 1;
        if(worth == 1)
            coins[i] = Ball(rand_FloatRange(0, 5), rand_FloatRange(-2, 3)+i/50, COLOR_YELLOW);
        else
            coins[i] = Ball(rand_FloatRange(0, 5), rand_FloatRange(-2, 3)+i/50, COLOR_ORANGE);
        
        coins[i].worth = worth;
        coins[i].speed.x = 0.01;
    }

    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int timer = 0;

int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1000;
    int height = 1000;

    window = initGLFW(width, height);

    initGL (window, width, height);

    while (!glfwWindowShouldClose(window)) {
        if (t60.processTick()) {
            draw();
            glfwSwapBuffers(window);

            tick_input(window);
            tick_elements();
            
            // enable magnet
            if(score%10 == 0 && score%20 != 0)
                magnetOn = true;

            if(score%20 == 0)
                magnetOn = false;
            
            // Boomerang
            if(score%9 == 0 && score%10 != 0)
                boomerOn = true;
            
            if(boomerOn)
            {
                if(boomer.position.x < ball1.position.x)
                    boomer.speed.x = -0.03;
                
                boomer.speed.y += 0.0005;

                if (!ball1.shieldon &&  detect_collision(ball1.bounding_box(), boomer.bounding_box()))
                {
                    score -= 10;
                    if(score < 0) score = 0;
                    boomerOn = false;
                    boomer = Brang(5, 3, {255, 0, 178});
                    boomer.speed.x = 0.03;
                }
                if (boomer.position.x > 4 || boomer.position.y < -3 || boomer.position.x < -4)
                {
                    boomerOn = false;
                    boomer = Brang(5, 3, {255, 0, 178});
                    boomer.speed.x = 0.03;
                }
            }

            if(magnetOn)
            {
                if(near_mag(ball1.bounding_box()))
                    ball1.position.y += 0.2;

                if(mag_col(ball1.bounding_box()))
                {
                    ball1.position.y = 0.24;
                    ball1.speed.y = 0;
                }

                if(mag_col_up(ball1.bounding_box()))
                {
                    ball1.speed.y = 0;
                    ball1.position.y = 1.64;
                }

                if (mag_col_side(ball1.bounding_box()) && ball1.position.x <= 1)
                    ball1.position.x = 1;

                if (mag_col_side(ball1.bounding_box()) && ball1.position.x >= 1)
                    ball1.position.y = 1.8;
                
                if(detect_collision(ball1.bounding_box(), mag.bounding_box()))
                    ball1.position.x = 1;
            }

            for (int i = 0; i < balloons.size(); i++)
                balloons[i].speed.y += 0.005;

            for (int i = 0; i < fire.size(); i++)
                fire[i].speed.y += 0.005;

            for (int i = 0; i < balloons.size(); i++)
            {
                if (zap1.size())
                {
                    for (int i = 0; i < zap1.size(); i++)
                    {
                        float xb = balloons[i].position.x;
                        float yb = balloons[i].position.y;

                        float xzt = zap1[i].position.x + cos(zap1[i].rotation * M_PI / 180.0f);
                        float yzt = zap1[i].position.y + sin(zap1[i].rotation * M_PI / 180.0f);

                        float xzb = zap1[i].position.x - cos(zap1[i].rotation * M_PI / 180.0f);
                        float yzb = zap1[i].position.y - sin(zap1[i].rotation * M_PI / 180.0f);

                        if (lineLine(xb + 0.1, yb + 0.1, xb + 0.1, yb - 0.1, xzt, yzt, xzb, yzb) || lineLine(xb - 0.1, yb + 0.1, xb - 0.1, yb - 0.1, xzt, yzt, xzb, yzb))
                            zap1[i].flameon = false;
                    }
                }
            }

            for (int i = 0; i < 10; i++)
            {
                if(coins[i].position.x < -4.2)
                {
                    int worth = i % 2 + 1;
                    if (worth == 1)
                        coins[i] = Ball(rand_FloatRange(4.5,10), rand_FloatRange(-2, 3) + i/50, COLOR_YELLOW);
                    else
                        coins[i] = Ball(rand_FloatRange(4.5,10), rand_FloatRange(-2, 3) + i/50, COLOR_ORANGE);
                    
                    coins[i].worth = worth;
                    coins[i].speed.x = 0.01;
                }
            }

            for(int i = 0; i < 10; i++)
            {
                if (detect_collision(ball1.bounding_box(), coins[i].bounding_box()))
                {
                    score += coins[i].worth;

                    int worth = i % 2 + 1;
                    if (worth == 1)
                        coins[i] = Ball(rand_FloatRange(5, 10), rand_FloatRange(-2, 3)+i/50, COLOR_YELLOW);
                    else
                        coins[i] = Ball(rand_FloatRange(5, 10), rand_FloatRange(-2, 3)+i/50, COLOR_ORANGE);

                    coins[i].worth = worth;
                    coins[i].speed.x = 0.01;
                }
            }

            for (int i = 0; i < fire.size(); i++)
            {
                if (detect_collision(ball1.bounding_box(), fire[i].bounding_box()))
                {
                    cout << "GAME OVER\nYour score was: " << score << "\n";
                    exit(0);
                }
            }

            if(zap1.size())
            {
                for(int i = 0; i < zap1.size(); i++)
                {
                    float xb = ball1.position.x;
                    float yb = ball1.position.y;

                    float xzt = zap1[i].position.x + cos(zap1[i].rotation * M_PI / 180.0f);
                    float yzt = zap1[i].position.y + sin(zap1[i].rotation * M_PI / 180.0f);

                    float xzb = zap1[i].position.x - cos(zap1[i].rotation * M_PI / 180.0f);
                    float yzb = zap1[i].position.y - sin(zap1[i].rotation * M_PI / 180.0f);

                    if (lineLine(xb + 0.1, yb + 0.1, xb + 0.1, yb - 0.1, xzt, yzt, xzb, yzb) || lineLine(xb - 0.1, yb + 0.1, xb - 0.1, yb - 0.1, xzt, yzt, xzb, yzb))
                        if (!ball1.shieldon &&  !isPower[0] && !onRing && zap1[i].flameon)
                        {    
                            score -= 1;
                            if(score < 0) score = 0;
                        }
                }
            }

            if(timer%500 == 0)
                zap1.push_back(Zapper(5, rand_FloatRange(-2,2), rand_FloatRange(1, 2), {0,236,255}));

            for (int ptr = 0; ptr < zap1.size(); ptr++)
            {
                if(!Powerf[0])
                    zap1[ptr].speed.x = 0.01;
                else
                    zap1[ptr].speed.x = 0.1;
            }

            for (int ptr = 0; ptr < zap1.size(); ptr++)
                if(zap1[ptr].position.x < -4)
                    zap1.erase(zap1.begin()+ptr);
            
            if(timer%720 == 0 && timer != 0 && !laserOn)
            {
                laserOn = true;
                las.speed.y = 0.01;
                las.start_time = timer;
            }

            if(timer%500 == 0)
                zap1.push_back(Zapper(5, rand_FloatRange(-2,2), rand_FloatRange(1, 2), {0,236,255}));

            // POWERUP FUNCTION

            if(timer % 1200 == 0)
            {
                isPower[0] = true;

                if(isPower[0])
                    powerup[0].speed.x = 0.05;
            }

            if(timer % 1800 == 0 && timer % 1200 != 0)
            {
                isPower[1] = true;

                if(isPower[1])
                    powerup[1].speed.x = 0.05;
            }

            int timeon;
            
            for(int i = 0; i < 2; i++)
            {
                if(isPower[i] || Powerf[i])
                {
                    if(detect_collision(ball1.bounding_box(), powerup[i].bounding_box()))
                    {
                        timeon = timer;
                        isPower[i] = false;
                        Powerf[i] = true;
                    }
                    
                    if(i == 0 && Powerf[0])      // Boost
                    {
                        powerup[0] = Ball(5, 3, {86, 255, 0});
                        powerup[0].speed.x = 0;
                        powerup[0].speed.y = 0;

                        for(int t = 0; t < 10; t++)
                            coins[t].speed.x = 0.1;
                        
                        if(Powerf[0])
                            for(int t = 0; t < zap1.size(); t++)
                                zap1[t].speed.x = 0.1;
                        
                        if(timer - timeon == 180)
                        {
                            Powerf[0] = false;

                            for (int t = 0; t < 10; t++)
                                coins[t].speed.x = 0.01;

                            for (int t = 0; t < zap1.size(); t++)
                                zap1[t].speed.x = 0.01;

                            powerup[0] = Ball(5, 3, {86, 255, 0});

                            isPower[0] = false;
                        }
                    }

                    if (i == 1 && Powerf[1]) // Boost
                    {
                        powerup[1] = Ball(5, 3, {227, 0, 255});
                        powerup[1].speed.x = 0;
                        powerup[1].speed.y = 0;
                        ball1.shieldon = true;

                        if(timer - timeon == 600)
                        {
                            ball1.shieldon = false;
                            Powerf[1] = false;
                            powerup[1] = Ball(5, 3, {227, 0, 255});
                            isPower[1] = false;
                        }
                    }
                    powerup[i].speed.y += 0.001;
                }
            }

            // POWERUP FUNCTION ENDS

            if (laserOn)
            {
                if (timer - las.start_time >= 120 && !isPower[0] && !onRing)
                    las.flameon = true;

                if(las.position.y <= -2.9)
                {
                    las.flameon = false;
                    laserOn = false;
                }

                if(las.flameon && !ball1.shieldon && !isPower[0] && !onRing && detect_collision(ball1.bounding_box(), las.bounding_box()))
                {
                    score -= 10;
                    if(score < 0) score = 0;
                    las.flameon = false;
                }
            }

            // SEMICIRCULAR RING

            if(timer%2040 == 0)
                semicOn = true;
            
            if(ring.position.x <= -5)
                semicOn = false;
            
            if(semic_collision(ball1.bounding_box()) && semicOn && !onRing)
                onRing = true;

            if (onRing && 1 - pow(ball1.position.x - ring.position.x, 2) < 0)
                onRing = false;

            if (onRing)
            {
                ball1.speed.y = 0;
                ball1.position.y = ring.position.y - sqrt(1 - pow(ball1.position.x - ring.position.x, 2));
                draw();

                if (ring.position.y - ball1.position.y <= 0.2)
                    onRing = false;
            }

            // Demon
            if (timer % 3600 == 0 && timer != 0)
            {    
                isDemon = true;
                demon_time = timer;
            }

            if(isDemon)
            {
                if(demon.position.x < 2)
                    demon.speed.x = 0;

                if(timer - demon_time == 300)
                {
                    fire.push_back(Ball(demon.position.x - 1, demon.position.y, {197, 56, 15}));
                    for(int i = 0; i < fire.size(); i++)
                        fire[i].speed.x = rand_FloatRange(0.07, 0.1);

                    demon_time = timer;
                }


                for(int i = 0; i < balloons.size(); i++)
                {
                    if (dist(demon.position.x, demon.position.y, balloons[i].position.x, balloons[i].position.y) < 1.1)
                    {
                        demon.lives -= 1;
                        balloons.clear();
                    }
                }

                if(demon.lives <= 0)
                {
                    score += 50;
                    isDemon = false;
                    demon = Dragon(5, 2);
                }
            }

            timer += 1;

        }
        glfwPollEvents();
    }
    cout << "GAME OVER\nYour score was: " << score << "\n";
    quit(window);
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

// COLLISION

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

bool near_mag(bounding_box_t a)
{
    return (a.x >= 1.1 && a.x <= 2.5 && a.y >= -2.5 && a.y <=0);
}

bool mag_col(bounding_box_t a)
{
    return (a.x >= 1.1 && a.x <= 2.5 && a.y >= -0.25 && a.y <= 1);
}

bool mag_col_up(bounding_box_t a)
{
    return (a.x >= 0.8 && a.x <= 2.5 && a.y >= 1 && a.y <= 1.5);
}

bool mag_col_side(bounding_box_t a)
{
    return (a.x <= 0.8 && a.x >= 2.5 && a.x <= 2.6 && a.y >= 1 && a.y <= 1.5);
}

bool semic_collision(bounding_box_t a)
{
    float u = dist(ring.position.x, ring.position.y, a.x, a.y);
    return (u >= 0.8 && u <= 1.2);
}

bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{

    // calculate the distance to intersection point
    float A = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    float B = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

    // if uA and uB are between 0-1, lines are colliding
    if (A >= 0 && A <= 1 && B >= 0 && B <= 1)
        return true;
    
    return false;
}

float dist(float x1, float y1, float x2, float y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2) +
                pow(y2 - y1, 2) * 1.0);
}
