#include <iostream>
#include <glut.h>
#include "stb_image.h"

GLfloat rotateX, rotateY = 0.0f;
GLfloat translateZ = -35.0f;

const float RubiksCubeSize = 13.0f;
const int timer = 30;

bool directionOfRotateIsNegative = false;

struct SmallCube
{
public:
	unsigned int sidesColors[6];
	GLuint sidesTextures[6];
	unsigned char colorCodeParts[4];
	float size;

	// поворот на плоскости yz
	void RotateX()
	{
		unsigned int tmpC = sidesColors[0];
		sidesColors[0] = sidesColors[4];
		sidesColors[4] = sidesColors[1];
		sidesColors[1] = sidesColors[5];
		sidesColors[5] = tmpC;

		GLuint tmpT = sidesTextures[0];
		sidesTextures[0] = sidesTextures[4];
		sidesTextures[4] = sidesTextures[1];
		sidesTextures[1] = sidesTextures[5];
		sidesTextures[5] = tmpT;
	}

	/*
	// поворот на плоскости -yz
	void RotateMinusX()
	{
		unsigned int tmpC[4] = { sidesColors[0], sidesColors[4], sidesColors[1], sidesColors[5] };
		sidesColors[0] = tmpC[3];
		sidesColors[4] = tmpC[0];
		sidesColors[1] = tmpC[1];
		sidesColors[5] = tmpC[2];

		GLuint tmpT[4] = { sidesTextures[0], sidesTextures[4], sidesTextures[1], sidesTextures[5] };
		sidesTextures[0] = tmpT[3];
		sidesTextures[4] = tmpT[0];
		sidesTextures[1] = tmpT[1];
		sidesTextures[5] = tmpT[2];
	} 
	*/

	// поворот на плоскости xz
	void RotateY()
	{
		unsigned int tmp = sidesColors[2];
		sidesColors[2] = sidesColors[1];
		sidesColors[1] = sidesColors[3];
		sidesColors[3] = sidesColors[0];
		sidesColors[0] = tmp;

		GLuint tmpT = sidesTextures[2];
		sidesTextures[2] = sidesTextures[1];
		sidesTextures[1] = sidesTextures[3];
		sidesTextures[3] = sidesTextures[0];
		sidesTextures[0] = tmpT;
	}

	// поворот на плоскости xy
	void RotateZ()
	{
		unsigned int tmp = sidesColors[5];
		sidesColors[5] = sidesColors[3];
		sidesColors[3] = sidesColors[4];
		sidesColors[4] = sidesColors[2];
		sidesColors[2] = tmp;

		GLuint tmpT = sidesTextures[5];
		sidesTextures[5] = sidesTextures[3];
		sidesTextures[3] = sidesTextures[4];
		sidesTextures[4] = sidesTextures[2];
		sidesTextures[2] = tmpT;
	}

	void SetColor(int sideNumber, int color)
	{
		this->sidesColors[sideNumber] = color;
	}

	void SetTexture(int sideNumber, int textureNumber)
	{
		this->sidesTextures[sideNumber] = textureNumber;
	}

	unsigned char* at(int i)
	{
		// разбиение color[i] на 3 составляющих
		colorCodeParts[0] = sidesColors[i] >> 16;
		colorCodeParts[1] = sidesColors[i] >> 8;
		colorCodeParts[2] = sidesColors[i];
		return colorCodeParts;
	}

	// отрисовка куба
	void DrawSmallCube()
	{
		glPushMatrix();
		// верхняя грань
		glColor3ubv(at(0));
		glBindTexture(GL_TEXTURE_2D, sidesTextures[0]);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(size, size, size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, size, size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size, 0.0f, size);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);

		// нижняя грань
		glColor3ubv(at(1));
		glBindTexture(GL_TEXTURE_2D, sidesTextures[1]);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(size, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, size, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size, size, 0.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);

		// передняя грань
		glColor3ubv(at(2));
		glBindTexture(GL_TEXTURE_2D, sidesTextures[2]);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(size, 0.0f, size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size, 0.0f, 0.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);

