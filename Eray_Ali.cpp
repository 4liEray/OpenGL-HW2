/*********
   CTIS164 - Homework II
----------
STUDENT : Ali Eray
SECTION : 002
HOMEWORK: HW II
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
1- When there's a hit the car explodes.
2- Each car has it's own color and brand.
3- If you press 'h' the scoreboard hides.
4- If the bazooka goes to the left end it comes out from the right.
5- Mercedes(The Gray car) is faster than the others and it worths extra 2 points and if it explodes it goes even faster.
6- I made the timer equivelent to the real time.
7- There can be two collisions if the cars are too close.
8- At the end winner of the race displays.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define TARGET_WIDTH  192


int timeCounter = 99, sec = 19, circ = 20, caset = 0, lapCounter = 1, hitCounter = 0, merS = 9;
int hitx, hity;


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, spacebar = false;
int  winWidth, winHeight; // current Window width and height

bool activeTimer = false;
bool fire = false;
bool hit = false;
bool carhit = false; 
bool hide = false; 
bool win = false;


typedef struct {
    float x, y;
} point_t;

typedef struct {
    point_t pos;   // position of the object
} player_t;

typedef struct {
    point_t pos;
    bool active;
} fire_t;

typedef struct {
    int r, g, b;
} color_t;

typedef struct {
    point_t loc;
    color_t color;
    float width;
    float speed;
} target_t;


#define MAX_FIRE 20 // 20 fires at a time.
#define FIRE_RATE 60 // after 8 frames you can throw another one.

fire_t fr[MAX_FIRE];
player_t p = { {0,-220} };
target_t target[5];
int fire_rate = 0;





// mouse position
point_t mouse;

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void pie_filled(float x, float y, float r, float start, float end) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    for (float angle = start; angle < end; angle += 10) {
        glVertex2f(r * cos(angle * D2R) + x, r * sin(angle * D2R) + y);
    }
    glVertex2f(r * cos(end * D2R) + x, r * sin(end * D2R) + y);
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void grass() {

    for (int i = 0; i < 1400; i+= 60) {
        glColor3f(51. / 255, 102. / 255, 0);
        glRectf(-700, -400 + i, 700, -370 + i);
    }
}

void road() {

    glColor3f(1, 1, 1);
    glRectf(-700, 160, 700, 180);

    for (int i = 0; i < 1440; i += 40) {
        glColor3f(1, 0, 0);
        glRectf(-700 + i, 160, -680 + i, 180);
    }

    glColor3f(1, 1, 1);
    glRectf(-700, 340, 700, 360);

    for (int i = 0; i < 1440; i += 40) {
        glColor3f(1, 0, 0);
        glRectf(-700 + i, 340, -680 + i, 360);
    }
   
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);

    glVertex2f(-700, 180);
    glVertex2f(700, 180);

    glVertex2f(-700, 181);
    glVertex2f(700, 181);

    glVertex2f(-700, 182);
    glVertex2f(700, 182);

    glVertex2f(-700, 339);
    glVertex2f(700, 339);

    glVertex2f(-700, 338);
    glVertex2f(700, 338);

    glVertex2f(-700, 337);
    glVertex2f(700, 337);

    glEnd();
    
    glColor3f(0.5, 0.5, 0.5);
    glRectf(-700, 337, 700, 183);


    glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_LINES);
    for (int i = 0; i < 12; i+= 3) {
        
        glVertex2f(300, 200 + i);
        glVertex2f(325, 200 + i);
        glVertex2f(300, 201 + i);
        glVertex2f(325, 201 + i);

        glVertex2f(-324, 280 + i);
        glVertex2f(-299, 280 + i);
        glVertex2f(-324, 281 + i);
        glVertex2f(-299, 281 + i);

    }
    glEnd();

    for (int i = 0; i < 1440; i += 100) {
        glColor3f(1, 1, 1);
        glRectf(-680 + i, 265, -620 + i, 255);
    }

}

void tireup(int x, int y) {

    glColor3f(0, 0, 0);
    glRectf(x -5, y -8, x + 5, y - 28);

    glColor3f(0, 0, 0);
    glRectf(x -3, y - 8, x + 5, y);

    glColor3f(0, 0, 0);
    circle(x - 10, y + 10, 10);

    glColor3f(0, 0, 0);
    circle(x + 10, y + 10, 10);

    glColor3f(0, 0, 0);
    glRectf(x - 10, y, x + 10, y + 20);

    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_LINES);
    for (int i = 0; i < 14; i += 6) {
        glVertex2f(x - 15, y + 3 + i);
        glVertex2f(x + 15, y + 3 + i);
        glVertex2f(x - 15, y + 4 + i);
        glVertex2f(x + 15, y + 4 + i);
    }
    glEnd();
}

void tiredown(int x, int y) {

    glColor3f(0, 0, 0);
    glRectf(x - 5, y + 8, x + 5, y + 28);

    glColor3f(0, 0, 0);
    glRectf(x - 3, y + 8, x + 5, y);

    glColor3f(0, 0, 0);
    circle(x - 10, y - 10, 10);

    glColor3f(0, 0, 0);
    circle(x + 10, y - 10, 10);

    glColor3f(0, 0, 0);
    glRectf(x - 10, y, x + 10, y - 20);

    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_LINES);
    for (int i = 0; i < 14; i += 6) {
        glVertex2f(x - 15, y - 3 - i);
        glVertex2f(x + 15, y - 3 - i);
        glVertex2f(x - 15, y - 4 - i);
        glVertex2f(x + 15, y - 4 - i);
    }
    glEnd();
}

void showCursorPosition() {
    //show mouse coordinate at top right region
    glColor3f(0, 0, 0);
    vprint(475, 375, GLUT_BITMAP_TIMES_ROMAN_24, "(%.0f, %.0f)", mouse.x, mouse.y);
}

void car(target_t* t) {

    for (int i = 0; i < 5; i++) {
        glColor3f(0, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(t[i].loc.x, t[i].loc.y + 45);
        glVertex2f(t[i].loc.x + 22, t[i].loc.y + 25);
        glVertex2f(t[i].loc.x + 22, t[i].loc.y - 25);
        glVertex2f(t[i].loc.x, t[i].loc.y - 45);
        glEnd();

        glColor3f(t[i].color.r / 255., t[i].color.g / 255., t[i].color.b / 255.);
        glBegin(GL_QUADS);
        glVertex2f(t[i].loc.x + 2, t[i].loc.y + 40);
        glVertex2f(t[i].loc.x + 20, t[i].loc.y + 23);
        glVertex2f(t[i].loc.x + 20, t[i].loc.y - 23);
        glVertex2f(t[i].loc.x + 2, t[i].loc.y - 43);
        glEnd();

        glColor3f(0, 0, 0);
        glRectf(t[i].loc.x + 20, t[i].loc.y + 10, t[i].loc.x + 30, t[i].loc.y + 13);

        glColor3f(0, 0, 0);
        glRectf(t[i].loc.x + 20, t[i].loc.y - 10, t[i].loc.x + 30, t[i].loc.y - 13);

        glColor3f(0, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(t[i].loc.x + 30, t[i].loc.y + 20);
        glVertex2f(t[i].loc.x + 50, t[i].loc.y + 10);
        glVertex2f(t[i].loc.x + 50, t[i].loc.y - 10);
        glVertex2f(t[i].loc.x + 30, t[i].loc.y - 20);
        glEnd();

        tireup(t[i].loc.x + 55, t[i].loc.y + 18);

        tiredown(t[i].loc.x + 55, t[i].loc.y - 18);

        glColor3f(0, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(t[i].loc.x + 60, t[i].loc.y + 10);
        glVertex2f(t[i].loc.x + 80, t[i].loc.y + 20);
        glVertex2f(t[i].loc.x + 80, t[i].loc.y - 20);
        glVertex2f(t[i].loc.x + 60, t[i].loc.y - 10);
        glEnd();

        glColor3f(t[i].color.r / 255., t[i].color.g / 255., t[i].color.b / 255.);
        glBegin(GL_QUADS);
        glVertex2f(t[i].loc.x + 27, t[i].loc.y);
        glVertex2f(t[i].loc.x + 100, t[i].loc.y + 20);
        glVertex2f(t[i].loc.x + 120, t[i].loc.y);
        glVertex2f(t[i].loc.x + 100, t[i].loc.y - 20);
        glEnd();

        glColor3f(0, 0, 0);
        glRectf(t[i].loc.x + 90, t[i].loc.y + 8, t[i].loc.x + 170, t[i].loc.y - 8);

        glColor3f(t[i].color.r / 255., t[i].color.g / 255., t[i].color.b / 255.);
        glRectf(t[i].loc.x + 90, t[i].loc.y + 6, t[i].loc.x + 170, t[i].loc.y - 6);

        tireup(t[i].loc.x + 130, t[i].loc.y + 14);
        tiredown(t[i].loc.x + 130, t[i].loc.y - 14);

        glColor3f(0, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(t[i].loc.x + 168, t[i].loc.y + 12);
        glVertex2f(t[i].loc.x + 192, t[i].loc.y + 5);
        glVertex2f(t[i].loc.x + 192, t[i].loc.y - 5);
        glVertex2f(t[i].loc.x + 168, t[i].loc.y - 12);
        glEnd();

        glColor3f(t[i].color.r / 255., t[i].color.g / 255., t[i].color.b / 255.);
        glBegin(GL_QUADS);
        glVertex2f(t[i].loc.x + 170, t[i].loc.y + 10);
        glVertex2f(t[i].loc.x + 190, t[i].loc.y + 3);
        glVertex2f(t[i].loc.x + 190, t[i].loc.y - 3);
        glVertex2f(t[i].loc.x + 170, t[i].loc.y - 10);
        glEnd();

        glColor3f(0, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(t[i].loc.x + 153, t[i].loc.y + 37);
        glVertex2f(t[i].loc.x + 167, t[i].loc.y + 25);
        glVertex2f(t[i].loc.x + 167, t[i].loc.y - 25);
        glVertex2f(t[i].loc.x + 153, t[i].loc.y - 37);
        glEnd();

        glColor3f(t[i].color.r / 255., t[i].color.g / 255., t[i].color.b / 255.);
        glBegin(GL_QUADS);
        glVertex2f(t[i].loc.x + 155, t[i].loc.y + 35);
        glVertex2f(t[i].loc.x + 165, t[i].loc.y + 23);
        glVertex2f(t[i].loc.x + 165, t[i].loc.y - 23);
        glVertex2f(t[i].loc.x + 155, t[i].loc.y - 35);
        glEnd();

        glColor3f(0, 0, 0);
        glBegin(GL_TRIANGLES);
        glVertex2f(t[i].loc.x + 70, t[i].loc.y);
        glVertex2f(t[i].loc.x + 80, t[i].loc.y + 10);
        glVertex2f(t[i].loc.x + 80, t[i].loc.y - 10);

        glEnd();

        glColor3f(0, 0, 0);
        glBegin(GL_TRIANGLES);
        glVertex2f(t[i].loc.x + 60, t[i].loc.y);
        glVertex2f(t[i].loc.x + 68, t[i].loc.y + 8);
        glVertex2f(t[i].loc.x + 68, t[i].loc.y - 8);

        glEnd();

        glColor3f(0, 0, 0);
        glBegin(GL_TRIANGLES);
        glVertex2f(t[i].loc.x + 52, t[i].loc.y);
        glVertex2f(t[i].loc.x + 58, t[i].loc.y + 6);
        glVertex2f(t[i].loc.x + 58, t[i].loc.y - 6);

        glEnd();

        glColor3f(0, 0, 0);
        glBegin(GL_TRIANGLES);
        glVertex2f(t[i].loc.x + 42, t[i].loc.y);
        glVertex2f(t[i].loc.x + 50, t[i].loc.y + 4);
        glVertex2f(t[i].loc.x + 50, t[i].loc.y - 4);

        glEnd();

        glColor3f(0, 0, 0);
        circle(t[i].loc.x + 100, t[i].loc.y, 8);

        glColor3f(0, 0, 0);
        glRectf(t[i].loc.x + 85, t[i].loc.y + 8, t[i].loc.x + 100, t[i].loc.y - 8);
    }

    glColor3f(1, 1, 0);
    glRectf(t[1].loc.x + 185, t[1].loc.y + 3, t[1].loc.x + 175, t[1].loc.y - 3);
    glColor3f(0, 0, 0);
    glRectf(t[1].loc.x + 182, t[1].loc.y + 1, t[1].loc.x + 178, t[1].loc.y - 1);

    glColor3f(1, 1, 1);
    glRectf(t[1].loc.x + 173, t[1].loc.y + 3, t[1].loc.x + 174, t[1].loc.y - 3);

    glColor3f(0, 1, 0);
    glRectf(t[1].loc.x + 173, t[1].loc.y + 3, t[1].loc.x + 171, t[1].loc.y - 3);

    glColor3f(1, 0.6, 0);
    circle(t[3].loc.x + 178, t[3].loc.y, 4);

    glColor3f(1 , 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(t[3].loc.x + 178, t[3].loc.y + 1);
    glVertex2f(t[3].loc.x + 182, t[3].loc.y -3);
    glVertex2f(t[3].loc.x + 180, t[3].loc.y - 5);
    glVertex2f(t[3].loc.x + 176, t[3].loc.y-1 );
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(t[3].loc.x + 178, t[3].loc.y - 1);
    glVertex2f(t[3].loc.x + 182, t[3].loc.y + 3);
    glVertex2f(t[3].loc.x + 180, t[3].loc.y + 5);
    glVertex2f(t[3].loc.x + 176, t[3].loc.y + 1);
    glEnd();

    glColor3f(1, 1, 1);
    circle(t[0].loc.x + 178, t[0].loc.y, 6);

    glColor3f(1, 153. / 255, 51. / 255);
    circle(t[0].loc.x + 178, t[0].loc.y -2, 4);

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(t[2].loc.x + 174, t[2].loc.y - 5);
    glVertex2f(t[2].loc.x + 174, t[2].loc.y -2);
    glVertex2f(t[2].loc.x + 182, t[2].loc.y);
    glVertex2f(t[2].loc.x + 184, t[2].loc.y - 1);
    glEnd();


    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(t[2].loc.x + 174, t[2].loc.y + 5);
    glVertex2f(t[2].loc.x + 174, t[2].loc.y + 2);
    glVertex2f(t[2].loc.x + 182, t[2].loc.y);
    glVertex2f(t[2].loc.x + 184, t[2].loc.y + 1);
    glEnd();

    glColor3f(0.7, 0.7, 0.7);
    circle(t[4].loc.x + 176, t[4].loc.y, 16);

    glColor3f(0.2, 0.2, 0.2);
    pie_filled(t[4].loc.x + 175, t[4].loc.y+1 , 14, 61, 179 );
    
    glColor3f(0.2, 0.2, 0.2);
    pie_filled(t[4].loc.x + 175, t[4].loc.y-1, 14, 181, 299);

    glColor3f(0.2, 0.2, 0.2);
    pie_filled(t[4].loc.x + 177, t[4].loc.y , 14, 301, 419 );
}

void bazooka(player_t p) {
    glColor3f(0, 0, 0.3);
    glRectf(p.pos.x - 18, -400, p.pos.x + 18, -392);

    glColor3f(0.7, 0.7, 0.7);
    glRectf(p.pos.x - 12, -380, p.pos.x + 12, -392);

    glColor3f(0.6, 0.6, 0.6);
    glRectf(p.pos.x - 13, -380, p.pos.x + 13, -375);

    glColor3f(0.7, 0.7, 0.7);
    glRectf(p.pos.x - 12, -363, p.pos.x + 12, -375);

    glColor3f(0.6, 0.6, 0.6);
    glRectf(p.pos.x - 13, -363, p.pos.x + 13, -358);

    glColor3f(0.7, 0.7, 0.7);
    glRectf(p.pos.x - 12, -358, p.pos.x + 12, -312);
    
    glColor3f(0.6, 0.6, 0.6);
    glRectf(p.pos.x - 13, -312, p.pos.x + 13, -307);

    glColor3f(0.7, 0.7, 0.7);
    glRectf(p.pos.x - 12, -307, p.pos.x + 12, -278);

    glColor3f(0, 0, 0.3);
    glRectf(p.pos.x - 18, -270, p.pos.x + 18, -278);

    glColor3f(0, 0, 0.3);
    glRectf(p.pos.x + 9, -283, p.pos.x + 15, -291);

    glColor3f(0, 0, 0.3);
    glRectf(p.pos.x + 19, -284, p.pos.x + 15, -290);

    glColor3f(0, 0, 0.3);
    glRectf(p.pos.x + 19, -282, p.pos.x + 36, -292);

    glColor3f(0, 0, 0.3);
    glRectf(p.pos.x + 6, -345, p.pos.x + 16, -325);

    glColor3f(0, 0, 0.2);
    glRectf(p.pos.x + 5, -323, p.pos.x + 17, -327);

    glColor3f(0, 0, 0.2);
    circle(p.pos.x + 11, -345, 6);

    glColor3f(0, 0, 0.3);
    glBegin(GL_QUADS);
    glVertex2f(p.pos.x - 18, -319);
    glVertex2f(p.pos.x - 10, -318);
    glVertex2f(p.pos.x - 10, -330);
    glVertex2f(p.pos.x -18, -326);
    glEnd();

    glColor3f(0.5, 0.5, 0.5);
    circle(p.pos.x - 12, -321, 1);
    
    glColor3f(0.5, 0.5, 0.5);
    circle(p.pos.x - 12, -326, 1);

    glColor3f(0, 0, 0.3);
    glRectf(p.pos.x - 18, -320, p.pos.x - 27, -325);

    glColor3f(0, 0, 0.4);
    glRectf(p.pos.x - 19, -321, p.pos.x - 26, -324);

    glColor3f(0, 0, 0.3);
    glBegin(GL_QUADS);
    glVertex2f(p.pos.x - 18, -335);
    glVertex2f(p.pos.x - 10, -333);
    glVertex2f(p.pos.x - 10, -355);
    glVertex2f(p.pos.x - 18, -353);
    glEnd();

    glColor3f(0.5, 0.5, 0.5);
    circle(p.pos.x - 12, -337, 1);

    glColor3f(0.5, 0.5, 0.5);
    circle(p.pos.x - 12, -351, 1);

    glColor3f(0, 0, 0.3);
    glRectf(p.pos.x - 18, -351, p.pos.x - 27, -345);

    glColor3f(0, 0, 0.4);
    glRectf(p.pos.x - 20, -350, p.pos.x - 25, -346);

    glColor3f(0, 0, 0.3);
    glBegin(GL_QUADS);
    glVertex2f(p.pos.x - 23, -345);
    glVertex2f(p.pos.x - 25, -345);
    glVertex2f(p.pos.x - 21, -340);
    glVertex2f(p.pos.x - 23, -340);
    glEnd();

    glColor3f(0, 0, 0.3);
    glRectf(p.pos.x - 18, -338, p.pos.x - 21, -340);

    glColor3f(0, 0, 0.3);
    glRectf(p.pos.x - 18, -343, p.pos.x - 21, -344);



}

void ammo() {

    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active) {
            glColor3f(0.3, 0.3, 0.3);
            glBegin(GL_QUADS);
            glVertex2f(fr[i].pos.x - 5, fr[i].pos.y);
            glVertex2f(fr[i].pos.x + 5, fr[i].pos.y);
            glVertex2f(fr[i].pos.x + 10, fr[i].pos.y - 20);
            glVertex2f(fr[i].pos.x - 10, fr[i].pos.y - 20);
            glEnd();

            glColor3f(0.4, 0.4, 0.4);
            glBegin(GL_QUADS);
            glVertex2f(fr[i].pos.x - 8, fr[i].pos.y - 20);
            glVertex2f(fr[i].pos.x + 8, fr[i].pos.y - 20);
            glVertex2f(fr[i].pos.x + 12, fr[i].pos.y - 25);
            glVertex2f(fr[i].pos.x - 12, fr[i].pos.y - 25);
            glEnd();

            glColor3f(0.2, 0.2, 0.2);
            glRectf(fr[i].pos.x - 12, fr[i].pos.y - 25, fr[i].pos.x + 12, fr[i].pos.y - 30);

            glColor3f(0.3, 0.3, 0.3);
            glBegin(GL_QUADS);
            glVertex2f(fr[i].pos.x - 12, fr[i].pos.y - 30);
            glVertex2f(fr[i].pos.x + 12, fr[i].pos.y - 30);
            glVertex2f(fr[i].pos.x + 7, fr[i].pos.y - 45);
            glVertex2f(fr[i].pos.x - 7, fr[i].pos.y - 45);
            glEnd();

            glColor3f(0.4, 0.4, 0.4);
            glBegin(GL_QUADS);
            glVertex2f(fr[i].pos.x - 7, fr[i].pos.y - 45);
            glVertex2f(fr[i].pos.x + 7, fr[i].pos.y - 45);
            glVertex2f(fr[i].pos.x + 10, fr[i].pos.y - 50);
            glVertex2f(fr[i].pos.x - 10, fr[i].pos.y - 50);
            glEnd();

            glColor3f(0.3, 0.3, 0.3);
            glRectf(fr[i].pos.x - 10, fr[i].pos.y - 50, fr[i].pos.x + 10, fr[i].pos.y - 55);

            glColor3f(0.3, 0.3, 0.3);
            glRectf(fr[i].pos.x - 5, fr[i].pos.y - 100, fr[i].pos.x + 5, fr[i].pos.y - 55);
        }
    }

    

}

void boom(int xcord, int ycord) {
    glColor3f(1, 0.5, 0);
    circle(xcord, ycord, circ * 1.5);
    glColor3f(1, 1, 0);
    circle(xcord, ycord, circ);
}

void scoreboard() {

    glColor3f(0, 0, 0);
    glRectf(-702, -20, -540, -210);

    glColor3ub(240, 240, 240);
    vprint(-615, -50, GLUT_BITMAP_TIMES_ROMAN_24, "TIME");

    glColor3ub(240, 240, 240);
    vprint(-690, -50, GLUT_BITMAP_TIMES_ROMAN_24, "LAP");

    glColor3ub(255, 255, 255);
    vprint2(-690, -80, 0.13, "%02d/24", lapCounter);

    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);

    glVertex2f(-618, -58);
    glVertex2f(-550, -58);

    glVertex2f(-693, -58);
    glVertex2f(-630, -58);

    glVertex2f(-623, -28);
    glVertex2f(-623, -90);

    glVertex2f(-703, -90);
    glVertex2f(-540, -90);

    glVertex2f(-665, -122);
    glVertex2f(-570, -122);

    glEnd();

    if (caset == 0) {
        glColor3ub(240, 240, 240);
        vprint(-610, -80, GLUT_BITMAP_TIMES_ROMAN_24, "20:00");
        glColor3ub(240, 240, 240);
        vprint(-630, -145, GLUT_BITMAP_TIMES_ROMAN_24, "00");

    }
    else {
        glColor3ub(255, 255, 255);
        vprint2(-610, -80, 0.15, "%2d:%02d", sec, timeCounter);

        glColor3ub(255, 255, 255);
        vprint2(-630, -145, 0.15, "%02d", hitCounter);
    }

    glColor3ub(240, 240, 240);
    vprint(-655, -115, GLUT_BITMAP_TIMES_ROMAN_24, "SCORE");

    if (!activeTimer && sec == 19 && timeCounter == 99)
    {
        glColor3ub(240, 240, 240);
        vprint(-690, -160, GLUT_BITMAP_HELVETICA_12, "Press F1 to start the game.");
    }
    else if (!activeTimer) {
        glColor3ub(240, 240, 240);
        vprint(-690, -160, GLUT_BITMAP_HELVETICA_12, "Press F1 to resume game.");
    } else
    {
        glColor3ub(240, 240, 240);
        vprint(-690, -160, GLUT_BITMAP_HELVETICA_12, "Press F1 to stop the game.");
    }

    glColor3ub(240, 240, 240);
    vprint(-690, -180, GLUT_BITMAP_HELVETICA_12, "Press Space to fire.");

    glColor3ub(240, 240, 240);
    vprint(-690, -200, GLUT_BITMAP_HELVETICA_12, "Press h to hide me.");
   
}

void displayWinner() {

    glColor3f(0, 0, 0);
    glRectf(-102, -92, 102, 102);

    glColor3f(0.3, 0.3, 0.3);
    glRectf(-100, -90, 100, 100);

    glColor3f(0, 0, 0);
    glRectf(-98, -88, 98, 98);

    glColor3ub(240, 240, 240);
    vprint(-65, 50, GLUT_BITMAP_TIMES_ROMAN_24, "The Winner");

    glColor3f(0.7, 0.7, 0.7);
    circle(0, 0, 30);

    glColor3f(0.2, 0.2, 0.2);
    pie_filled(-1, 1, 28, 90, 210);

    glColor3f(0.2, 0.2, 0.2);
    pie_filled(-1, - 1, 28, 210, 330);

    glColor3f(0.2, 0.2, 0.2);
    pie_filled(1, 0, 28, 330, 450);
}

int findAvailableFire() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active == false) return i;
    }
    return -1;
}

void display() {
   
    glClearColor(76./255, 153./255, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    grass();
    road();
    car(target);
    ammo();
    bazooka(p);
    
    if (carhit)
        boom(hitx + 100, hity);

    if(!hide)
        scoreboard();

    if (win)
    {
        displayWinner();
    }
    


    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//

void resetTarget(target_t target[], int i) {
    float yPos;
    if (i == 4) {
        yPos = rand() % 200;
        target[i].loc = { -900, yPos + 150 };
        target[i].width = TARGET_WIDTH;
        target[i].speed = merS;
    }
    else {
        do
        {
           yPos = rand() % 350;

        } while (yPos < target[0].loc.y + 10 && yPos > target[0].loc.y - 10 || yPos < target[1].loc.y + 10 && yPos > target[1].loc.y - 10 || yPos < target[2].loc.y + 10 && yPos > target[2].loc.y - 10 || yPos < target[3].loc.y + 10 && yPos > target[3].loc.y - 10 || yPos < target[4].loc.y + 45 && yPos > target[4].loc.y - 45);
        target[i].loc = { -900, yPos };
        target[i].width = TARGET_WIDTH;
        target[i].speed = 7;
    }

}

void resetTarget2(target_t target[], int i, float temp) {
    float yPos;
    if (i == 4) {
        yPos = rand() % 200;
        target[i].loc = { -900 - temp, yPos + 150 };
        target[i].width = TARGET_WIDTH;
        target[i].speed = merS;
    }
    else {
        yPos = rand() % 350;
        target[i].loc = { -900 - temp, yPos };
        target[i].width = TARGET_WIDTH;
        target[i].speed = 7;
    }
}

void colorCar(target_t target[]) {
    target[0].color.r = 255;
    target[0].color.g = 153;
    target[0].color.b = 51;

    target[1].color.r = 200;
    target[1].color.g = 0;
    target[1].color.b = 0;

    target[2].color.r = 0;
    target[2].color.g = 0;
    target[2].color.b = 200;

    target[3].color.r = 230;
    target[3].color.g = 230;
    target[3].color.b = 0;

    target[4].color.r = 60;
    target[4].color.g = 60;
    target[4].color.b = 60;
}

bool testCollisionX(fire_t fr, target_t t) {
    float dx = t.loc.x + 96 - fr.pos.x;
    float d = sqrt(dx * dx);
    return d <= t.width / 2;
}

bool testCollisionY(fire_t fr, target_t t) {
    float dy = t.loc.y - fr.pos.y;
    float d = sqrt(dy * dy);
    return d <= 30;
}


void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ')
        spacebar = true;

    if (key == 'h') {
        if(!hide)
            hide = true;
        else
        {
            hide = false;
        }
    }

   



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ') {
        spacebar = false;
         
    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}


void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    }

    if (key == GLUT_KEY_LEFT) {
        p.pos.x -=20;

        if (p.pos.x < -750)
            p.pos.x = 750;
    }

    if (key == GLUT_KEY_RIGHT) {
        p.pos.x +=20;
        if (p.pos.x > 750)
            p.pos.x = -750;
    }

    if (key == GLUT_KEY_F1) {
        if (win)
        {
            win = false;
        }
        if (!activeTimer)
            activeTimer = true;
        else
            activeTimer = false;
    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}


void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}


void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}


void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}


void onMove(int x, int y) {
    // Write your codes here.
    mouse.x = x - winWidth / 2;
    mouse.y = winHeight / 2 - y;


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
   
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    
    if (activeTimer)
    {
        caset = 1;
        timeCounter--;
        if (timeCounter == 0)
        {
            sec--;
            timeCounter = 99;
        }
        if (sec == -1) {

            sec = 19;

            activeTimer = !activeTimer;
            caset = 0;
            float temp = 0;
            for (int i = 0; i < 5; i++) {
                resetTarget2(target, i, temp);
                temp += 300;
            }
            merS = 9;
            win = true;
        }



        for (int i = 0; i < 5; i++) {
            target[i].loc.x += target[i].speed;
            if (target[i].loc.x > 800) {
                resetTarget(target, i);
            }
            
        }

        if (target[4].loc.x > 790) {
            lapCounter++;
        }

        if (spacebar && fire_rate == 0) {
            int availFire = findAvailableFire();
            if (availFire != -1) {
                fr[availFire].pos = p.pos;
                fr[availFire].active = true;
                fire_rate = FIRE_RATE;
            }
        }

        if (fire_rate > 0) fire_rate--;

        // Move all fires that are active.
        for (int i = 0; i < MAX_FIRE; i++) {
            if (fr[i].active) {
                fr[i].pos.y += 7;

                if (fr[i].pos.y > 500) {
                    fr[i].active = false;
                    spacebar = false;
                }

                for (int c = 0; c < 5; c++) {
                    if (testCollisionX(fr[i], target[c]) && testCollisionY(fr[i], target[c])) {
                        hitx = target[c].loc.x;
                        hity = target[c].loc.y;
                        resetTarget(target, c);
                        carhit = true;
                        fr[i].active = false;
                        spacebar = false;
                        hitCounter++;
                        if (c == 4) {
                            hitCounter+=2;
                            merS++;
                        }
                    }
                }

            }

        }
        if (carhit) {
            if (circ <= 50)
                circ += 1;
            else
                carhit = false;
              

        }
        else {
            circ = 20;
        }
    }

    if (!activeTimer) {
        hitCounter = 0;
        lapCounter = 0;
     }
   


    
    glutPostRedisplay(); 

}
#endif

void Init() {
    float temp = 0;
    for (int i = 0; i < 5; i++) {
        resetTarget2(target, i, temp);
        temp += 300;
    }

    colorCar(target);

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Bazooka on F1 Competition by Ali Eray");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}