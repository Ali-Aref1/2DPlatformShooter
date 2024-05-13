#include <GL/glut.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

int phyWidth = 700;
int phyHeight = 700;
int logWidth = 100;
int logHeight = 100;
int spos = -50;

bool moveUpWASD = false;
bool moveDownWASD = false;
bool moveLeftWASD = false;
bool moveRightWASD = false;

bool moveUpArrow = false;
bool moveDownArrow = false;
bool moveLeftArrow = false;
bool moveRightArrow = false;

int oldTimeSinceStart = 0;
int timeSinceStart;
int deltaTime;
void moveShip(int &ypos)
{
    ypos++;
}

int gameTimer = 90;
void printTimer()
{
    char str[3];
    sprintf(str, "%d", gameTimer);
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2d(48, 97);
    for (int i = 0; i < strlen(str); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    glFlush();
}

class GameObject
{
public:
    int getY() { return y; }
    int getX() { return x; }

protected:
    float x = 0;
    float y = 0;
    int moveSpeed = 1;
};

class Terrain : public GameObject
{
private:
    int w;
    int h;

public:
    int top;
    int left;
    int right;
    int bottom;

    Terrain(int x, int y, int w, int h) : GameObject()
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        // Calculate bounds
        top = y + h / 2;
        left = x - w / 2;
        right = x + w / 2;
        bottom = y - h / 2;
    }

    void Render()
    {
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        glVertex2i(left, bottom);
        glVertex2i(left, top);
        glVertex2i(right, top);
        glVertex2i(right, bottom);
        glEnd();
    }
};

std::vector<Terrain> terrainObjects;

class Player : public GameObject
{
private:
    int num;
    int score;

public:
    int jumpTimer = 0;
    void incScore()
    {
        score++;
        glutPostRedisplay();
    }
    void moveLeft(float distance)
{
    // Check if there is any terrain obstructing the movement to the left
    for (size_t i = 0; i < terrainObjects.size(); ++i)
    {
        if (x - distance >= terrainObjects[i].left && x - distance <= terrainObjects[i].right && y + 9 >= terrainObjects[i].top && y - 9 <= terrainObjects[i].bottom)
        {
            // If there is terrain in the way, exit the function without moving
            return;
        }
    }
    // If no terrain obstructs the movement, proceed to move the player to the left
    x -= distance;
    glutPostRedisplay();
}

void moveRight(float distance)
{
    // Check if there is any terrain obstructing the movement to the right
    for (size_t i = 0; i < terrainObjects.size(); ++i)
    {
        if (x + distance >= terrainObjects[i].left && x + distance <= terrainObjects[i].right && y + 9 >= terrainObjects[i].top && y - 9 <= terrainObjects[i].bottom)
        {
            // If there is terrain in the way, exit the function without moving
            return;
        }
    }
    // If no terrain obstructs the movement, proceed to move the player to the right
    x += distance;
    glutPostRedisplay();
}