		// задняя грань
		glColor3ubv(at(3));
		glBindTexture(GL_TEXTURE_2D, sidesTextures[3]);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(size, size, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, size, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, size, size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size, size, size);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);

		// левая грань
		glColor3ubv(at(4));
		glBindTexture(GL_TEXTURE_2D, sidesTextures[4]);
		glBegin(GL_QUADS);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, size, size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, size, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, size);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);

		// правая грань
		glColor3ubv(at(5));
		glBindTexture(GL_TEXTURE_2D, sidesTextures[5]);
		glBegin(GL_QUADS);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(size, size, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(size, size, size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size, 0.0f, size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size, 0.0f, 0.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	}

	void DrawSmallCube(float x, float y, float z)
	{
		glPushMatrix();
		glTranslatef(x, y, z);
		DrawSmallCube();
		glPopMatrix();
	}
};

class RubiksCube
{
public:
	SmallCube smallCubes[3][3][3] = { 0 };	// 27 частей
	int rotateAngles[9] = { 0 };	// угол поворота каждой грани
	float size = 0.0f;
	unsigned int sidesColors[6] = { 0 };
	GLuint sidesTextures[6] = { 0 };
	int currentRotatingSide = 0;	// номер грани, которая в данный момент поварачивается(иначе -1)
	bool isNotRotating[4][4][4] = { true };	// показывает, находится ли в состоянии покоя деталь с координатами (i, j, k)
	SmallCube tmp[4][4] = { 0 };

	RubiksCube() {}

	RubiksCube(float size, unsigned int* sidesColors)
	{
		Reset(size, sidesColors, sidesTextures);
	}

