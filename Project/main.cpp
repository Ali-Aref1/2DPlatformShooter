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
float spos = -50;

bool gameOver = false;

bool moveUpWASD = false;
bool moveDownWASD = false;
bool moveLeftWASD = false;
bool moveRightWASD = false;

bool moveUpArrow = false;
bool moveDownArrow = false;
bool moveLeftArrow = false;
bool moveRightArrow = false;

int ms=0;
int TOTALSTARS = 1000;
float STARSx[1000];
float STARSy[1000];
float ASTEROIDX[10];
float ASTEROIDY[10];
float ASTEROIDR[10];
char gameOverText[50];

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
    glRasterPos2d(48, 95);
    for (int i = 0; i < strlen(str); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

    glFlush();
}

class GameObject
{
public:
    float getY() { return y; }
    float getX() { return x; }

    void setY(float value){y=value;}
    void setX(float value){x=value;}


protected:
    float x = 0;
    float y = 0;
    float moveSpeed = 1.0f;
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

class Bullet : public GameObject{
public:
    int player;
    bool right;
    void Render(){
        glColor3f(1.0f,1.0f,0);
        glBegin(GL_POLYGON);
            glVertex2f(x-1,y-1);
            glVertex2f(x-1,y);
            glVertex2f(x,y);
            glVertex2f(x,y-1);
        glEnd();
        glutPostRedisplay();
    }
    Bullet(float x, float y, int player, bool right){
        this->x=x;
        this->y=y;
        this->player=player;
        this->right=right;
    }
};

class Player : public GameObject
{
private:
    int num;



public:
    int score;
    char respawnString[50];
    int jumpTimer = 0;
    bool canShoot=true;
    bool jumping=false;
    bool jumpLock = false;
    bool right=true;
    bool dead=false;
    int health=10;
    int deathTimer=0;
    std::vector<Bullet> bullets;
    void addScore(int value)

    {
        score+=value;
        glutPostRedisplay();
    }
    void moveLeft()
    {
        // Check if there is any terrain obstructing the movement to the left
        for (size_t i = 0; i < terrainObjects.size(); ++i)
        {
            if (x - moveSpeed >= terrainObjects[i].left && x - moveSpeed <= terrainObjects[i].right && y + 9 >= terrainObjects[i].top && y - 9 <= terrainObjects[i].bottom)
            {
                // If there is terrain in the way, exit the function without moving
                return;
            }
        }
        // If no terrain obstructs the movement, proceed to move the player to the left
        right=false;
        x -= moveSpeed;
        glutPostRedisplay();
    }