    void moveUp()
    {
        // Check if there is any terrain obstructing the movement to the left

        // If no terrain obstructs the movement, proceed to move the player to the left
        for (size_t i = 0; i < terrainObjects.size(); ++i)
        {
            if (y + 9 >= terrainObjects[i].bottom && y + 9 <= terrainObjects[i].top && x + 2 <= terrainObjects[i].right && x - 2 >= terrainObjects[i].left)
            {
                // If there is terrain in the way, exit the function without moving
                return;
            }
        }
        y += 3;
        glutPostRedisplay();
    }
    void moveDown()
    {
        for (size_t i = 0; i < terrainObjects.size(); ++i)
        {
            if (x + 2 >= terrainObjects[i].left && x + 2 <= terrainObjects[i].right && y - 9 <= terrainObjects[i].top && y - 9 >= terrainObjects[i].bottom)
            {
                // If there is terrain in the way, exit the function without moving
                return;
            }
        }
        y -= 6;
        if(y<10) y=10;
        glutPostRedisplay();
    }
    void printScore()
    {
        char str[50];
        sprintf(str, "Score: %d", score);
        glColor3f(1.0, 1.0, 1.0);
        switch (num)
        {
        case 1:
            glRasterPos2d(1, 97);
            break;
        case 2:
            glRasterPos2d(97 - (strlen(str)), 97);
        }
        for (int i = 0; i < strlen(str); i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
        glFlush();
    }
    Player(int num, int x, int y)
    {
        this->num = num;
        this->x = x;
        this->y = y;
        score = 0;
    }
    bool isGrounded()
    {
        for (size_t i = 0; i < terrainObjects.size(); ++i)
        {
            if (x + 2 >= terrainObjects[i].left && x + 2 <= terrainObjects[i].right && y - 9 <= terrainObjects[i].top && y - 9 >= terrainObjects[i].bottom)
            {
                return true;
            }
        }
        return false;
    }
    void Render()
    {
        switch (num)
        {
        case 1:
            glColor3f(1.0, 0.0, 0.0);
            break;
        case 2:
            glColor3f(0.0, 0.0, 1.0);
            break;
        }
        glBegin(GL_QUADS); // head
        glVertex2i(x - 3, y + 3);
        glVertex2i(x - 3, y + 9);
        glVertex2i(x + 3, y + 9);
        glVertex2i(x + 3, y + 3);
        glEnd();

        glBegin(GL_QUADS); // body
        glVertex2i(x - 2, y - 4);
        glVertex2i(x - 2, y + 3);
        glVertex2i(x + 2, y + 3);
        glVertex2i(x + 2, y - 4);
        glEnd();

        glBegin(GL_QUADS); // arms
        glVertex2i(x - 5, y + 1);
        glVertex2i(x - 5, y + 2);
        glVertex2i(x + 5, y + 2);
        glVertex2i(x + 5, y + 1);
        glEnd();

        glBegin(GL_QUADS); // legs
        glVertex2i(x - 1, y - 4);
        glVertex2i(x - 2, y - 4);
        glVertex2i(x - 2, y - 9);
        glVertex2i(x - 1, y - 9);

        glVertex2i(x + 1, y - 4);
        glVertex2i(x + 2, y - 4);
        glVertex2i(x + 2, y - 9);
        glVertex2i(x + 1, y - 9);
        glEnd();
    }
    void Gravity()
    {
        if (y < 11)
            y = 11;

        if (jumpTimer > 0)
        {
            for (size_t i = 0; i < terrainObjects.size(); ++i)
            {
                if (y + 9 >= terrainObjects[i].bottom && y + 9 <= terrainObjects[i].top && x + 2 <= terrainObjects[i].right && x - 2 >= terrainObjects[i].left)
                {
                    jumpTimer = 0;
                    return;
                }
            }
            moveUp();
            jumpTimer--;
            return;
        }

        // Simulate falling if not grounded
        if (!isGrounded())
        {
            moveDown();
        }
    }

    void Jump()
    {
        if (isGrounded())
        {
            jumpTimer = 12;
        }
    }
};

Player player1(1, 20, 100);
Player player2(2, 80, 100);

void Timer(int value)
{
    if (gameTimer > 0)
        gameTimer--;
    glutPostRedisplay();
    glutTimerFunc(1000, Timer, 0);
}

void init2D()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, logWidth, 0.0, logHeight);
}

void DrawCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments); // get the current angle
        float x = r * cosf(theta);                                        // calculate the x component
        float y = r * sinf(theta);                                        // calculate the y component
        glVertex2f(x + cx, y + cy);                                       // output vertex
    }
    glEnd();
}

