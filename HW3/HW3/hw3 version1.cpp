/*********
   CTIS164 - Template Source Program
----------
STUDENT : Kamila Aliyeva
SECTION : 002
HOMEWORK: 3
----------
PROBLEMS: something wrong with the angle during mouseOutside
----------
ADDITIONAL FEATURES: Cool visuals. I spend more time on a design of this frog than the rest of the homework together.
                     There are flies which frog is supporsed to catch/eat
                     When he catches all of the flies he get big enough to leave his box
                     But the game still works after frog grows, it is just paused, press the spacebar and it will resume again
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "vec.h"


#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

float px = 4.0;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

bool mouseInside = true;
bool pause = false;
bool fliesCreated = false;
bool win = false;
int score  = 0;

int t;

//this part is messy
typedef struct {
    float ang;
    vec_t speed;
    vec_t pos; //pos
}char_t;

typedef struct {
    vec_t pos;
    float angle;
}cursor_t;

cursor_t c{ {-100,200 }, 0 };
cursor_t flies[8];
char_t fr{ 0, { 0, 0 }, {0, 0} };

//
// to draw circle, center at (x,y)
// radius r
//

void vertex(vec_t P, vec_t Tr, double angle) {
    double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2d(xp, yp);
}

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

//Original version of this function was taken from the internet




void drawFroggo() {

    //body
    glColor3ub(149, 207, 154);
    glBegin(GL_QUADS);
    vertex({ -6 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ -2 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ -2 * px, 4 * px }, fr.pos, fr.ang);
    vertex({ -6 * px, 4 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -5 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ -1 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ -1 * px, 3 * px }, fr.pos, fr.ang);
    vertex({ -5 * px, 3 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -4 * px, 1 * px }, fr.pos, fr.ang);
    vertex({ px, 1 * px }, fr.pos, fr.ang);
    vertex({ px, 6 * px }, fr.pos, fr.ang);
    vertex({ -4 * px, 6 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -3 * px, 4 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, 4 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, 0 * px }, fr.pos, fr.ang);
    vertex({ -3 * px, 0 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -2 * px, 3 * px }, fr.pos, fr.ang);
    vertex({ 4 * px, 3 * px }, fr.pos, fr.ang);
    vertex({ 4 * px, -2 * px }, fr.pos, fr.ang);
    vertex({ -2 * px, -2 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -9 * px, 1 * px }, fr.pos, fr.ang);
    vertex({ -4 * px, 1 * px }, fr.pos, fr.ang);
    vertex({ -4 * px, 2 * px }, fr.pos, fr.ang);
    vertex({ -9 * px, 2 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -8 * px, 3 * px }, fr.pos, fr.ang);
    vertex({ -7 * px, 3 * px }, fr.pos, fr.ang);
    vertex({ -7 * px, 0 * px }, fr.pos, fr.ang);
    vertex({ -8 * px, 0 * px }, fr.pos, fr.ang);
    glEnd();

    //hands
    glBegin(GL_QUADS);
    vertex({ 0 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ 3 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ 3 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ 0 * px, 7 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 1 * px, 9 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, 9 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, 5 * px }, fr.pos, fr.ang);
    vertex({ 1 * px, 5 * px }, fr.pos, fr.ang);
    glEnd();

    //legs
    int h = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 3; j > i; j--)
        {
            glBegin(GL_QUADS);
            vertex({ -j * px, (-2 + h) * px }, fr.pos, fr.ang);
            vertex({ (-j + 1) * px, (-2 + h) * px }, fr.pos, fr.ang);
            vertex({ (-j + 1) * px,(-3 + h) * px }, fr.pos, fr.ang);
            vertex({ -j * px, (-3 + h) * px }, fr.pos, fr.ang);
            glEnd();
        }
        h--;
    }
    glBegin(GL_QUADS);
    vertex({ -4 * px, -4 * px }, fr.pos, fr.ang);
    vertex({ -3 * px, -4 * px }, fr.pos, fr.ang);
    vertex({ -3 * px, -7 * px }, fr.pos, fr.ang);
    vertex({ -4 * px, -7 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 2 * px, -6 * px }, fr.pos, fr.ang);
    vertex({ -3 * px, -6 * px }, fr.pos, fr.ang);
    vertex({ -3 * px, -7 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, -7 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 1 * px, -8 * px }, fr.pos, fr.ang);
    vertex({ 3 * px, -8 * px }, fr.pos, fr.ang);
    vertex({ 3 * px, -7 * px }, fr.pos, fr.ang);
    vertex({ 1 * px, -7 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -2 * px, -8 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, -8 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, -9 * px }, fr.pos, fr.ang);
    vertex({ -2 * px, -9 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -1 * px, -10 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, -10 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, -9 * px }, fr.pos, fr.ang);
    vertex({ -1 * px, -9 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 3 * px, 6 * px }, fr.pos, fr.ang);
    vertex({ 6 * px, 6 * px }, fr.pos, fr.ang);
    vertex({ 6 * px, 3 * px }, fr.pos, fr.ang);
    vertex({ 3 * px, 3 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 7 * px, 5 * px }, fr.pos, fr.ang);
    vertex({ 6 * px, 5 * px }, fr.pos, fr.ang);
    vertex({ 6 * px, 2 * px }, fr.pos, fr.ang);
    vertex({ 7 * px, 2 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 7 * px, 4 * px }, fr.pos, fr.ang);
    vertex({ 8 * px, 4 * px }, fr.pos, fr.ang);
    vertex({ 8 * px, 2 * px }, fr.pos, fr.ang);
    vertex({ 7 * px, 2 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 9 * px, 3 * px }, fr.pos, fr.ang);
    vertex({ 8 * px, 3 * px }, fr.pos, fr.ang);
    vertex({ 8 * px, 1 * px }, fr.pos, fr.ang);
    vertex({ 9 * px, 1 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 9 * px, 2 * px }, fr.pos, fr.ang);
    vertex({ 11 * px, 2 * px }, fr.pos, fr.ang);
    vertex({ 11 * px, 1 * px }, fr.pos, fr.ang);
    vertex({ 9 * px, 1 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 12 * px, 2 * px }, fr.pos, fr.ang);
    vertex({ 10 * px, 2 * px }, fr.pos, fr.ang);
    vertex({ 10 * px, 5 * px }, fr.pos, fr.ang);
    vertex({ 12 * px, 5 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 9 * px, 4 * px }, fr.pos, fr.ang);
    vertex({ 14 * px, 4 * px }, fr.pos, fr.ang);
    vertex({ 14 * px, 5 * px }, fr.pos, fr.ang);
    vertex({ 9 * px, 5 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 9 * px, 6 * px }, fr.pos, fr.ang);
    vertex({ 10 * px, 6 * px }, fr.pos, fr.ang);
    vertex({ 10 * px, 5 * px }, fr.pos, fr.ang);
    vertex({ 9 * px, 5 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 9 * px, 6 * px }, fr.pos, fr.ang);
    vertex({ 8 * px, 6 * px }, fr.pos, fr.ang);
    vertex({ 8 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ 9 * px, 7 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 11 * px, 5 * px }, fr.pos, fr.ang);
    vertex({ 12 * px, 5 * px }, fr.pos, fr.ang);
    vertex({ 12 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ 11 * px, 7 * px }, fr.pos, fr.ang);
    glEnd();

    //details
    glColor3ub(73, 112, 76);
    glBegin(GL_QUADS);
    vertex({ -10 * px, 1 * px }, fr.pos, fr.ang);
    vertex({ -9 * px, 1 * px }, fr.pos, fr.ang);
    vertex({ -9 * px, 2 * px }, fr.pos, fr.ang);
    vertex({ -10 * px, 2 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -8 * px, 3 * px }, fr.pos, fr.ang);
    vertex({ -7 * px, 3 * px }, fr.pos, fr.ang);
    vertex({ -7 * px, 4 * px }, fr.pos, fr.ang);
    vertex({ -8 * px, 4 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -8 * px, 0 * px }, fr.pos, fr.ang);
    vertex({ -7 * px, 0 * px }, fr.pos, fr.ang);
    vertex({ -7 * px, -1 * px }, fr.pos, fr.ang);
    vertex({ -8 * px, -1 * px }, fr.pos, fr.ang);
    glEnd();
    for (int i = 0; i < 8; i++) {
        glBegin(GL_QUADS);
        vertex({ (i - 4) * px, (6 - i) * px }, fr.pos, fr.ang);
        vertex({ (i - 3) * px, (6 - i) * px }, fr.pos, fr.ang);
        vertex({ (i - 3) * px, (5 - i) * px }, fr.pos, fr.ang);
        vertex({ (i - 4) * px, (5 - i) * px }, fr.pos, fr.ang);
        glEnd();
    }
    for (int i = 0; i < 4; i++) {
        glBegin(GL_QUADS);
        vertex({ 5 * px, (3 - i) * px }, fr.pos, fr.ang);
        vertex({ 4 * px, (3 - i) * px }, fr.pos, fr.ang);
        vertex({ 4 * px, (4 - i) * px }, fr.pos, fr.ang);
        vertex({ 5 * px, (4 - i) * px }, fr.pos, fr.ang);
        glEnd();
        glBegin(GL_QUADS);
        vertex({ -i * px, (-2 - i) * px }, fr.pos, fr.ang);
        vertex({ (-i + 1) * px, (-2 - i) * px }, fr.pos, fr.ang);
        vertex({ (-i + 1) * px, (-3 - i) * px }, fr.pos, fr.ang);
        vertex({ -i * px, (-3 - i) * px }, fr.pos, fr.ang);
        glEnd();
    }
    glBegin(GL_QUADS);
    vertex({ -1 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ 0 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ 0 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ -1 * px, 7 * px }, fr.pos, fr.ang);
    glEnd(); glBegin(GL_QUADS);
    vertex({ 3 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ 4 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ 4 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ 3 * px, 7 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 1 * px, 10 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, 10 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, 9 * px }, fr.pos, fr.ang);
    vertex({ 1 * px, 9 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 8 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ 9 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ 9 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ 8 * px, 7 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 11 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ 12 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ 12 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ 11 * px, 7 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 13 * px, 6 * px }, fr.pos, fr.ang);
    vertex({ 14 * px, 6 * px }, fr.pos, fr.ang);
    vertex({ 14 * px, 5 * px }, fr.pos, fr.ang);
    vertex({ 13 * px, 5 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 13 * px, 1 * px }, fr.pos, fr.ang);
    vertex({ 12 * px, 1 * px }, fr.pos, fr.ang);
    vertex({ 12 * px, 2 * px }, fr.pos, fr.ang);
    vertex({ 13 * px, 2 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -3 * px, -10 * px }, fr.pos, fr.ang);
    vertex({ -2 * px, -10 * px }, fr.pos, fr.ang);
    vertex({ -2 * px, -9 * px }, fr.pos, fr.ang);
    vertex({ -3 * px, -9 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -1 * px, -10 * px }, fr.pos, fr.ang);
    vertex({ 0 * px, -10 * px }, fr.pos, fr.ang);
    vertex({ 0 * px, -11 * px }, fr.pos, fr.ang);
    vertex({ -1 * px, -11 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 1 * px, -10 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, -10 * px }, fr.pos, fr.ang);
    vertex({ 2 * px, -11 * px }, fr.pos, fr.ang);
    vertex({ 1 * px, -11 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 3 * px, -8 * px }, fr.pos, fr.ang);
    vertex({ 4 * px, -8 * px }, fr.pos, fr.ang);
    vertex({ 4 * px, -9 * px }, fr.pos, fr.ang);
    vertex({ 3 * px, -9 * px }, fr.pos, fr.ang);
    glEnd();
    //eyes
    glColor3ub(160, 37, 37);
    for (int i = 0; i < 2; i++)
    {
        glBegin(GL_QUADS);
        vertex({ ( - 6  + i)* px, ( 6 - i) * px}, fr.pos, fr.ang);
        vertex({ ( - 5 + i) * px, (6 - i) * px}, fr.pos, fr.ang);
        vertex({ ( - 5 + i) * px, (5 - i) * px}, fr.pos, fr.ang);
        vertex({( - 6 + i) * px, ( 5 - i) * px}, fr.pos, fr.ang);
        glEnd();
        glBegin(GL_QUADS);
        vertex({ (-4 + i) * px, (8 - i) * px }, fr.pos, fr.ang);
        vertex({ (-3 + i) * px, (8 - i) * px }, fr.pos, fr.ang);
        vertex({ (-3 + i) * px, (7 - i) * px }, fr.pos, fr.ang);
        vertex({ (-4 + i) * px, (7 - i) * px }, fr.pos, fr.ang);
        glEnd();
    }
    glColor3ub(77, 2, 2);
    glBegin(GL_QUADS);
    vertex({ -6  * px, 4 * px }, fr.pos, fr.ang);
    vertex({ -5 * px, 4  * px }, fr.pos, fr.ang);
    vertex({ -5 * px, 5 * px }, fr.pos, fr.ang);
    vertex({ -6 * px, 5 * px }, fr.pos, fr.ang);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -3 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ -2 * px, 7 * px }, fr.pos, fr.ang);
    vertex({ -2 * px, 8 * px }, fr.pos, fr.ang);
    vertex({ -3 * px, 8 * px }, fr.pos, fr.ang);
    glEnd();


}

void drawFlies (cursor_t c1)
{
    glColor3ub(37, 8, 59);
    glBegin(GL_QUADS);
    vertex({ 2.5 * px, 1.5 * px },c1.pos, c1.angle);
    vertex({ -2 * px, 1.5 * px }, c1.pos, c1.angle);
    vertex({ -2 * px, -1 * px }, c1.pos, c1.angle);
    vertex({ 2.5 * px, -1 * px }, c1.pos, c1.angle);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -1 * px, -2 * px }, c1.pos, c1.angle);
    vertex({ -2 * px, -2 * px }, c1.pos, c1.angle);
    vertex({ -2 * px, -1 * px }, c1.pos, c1.angle);
    vertex({ -1 * px, -1 * px }, c1.pos, c1.angle);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 0.5 * px, -2 * px }, c1.pos, c1.angle);
    vertex({ 1.5 * px, -2 * px }, c1.pos, c1.angle);
    vertex({ 1.5 * px, -1 * px }, c1.pos, c1.angle);
    vertex({ 0.5 * px, -1 * px }, c1.pos, c1.angle);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -3 * px, 0.5 * px }, c1.pos, c1.angle);
    vertex({ -1 * px, 0.5 * px }, c1.pos, c1.angle);
    vertex({ -1 * px, 3 * px }, c1.pos, c1.angle);
    vertex({ -3 * px, 3 * px }, c1.pos, c1.angle);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -4 * px, 1.5 * px }, c1.pos, c1.angle);
    vertex({ -2 * px, 1.5 * px }, c1.pos, c1.angle);
    vertex({ -2 * px, 4 * px }, c1.pos, c1.angle);
    vertex({ -4 * px, 4 * px }, c1.pos, c1.angle);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 2 * px, 1 * px }, c1.pos, c1.angle);
    vertex({ 4 * px, 1 * px }, c1.pos, c1.angle);
    vertex({ 4 * px, 3 * px }, c1.pos, c1.angle);
    vertex({ 2 * px, 3 * px }, c1.pos, c1.angle);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 5 * px, 2 * px }, c1.pos, c1.angle);
    vertex({ 3 * px, 2 * px }, c1.pos, c1.angle);
    vertex({ 3 * px, 4 * px }, c1.pos, c1.angle);
    vertex({ 5 * px, 4 * px }, c1.pos, c1.angle);
    glEnd();
    //details
    glColor3ub(118, 83, 122);
    glBegin(GL_QUADS);
    vertex({ -2 * px, 2 * px }, c1.pos, c1.angle);
    vertex({ -3 * px, 2 * px }, c1.pos, c1.angle);
    vertex({ -3 * px, 0.5 * px }, c1.pos, c1.angle);
    vertex({ -2 * px, 0.5 * px }, c1.pos, c1.angle);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ -4 * px, 3 * px }, c1.pos, c1.angle);
    vertex({ -3 * px, 3 * px }, c1.pos, c1.angle);
    vertex({ -3 * px, 1.5 * px }, c1.pos, c1.angle);
    vertex({ -4 * px, 1.5 * px }, c1.pos, c1.angle);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 2.5 * px, 2 * px }, c1.pos, c1.angle);
    vertex({ 4 * px, 2 * px }, c1.pos, c1.angle);
    vertex({ 4 * px, 1 * px }, c1.pos, c1.angle);
    vertex({ 2.5 * px, 1 * px }, c1.pos, c1.angle);
    glEnd();
    glBegin(GL_QUADS);
    vertex({5 * px, 2 * px }, c1.pos, c1.angle);
    vertex({ 4 * px, 2 * px }, c1.pos, c1.angle);
    vertex({ 4 * px, 3 * px }, c1.pos, c1.angle);
    vertex({ 5 * px, 3 * px }, c1.pos, c1.angle);
    glEnd();
    //eyes
    glColor3ub(210, 79, 227);
    glBegin(GL_QUADS);
    vertex({ 0 * px, 0 * px }, c1.pos, c1.angle);
    vertex({ -1 * px, 0 * px }, c1.pos, c1.angle);
    vertex({ -1 * px, 1 * px }, c1.pos, c1.angle);
    vertex({ 0 * px, 1* px }, c1.pos, c1.angle);
    glEnd();
    glBegin(GL_QUADS);
    vertex({ 2.5 * px, 0 * px }, c1.pos, c1.angle);
    vertex({ 1.5 * px, 0 * px }, c1.pos, c1.angle);
    vertex({ 1.5 * px, 1 * px }, c1.pos, c1.angle);
    vertex({ 2.5 * px, 1 * px }, c1.pos, c1.angle);
    glEnd();
}

void drawBox()
{
    glColor3ub(69, 37, 1);
    glBegin(GL_QUADS);
    glVertex2f(-420, -260);
    glVertex2f(-400, -260);
    glVertex2f(-400, 260);
    glVertex2f(-420, 260);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(420, -260);
    glVertex2f(400, -260);
    glVertex2f(400, 260);
    glVertex2f(420, 260);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-400, -270);
    glVertex2f(400, -270);
    glVertex2f(400, -250);
    glVertex2f(-400, -250);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-400, 270);
    glVertex2f(400, 270);
    glVertex2f(400, 250);
    glVertex2f(-400, 250);
    glEnd();
}

void createFlies() {
    for (int i = 0; i < 8; i++)
    {
        flies[i].pos.x = rand() % 780 - 390;
        flies[i].pos.y = rand() % 480 - 240;
        flies[i].angle = 0;
    }
}

void drawSand() {
    glColor3ub(242, 235, 196);
    glBegin(GL_QUADS);
    glVertex2f(-400, -250);
    glVertex2f(400, -250);
    glVertex2f(400, 250);
    glVertex2f(-400, 250);
    glEnd();
}
//
// To display onto window using OpenGL commands
//
void display() {

    //
    // clear window to black
    //
    glClearColor(0.7, 0.88, 0.88, 0);
    glClear(GL_COLOR_BUFFER_BIT);
   
    //Background Stuff
    drawSand();
    drawBox();

    //labels
    glColor3f(0,0,0);
    vprint(-400, 300, GLUT_BITMAP_8_BY_13, "Kamila Aliyeva");
    vprint( 0, 300, GLUT_BITMAP_8_BY_13, "Score: %d", score);
    vprint( 270, 300, GLUT_BITMAP_8_BY_13, "Mouse is ");
    if(mouseInside)
        vprint(342, 300, GLUT_BITMAP_8_BY_13, "inside");
    else
        vprint(342, 300, GLUT_BITMAP_8_BY_13, "outside");
    if (!win) {

        vprint(-140, -300, GLUT_BITMAP_8_BY_13, "Press spacebar to ");
        if (pause)
            vprint(7, -300, GLUT_BITMAP_8_BY_13, "restart");
        else
            vprint(7, -300, GLUT_BITMAP_8_BY_13, "pause");

        double a = fr.ang / D2R;
        a += 40;
        a = (a < 0) ? a + 360 : a;
        vprint(200, -300, GLUT_BITMAP_8_BY_13, "Angle: %.0f", a);
    }
    else {
        glColor3f(0.8, 0, 0.8);
        vprint(-200, -300, GLUT_BITMAP_8_BY_13, "Congratulations you're too big for this box. Be free Froggo");
    }
        
   

    //characters
    if (mouseInside)
    {
        glColor3f(0.4, 0.4, 0.4);
        circle(c.pos.x, c.pos.y, 5);
    }

    //Flies
    if (!fliesCreated)
    {
        createFlies();
        fliesCreated = true;
    }
      
    for (int i = 0; i < 8; i++)
    {
        drawFlies(flies[i]);
    }

    
       
    drawFroggo();


    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
   
    if (key == 32 && pause == false)
    {
        pause = true;
        t = 0;
    }

    if (key == 32 && pause == true && t > 10)
        pause = false;
  


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
   
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
 


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
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

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.
    x = x - winWidth / 2;
    y = winHeight / 2 - y;
    if (!pause)
    {
        c.pos.x = x;
        c.pos.y = y;
    }
   
   

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
 
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (!pause) {
        if (mouseInside)
        {
            fr.speed = mulV(5, unitV(subV(c.pos, fr.pos)));

            if (fabs(fr.pos.x - c.pos.x) > 5 && fabs(fr.pos.y - c.pos.y) > 5)
            {
                fr.ang = atan2(fr.pos.y - c.pos.y, fr.pos.x - c.pos.x) + 0.78;
                fr.pos = addV(fr.pos, fr.speed);
            }
           
        }
        else {
            if (fr.pos.x > 390 || fr.pos.x < -390)
            {
                fr.speed.x = -fr.speed.x;
                //something wrong here
                //fr.ang = atan2(fr.pos.y - fr.speed.y, fr.pos.x - fr.speed.x) + PI / 2;
                fr.ang *= -1;
            }

            if (fr.pos.y > 240 || fr.pos.y < -240)
            {
                fr.speed.y = -fr.speed.y;
                fr.ang = atan2(fr.pos.y - fr.speed.y, fr.pos.x - fr.speed.x) + PI / 2;
            }
            fr.pos = addV(fr.pos, fr.speed);

        }

        if (c.pos.x > 390 || c.pos.x < -390 || c.pos.y > 240 || c.pos.y < -240)
            mouseInside = false;
        else
            mouseInside = true;

        for (int i = 0; i < 8; i++)
        {
            if ( fliesCreated && fabs(fr.pos.x - flies[i].pos.x) <= 15 && fabs(fr.pos.y - flies[i].pos.y) <= 15)
            {
               flies[i].pos = { -4000, -4000 };
               score += 100;
            }
        }

        if (score == 800 && !win)
        {
            px *= 5;
            win = true;
            pause = true;
        }
    }
    t++;
    


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Frog is hungry | Collect all the flies");

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

    srand(time(NULL));

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}