	void Reset(float size, unsigned int* sidesColors, GLuint* sidesTextures)
	{
		memset(rotateAngles, 0, sizeof(rotateAngles));
		this->size = size;
		currentRotatingSide = -1;
		for (int i = 0; i < 6; i++) {
			this->sidesColors[i] = sidesColors[i];
			this->sidesTextures[i] = sidesTextures[i];
		}

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				smallCubes[i][j][2].SetColor(0, sidesColors[0]);	// верхняя грань
				smallCubes[i][j][0].SetColor(1, sidesColors[1]);	// нижняя грань
				smallCubes[j][0][i].SetColor(2, sidesColors[2]);	// передняя грань
				smallCubes[j][2][i].SetColor(3, sidesColors[3]);	// задняя грань
				smallCubes[0][j][i].SetColor(4, sidesColors[4]);	// левая грань
				smallCubes[2][j][i].SetColor(5, sidesColors[5]);	// правая грань

				smallCubes[i][j][2].SetTexture(0, sidesTextures[0]);	// верхняя грань
				smallCubes[i][j][0].SetTexture(1, sidesTextures[1]);	// нижняя грань
				smallCubes[j][0][i].SetTexture(2, sidesTextures[2]);	// передняя грань
				smallCubes[j][2][i].SetTexture(3, sidesTextures[3]);	// задняя грань
				smallCubes[0][j][i].SetTexture(4, sidesTextures[4]);	// левая грань
				smallCubes[2][j][i].SetTexture(5, sidesTextures[5]);	// правая грань
			}
		}

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					smallCubes[i][j][k].size = (size / 3.0f) * 0.95f;
	}

	void DrawRubiksCube()
	{
		const float K = 0.65;
		const float positionXYZ = ((1.0f - K) / 2.0f) * size + K * size / 2.0f;

		// рисуем корпус - куб черного цвета, размер которого равен K * size
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(positionXYZ, positionXYZ, positionXYZ);
		glutSolidCube(static_cast<unsigned __int64>(size) * static_cast<unsigned __int64>(K));
		glPopMatrix();

		memset(isNotRotating, true, sizeof(isNotRotating));
		if (currentRotatingSide != -1) {
			glPushMatrix();
			if (currentRotatingSide == 0 || currentRotatingSide == 1) {
				// 0 <= currentRotatingSide <= 1 показывает, что сейчас крутится грань на плоскости xy
				// currentRotatingSide = 0 - нижняя грань
				// currentRotatingSide = 1 - верхняя грань
				int k = currentRotatingSide == 0 ? 0 : 2;
				// следовательно isNotRotating слоя k  устанавливаем в false
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						isNotRotating[i][j][k] = false;

				// теперь нужно покрутить грань под номером currentRotatingSide на угол rotateAngles[currentRotatingSide]
				// относительно центра этой грани,
				// для этого сдвинемся к центру, покрутим, сдвинемся обратно
				glTranslatef(size / 2.0f, size / 2.0f, 0.0f);
				glRotatef(rotateAngles[currentRotatingSide], 0.0f, 0.0f, 1.0f);
				glTranslatef(-size / 2.0f, -size / 2.0f, 0.0f);
				// рисуем
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						smallCubes[i][j][k].DrawSmallCube(size / 3.0f * i, size / 3.0f * j, size / 3.0f * k);
			}
			// аналагично с остальными гранями
			else if (currentRotatingSide == 2 || currentRotatingSide == 3) {
				int j = currentRotatingSide == 2 ? 0 : 2;
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++)
						isNotRotating[i][j][k] = false;

				glTranslatef(size / 2.0f, 0.0f, size / 2.0f);
				glRotatef(rotateAngles[currentRotatingSide], 0.0f, 1.0f, 0.0f);
				glTranslatef(-size / 2.0f, 0.0f, -size / 2.0f);
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++)
						smallCubes[i][j][k].DrawSmallCube(size / 3.0f * i, size / 3.0f * j, size / 3.0f * k);
			}
			else if (currentRotatingSide == 4 || currentRotatingSide == 5) {
				int i = currentRotatingSide == 4 ? 0 : 2;
				for (int j = 0; j < 3; j++)
					for (int k = 0; k < 3; k++)
						isNotRotating[i][j][k] = false;

				glTranslatef(0.0f, size / 2.0f, size / 2.0f);
				glRotatef(rotateAngles[currentRotatingSide], 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, -size / 2.0f, -size / 2.0f);
				for (int j = 0; j < 3; j++)
					for (int k = 0; k < 3; k++)
						smallCubes[i][j][k].DrawSmallCube(size / 3.0f * i, size / 3.0f * j, size / 3.0f * k);
			}
			if (currentRotatingSide == 6) {
				int k = 1;
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						isNotRotating[i][j][k] = false;

				glTranslatef(size / 2.0f, size / 2.0f, 0.0f);
				glRotatef(rotateAngles[currentRotatingSide], 0.0f, 0.0f, 1.0f);
				glTranslatef(-size / 2.0f, -size / 2.0f, 0.0f);
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						smallCubes[i][j][k].DrawSmallCube(size / 3.0f * i, size / 3.0f * j, size / 3.0f * k);
			}
			else if (currentRotatingSide == 7) {
				int j = 1;
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++)
						isNotRotating[i][j][k] = false;

				glTranslatef(size / 2.0f, 0.0f, size / 2.0f);
				glRotatef(rotateAngles[currentRotatingSide], 0.0f, 1.0f, 0.0f);
				glTranslatef(-size / 2.0f, 0.0f, -size / 2.0f);
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++)
						smallCubes[i][j][k].DrawSmallCube(size / 3.0f * i, size / 3.0f * j, size / 3.0f * k);
			}
			else if (currentRotatingSide == 8) {
				int i = 1;
				for (int j = 0; j < 3; j++)
					for (int k = 0; k < 3; k++)
						isNotRotating[i][j][k] = false;

				glTranslatef(0.0f, size / 2.0f, size / 2.0f);
				glRotatef(rotateAngles[currentRotatingSide], 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, -size / 2.0f, -size / 2.0f);
				for (int j = 0; j < 3; j++)
					for (int k = 0; k < 3; k++)
						smallCubes[i][j][k].DrawSmallCube(size / 3.0f * i, size / 3.0f * j, size / 3.0f * k);
			}
			glPopMatrix();
		}

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					if (isNotRotating[i][j][k])
						// теперь рисуем те детали, которые не поварачивались выше,
						// они отмечены isNotRotating[i][j][k] = true
						smallCubes[i][j][k].DrawSmallCube(size / 3.0f * i, size / 3.0f * j, size / 3.0f * k);
	}

	void Rotate90(int rotatingSide, int turnsNumber)
	{
		// turnsNumber задаётся в зависимости он направления
		// (если turnsNumber = -1, значит крутим 3 раза)
		if (turnsNumber == -1) turnsNumber = 3;
		while (turnsNumber--) {
			if (rotatingSide == 0 || rotatingSide == 1) {
				int k = rotatingSide == 0 ? 0 : 2;
				// копируем повернутую на 90 градусов верхнюю/нижнюю грань
				// в массив tmp, затем грани присваиваем tmp
				// и поворачиваем каждую деталь этой грани
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						tmp[j][2 - i] = smallCubes[i][j][k];
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++) {
						tmp[i][j].RotateZ();
						smallCubes[i][j][k] = tmp[i][j];
					}
			}
			// аналогично с остальными гранями
			else if (rotatingSide == 2 || rotatingSide == 3) {
				int j = rotatingSide == 2 ? 0 : 2;
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++)
						tmp[k][2 - i] = smallCubes[i][j][k];
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++) {
						tmp[i][k].RotateX();
						smallCubes[i][j][k] = tmp[i][k];
					}
			}
			else if (rotatingSide == 4 || rotatingSide == 5) {
				int i = rotatingSide == 4 ? 0 : 2;
				for (int j = 0; j < 3; j++)
					for (int k = 0; k < 3; k++)
						tmp[k][2 - j] = smallCubes[i][j][k];
				for (int j = 0; j < 3; j++)
					for (int k = 0; k < 3; k++) {
						tmp[j][k].RotateY();
						smallCubes[i][j][k] = tmp[j][k];
					}
			}
			else if (rotatingSide == 6) {
				int k = 1;
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						tmp[j][2 - i] = smallCubes[i][j][k];
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++) {
						tmp[i][j].RotateZ();
						smallCubes[i][j][k] = tmp[i][j];
					}
			}
			else if (rotatingSide == 7) {
				int j = 1;
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++)
						tmp[k][2 - i] = smallCubes[i][j][k];
				for (int i = 0; i < 3; i++)
					for (int k = 0; k < 3; k++) {
						tmp[i][k].RotateX();
						smallCubes[i][j][k] = tmp[i][k];
					}
			}
			else if (rotatingSide == 8) {
				int i = 1;
				for (int j = 0; j < 3; j++)
					for (int k = 0; k < 3; k++)
						tmp[k][2 - j] = smallCubes[i][j][k];
				for (int j = 0; j < 3; j++)
					for (int k = 0; k < 3; k++) {
						tmp[j][k].RotateY();
						smallCubes[i][j][k] = tmp[j][k];
					}
			}
		}
	}

	// вращение грани под номером rotatingSide на угол angle
	void RotateSide(int rotatingSide, int angle)
	{
		// пытаемся покрутить грань с номером rotatingSide,
		// нужно проверить, что другая грань уже не крутится
		if (currentRotatingSide == -1 || currentRotatingSide == rotatingSide) {
			// обновляем поворот
			if (directionOfRotateIsNegative) rotateAngles[rotatingSide] -= angle;
			else rotateAngles[rotatingSide] += angle;

			if (rotateAngles[rotatingSide] % 90 != 0) {
				currentRotatingSide = rotatingSide;
			}
			else {
				// если угол стал кратным 90, то поварачиваем на массиве
				if ((rotateAngles[rotatingSide] < 0) ^ (currentRotatingSide == 2 || currentRotatingSide == 3))
					Rotate90(rotatingSide, 1);
				else
					Rotate90(rotatingSide, -1);
				rotateAngles[rotatingSide] = 0;
				currentRotatingSide = -1;
			}
		}
	}
};

