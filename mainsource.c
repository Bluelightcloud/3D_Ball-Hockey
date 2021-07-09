
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include "myShape.h"
#include "read_bitmap.h"
#include <mmsystem.h>
#include <windows.h>
#pragma comment(lib, "WinMM.lib") 
#define KEY_ESC 27
#define imageWidth 8
#define imageHeight 8

int ReadBitMapData();
void polarview(void);
void resetview(void);
unsigned char wireFlag = GL_TRUE;
int xBegin = 0, yBegin = 0;
int mButton;
float distance, twist, elevation, azimuth;
float nearw = 1.0, farw = 30.0, fovy = 60.0;
float playerz, povx, povy; //�v���[���[�̉��ʒu�ƈ�l�̎��_
float cpuz; //CPU�̉��ʒu
float xpla, ypla, zpla; //���̍��W
float xspeed, zspeed; //���̃X�s�[�h
float kasoku; //�����x�{���ۑ�
int myp, cpup;
unsigned char prespeedz = GL_TRUE; //�������Ă��邩�ǂ���
unsigned char dkey = GL_TRUE, akey = GL_TRUE, pov = GL_TRUE;
unsigned char mode = GL_TRUE; //�ł������邩���ʂ�
float obs; //��Q��
float textviewx, textviewy; //�e�L�X�g���W
unsigned char farewell = GL_TRUE; //�����̂�
float distancepls;
float cycle;

void initTexture(void);

void writestr(float x, float y, const char* str) { //�����̕`��
	glRasterPos2f(x, y);

	const char* c = str;
	while (*c) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c++);
	}
}

void gameset(void) { //���_�l���ōēx
	xpla = 0;
	ypla = 0;
	zpla = 3.3;
	xspeed = 0.05;
	zspeed = 0.04;
	playerz = 3.3;
	cpuz = 3.3;
	if (farewell == GL_TRUE) { //����Ȃ炵�Ă���Ƃ܂�Ȃ��悤��
		wireFlag = GL_TRUE;
	}
	dkey = GL_TRUE;
	akey = GL_TRUE;
	prespeedz = GL_TRUE;
	kasoku = 1.0;
	glutPostRedisplay();
}

float parabola(float x) { //�{��̃Q�[�������̂ċO��
	float ans;
	ans = -1 * (x - 12) * (x - 12) + 4;
	return ans;
}

