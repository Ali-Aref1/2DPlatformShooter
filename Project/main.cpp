#include <GL/glut.h>
#include <stdio.h>

int phyWidth = 700;
int phyHeight = 700;
int logWidth = 100;
int logHeight = 100;
int centerX = logWidth / 2;
int centerY = logHeight / 2;

// Define movement directions for WASD keys
bool moveUpWASD = false;
bool moveDownWASD = false;
bool moveLeftWASD = false;
bool moveRightWASD = false;

// Define movement directions for arrow keys
bool moveUpArrow = false;
bool moveDownArrow = false;
bool moveLeftArrow = false;
bool moveRightArrow = false;

class GameObject {
public:
    void moveRight() {
        x += moveSpeed;
        glutPostRedisplay();
    }
    void moveLeft() {
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

protected:
    int x = 0;
    int y = 0;

private:
    int moveSpeed = 1;
};

class Player : public GameObject {
private:
    int num;
public:
    Player(int num, int x, int y) {
        this->num = num;
        this->x = x;
        this->y = y;
    }
    void Render() {
        switch(num){
            case 1: glColor3f(1.0, 0.0, 0.0);break;
            case 2: glColor3f(0.0, 0.0, 1.0);
        }
        glBegin(GL_QUADS);
        glVertex2i(x - 10, y - 10);
        glVertex2i(x - 10, y + 10);
        glVertex2i(x + 10, y + 10);
        glVertex2i(x + 10, y - 10);
        glEnd();
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
    glutSwapBuffers();
    glFlush();
}

void updatePlayerMovement() { //Update player position live, creates more smooth inputs and allows simultaneous input of Player 1 and Player 2
    if (moveUpWASD)
        player1.moveUp();
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

void Keyboard(unsigned char key, int x, int y) { //Check when WASD is held
    if (key == 'w')
        moveUpWASD = true;
    if (key == 'a')
        moveLeftWASD = true;
    if (key == 's')
        moveDownWASD = true;
    if (key == 'd')
        moveRightWASD = true;
}

void KeyboardUp(unsigned char key, int x, int y) { //Check when WASD is released
    if (key == 'w')
        moveUpWASD = false;
    if (key == 'a')
        moveLeftWASD = false;
    if (key == 's')
        moveDownWASD = false;
    if (key == 'd')
        moveRightWASD = false;
}

void specialKeyboard(int key, int x, int y) { //Check when Arrow keys are held
    if (key == GLUT_KEY_UP)
        moveUpArrow = true;
    if (key == GLUT_KEY_DOWN)
        moveDownArrow = true;
    if (key == GLUT_KEY_LEFT)
        moveLeftArrow = true;
    if (key == GLUT_KEY_RIGHT)
        moveRightArrow = true;
}

void specialKeyboardUp(int key, int x, int y) { //Check when Arrow keys are released
    if (key == GLUT_KEY_UP)
        moveUpArrow = false;
    if (key == GLUT_KEY_DOWN)
        moveDownArrow = false;
    if (key == GLUT_KEY_LEFT)
        moveLeftArrow = false;
    if (key == GLUT_KEY_RIGHT)
        moveRightArrow = false;
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(phyWidth, phyHeight);
    glutCreateWindow("2D Platform Shooter");
    init2D();
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutIdleFunc(updatePlayerMovement); // Idle function updates player movement regularly
    glutMainLoop();
}
