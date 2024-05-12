
#include <GL/glut.h>
#include <vector>
#include <stdio.h>

int phyWidth = 700;
int phyHeight = 700;
int logWidth = 100;
int logHeight = 100;

bool moveUpWASD = false;
bool moveDownWASD = false;
bool moveLeftWASD = false;
bool moveRightWASD = false;

bool moveUpArrow = false;
bool moveDownArrow = false;
bool moveLeftArrow = false;
bool moveRightArrow = false;

class GameObject {
public:

    int getY(){return y;}
    int getX() {return x;}

protected:
    int x = 0;
    int y = 0;
    int moveSpeed = 1;

};

class Terrain : public GameObject {
private:
    int w;
    int h;
public:
    int top;
    int left;
    int right;
    int bottom;

    Terrain(int x, int y, int w, int h) : GameObject() {
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

    void Render() {
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

class Player : public GameObject {
private:
    int num;
    bool grounded=false;

public:
    void moveRight() {
    // Check if there is any terrain obstructing the movement to the right
    for (size_t i = 0; i < terrainObjects.size(); ++i) {
        if (x + moveSpeed >= terrainObjects[i].left && x + moveSpeed <= terrainObjects[i].right && y >= terrainObjects[i].bottom && y <= terrainObjects[i].top) {
            // If there is terrain in the way, exit the function without moving
            return;
        }
    }
    // If no terrain obstructs the movement, proceed to move the player to the right
    x += moveSpeed;
    glutPostRedisplay();
}

void moveLeft() {
    // Check if there is any terrain obstructing the movement to the left
    for (size_t i = 0; i < terrainObjects.size(); ++i) {
        if (x - moveSpeed >= terrainObjects[i].left && x - moveSpeed <= terrainObjects[i].right && y >= terrainObjects[i].bottom && y <= terrainObjects[i].top) {
            // If there is terrain in the way, exit the function without moving
            return;
        }
    }
    // If no terrain obstructs the movement, proceed to move the player to the left
    x -= moveSpeed;
    glutPostRedisplay();
}

    void moveUp() {
        y += moveSpeed;
        glutPostRedisplay();
    }
    void moveDown() {
        y -= moveSpeed;
        glutPostRedisplay();
    }
    Player(int num, int x, int y) {
        this->num = num;
        this->x = x;
        this->y = y;
    }

    bool isGrounded(){return grounded;}
    void Render() {
        switch(num){
            case 1: glColor3f(1.0, 0.0, 0.0); break;
            case 2: glColor3f(0.0, 0.0, 1.0); break;
        }
        glBegin(GL_QUADS); // head
            glVertex2i(x-3,y+3);
            glVertex2i(x-3,y+9);
            glVertex2i(x+3,y+9);
            glVertex2i(x+3,y+3);
        glEnd();

        glBegin(GL_QUADS); // body
            glVertex2i(x-2,y-4);
            glVertex2i(x-2,y+3);
            glVertex2i(x+2,y+3);
            glVertex2i(x+2,y-4);
        glEnd();

        glBegin(GL_QUADS); // arms
            glVertex2i(x-5,y+1);
            glVertex2i(x-5,y+2);
            glVertex2i(x+5,y+2);
            glVertex2i(x+5,y+1);
        glEnd();

        glBegin(GL_QUADS); // legs
            glVertex2i(x-1,y-4);
            glVertex2i(x-2,y-4);
            glVertex2i(x-2,y-9);
            glVertex2i(x-1,y-9);

            glVertex2i(x+1,y-4);
            glVertex2i(x+2,y-4);
            glVertex2i(x+2,y-9);
            glVertex2i(x+1,y-9);
        glEnd();
    }

    void Gravity(){
    // Decrease Y value by 1 unit
    if(!grounded){
        y-=1;
        glutPostRedisplay();

    }
    // Check if the player is standing on terrain
    for (size_t i = 0; i < terrainObjects.size(); ++i) {
        if (y-9 <= terrainObjects[i].top && x >= terrainObjects[i].left && x <= terrainObjects[i].right) {
            y = terrainObjects[i].top + 9; // Adjust player's Y position to be on top of the terrain
            grounded = true; // Set grounded to true
            return; // Exit the function as the player is grounded
        }
        grounded=false;
    }

    grounded = false; // If no terrain is found, set grounded to false
}

    void Jump(){

}

};

Player player1(1, 20, 50);
Player player2(2, 80, 50);

void init2D() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, logWidth, 0.0, logHeight);
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    player1.Render();
    player2.Render();

    for (size_t i = 0; i < terrainObjects.size(); ++i) {
        terrainObjects[i].Render();
    }

    glutSwapBuffers();
    glFlush();
}

void updatePlayerMovement() {
    player1.Gravity();
    player2.Gravity();



    if (moveUpWASD)
        player1.Jump();
    if (moveDownWASD)
        player1.moveDown();
    if (moveLeftWASD)
        player1.moveLeft();
    if (moveRightWASD)
        player1.moveRight();

    if (moveUpArrow)
        player2.moveUp();
    if (moveDownArrow)
        player2.moveDown();
    if (moveLeftArrow)
        player2.moveLeft();
    if (moveRightArrow)
        player2.moveRight();


}

void Keyboard(unsigned char key, int x, int y) {
    if (key == 'w')
        moveUpWASD = true;
    if (key == 'a')
        moveLeftWASD = true;
    if (key == 's')
        moveDownWASD = true;
    if (key == 'd')
        moveRightWASD = true;
}

void KeyboardUp(unsigned char key, int x, int y) {
    if (key == 'w')
        moveUpWASD = false;
    if (key == 'a')
        moveLeftWASD = false;
    if (key == 's')
        moveDownWASD = false;
    if (key == 'd')
        moveRightWASD = false;
}

void specialKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        moveUpArrow = true;
    if (key == GLUT_KEY_DOWN)
        moveDownArrow = true;
    if (key == GLUT_KEY_LEFT)
        moveLeftArrow = true;
    if (key == GLUT_KEY_RIGHT)
        moveRightArrow = true;
}

void specialKeyboardUp(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        moveUpArrow = false;
    if (key == GLUT_KEY_DOWN)
        moveDownArrow = false;
    if (key == GLUT_KEY_LEFT)
        moveLeftArrow = false;
    if (key == GLUT_KEY_RIGHT)
        moveRightArrow = false;
}

void createTerrainObjects() {
    terrainObjects.push_back(Terrain(50, 10, 100, 5));
    terrainObjects.push_back(Terrain(70, 20, 10, 8));
}


int main(int argc, char **argv) {
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
    glutMainLoop();
}