void display(void)
{
	float diffuse[] = { 0.1, 0.4, 0.1, 1.0 };
	float specular[] = { 0.5, 0.6, 0.5, 1.0 };
	float ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	char text[256];

	if (pov != GL_TRUE) {
		povx = -playerz;
		distance = 5; //��l�̎��_
		twist = 0.0;
		elevation = 0.0;
		azimuth = -90.0;
		textviewx = 0.0;
		textviewy = 1.7;
	}
	else {
		povx = -3.3;
		distance = 10 + distancepls; //�������_
		twist = 0.0;
		elevation = -20.0 + cycle;
		azimuth = -90.0;
		textviewx = -4.2;
		textviewy = 2.6;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	polarview();

	glEnable(GL_DEPTH_TEST);
	glColor3d(1, 1, 1);

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 4);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-5.0, -0.5, 0.5);
	glTexCoord2f(0, 1); glVertex3f(-5.0, 0.5, 0.5);
	glTexCoord2f(1, 1); glVertex3f(5.0, 0.5, 0.5);
	glTexCoord2f(1, 0); glVertex3f(5.0, -0.5, 0.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 4);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(6.0, -0.5, 0.5);
	glTexCoord2f(1, 0); glVertex3f(6.0, -0.5, 6.0);
	glTexCoord2f(1, 1); glVertex3f(6.0, 0.5, 6.0);
	glTexCoord2f(0, 1); glVertex3f(6.0, 0.5, 0.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 4);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(-5.0, -0.5, 6.0);
	glTexCoord2f(1, 1); glVertex3f(-5.0, 0.5, 6.0);
	glTexCoord2f(0, 1); glVertex3f(5.0, 0.5, 6.0);
	glTexCoord2f(0, 0); glVertex3f(5.0, -0.5, 6.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 4);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-6.0, -0.5, 0.5);
	glTexCoord2f(1, 0); glVertex3f(-6.0, -0.5, 6.0);
	glTexCoord2f(1, 1); glVertex3f(-6.0, 0.5, 6.0);
	glTexCoord2f(0, 1); glVertex3f(-6.0, 0.5, 0.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D); //�n��
	glBindTexture(GL_TEXTURE_2D, 3);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(6.0, -0.5, 6.0);
	glTexCoord2f(0, 0); glVertex3f(-6.0, -0.5, 6.0);
	glTexCoord2f(1, 0); glVertex3f(-6.0, -0.5, 0.5);
	glTexCoord2f(1, 1); glVertex3f(6.0, -0.5, 0.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glColor3d(1, 1, 1);
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient );
	glMaterialf( GL_FRONT, GL_SHININESS, 128.0 );
	glEnable( GL_LIGHTING );
	glPushMatrix();
	glTranslatef(xpla, ypla, zpla); //����
	glutSolidSphere(0.3, 15, 15);
	glPopMatrix();
	glDisable( GL_LIGHTING );

	glColor3d(1, 0, 0);
	glPushMatrix();
	glTranslatef(-5.0, 0.0, playerz); //�v���C���[��
	glScalef(1.0, 3.0, 2.0);
	glutSolidCube(0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0, 0.0, cpuz); //cpu��
	glScalef(1.0, 3.0, 2.0);
	glutSolidCube(0.3);
	glPopMatrix();

	if (obs == 1) { //��Q��1
		glColor3d(1, 1, 1);
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);  //��Q���̃f�U�C��
		glBindTexture(GL_TEXTURE_2D, 5);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(0.0, -0.5, 0.5);
		glTexCoord2f(1, 0); glVertex3f(0.0, -0.5, 6.0);
		glTexCoord2f(1, 1); glVertex3f(0.0, 3.0, 6.0);
		glTexCoord2f(0, 1); glVertex3f(0.0, 3.0, 0.5);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	if (xpla >= 5.3) {  //���_����
		myp++;
		if (myp < 5 || cpup < 5) {
			gameset();
		}
	}
	if (xpla <= -5.3) {
		cpup++;
		if (myp < 5 || cpup < 5) {
			gameset();
		}
	}

	glColor3d(0, 1, 0); //�_���`��
	sprintf(text, "%d --- %d", myp, cpup);
	writestr(textviewx, textviewy + 0.8, text);
	if (mode == GL_TRUE) { //���[�h�ύX
		sprintf(text, "mode(x): normal");
	}
	else {
		sprintf(text, "mode(x): practice");
	}
	writestr(textviewx - 0.2, textviewy + 0.2, text);
	if (pov == GL_TRUE) { //���_�ύX
		sprintf(text, "pov(z): normal");
	}
	else {
		sprintf(text, "pov(z): fpv");
	}
	writestr(textviewx - 0.2, textviewy, text);
	if (obs == 0) { //��Q���ύX
		sprintf(text, "obstacle(y): normal");
	}
	else {
		sprintf(text, "obstacle(y): half hidden");
	}
	writestr(textviewx - 0.2, textviewy - 0.2, text);
	sprintf(text, "w key: start/stop");
	writestr(textviewx - 0.4, textviewy - 0.5, text);
	sprintf(text, "a/d key: left/right");
	writestr(textviewx - 0.4, textviewy - 0.7, text);
	sprintf(text, "r key: reset");
	writestr(textviewx - 0.5, textviewy - 0.9, text);
	sprintf(text, "j key: farewell");
	writestr(textviewx - 0.5, textviewy - 1.1, text);
	if (myp >= 5) {
		PlaySound("result.wav", NULL, SND_FILENAME | SND_ASYNC);
		sprintf(text, "You Win  Press'r'key");
		writestr(textviewx - 0.1, textviewy + 0.5, text);
		glutIdleFunc(NULL);
	}
	else if (cpup >= 5) {
		PlaySound("result.wav", NULL, SND_FILENAME | SND_ASYNC);
		sprintf(text, "You Lose  Press'r'key");
		writestr(textviewx - 0.1, textviewy + 0.5, text);
		glutIdleFunc(NULL);
	}


	glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();

	if (wireFlag == GL_TRUE) {
		glutIdleFunc(NULL);
	}
}