RubiksCube rubiksCube;

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Кубик Рубика
    glPushMatrix();
	glTranslatef(0.0f, 0.0f, translateZ);
	glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(RubiksCubeSize / -2.0f, RubiksCubeSize / -2.0f, RubiksCubeSize / -2.0f);
	rubiksCube.DrawRubiksCube();
    glPopMatrix();

    glutSwapBuffers();
}

void RotateSides(unsigned char key, int, int)
{
	switch (key) {
	case ('q'): {
		if (rubiksCube.currentRotatingSide == -1) {
			directionOfRotateIsNegative = false;
			rubiksCube.RotateSide(0, 3);
			Display();
		}
	}
	case ('a'): {
		if (rubiksCube.currentRotatingSide == -1) {
			directionOfRotateIsNegative = false;
			rubiksCube.RotateSide(6, 3);
			Display();
		}
	}
	case ('z'): {
		if (rubiksCube.currentRotatingSide == -1) {
			directionOfRotateIsNegative = false;
			rubiksCube.RotateSide(1, 3);
			Display();
		}
	}
	case ('w'): {
		if (rubiksCube.currentRotatingSide == -1) {
			directionOfRotateIsNegative = false;
			rubiksCube.RotateSide(2, 3);
			Display();
		}
	}
	case ('s'): {
		if (rubiksCube.currentRotatingSide == -1) {
			directionOfRotateIsNegative = true;
			rubiksCube.RotateSide(7, 3);
			Display();
		}
	}
	case ('x'): {
		if (rubiksCube.currentRotatingSide == -1) {
			directionOfRotateIsNegative = false;
			rubiksCube.RotateSide(3, 3);
			Display();
		}
	}
	case ('e'): {
		if (rubiksCube.currentRotatingSide == -1) {
			directionOfRotateIsNegative = false;
			rubiksCube.RotateSide(4, 3);
			Display();
		}
	}
	case ('d'): {
		if (rubiksCube.currentRotatingSide == -1) {
			directionOfRotateIsNegative = false;
			rubiksCube.RotateSide(8, 3);
			Display();
		}
	}
	case ('c'): {
		if (rubiksCube.currentRotatingSide == -1) {
			directionOfRotateIsNegative = false;
			rubiksCube.RotateSide(5, 3);
			Display();
		}
	}
	}
}

