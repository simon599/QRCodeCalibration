#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "qrcodegen.hpp"
#include "qrcodegen.cpp"
#include <chrono>
#include <thread>
#include <GL/glut.h>
#include <stdio.h>

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

bool A[21][21]={{false}};

static void printQr(const QrCode &qr);
static void generateQR();
long timeSinceEpochMillisec();
void display();
void drawBlock(int i, int j);
void drawQR();

int main(int argc, char **argv) {
	generateQR();
    glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(460, 460);
	glutCreateWindow("Draw Rectangle");
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(display);
	glutMainLoop();
}

static void generateQR() {
	//const char *text = "Hello, world!";
    std::ostringstream oss;
    oss << timeSinceEpochMillisec();
    const char *text = oss.str().c_str();
    //std::cout<<text;
	const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level
	
	// Make and print the QR Code symbol
	const QrCode qr = QrCode::encodeText(text, errCorLvl);
    printQr(qr);
}

void drawBlock(int i, int j)
{
	int xmin, ymin, xmax, ymax;
	xmin = (20 + (i * 20));
	ymin = (20 + (j * 20));
	xmax = xmin + 20;
	ymax = ymin + 20;
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2);
	glBegin(GL_POLYGON);
	glVertex2f(xmin, ymin);
	glVertex2f(xmin, ymax);
	glVertex2f(xmax, ymax);
	glVertex2f(xmax, ymin);
	glEnd();
	glFlush();
}

void drawQR()
{
	int i, j;
	for (i = 0; i < 21; i++)
	{
		for (j = 0; j < 21; j++)
		{
			if (A[i][j])
			{
				drawBlock(i,j);
			}
		}	
	}
}

void display()
{
    int frame=0, timebase=0;
    for (int i = 0; i < 1000; i++){
        glClear(GL_COLOR_BUFFER_BIT);
	    glLoadIdentity();
	    gluOrtho2D(0, 460, 0, 460);
        generateQR();
	    drawQR();
        int time = glutGet(GLUT_ELAPSED_TIME);
        frame++;
        time = glutGet(GLUT_ELAPSED_TIME);
        if (time - timebase > 1000) {
            float fps = frame*1000.0/(time-timebase);
            timebase = time;
            frame = 0;
            std::cout<<fps<<std::endl;
        }
        
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
	glFlush();
}

long timeSinceEpochMillisec(){
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

static void printQr(const QrCode &qr) {
	for (int y = 0; y < qr.getSize(); y++) {
		for (int x = 0; x < qr.getSize(); x++) {
			if (qr.getModule(x,y)){A[x][y] = true;}
            else {A[x][y] = false;}
		}
	}
}