void idle(void)
{
	srand((unsigned int)time(NULL));

	//�c����x������z
	xpla += xspeed; //pla�̓{�[���̈ʒu,speed�̓{�[���̑��x
	zpla += zspeed;

	if (mode == GL_TRUE) {  //�ʏ탂�[�h
		if (prespeedz != GL_TRUE) { //�������Ă���~�X��m��up
			if (rand() % 10 + 1 <= 3) {
				cpuz = zpla;
			}
			else {
				cpuz = zpla / 2 + 2;
			}
		}
		else { //�ʏ�͂��܂�~�X���Ȃ�
			if (rand() % 10 + 1 <= 9) {
				cpuz = zpla;
			}
			else {
				cpuz = zpla / 2 + 2;
			}
		}
	}
	else { //�ł������郂�[�h
		cpuz = zpla;
	}

	if (zpla >= 5.7 || zpla <= 0.8) { //�ǂ̔���
		srand((unsigned int)time(NULL));
		if (rand() % 10 + 1 <= 3) { //���܂ɕǂő��x���オ��(�����W��1.2)
			zspeed = -1.2 * zspeed;
			kasoku *= 1.2;
		}
		else {
			zspeed = -zspeed;
		}
	}

	if ((xpla >= -4.8) && (xpla <= -4.7) && (playerz - 0.5 <= zpla) && (playerz + 0.5 >= zpla)) { //�v���C���[���̃{�[������
		if (dkey != GL_TRUE) { //�v���C���[�����X���C�h���Ă��牡�����ɉ���
			prespeedz = !prespeedz;
			zspeed = 2.0 * zspeed;
			kasoku *= 2.0; //�����{���̕ۑ�
		}
		if (akey != GL_TRUE) {
			prespeedz = !prespeedz;
			zspeed = 2.0 * zspeed;
			kasoku *= 2.0;
		}
		xspeed = -xspeed;
		PlaySound("racket.wav", NULL, SND_FILENAME | SND_ASYNC);
	}

	if ((xpla <= 4.8) && (xpla >= 4.7) && (cpuz - 0.5 <= zpla) && (cpuz + 0.5 >= zpla)) { //cpu���̃{�[������
		if (prespeedz != GL_TRUE) { //CPU�����P�b�g�ɓ�����Ƃ��ɉ������Ă��瑬�x��߂�
			zspeed /= kasoku;
			kasoku = 1.0; //�����̏�����
			prespeedz = !prespeedz;
		}
		xspeed = -xspeed;
		PlaySound("racket.wav", NULL, SND_FILENAME | SND_ASYNC);
	}

	if (dkey != GL_TRUE) { //�v���C���[���̉E�փ��P�b�g����
		if (playerz <= 5.7 && playerz >= 0.6) {
			playerz += 0.03;
		}
	}

	if (akey != GL_TRUE) { //�v���C���[���̍��փ��P�b�g����
		if (playerz <= 5.9 && playerz >= 0.9) {
			playerz -= 0.03;
		}
	}

	if (farewell != GL_TRUE) {
		distancepls += 0.01;
		cycle += 2;
		povy = parabola(distance);
	}
	glutPostRedisplay();
}