void printSome(char *str, int x, int y)
{
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2d(x, y);
    for (int i = 0; i < strlen(str); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    glFlush();
}

void Display()
{
    glClearColor(0, 0, 0, 1.0); // black color
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a few white clouds
    glColor3f(1.0, 1.0, 1.0); // White color
    glPointSize(2.0);         // Set point size for stars
    glBegin(GL_POINTS);
    for (int i = 0; i < 1000; i++)
    {
        float x = rand() % 100;
        float y = rand() % 100;
        glVertex2f(x, y);
    }
    glEnd();

    // Draw asteroids with black holes
    for (int i = 0; i < 10; i++)
    {
        float x = rand() % 100;
        float y = rand() % 100;
        float r = rand() % 5 + 2; // Random radius between 2 and 7

        // Draw asteroid (grey circle)
        glColor3f(0.5, 0.5, 0.5); // Grey color
        DrawCircle(x, y, r, 30);  // Draw asteroid

        // Draw black holes (tiny dark grey circles)
        glColor3f(0.2, 0.2, 0.2);                              // Dark grey color
        float hole_radius = r * 0.2;                           // Size of black hole relative to asteroid radius
        DrawCircle(x + r / 2.0, y + r / 2.0, hole_radius, 10); // Draw first black hole
        DrawCircle(x - r / 2.0, y - r / 2.0, hole_radius, 10); // Draw second black hole
    }

    // Draw spaceship body
    glColor3f(0.75, 0.75, 0.75); // Silver color
    glBegin(GL_POLYGON);
    glVertex2f(46, 55 + spos);
    glVertex2f(46, 65 + spos);
    glVertex2f(50, 70 + spos);
    glVertex2f(54, 65 + spos);
    glVertex2f(54, 55 + spos);
    glEnd();

    // Spaceship Coloring
    glColor3f(1.0, 1.0, 1.0); // Silver color
    glBegin(GL_POLYGON);
    glVertex2f(47, 55 + spos);
    glVertex2f(47, 65 + spos);
    glVertex2f(53, 65 + spos);
    glVertex2f(53, 55 + spos);
    glEnd();

    // Draw spaceship leftwing
    glColor3f(0.75, 0.75, 0.75); // Silver color
    glBegin(GL_POLYGON);
    glVertex2f(46, 55 + spos);
    glVertex2f(46, 60 + spos);
    glVertex2f(43, 60 + spos);
    glVertex2f(41, 55 + spos);
    glEnd();

    // Draw spaceship rightwing
    glColor3f(0.75, 0.75, 0.75); // Silver color
    glBegin(GL_POLYGON);
    glVertex2f(54, 55 + spos);
    glVertex2f(54, 60 + spos);
    glVertex2f(57, 60 + spos);
    glVertex2f(59, 55 + spos);
    glEnd();

    // Draw spaceship glass
    glColor3f(0.529, 0.808, 0.922); // Light blue color
    glBegin(GL_TRIANGLES);
    glVertex2f(48, 66 + spos); // Bottom vertex
    glVertex2f(50, 69 + spos); // Top left vertex
    glVertex2f(52, 66 + spos); // Top right vertex
    glEnd();

    // Draw spaceship engine fires (orange rectangles)
    glColor3f(1.0, 0.5, 0.0); // Orange color
    glBegin(GL_POLYGON);
    glVertex2f(43, 52 + spos); // Left fire
    glVertex2f(43, 55 + spos);
    glVertex2f(45, 55 + spos);
    glVertex2f(45, 52 + spos);
    glEnd();
    // Draw spaceship engine fires (orange rectangles)
    glColor3f(1.0, 0.5, 0.0); // Orange color
    glBegin(GL_POLYGON);
    glVertex2f(55, 52 + spos); // Left fire
    glVertex2f(55, 55 + spos);
    glVertex2f(57, 55 + spos);
    glVertex2f(57, 52 + spos);
    glEnd();

    glColor3f(1.0, 1.0, 0.0);         // Yellow color
    DrawCircle(90.0, 90.0, 10.0, 30); // Draw sun

    player1.Render();
    player2.Render();
    player1.printScore();
    player2.printScore();
    printTimer();

    for (size_t i = 0; i < terrainObjects.size(); ++i)
    {
        terrainObjects[i].Render();
    }

    glutSwapBuffers();
    glFlush();
}

void updatePlayerMovement()
{
    // Calculate elapsed time since the last frame
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentTime - oldTimeSinceStart;
    oldTimeSinceStart = currentTime;

    // Update player movement based on elapsed time
    float playerSpeed = 0.1f; // Adjust this value to control player speed
    float playerMovement = playerSpeed * deltaTime;

    player1.Gravity();
    player2.Gravity();

    if (moveLeftWASD)
        player1.moveLeft(playerMovement);
    if (moveRightWASD)
        player1.moveRight(playerMovement);

    if (moveLeftArrow)
        player2.moveLeft(playerMovement);
    if (moveRightArrow)
        player2.moveRight(playerMovement);
}


void Keyboard(unsigned char key, int x, int y)
{
    if (key == 'w')
        player1.Jump();
    if (key == 'a')
        moveLeftWASD = true;
    if (key == 'd')
        moveRightWASD = true;

    // Fire here, increments score for now
    if (key == 'q')
        player1.incScore();

    if (key == '0')
        player2.incScore();
}

void KeyboardUp(unsigned char key, int x, int y)
{
    if (key == 'a')
        moveLeftWASD = false;
    if (key == 'd')
        moveRightWASD = false;
}

void specialKeyboard(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
        player2.Jump();
    if (key == GLUT_KEY_LEFT)
        moveLeftArrow = true;
    if (key == GLUT_KEY_RIGHT)
        moveRightArrow = true;

    if (key == GLUT_KEY_INSERT)
        player2.incScore();
}

void specialKeyboardUp(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
        moveLeftArrow = false;
    if (key == GLUT_KEY_RIGHT)
        moveRightArrow = false;
}

void createTerrainObjects()
{
    terrainObjects.push_back(Terrain(50, 0, 100, 5)); // Floor
    terrainObjects.push_back(Terrain(20, 25, 20, 3));
    terrainObjects.push_back(Terrain(80, 25, 20, 3));
    terrainObjects.push_back(Terrain(50, 50, 20, 3));
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(phyWidth, phyHeight);
    glutCreateWindow("2D Platform Shooter");
    init2D();

    createTerrainObjects();

    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutIdleFunc(updatePlayerMovement);
    glutTimerFunc(1000, Timer, 0);
    glutMainLoop();
}
