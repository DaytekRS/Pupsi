#include <iostream>
#include <time.h>
#include <ctime>
#include <fstream>
#include <windows.h>
#include "gl/glut.h"
#include "SOIL.h"
#pragma comment (lib, "SOIL.lib")
#pragma warning(disable:  4996)
using namespace std;
struct STATE {
	bool shop;
	bool game;
	struct GAME {
		bool doodle;
		int point;
	}gameS;
};
struct GAME {
	float hp;
	float energy;
	float eat;
	int money;
	bool sleep;
};
struct SHIT {
	float x, y;
	SHIT *next;
	SHIT *back;
	SHIT *end;
	int index;
};
struct PARAMETRS {
	char name[256];
	GLuint* textures;
};
GLuint hp, energy, eat, pet,fon,sleep,Sfon,eatS,energyS,money,petS,getup,shit,
apple,shop,shopButton,back,load,banan,hotdog,kola, raspberry, strawberry,pizza,orange,gameButton,winT,lose,over;
GAME *game = NULL;
SHIT *startShit = NULL;
STATE *state = NULL;
bool pole[3][3];
bool cross[3][3];
bool win;
int n = 0;
int load_num = 1;
void glWrite(float x, float y, float z, int *font, char text[256], int kls)
{
	glRasterPos3f(x, y, z);
	for (int i = 0; i<kls; i++)
		glutBitmapCharacter(font, (int)text[i]);
}
void del(int index) {
		SHIT* temp = startShit;
		if (index == 0) {
			startShit = startShit->next;
			if (startShit != NULL) startShit->back = NULL;
			SHIT* tmp = startShit;
			while (tmp != NULL) {
				tmp->index = tmp->index - 1;
				if (temp->next == NULL) startShit->end = temp;
				tmp = tmp->next;
			}
			delete(temp);
		}
		else {
			while (temp->index != index) temp = temp->next;
			temp->back->next = temp->next;
			if (temp->next != NULL) temp->next->back = temp->back;
			SHIT* tmp = temp;
			temp = temp->next;
			delete(tmp);
			while (temp != NULL) {
				temp->index = temp->index - 1;
				if (temp->next == NULL) startShit->end = temp;
				temp = temp->next;
			}
		}
}
void newGame() {
	if (game == NULL) game = new GAME;
	if (state == NULL) state = new STATE;
	game->hp = 1000;
	game->eat = 1000;
	game->energy = 1000;
	game->money = 100;
	game->sleep = false;
	state->game = false;
	state->shop = false;
}
void LoadGame() {
	ifstream in("save.txt", ios::binary | ios::in);
	if (in) {
		time_t seconds = time(NULL);
		tm* timeinfo = localtime(&seconds);
		tm* timeinfo2=new tm;
		in.read((char*)&*game, sizeof(GAME));
		in.read((char*)&*timeinfo2, sizeof(tm));
		while (true) {
			SHIT *cur = new SHIT;
			in.read((char*)&*cur, sizeof(SHIT));
			if (in.eof()) { delete cur; break; }
			cur->next = NULL;
				if (startShit == NULL) {
					startShit = cur;
					cur->back = NULL;
					startShit->end = cur;
					cur->index = 0;
				}
				else {
					startShit->end->next = cur;
					cur->back = startShit->end;
					startShit->end = cur;
					cur->index = cur->back->index + 1;
				}
		}
		in.close();
		unsigned long int raz = 0;
		if ((timeinfo->tm_year - timeinfo2->tm_year != 0) || ((timeinfo->tm_mon - timeinfo2->tm_mon != 0) && (timeinfo->tm_mday - timeinfo2->tm_mday > 1))) {
			game->hp = 0;
			game->energy = 0;
			game->eat = 0;
			delete timeinfo2;
			return;
		}
		else {
			if (timeinfo->tm_mday - timeinfo2->tm_mday == 0) {
				raz = (timeinfo->tm_hour * 3600 + timeinfo->tm_min * 60 + timeinfo->tm_sec) - (timeinfo2->tm_hour * 3600 + timeinfo2->tm_min * 60 + timeinfo2->tm_sec);
			}
			else {
				raz = (timeinfo->tm_hour * 3600 + timeinfo->tm_min * 60 + timeinfo->tm_sec) - (timeinfo2->tm_hour * 3600 + timeinfo2->tm_min * 60 + timeinfo2->tm_sec) + 86400;
			}
			game->hp = game->hp - (raz / 54);
			game->eat = game->eat - (raz / 36);
			if (game->sleep == false)
				game->energy = game->energy - raz / 86.4;
			else 
				game->energy = game->energy + raz / 7.2;
			delete timeinfo2;
		}
		if (game->hp <= 1) game->hp = 0;
		if (game->energy <= 1) game->energy = 0;
		if (game->energy > 1000) game->energy = 1000;
	}
}
void Load(int frame) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, load);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(0, 600);
	glTexCoord2f(1, 1); glVertex2f(600, 600);
	glTexCoord2f(1, 0); glVertex2f(600, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2f(150, 340);
	glVertex2f(450, 340);
	glVertex2f(450, 300);
	glVertex2f(150, 300);
	glEnd();

	glColor3f(1, 0, 0.2);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(155, 330);
	glTexCoord2f(1, 1); glVertex2f(155+(290/29)*frame, 330);
	glTexCoord2f(1, 0); glVertex2f(155 + (290 / 29)*frame, 310);
	glTexCoord2f(0, 0); glVertex2f(155, 310);
	glEnd();
	glutSwapBuffers();
}