void myKbd(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w': //�X�^�[�g���|�[�Y��
		wireFlag = !wireFlag;
		glutIdleFunc(idle);
		break;
	case KEY_ESC:
		exit(0);
	case 'a': //��x�����ƍ��ֈړ��A������x�����Ǝ~�܂�
		akey = !akey;
		if (dkey != GL_TRUE) {
			dkey = !dkey;
		}
		break;
	case 'd': //��x�����ƉE�ֈړ��A������x�����Ǝ~�܂�
		dkey = !dkey;
		if (akey != GL_TRUE) {
			akey = !akey;
		}
		break;
	case 'r': //�S�čŏ�����
		resetview();
		break;
	case 'z': //���_�ύX
		PlaySound("select.wav", NULL, SND_FILENAME | SND_ASYNC);
		pov = !pov;
		break;
	case 'x': //cpu�̃��[�h�؂�ւ�
		PlaySound("select.wav", NULL, SND_FILENAME | SND_ASYNC);
		mode = !mode;
		break;
	case 'y': //��Q����2���
		PlaySound("select.wav", NULL, SND_FILENAME | SND_ASYNC);
		obs++;
		if (obs >= 2) {
			obs = 0;
		}
		break;
	case 'j':
		farewell = !farewell;
		pov = GL_TRUE;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void initTexture3(void)
{
	unsigned char *image3[1];
	int imageHeightb, imageWidthb;


	if (!ReadBitMapData("table.bmp", &imageWidthb, &imageHeightb, image3)) {
		printf("Error! \n");
		exit(0);
	}

	glBindTexture(GL_TEXTURE_2D, 3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, imageWidthb, imageHeightb, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, *image3);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void initTexture4(void)
{
	unsigned char *image4[1];
	int imageHeightb, imageWidthb;


	if (!ReadBitMapData("card02.bmp", &imageWidthb, &imageHeightb, image4)) {
		printf("Error! \n");
		exit(0);
	}

	glBindTexture(GL_TEXTURE_2D, 4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, imageWidthb, imageHeightb, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, *image4);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void initTexture5(void)
{
	unsigned char *image5[1];
	int imageHeightb, imageWidthb;


	if (!ReadBitMapData("obstacle.bmp", &imageWidthb, &imageHeightb, image5)) {
		printf("Error! \n");
		exit(0);
	}

	glBindTexture(GL_TEXTURE_2D, 5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, imageWidthb, imageHeightb, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, *image5);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void myInit(char *progname)
{
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(progname);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutKeyboardFunc(myKbd);

	resetview();
	initTexture3();
	initTexture4();
	initTexture5();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
}


void myReshape(int width, int height)
{
	float aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, nearw, farw);
	glMatrixMode(GL_MODELVIEW);
}


void polarview(void)
{
	glTranslatef(povx, povy, -distance);
	glRotatef(-twist, 0.0, 0.0, 1.0);
	glRotatef(-elevation, 1.0, 0.0, 0.0);
	glRotatef(-azimuth, 0.0, 1.0, 0.0);
}

void resetview(void)
{
	distance = 10; //�������_
	twist = 0.0;
	elevation = -20.0;
	azimuth = -90.0;
	xpla = 0;
	ypla = 0;
	zpla = 3.3;
	xspeed = 0.05;
	zspeed = 0.04;
	playerz = 3.3;
	cpuz = 3.3;
	wireFlag = GL_TRUE;
	dkey = GL_TRUE;
	akey = GL_TRUE;
	prespeedz = GL_TRUE;
	pov = GL_TRUE;
	kasoku = 1.0;
	myp = 0;
	cpup = 0;
	povx = -3.3;
	povy = 0.0;
	mode = GL_TRUE;
	obs = 0;
	textviewx = -4;
	textviewy = 2.6;
	farewell = GL_TRUE;
	distancepls = 0;
	cycle = 0;
	PlaySound(NULL, NULL, 0);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(NULL);
	glutMainLoop();
}


