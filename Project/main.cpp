
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

int oldTimeSinceStart = 0;
int timeSinceStart;
int deltaTime;

int gameTimer=90;
void printTimer() {
        char str[3];
        sprintf(str, "%d", gameTimer);
        glColor3f (1.0, 1.0, 1.0);
        glRasterPos2d(48,97);
        for (int i=0;i<strlen(str);i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
        glFlush();
    }

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
    int score;


public:
    int jumpTimer=0;
    void incScore(){
        score++;
        glutPostRedisplay();
        }
    void moveRight() {
    // Check if there is any terrain obstructing the movement to the right
    for (size_t i = 0; i < terrainObjects.size(); ++i) {
        if (x + 2>= terrainObjects[i].left && x + 2 <= terrainObjects[i].right && y+9 >= terrainObjects[i].top && y-9 <= terrainObjects[i].bottom) {
            // If there is terrain in the way, exit the function without moving
            return;
        }
    }
    // If no terrain obstructs the movement, proceed to move the player to the right
    x += moveSpeed*deltaTime/16;
    glutPostRedisplay();
}

    void moveLeft() {
        // Check if there is any terrain obstructing the movement to the left
        for (size_t i = 0; i < terrainObjects.size(); ++i) {
            if (x - 2 >= terrainObjects[i].left && x - 2 <= terrainObjects[i].right && y+9 >= terrainObjects[i].top && y-9 <= terrainObjects[i].bottom) {
                // If there is terrain in the way, exit the function without moving
                return;
            }
        }
        // If no terrain obstructs the movement, proceed to move the player to the left
        x -= moveSpeed*deltaTime/16;
        glutPostRedisplay();
    }

    void moveUp() {
        // Check if there is any terrain obstructing the movement to the left

        // If no terrain obstructs the movement, proceed to move the player to the left
        for (size_t i = 0; i < terrainObjects.size(); ++i) {
        if (y + 9>= terrainObjects[i].bottom && y + 9 <= terrainObjects[i].top && x+2 <= terrainObjects[i].right && x-2 >= terrainObjects[i].left) {
            // If there is terrain in the way, exit the function without moving
            return;
            }
        }
        y += 3*deltaTime/16;
        glutPostRedisplay();
    }
    void moveDown() {
        for (size_t i = 0; i < terrainObjects.size(); ++i) {
        if (x + 2>= terrainObjects[i].left && x + 2 <= terrainObjects[i].right && y-9 <= terrainObjects[i].top && y-9 >= terrainObjects[i].bottom) {
            // If there is terrain in the way, exit the function without moving
            return;
        }
    }
        y -= deltaTime/5;
        glutPostRedisplay();
    }
    void printScore() {
        char str[50];
        sprintf(str, "Score: %d", score);
        glColor3f (1.0, 1.0, 1.0);
        switch(num){
        case 1:glRasterPos2d(1,97);break;
        case 2:glRasterPos2d(97-(strlen(str)),97);
        }
        for (int i=0;i<strlen(str);i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
        glFlush();
    }
    Player(int num, int x, int y) {
        this->num = num;
        this->x = x;
        this->y = y;
        score=0;
    }
    bool isGrounded(){
    for (size_t i = 0; i < terrainObjects.size(); ++i) {
        if (x + 2>= terrainObjects[i].left && x + 2 <= terrainObjects[i].right && y-9 <= terrainObjects[i].top && y-9 >= terrainObjects[i].bottom) {
            return true;
        }
    }
    return false;
    }
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
    void Gravity() {
        if(y<11) y=11;

    if(jumpTimer>0){
        for (size_t i = 0; i < terrainObjects.size(); ++i) {
        if (y + 9>= terrainObjects[i].bottom && y + 9 <= terrainObjects[i].top && x+2 <= terrainObjects[i].right && x-2 >= terrainObjects[i].left) {
            jumpTimer=0;
            return;
            }
        }
        moveUp();
        jumpTimer--;
        return;
    }

    // Simulate falling if not grounded
    if (!isGrounded()) {
        moveDown();
    }


}


    void Jump() {
    if (isGrounded()) {
        jumpTimer=12;
    }
    }

};

Player player1(1, 20, 100);
Player player2(2, 80, 100);

void Timer(int value){
    if(gameTimer>0) gameTimer--;
    glutPostRedisplay();
    glutTimerFunc(1000,Timer,0);
}

void init2D() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, logWidth, 0.0, logHeight);
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    player1.Render();
    player2.Render();
    player1.printScore();
    player2.printScore();
    printTimer();

    for (size_t i = 0; i < terrainObjects.size(); ++i) {
        terrainObjects[i].Render();
    }

    glutSwapBuffers();
    glFlush();
}

void updatePlayerMovement() {
    timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;
    printf("%d,%d\n",player1.getX(),player1.getY());
    player1.Gravity();
    player2.Gravity();

    if (moveLeftWASD)
        player1.moveLeft();
    if (moveRightWASD)
        player1.moveRight();

    if (moveLeftArrow)
        player2.moveLeft();
    if (moveRightArrow)
        player2.moveRight();


}

void Keyboard(unsigned char key, int x, int y) {
    if (key == 'w')
        player1.Jump();
    if (key == 'a')
        moveLeftWASD = true;
    if (key == 'd')
        moveRightWASD = true;

    //Fire here, increments score for now
    if(key=='q')
        player1.incScore();

    if(key=='0')
        player2.incScore();
}

void KeyboardUp(unsigned char key, int x, int y) {
    if (key == 'a')
        moveLeftWASD = false;
    if (key == 'd')
        moveRightWASD = false;
}

void specialKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
       player2.Jump();
    if (key == GLUT_KEY_LEFT)
        moveLeftArrow = true;
    if (key == GLUT_KEY_RIGHT)
        moveRightArrow = true;

    if (key == GLUT_KEY_INSERT)
        player2.incScore();
}

void specialKeyboardUp(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT)
        moveLeftArrow = false;
    if (key == GLUT_KEY_RIGHT)
        moveRightArrow = false;
}

void createTerrainObjects() {
    terrainObjects.push_back(Terrain(50, 0, 100, 5)); //Floor
    terrainObjects.push_back(Terrain(20, 25, 20, 3));
    terrainObjects.push_back(Terrain(80, 25, 20, 3));
    terrainObjects.push_back(Terrain(50, 50, 20, 3));
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
    glutTimerFunc(1000,Timer,0);
    glutMainLoop();
}