    void moveRight()
    {
        // Check if there is any terrain obstructing the movement to the right
        for (size_t i = 0; i < terrainObjects.size(); ++i)
        {
            if (x + moveSpeed >= terrainObjects[i].left && x + moveSpeed <= terrainObjects[i].right && y + 9 >= terrainObjects[i].top && y - 9 <= terrainObjects[i].bottom)
            {
                // If there is terrain in the way, exit the function without moving
                return;
            }
        }
        // If no terrain obstructs the movement, proceed to move the player to the right
        right=true;
        x += moveSpeed;
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
        y += 2;
        glutPostRedisplay();
    }
    void moveDown()
    {
        for (size_t i = 0; i < terrainObjects.size(); ++i)
        {
            if (x + 2 >= terrainObjects[i].left && x + 2 <= terrainObjects[i].right && y - 10 <= terrainObjects[i].top && y - 10 >= terrainObjects[i].bottom)
            {
                // If there is terrain in the way, exit the function without moving
                y=terrainObjects[i].top+9;
                return;
            }
        }
        y -= 4;
        if (y < 10)
            y = 10;
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
            glRasterPos2d(1, 90);
            break;
        case 2:
            glRasterPos2d(97 - 1.2*(strlen(str)), 90);
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
                y=terrainObjects[i].top+9;
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
        if(jumping){
            for (size_t i = 0; i < terrainObjects.size(); ++i)
            {
                if (y + 9 >= terrainObjects[i].bottom && y + 9 <= terrainObjects[i].top && x + 2 <= terrainObjects[i].right && x - 2 >= terrainObjects[i].left)
                {
                    jumping = false;
                    jumpTimer = 0;
                    jumpLock=true;
                    return;
                }
            }
            if(jumpTimer>=20){
                jumping = false;
                jumpTimer = 0;
                jumpLock=true;
                return;
            }
            moveUp();
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
            jumping=true;
        }
        if(jumping && jumpTimer<=20){
            jumpTimer++;
        }
        if (jumpTimer>=20)
            {
                jumping=false;
                jumpLock=true;
            }
    }
    void Shoot(){
        if(canShoot){
            canShoot=false;
        if(right){
            bullets.push_back(Bullet(x+2,y+2,num,right));
        }
        else bullets.push_back(Bullet(x-2,y+2,num,right));
    }
    }
    void HealthBar(){

    glBegin(GL_QUADS);
    if(num==1){
        //outer frame
        glColor3f(0.5,0.5,0.5);

        glVertex2i(1,93);
        glVertex2i(1,98);
        glVertex2i(43,98);
        glVertex2i(43,93);

        //red missing health

        glColor3f(1.0,0.0,0.0);

        glVertex2i(2,94);
        glVertex2i(2,97);
        glVertex2i(42,97);
        glVertex2i(42,94);

        //green health


        glColor3f(0.0,1.0,0.0);

        glVertex2i(2,94);
        glVertex2i(2,97);
        glVertex2i(2+4*health,97);
        glVertex2i(2+4*health,94);
    }
    if(num==2){
            //outer frame
        glColor3f(0.5,0.5,0.5);

        glVertex2i(56,93);
        glVertex2i(56,98);
        glVertex2i(98,98);
        glVertex2i(98,93);

        //red missing health

        glColor3f(1.0,0.0,0.0);

        glVertex2i(57,94);
        glVertex2i(57,97);
        glVertex2i(97,97);
        glVertex2i(97,94);

        //green health


        glColor3f(0.0,1.0,0.0);

        glVertex2i(97-4*health,94);
        glVertex2i(97-4*health,97);
        glVertex2i(97,97);
        glVertex2i(97,94);

    }
    glEnd();

    }
    void respawnText(){
        glColor3f(1.0, 1.0, 1.0);
        if(dead && !gameOver)
        sprintf(respawnString, "Respawning in: %d",deathTimer);
        else sprintf(respawnString, "");

        switch (num)
        {
        case 1:
            glRasterPos2d(1, 3);
            break;
        case 2:
            glRasterPos2d(79, 3);
        }
        for (int i = 0; i < strlen(respawnString); i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, respawnString[i]);
        glFlush();
    }
};



Player player1(1, 20, 100);
Player player2(2, 80, 100);

void Randomize()
{
    for (int i = 0; i < TOTALSTARS; i++)
    {

        STARSy[i]-=0.1f;
        if(STARSy[i]<0){
            STARSx[i] = rand() % 100;
            STARSy[i]+=100;
        }
    }
    for (int i = 0; i < 10; i++)
    {
        ASTEROIDY[i] -=0.1f;
        if(ASTEROIDY[i]+ASTEROIDR[i]<0){
            ASTEROIDX[i] = rand() % 100;
            ASTEROIDY[i]+=100+2*ASTEROIDR[i];
        }
        //ASTEROIDR[i] = rand() % 5 + 2; // Random radius between 2 and 7
    }
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

void renderGameOver(){
    glRasterPos2d(50-strlen(gameOverText), 50);
    for (int i = 0; i < strlen(gameOverText); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, gameOverText[i]);
    glFlush();

}

void Display()
{
    glClearColor(0, 0, 0, 1.0); // black color
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0); // White color
    glPointSize(2.0);
    // Set point size for stars
    glBegin(GL_POINTS);

    for (int i = 0; i < TOTALSTARS; i++) // STARS LOOP
    {
        glVertex2f(STARSx[i], STARSy[i]);
    }

    glEnd();

    for (int i = 0; i < 10; i++)
    {
        glColor3f(0.5, 0.5, 0.5);                                 // Grey color
        DrawCircle(ASTEROIDX[i], ASTEROIDY[i], ASTEROIDR[i], 30); // Draw asteroid

        // Draw black holes (tiny dark grey circles)
        glColor3f(0.2, 0.2, 0.2);                                                                          // Dark grey color
        float hole_radius = ASTEROIDR[i] * 0.2;                                                            // Size of black hole relative to asteroid radius
        DrawCircle(ASTEROIDX[i] + ASTEROIDR[i] / 2.0, ASTEROIDY[i] + ASTEROIDR[i] / 2.0, hole_radius, 10); // Draw first black hole
        DrawCircle(ASTEROIDX[i] - ASTEROIDR[i] / 2.0, ASTEROIDY[i] - ASTEROIDR[i] / 2.0, hole_radius, 10); // Draw second black hole
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
    DrawCircle(90.0, 77.0, 10.0, 30); // Draw sun

    player1.Render();
    player2.Render();
    player1.printScore();
    player2.printScore();
    player1.HealthBar();
    player2.HealthBar();

    printTimer();

    for (size_t i = 0; i < terrainObjects.size(); ++i)
    {
        terrainObjects[i].Render();
    }

    for (size_t i = 0; i < player1.bullets.size(); ++i)
    {
        player1.bullets[i].Render();
    }
    for (size_t i = 0; i < player2.bullets.size(); ++i)
    {
        player2.bullets[i].Render();
    }
    player1.respawnText();
    player2.respawnText();

    renderGameOver();

    glutSwapBuffers();
    glFlush();
}

void updatePlayerMovement()
{
    if(gameOver){
        player1.setX(-20);
        player2.setX(120);
    }
    if(player1.dead){
        player1.setX(-20);
    }
    if(player2.dead){
        player2.setX(120);
    }
    player1.Gravity();
    player2.Gravity();
    if (moveUpWASD && !player1.jumpLock){
        if(player1.isGrounded())
            player1.jumping=true;
        if(player1.jumping){
            player1.jumpTimer++;
        }

    }
    else{
        player1.jumping=false;
        player1.jumpTimer=0;
    }
    if (moveLeftWASD)
        player1.moveLeft();
    if (moveRightWASD)
        player1.moveRight();



    if (moveUpArrow && !player2.jumpLock){
        if(player2.isGrounded())
            player2.jumping=true;
        if(player2.jumping){
            player2.jumpTimer++;
        }

    }
    else{
        player2.jumping=false;
        player2.jumpTimer=0;
    }
    if (moveLeftArrow)
        player2.moveLeft();
    if (moveRightArrow)
        player2.moveRight();

    for(size_t i = 0; i < player1.bullets.size(); ++i){
        if(player1.bullets[i].right){
        player1.bullets[i].setX(player1.bullets[i].getX()+1.5f);
        if(player1.bullets[i].getX()+1.5f >=player2.getX()-2 && player1.bullets[i].getX()+1.5f <= player2.getX()+2 && player1.bullets[i].getY() >= player2.getY()-9 && player1.bullets[i].getY() <= player2.getY()+9){
            player2.health--;
            player1.bullets.erase(player1.bullets.begin() + i);
            glutPostRedisplay();

        }}
        else {
        player1.bullets[i].setX(player1.bullets[i].getX()-1.5f);
        if(player1.bullets[i].getX()-1.5f <=player2.getX()+2 && player1.bullets[i].getX()-1.5f >= player2.getX()-2 && player1.bullets[i].getY() >= player2.getY()-9 && player1.bullets[i].getY() <= player2.getY()+9){
            player2.health--;
            player1.bullets.erase(player1.bullets.begin() + i);
            glutPostRedisplay();

        }}

        // Check if the bullet is out of the frame boundaries
        if (player1.bullets[i].getX() < 0 || player1.bullets[i].getX() > logWidth ||
            player1.bullets[i].getY() < 0 || player1.bullets[i].getY() > logHeight)
        {
            // Remove the bullet from the vector
            player1.bullets.erase(player1.bullets.begin() + i);
            // Adjust index to account for the removed element
            i--;
        }
    }
    for(size_t i = 0; i < player2.bullets.size(); ++i){
        if(player2.bullets[i].right){
        player2.bullets[i].setX(player2.bullets[i].getX()+1.5f);
        if(player2.bullets[i].getX()+1.5f >=player1.getX()-2 && player2.bullets[i].getX()+1.5f <= player1.getX()+2 && player2.bullets[i].getY() >= player1.getY()-9 && player2.bullets[i].getY() <= player1.getY()+9){
            player1.health--;
            player2.bullets.erase(player2.bullets.begin() + i);
            glutPostRedisplay();

        }}

        else{
        player2.bullets[i].setX(player2.bullets[i].getX()-1.5f);
        if(player2.bullets[i].getX()-1.5f <=player1.getX()+2 && player2.bullets[i].getX()-1.5f >= player1.getX()-2 && player2.bullets[i].getY() >= player1.getY()-9 && player2.bullets[i].getY() <= player1.getY()+9){
            player1.health--;
            player2.bullets.erase(player2.bullets.begin() + i);
            glutPostRedisplay();

        }}

        // Check if the bullet is out of the frame boundaries
        if (player2.bullets[i].getX() < 0 || player2.bullets[i].getX() > logWidth ||
            player2.bullets[i].getY() < 0 || player2.bullets[i].getY() > logHeight)
        {
            // Remove the bullet from the vector
            player2.bullets.erase(player2.bullets.begin() + i);
            // Adjust index to account for the removed element
            i--;
        }
    }
}

int deathwait1=0;
int deathwait2=0;
void Timer(int value)
{
    if(player1.health<0)
        player1.health=0;
    if(player2.health<0)
        player2.health=0;

    if(player1.health==0 && !player1.dead){
        player2.addScore(100);
        player1.deathTimer=5;
        player1.dead=true;
    }
    if(player2.health==0 && !player2.dead){

        player1.addScore(100);
        player2.deathTimer=5;
        player2.dead=true;
    }
    //every 10 frames
    updatePlayerMovement();
    ms+=10;
    if(player1.dead) deathwait1+=10;
    if(player2.dead) deathwait2+=10;
    spos+=0.01f;
    Randomize();
    //every 1s
    if(ms>=625){
        if (!gameOver)
            gameTimer--;

        ms=0;

    }

    if(gameTimer<=0 && !gameOver){
        gameOver=true;
        terrainObjects.clear();
        if(player1.score>player2.score)sprintf(gameOverText,"PLAYER 1 WINS");
        if(player2.score>player1.score)sprintf(gameOverText,"PLAYER 2 WINS");
        if(player1.score==player2.score) sprintf(gameOverText,"DRAW");
    }
    if(deathwait1>=625){
        player1.deathTimer--;
        if(player1.deathTimer==0){
            player1.dead=false;
            sprintf(player1.respawnString,"");
            player1.health=10;
            player1.setX(20);
            player1.setY(120);
        }
        deathwait1=0;
    }

    if(deathwait2>=625){
        player2.deathTimer--;
        if(player2.deathTimer==0){
            player2.dead=false;
            sprintf(player2.respawnString,"");
            player2.health=10;
            player2.setX(80);
            player2.setY(120);
        }
        deathwait2=0;
    }

    glutPostRedisplay();
    glutTimerFunc(10, Timer, 0);
}

void Keyboard(unsigned char key, int x, int y)
{
    if (key == 'w')
        moveUpWASD = true;
    if (key == 'a')
        moveLeftWASD = true;
    if (key == 'd')
        moveRightWASD = true;

    if (key == 'q')
        player1.Shoot();

    if (key == '0')
        player2.Shoot();
}

void KeyboardUp(unsigned char key, int x, int y)
{
    if (key == 'w'){
        moveUpWASD = false;
        player1.jumpLock=false;
        }
    if (key == 'a')
        moveLeftWASD = false;
    if (key == 'd')
        moveRightWASD = false;
    if (key == 'q')
        player1.canShoot=true;
    if (key == '0')
        player2.canShoot=true;
}

void specialKeyboard(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
        moveUpArrow=true;
    if (key == GLUT_KEY_LEFT)
        moveLeftArrow = true;
    if (key == GLUT_KEY_RIGHT)
        moveRightArrow = true;
    if (key == GLUT_KEY_INSERT)
        player2.Shoot();
}

void specialKeyboardUp(int key, int x, int y)
{
    if(key == GLUT_KEY_UP){
        moveUpArrow = false;
        player2.jumpLock=false;
        }
    if (key == GLUT_KEY_LEFT)
        moveLeftArrow = false;
    if (key == GLUT_KEY_RIGHT)
        moveRightArrow = false;
    if (key == GLUT_KEY_INSERT)
        player2.canShoot=true;
}

void createTerrainObjects()
{
    terrainObjects.push_back(Terrain(50, 0, 100, 5)); // Floor
    terrainObjects.push_back(Terrain(20, 25, 20, 5));
    terrainObjects.push_back(Terrain(80, 25, 20, 5));
    terrainObjects.push_back(Terrain(50, 50, 20, 5));
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
    for (int i = 0; i < TOTALSTARS; i++)
    {
        STARSx[i] = rand() % 100;
        STARSy[i] = rand() % 100;
    }
    for (int i = 0; i < 10; i++)
    {
        ASTEROIDX[i] = rand() % 100;
        ASTEROIDY[i] = rand() % 100;
        ASTEROIDR[i] = rand() % 5 + 2;
    }
    //Randomize();
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
    glutTimerFunc(50, Timer, 0);
    glutMainLoop();
}
