#include <GL/glut.h>
#include <stdio.h>

int phyWidth = 700;
int phyHeight = 700;
int logWidth = 100;
int logHeight = 100;
int centerX = logWidth / 2;
int centerY = logHeight / 2;

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
public:
    Player(int num, int x, int y) {
        this->num = num;
        this->x = x;
        this->y = y;
    }
    void Render() {
        glBegin(GL_QUADS);
        glVertex2i(x - 10, y - 10);
        glVertex2i(x - 10, y + 10);
        glVertex2i(x + 10, y + 10);
        glVertex2i(x + 10, y - 10);
        glEnd();
    }

private:
    int num;
};

Player player1(1, 50, 50);

void init2D() {
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, logWidth, 0.0, logHeight);
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    player1.Render();
    glutSwapBuffers();
    glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == 'w')
        player1.moveUp();
    if (key == 'a')
        player1.moveLeft();
    if (key == 's')
        player1.moveDown();
    if (key == 'd')
        player1.moveRight();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(phyWidth, phyHeight);
    glutCreateWindow("problem demonstration");
    init2D();
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
}