void LoadTextures() {
	unsigned int start_time = clock();
	setlocale(LC_ALL, "Russian");

	
	load = SOIL_load_OGL_texture("img/load.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(1);
	hp = SOIL_load_OGL_texture("img/hp.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(2);
	eat = SOIL_load_OGL_texture("img/eat.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(3);
	energy = SOIL_load_OGL_texture("img/energy.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(4);
	fon = SOIL_load_OGL_texture("img/fon.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(5);


	pet = SOIL_load_OGL_texture("img/pupsi.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(6);
	sleep = SOIL_load_OGL_texture("img/sleep.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(7);
	Sfon = SOIL_load_OGL_texture("img/Sfon.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(8);
	eatS = SOIL_load_OGL_texture("img/eatS.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(9);
	energyS = SOIL_load_OGL_texture("img/energyS.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(10);
	money = SOIL_load_OGL_texture("img/money.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(11);
	petS = SOIL_load_OGL_texture("img/pupsiS.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(12);
	getup = SOIL_load_OGL_texture("img/GetUp.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(13);
	shit = SOIL_load_OGL_texture("img/shit.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(14);
	shop = SOIL_load_OGL_texture("img/shop.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(15);
	shopButton = SOIL_load_OGL_texture("img/shop.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(16);
	back = SOIL_load_OGL_texture("img/back.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(17);


	apple = SOIL_load_OGL_texture("img/apple.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(18);
	banan = SOIL_load_OGL_texture("img/banan.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(19);
	hotdog = SOIL_load_OGL_texture("img/hotdog.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(20);
	kola = SOIL_load_OGL_texture("img/kola.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(21);
	raspberry = SOIL_load_OGL_texture("img/raspberry.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(22);
	strawberry = SOIL_load_OGL_texture("img/strawberry.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(23);
	pizza = SOIL_load_OGL_texture("img/pizza.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(24);
	orange = SOIL_load_OGL_texture("img/orange.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(25);
	gameButton = SOIL_load_OGL_texture("img/game.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(26);
	lose = SOIL_load_OGL_texture("img/lose.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(27);
	winT = SOIL_load_OGL_texture("img/win.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(28);
	over = SOIL_load_OGL_texture("img/over.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	Load(29);


}

void giveShit() {
	SHIT *cur = new SHIT;
	int r1 = 1 + rand() % 2;
	if (r1 == 1) {
		cur->x = 1 + rand() % 140;
		cur->y = 40 + rand() % 140;
	}
	else {
		cur->x = 400 + rand() % 165;
		cur->y = 40 + rand() % 140;
	}
	cur->next = NULL;
	if (startShit == NULL) {
		startShit = cur;
		cur->back = NULL;
		startShit->end = cur;
		cur->index = 0;
	}
	else {
		startShit->end->next = cur;
		cur->back = startShit->end;
		startShit->end = cur;
		cur->index = cur->back->index + 1;
	}
}
void hpFunc() {
	static int STARThp = GetTickCount();
	if (game->eat!=0)
	if (GetTickCount()- STARThp >= 54000) { game->hp = game->hp - 1;
	STARThp = GetTickCount();
	}
	else
		if (GetTickCount() - STARThp >= 27000) {
			game->hp = game->hp - 1;
			STARThp = GetTickCount();
		}
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, hp);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	glTexCoord2f(0,1); glVertex2f(15, 590);
	glTexCoord2f(1,1); glVertex2f(35, 590);
	glTexCoord2f(1,0); glVertex2f(35, 570);
	glTexCoord2f(0,0); glVertex2f(15, 570);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex2f(45, 590);
	glVertex2f(145, 590);
	glVertex2f(145, 570);
	glVertex2f(45, 570);
	glEnd();
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(47, 588);
	glVertex2f(143-(1000-game->hp)*0.096, 588);
	glVertex2f(143 - (1000 - game->hp)*0.096, 572);
	glVertex2f(47, 572);
	glEnd();
}
void eatFunc() {
	static int STARTeat = GetTickCount();
	if (GetTickCount() - STARTeat >= 36000) {
		game->eat = game->eat - 1;
		STARTeat = GetTickCount();
	}
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	if (game->sleep == false)
		glBindTexture(GL_TEXTURE_2D, eat);
	else 
		glBindTexture(GL_TEXTURE_2D, eatS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(163, 590);
	glTexCoord2f(1, 1); glVertex2f(183, 590);
	glTexCoord2f(1, 0); glVertex2f(183, 570);
	glTexCoord2f(0, 0); glVertex2f(163, 570);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(193, 590);
	glVertex2f(293, 590);
	glVertex2f(293, 570);
	glVertex2f(193, 570);
	glEnd();
	glColor3f(0, 1, 0.3);
	glBegin(GL_POLYGON);
	glVertex2f(195, 588);
	glVertex2f(291 - (1000 - game->eat)*0.096, 588);
	glVertex2f(291 - (1000 - game->eat)*0.096, 572);
	glVertex2f(195, 572);
	glEnd();
}
void energeFunc() {
	static int STARTenergy = GetTickCount();
	if (game->sleep == false) {
		if (GetTickCount() - STARTenergy >= 86400) {
			game->energy = game->energy - 1;
			STARTenergy = GetTickCount();
		}
	}
		else 
			if (GetTickCount() - STARTenergy >= 7200) {
				game->energy = game->energy + 1;
				STARTenergy = GetTickCount();
			}
	if (game->energy < 0) game->energy = 0;
	if (game->energy > 1000) game->energy = 1000;
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	if (game->sleep==false)
		glBindTexture(GL_TEXTURE_2D, energy);
	else 
		glBindTexture(GL_TEXTURE_2D, energyS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(313, 590);
	glTexCoord2f(1, 1); glVertex2f(333, 590);
	glTexCoord2f(1, 0); glVertex2f(333, 570);
	glTexCoord2f(0, 0); glVertex2f(313, 570);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(343, 590);
	glVertex2f(443, 590);
	glVertex2f(443, 570);
	glVertex2f(343, 570);
	glEnd();
	glColor3f(0, 1, 0.8);
	glBegin(GL_POLYGON);
	glVertex2f(345, 588);
	glVertex2f(441 - (1000 - game->energy)*0.096, 588);
	glVertex2f(441 - (1000 - game->energy)*0.096, 572);
	glVertex2f(345, 572);
	glEnd();
}
void moneyFunc() {
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, money);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(461, 590);
	glTexCoord2f(1, 1); glVertex2f(481, 590);
	glTexCoord2f(1, 0); glVertex2f(481, 570);
	glTexCoord2f(0, 0); glVertex2f(461, 570);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	char buf[256];
	itoa(game->money, buf, 10);
	glWrite(491, 573, 0, (int*)GLUT_BITMAP_TIMES_ROMAN_24, buf, strlen(buf));
}
void fonFunc() {
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	if (game->sleep == false) {
		glBindTexture(GL_TEXTURE_2D, fon);
	}
	else glBindTexture(GL_TEXTURE_2D, Sfon);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(0, 600);
	glTexCoord2f(1, 1); glVertex2f(600, 600);
	glTexCoord2f(1, 0); glVertex2f(600, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void overFunc() {
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, over);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(0, 600);
	glTexCoord2f(1, 1); glVertex2f(600, 600);
	glTexCoord2f(1, 0); glVertex2f(600, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void pupsiFunc() {
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	if (game->sleep == false) {
		glBindTexture(GL_TEXTURE_2D, pet);
	}
	else glBindTexture(GL_TEXTURE_2D, petS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(215, 190);
	glTexCoord2f(1, 1); glVertex2f(385, 190);
	glTexCoord2f(1, 0); glVertex2f(385, 20);
	glTexCoord2f(0, 0); glVertex2f(215, 20);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
void shitFunc() {
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, shit);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SHIT *temp = startShit;
	while(temp != NULL) {
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 1); glVertex2f(temp->x, temp->y);
		glTexCoord2f(1, 1); glVertex2f(temp->x+30, temp->y);
		glTexCoord2f(1, 0); glVertex2f(temp->x + 30, temp->y-30);
		glTexCoord2f(0, 0); glVertex2f(temp->x, temp->y - 30);
		glEnd();
		temp = temp->next;
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
void drawButton() {
	if ((state->shop == false)&&(state->game==false)) {
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (game->sleep == false)
			glBindTexture(GL_TEXTURE_2D, sleep);
		else  glBindTexture(GL_TEXTURE_2D, getup);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 1); glVertex2f(485, 560);
		glTexCoord2f(1, 1); glVertex2f(590, 560);
		glTexCoord2f(1, 0); glVertex2f(590, 530);
		glTexCoord2f(0, 0); glVertex2f(485, 530);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, shopButton);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 1); glVertex2f(485, 520);
		glTexCoord2f(1, 1); glVertex2f(590, 520);
		glTexCoord2f(1, 0); glVertex2f(590, 490);
		glTexCoord2f(0, 0); glVertex2f(485, 490);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, gameButton);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 1); glVertex2f(485, 480);
		glTexCoord2f(1, 1); glVertex2f(590, 480);
		glTexCoord2f(1, 0); glVertex2f(590, 450);
		glTexCoord2f(0, 0); glVertex2f(485, 450);
		glEnd();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
	else {
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, back);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 1); glVertex2f(485, 560);
		glTexCoord2f(1, 1); glVertex2f(590, 560);
		glTexCoord2f(1, 0); glVertex2f(590, 530);
		glTexCoord2f(0, 0); glVertex2f(485, 530);
		glEnd();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}
}
void drawShop() {
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shop);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(0, 600);
	glTexCoord2f(1, 1); glVertex2f(600, 600);
	glTexCoord2f(1, 0); glVertex2f(600, 0);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glEnd();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, apple);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(40, 480);
	glTexCoord2f(1, 1); glVertex2f(80, 480);
	glTexCoord2f(1, 0); glVertex2f(80, 430);
	glTexCoord2f(0, 0); glVertex2f(40, 430);
	glEnd();
	glWrite(47, 405, 0, (int*)GLUT_BITMAP_TIMES_ROMAN_10, "35", 2);

	glBindTexture(GL_TEXTURE_2D, banan);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(120, 480);
	glTexCoord2f(1, 1); glVertex2f(180, 480);
	glTexCoord2f(1, 0); glVertex2f(180, 420);
	glTexCoord2f(0, 0); glVertex2f(120, 420);
	glEnd();
	glWrite(140, 405, 0, (int*)GLUT_BITMAP_TIMES_ROMAN_24, "35", 2);

	glBindTexture(GL_TEXTURE_2D, hotdog);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(480, 460);
	glTexCoord2f(1, 1); glVertex2f(560, 460);
	glTexCoord2f(1, 0); glVertex2f(540, 430);
	glTexCoord2f(0, 0); glVertex2f(500, 430);
	glEnd();
	glWrite(507, 405, 0, (int*)GLUT_BITMAP_TIMES_ROMAN_24, "45", 2);

	glBindTexture(GL_TEXTURE_2D, kola);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(400, 510);
	glTexCoord2f(1, 1); glVertex2f(480, 510);
	glTexCoord2f(1, 0); glVertex2f(480, 430);
	glTexCoord2f(0, 0); glVertex2f(400, 430);
	glEnd();
	glWrite(427, 405, 0, (int*)GLUT_BITMAP_TIMES_ROMAN_24, "50", 2);

	glBindTexture(GL_TEXTURE_2D, raspberry);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(40, 350);
	glTexCoord2f(1, 1); glVertex2f(80, 350);
	glTexCoord2f(1, 0); glVertex2f(80, 300);
	glTexCoord2f(0, 0); glVertex2f(40, 300);
	glEnd();
	glWrite(47, 280, 0, (int*)GLUT_BITMAP_TIMES_ROMAN_24, "30", 2);

	glBindTexture(GL_TEXTURE_2D, strawberry);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(120, 350);
	glTexCoord2f(1, 1); glVertex2f(180, 350);
	glTexCoord2f(1, 0); glVertex2f(180, 300);
	glTexCoord2f(0, 0); glVertex2f(120, 300);
	glEnd();
	glWrite(140, 280, 0, (int*)GLUT_BITMAP_TIMES_ROMAN_24, "30", 2);


	glBindTexture(GL_TEXTURE_2D, pizza);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(400, 350);
	glTexCoord2f(1, 1); glVertex2f(480, 350);
	glTexCoord2f(1, 0); glVertex2f(480, 300);
	glTexCoord2f(0, 0); glVertex2f(400, 300);
	glEnd();
	glWrite(427, 280, 0, (int*)GLUT_BITMAP_TIMES_ROMAN_24, "75", 2);


	glBindTexture(GL_TEXTURE_2D, orange);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1); glVertex2f(30, 220);
	glTexCoord2f(1, 1); glVertex2f(95, 220);
	glTexCoord2f(1, 0); glVertex2f(95, 175);
	glTexCoord2f(0, 0); glVertex2f(30, 175);
	glEnd();
	glWrite(47, 150, 0, (int*)GLUT_BITMAP_TIMES_ROMAN_24, "20", 2);


	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
void stepC() {
	n++;
	int i, j;
	while (true) {
		i = rand() % 3;
		j = rand() % 3;
		if (pole[i][j] == false) {
			pole[i][j] = true;
			break;
		}
	}
}
void drawCross() {
	 for (int i=0;i<3;i++)
		 for (int j=0;j<3;j++)
			 if (pole[i][j] == true) {
				 glColor3f(0, 0, 0);
				 glLineWidth(10);
				 int x, y;
				 if (j == 0) x = 120;
					 else 
						 if (j == 1) x = 250;
							else  x = 406;
				 if (i == 0) y = 500;
			     else 
					if (i == 1) y = 356;
					else y = 220;
				if (cross[i][j] == true) {
						glBegin(GL_LINES);
						glVertex2f(x, y);
						glVertex2f(x + 100, y - 100);
						glVertex2f(x, y - 100);
						glVertex2f(x + 100, y);
						glEnd();
					}
				else {
					int r = 50;
					x += 50;
					y -= 50;
					glBegin(GL_LINES);
					for (int i = 0; i < 360; i++) {
						glVertex3f(x + r * sin(i * 3.14 / 180), y + r * cos(i * 3.14 / 180), 0);
					}
					glEnd();
				}

			 }
}
int check() {
	if ((pole[0][0] == true) && (pole[0][1] == true) && (pole[0][2] == true)) {
		if ((cross[0][0] == true) && (cross[0][1] == true) && (cross[0][2] == true)) return 1;
		else 
			if ((cross[0][0] == false) && (cross[0][1] == false) && (cross[0][2] == false))
				return -1;
	}
	if ((pole[1][0] == true) && (pole[1][1] == true) && (pole[1][2] == true)) {
		if ((cross[1][0] == true) && (cross[1][1] == true) && (cross[1][2] == true)) return 1;
		else if ((cross[1][0] == false) && (cross[1][1] == false) && (cross[1][2] == false))
			return -1;
	}
	if ((pole[2][0] == true) && (pole[2][1] == true) && (pole[2][2] == true)) {
		if ((cross[2][0] == true) && (cross[2][1] == true) && (cross[2][2] == true)) return 1;
		else 
			if ((cross[2][0] == false) && (cross[2][1] == false) && (cross[2][2] == false))
			return -1;
	}


	if ((pole[0][0] == true) && (pole[1][0] == true) && (pole[2][0] == true)) {
		if ((cross[0][0] == true) && (cross[1][0] == true) && (cross[2][0] == true)) return 1;
		else 
			if ((cross[0][0] == false) && (cross[1][0] == false) && (cross[2][0] == false))
				return -1;
	}
	if ((pole[0][1] == true) && (pole[1][1] == true) && (pole[2][1] == true)) {
		if ((cross[0][1] == true) && (cross[1][1] == true) && (cross[2][1] == true)) return 1;
		else 
			if ((cross[0][1] == false) && (cross[1][1] == false) && (cross[2][1] == false))
			return -1;
	}
	if ((pole[0][2] == true) && (pole[1][2] == true) && (pole[2][2] == true)) {
		if ((cross[0][2] == true) && (cross[1][2] == true) && (cross[2][2] == true)) return 1;
		else 
			if ((cross[0][2] == false) && (cross[1][2] == false) && (cross[2][2] == false))
			return -1;
	}



	if ((pole[2][0] == true) && (pole[1][1] == true) && (pole[0][2] == true)) {
		if ((cross[2][0] == true) && (cross[1][1] == true) && (cross[0][2] == true)) return 1;
		else 
			if ((cross[2][0] == false) && (cross[1][1] == false) && (cross[0][2] == false))
				return -1;
	}
	if ((pole[0][0] == true) && (pole[1][1] == true) && (pole[2][2] == true)) {
		if ((cross[0][0] == true) && (cross[1][1] == true) && (cross[2][2] == true)) return 1;
		else
			if ((cross[2][0] == false) && (cross[1][1] == false) && (cross[2][2] == false))
			return -1;
	}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (pole[i][j] == false)
				return 0;
	return 2;
	
}
void drawGame(bool refresh) {
	static bool wincross = false;
	if (refresh == true) {
		win = false;
		wincross = false;
		return;
	}
	if (win == false) {
		glColor3ub(135, 206, 250);
		glBegin(GL_POLYGON);
		glVertex2f(0, 600);
		glVertex2f(600, 600);
		glVertex2f(600, 0);
		glVertex2f(0, 0);
		glEnd();
		glLineWidth(20);
		glColor3ub(172, 158, 158);
		glBegin(GL_LINES);
		glVertex2f(230, 100);
		glVertex2f(230, 500);
		glVertex2f(386, 100);
		glVertex2f(386, 500);

		glVertex2f(100, 230);
		glVertex2f(500, 230);
		glVertex2f(100, 386);
		glVertex2f(500, 386);
		glEnd();
		drawCross();
		if (check() == 1) {
			win = true;
			wincross = true;
			game->money += 20;
		}
		if ((check() == -1)||(check() == 2)) {
			win = true;
		}
	}
	else {
		if (wincross==true)
			glBindTexture(GL_TEXTURE_2D, winT);
		else 
			glBindTexture(GL_TEXTURE_2D, lose);
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 1); glVertex2f(0, 600);
		glTexCoord2f(1, 1); glVertex2f(600, 600);
		glTexCoord2f(1, 0); glVertex2f(600, 0);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}
void Draw()
{
	static bool textures = false;
	if (textures == false) {
		LoadTextures();
		textures = true;
	}
	static int start = GetTickCount();
	bool shitB=false;
	if (GetTickCount() - start >= 36000) {
		game->money++;
		start = GetTickCount();
        int y = 1 + rand() % 10;
		if (y == 1) shitB = true;
	}
	if (shitB == true)
		giveShit();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (game->hp > 0) {
		if ((state->shop == false) && (state->game == false)) {
			fonFunc();
			pupsiFunc();
			shitFunc();
		}
		else
			if ((state->shop == true))
				drawShop();
			else
				drawGame(false);
		moneyFunc();
		hpFunc();
		eatFunc();
		energeFunc();
		drawButton();
	}
	else {
		overFunc();
	}
	glutSwapBuffers();
}
void Timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 0);
}
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY: {
		time_t seconds = time(NULL);
		tm* timeinfo = localtime(&seconds);
		ofstream f("save.txt", ios::binary | ios::out);
		f.write((char*)&*game, sizeof(GAME));
		f.write((char*)&*timeinfo, sizeof(tm));
		SHIT *temp = startShit;
		while (temp != NULL) {
			cout << "save" << endl;
			cout << temp->x << " " << temp->y << " " << endl;
			f.write((char*)&*temp, sizeof(SHIT));
			temp = temp->next;
		}
		f.close();
		PostQuitMessage(0);
		return 0;
	}
	case WM_LBUTTONUP: {
		int x, y;
		x = LOWORD(lparam);
		y = HIWORD(lparam);
		if (game->hp != 0) {
			if ((state->shop == false) && (state->game == false)) {
				if (((x >= 485) && (x <= 590)) && ((y >= 40) && (y <= 70))) {
					if (game->sleep == false)
						game->sleep = true;
					else game->sleep = false;
				}
				if (((x >= 485) && (x <= 590)) && ((y >= 80) && (y <= 110))) {
					state->shop = true;
				}
				if (((x >= 485) && (x <= 590)) && ((y >= 120) && (y <= 150))) {
					if (game->energy - 20 < 0) MessageBox(0, "У вас не достаточно энергии", "Ошибка", MB_OK);
					else {
						state->game = true;
						game->energy -= 20;
					}
				}
				SHIT *temp = startShit;
				while (temp != NULL) {
					if ((x >= temp->x) && (x <= temp->x + 30) && (y <= 600 - temp->y + 30) && (y >= 600 - temp->y)) {
						int index = temp->index;
						temp = temp->next;
						del(index);
						game->money += 20;
					}
					else
						temp = temp->next;
				}
			}
			else
				if (state->game == false) {
					if (((x >= 485) && (x <= 590)) && ((y >= 40) && (y <= 70))) {
						state->shop = false;
					}

					if (((x >= 40) && (x <= 80)) && ((y >= 120) && (y <= 170))) {
						if (game->money - 35 >= 0) {
							game->money = game->money - 35;
							game->hp = game->hp + 35;
							if (game->hp > 1000) game->hp = 1000;
							game->eat = game->eat + 35;
							if (game->eat > 1000) game->eat = 1000;
						}
						else {
							MessageBox(0, "У вас недостаточно денег", "Покупка невозможна", MB_OK);
						}
					}
					if (((x >= 120) && (x <= 180)) && ((y >= 120) && (y <= 180))) {
						if (game->money - 35 >= 0) {
							game->money = game->money - 35;
							game->hp = game->hp + 35;
							if (game->hp > 1000) game->hp = 1000;
							game->eat = game->eat + 35;
							if (game->eat > 1000) game->eat = 1000;
						}
						else {
							MessageBox(0, "У вас недостаточно денег", "Покупка невозможна", MB_OK);
						}
					}
					if (((x >= 480) && (x <= 560)) && ((y >= 140) && (y <= 170))) {
						if (game->money - 45 >= 0) {
							game->money = game->money - 45;
							game->hp = game->hp + 45;
							if (game->hp > 1000) game->hp = 1000;
							game->eat = game->eat + 45;
							if (game->eat > 1000) game->eat = 1000;
						}
						else {
							MessageBox(0, "У вас недостаточно денег", "Покупка невозможна", MB_OK);
						}
					}
					if (((x >= 400) && (x <= 480)) && ((y >= 90) && (y <= 170))) {
						if (game->money - 50 >= 0) {
							game->money = game->money - 50;
							game->hp = game->hp + 50;
							if (game->hp > 1000) game->hp = 1000;
							game->eat = game->eat + 50;
							if (game->eat > 1000) game->eat = 1000;
						}
						else {
							MessageBox(0, "У вас недостаточно денег", "Покупка невозможна", MB_OK);
						}
					}
					if (((x >= 40) && (x <= 80)) && ((y >= 250) && (y <= 300))) {
						if (game->money - 30 >= 0) {
							game->money = game->money - 30;
							game->hp = game->hp + 30;
							if (game->hp > 1000) game->hp = 1000;
							game->eat = game->eat + 30;
							if (game->eat > 1000) game->eat = 1000;
						}
						else {
							MessageBox(0, "У вас недостаточно денег", "Покупка невозможна", MB_OK);
						}
					}
					if (((x >= 120) && (x <= 180)) && ((y >= 250) && (y <= 300))) {
						if (game->money - 30 >= 0) {
							game->money = game->money - 30;
							game->hp = game->hp + 30;
							if (game->hp > 1000) game->hp = 1000;
							game->eat = game->eat + 30;
							if (game->eat > 1000) game->eat = 1000;
						}
						else {
							MessageBox(0, "У вас недостаточно денег", "Покупка невозможна", MB_OK);
						}
					}
					if (((x >= 400) && (x <= 480)) && ((y >= 250) && (y <= 300))) {
						if (game->money - 75 >= 0) {
							game->money = game->money - 75;
							game->hp = game->hp + 75;
							if (game->hp > 1000) game->hp = 1000;
							game->eat = game->eat + 75;
							if (game->eat > 1000) game->eat = 1000;
						}
						else {
							MessageBox(0, "У вас недостаточно денег", "Покупка невозможна", MB_OK);
						}
					}
					if (((x >= 30) && (x <= 95)) && ((y >= 380) && (y <= 425))) {
						if (game->money - 20 >= 0) {
							game->money = game->money - 20;
							game->hp = game->hp + 20;
							if (game->hp > 1000) game->hp = 1000;
							game->eat = game->eat + 20;
							if (game->eat > 1000) game->eat = 1000;
						}
						else {
							MessageBox(0, "У вас недостаточно денег", "Покупка невозможна", MB_OK);
						}
					}
				}
				else
				{
					if (((x >= 485) && (x <= 590)) && ((y >= 40) && (y <= 70))) {
						state->game = false;
						n = 0;
						for (int i = 0; i < 3; i++)
							for (int j = 0; j < 3; j++)
							{
								cross[i][j] = false;
								pole[i][j] = false;
							}
						drawGame(true);
					}
					if (win == false) {
						if ((x > 110) && (x < 220) && (y > 100) && (y < 204) && (pole[0][0] == false)) {
							n++;
							pole[0][0] = true;
							cross[0][0] = true;
							if (n != 9)
								stepC();
						}
						if ((x > 240) && (x < 370) && (y > 100) && (y < 204) && (pole[0][1] == false)) {
							n++;
							pole[0][1] = true;
							cross[0][1] = true;
							if (n != 9)
								stepC();
						}
						if ((x > 396) && (x < 490) && (y > 100) && (y < 204) && (pole[0][2] == false)) {
							n++;
							pole[0][2] = true;
							cross[0][2] = true;
							if (n != 9)
								stepC();
						}




						if ((x > 110) && (x < 220) && (y > 224) && (y < 360) && (pole[1][0] == false)) {
							n++;
							pole[1][0] = true;
							cross[1][0] = true;
							if (n != 9)
								stepC();
						}
						if ((x > 240) && (x < 370) && (y > 224) && (y < 360) && (pole[1][1] == false)) {
							pole[1][1] = true;
							cross[1][1] = true;
							n++;
							if (n != 9)
								stepC();
						}
						if ((x > 396) && (x < 490) && (y > 224) && (y < 360) && (pole[1][2] == false)) {
							pole[1][2] = true;
							cross[1][2] = true;
							n++;
							if (n != 9)
								stepC();
						}





						if ((x > 110) && (x < 220) && (y > 380) && (y < 490) && (pole[2][0] == false)) {
							pole[2][0] = true;
							cross[2][0] = true;
							n++;
							if (n != 9)
								stepC();
						}
						if ((x > 240) && (x < 370) && (y > 380) && (y < 490) && (pole[2][1] == false)) {
							pole[2][1] = true;
							cross[2][1] = true;
							n++;
							if (n != 9)
								stepC();
						}
						if ((x > 396) && (x < 490) && (y > 380) && (y < 490) && (pole[2][2] == false)) {
							pole[2][2] = true;
							cross[2][2] = true;
							n++;
							if (n != 9)
								stepC();
						}
					}
				}
		}
		else newGame();
	}
	default:
		return DefWindowProc(wnd, msg, wparam, lparam);
	}
}
void Initialize(int WW, int WH)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 600, 0, 600, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
int main(int argc, char **argv) {
	for (int i=0;i<3;i++)
		for (int j = 0; j < 3; j++)
		{
			cross[i][j] = false;
			pole[i][j] = false;
		}
	win = false;
	srand(time(0));
	newGame();
	LoadGame();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - 600) / 2, (GetSystemMetrics(SM_CYSCREEN) - 600) / 2);
	glutCreateWindow("PUPSI");
	HWND m_window = FindWindowA(NULL, "PUPSI");
	SetWindowLong(m_window, GWL_STYLE, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	SetWindowLong(m_window, GWL_WNDPROC, (LONG)WndProc);
	glClearColor(1, 1, 1, 1);
	glutReshapeFunc(Initialize);
	glutDisplayFunc(Draw);
	glutTimerFunc(50, Timer, 0);
	glutMainLoop();

}