void RotateRubicsCube(int key, int, int)
{
	switch (key) {
	case (GLUT_KEY_DOWN): {
		rotateX += 3.0f;
		if (rotateX >= 360.0f) rotateX -= 360.0f;
		glutPostRedisplay();
		break;
	}
	case (GLUT_KEY_UP): {
		rotateX -= 3.0f;
		if (rotateX < 0.0f) rotateX += 360.0f;
		glutPostRedisplay();
		break;
	}
	case (GLUT_KEY_RIGHT): {
		rotateY += 3.0f;
		if (rotateY >= 360.0f) rotateY -= 360.0f;
		glutPostRedisplay();
		break;
	}
	case (GLUT_KEY_LEFT): {
		rotateY -= 3.0f;
		if (rotateY < 0.0f) rotateY += 360.0f;
		glutPostRedisplay();
		break;
	}
	case (GLUT_KEY_PAGE_UP): {
		translateZ += 5.0f;
		glutPostRedisplay();
		break;
	}
	case (GLUT_KEY_PAGE_DOWN): {
		translateZ -= 5.0f;
		glutPostRedisplay();
		break;
	}
	}
}

void Timer(int id)
{
	glutTimerFunc(timer, Timer, 0);
	if (rubiksCube.currentRotatingSide != -1)
		rubiksCube.RotateSide(rubiksCube.currentRotatingSide, 3);
	Display();
}

void LoadTexture(GLuint* texture, const char* imageName)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    int width, height, cnt;
    unsigned char* data = stbi_load(imageName, &width, &height, &cnt, 0);
    if (data) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
            0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else std::cout << "Failed to load texture" << std::endl;
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLdouble aspect = (GLdouble)w / (GLdouble)h;
	gluPerspective(60.0, aspect, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(50, 50);
    glutInitWindowSize(800, 800);
    glutCreateWindow("OpenGL project 23");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

	GLuint textures[6];
	LoadTexture(&textures[0], "topSideTexture.png");
	LoadTexture(&textures[1], "bottomSideTexture.png");
	LoadTexture(&textures[2], "frontSideTexture.png");
	LoadTexture(&textures[3], "backSideTexture.png");
	LoadTexture(&textures[4], "leftSideTexture.png");
	LoadTexture(&textures[5], "rightSideTexture.png");

	glClearColor(0.2f, 0.4f, 0.6f, 0.0f);

	GLfloat materialSpecular[] = { 0.3f, 0.3f, 0.3f, 0.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glColorMaterial(GL_FRONT, GL_TRUE);
	GLfloat light0Diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
	GLfloat light0Ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
	GLfloat light0Зosition[] = { 40.0f, 40.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light0Зosition);
	glEnable(GL_LIGHT0);

					        // белый	 желтый	   синий	 зеленый   красный   оранжевый
	//unsigned int colors[9] = { 0xFFFFFF, 0xFFFF00, 0x0000FF, 0x00FF00, 0xFF0000, 0xFFA500 };
	unsigned int colors[9] = { 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF };
	rubiksCube.Reset(RubiksCubeSize, colors, textures);

    glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutTimerFunc(timer, Timer, 0);
	glutKeyboardFunc(RotateSides);
	glutSpecialFunc(RotateRubicsCube);

    glutMainLoop();

    return 0;
}