#include "Terrain.h"

void Terrain::init(){
	int x, z;
	xpos = 0;
	ypos = 0;
	//zpos = 0;
	scope = 0;
	tex_file = L"grass.bmp";
	for (x = 0; x <= 249; x++)
	for (z = 0; z <= 249; z++)
	{
		terrain[x][z].x = (float)(x - 125) * xscale;
		terrain[x][z].y = (float)(z - 125) * yscale;
		terrain[x][z].z = 0.6f + 2.0f*p->PerlinNoise_2D((x + 10000) / 10.0f, (z + 10000) / 10.0f);
		terrain[x][z].s = x / 250.0f;
		terrain[x][z].t = z / 250.0f;
	}

	for (x = 0; x <= 249; x++)
	for (z = 0; z <= 249; z++)
	{
		if (x>0 && z>0 && x<249 && z<249)//·¨Ïò¼ÆËã
		{
			vec v1;
			v1.x = terrain[x + 1][z].z - terrain[x - 1][z].z;
			v1.y = terrain[x][z + 1].z - terrain[x][z - 1].z;
			v1.z = 0.5f;
			v1.Normalize();
			terrain[x][z].norx = v1.x;
			terrain[x][z].nory = v1.y;
			terrain[x][z].norz = v1.z;
		}
		else
		{
			terrain[x][z].norx = 0.0f;
			terrain[x][z].nory = 0.0f;
			terrain[x][z].norz = 1.0f;
		}
	}
}

Terrain::Terrain(){
	p = new Perlin();
	tools = new Tools();
	init();
	ter_texture = tools->GetBmp(tex_file);
}

void Terrain::draw(){
	int z, x;
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat no_shininess[] = { 0.0 };
	GLfloat low_shininess[] = { 5.0 };
	GLfloat high_shininess[] = { 100.0 };
	GLfloat mat_emission[] = { 0.3, 0.2, 0.2, 0.0 };
	
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texid1);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	for (z = 0; z <= 248; z++)
	{
		glBegin(GL_QUAD_STRIP);
		for (x = 0; x <= 249; x++)
		{
			glNormal3f(terrain[x][z].norx, terrain[x][z].nory, terrain[x][z].norz);
			glTexCoord2f(terrain[x][z].s, terrain[x][z].t);
			glVertex3f(terrain[x][z].x, terrain[x][z].y, terrain[x][z].z);
			glNormal3f(terrain[x][z + 1].norx, terrain[x][z + 1].nory, terrain[x][z + 1].norz);
			glTexCoord2f(terrain[x][z + 1].s, terrain[x][z + 1].t);
			glVertex3f(terrain[x][z + 1].x, terrain[x][z + 1].y, terrain[x][z + 1].z);
		}
		glEnd();
	}
	glPopMatrix();

	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE);
	glColor4f(1, 1, 1, 1);
	int x1, z1;
	x1 = xpos / xscale + 125;
	z1 = ypos / yscale + 125;

	float zpos = getHeight(xpos, ypos);
	scope = zpos;
	if (scope <= 0)
		scope = 0;
	if (scope % 2 != 0)
		scope++;
	scope = scope % 50;

	if (130 + z1 - scope / 2<0)
		scope = 0;

	for (z = -5 + z1; z <= 5 + z1; z++)
	{
		glBegin(GL_QUAD_STRIP);
		for (x = -5 + x1; x <= 5 + x1; x++)
		{
			glTexCoord2f((x - 130 - x1 + scope / 2) / (18.0f + scope), (z - 130 - z1) / (18.0f + scope));
			glVertex3f(terrain[x][z].x, terrain[x][z].y, terrain[x][z].z - 0.2);
			glTexCoord2f((x - 130 - x1 + scope / 2) / (18.0f + scope), (z - 129 - z1) / (18.0f + scope));
			glVertex3f(terrain[x][z + 1].x, terrain[x][z + 1].y, terrain[x][z + 1].z - 0.2);
		}
		glEnd();
	}

	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
	//glEnable(GL_FOG);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void Terrain::setTex(){
	glGenTextures(1, &texid1);
	glBindTexture(GL_TEXTURE_2D, texid1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, ter_texture.bmWidth, ter_texture.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, ter_texture.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

float Terrain::getHeight(float x, float y){
	float result;
	int x1 = x / xscale + 125;
	int x2 = x / xscale + 126;
	int y1 = y / yscale + 125;
	int y2 = y / yscale + 126;
	if (x1 > 250 || x1 < 0 || y1 > 250 || y1 <0 || x2 > 250 || x2 < 0 || y2 > 250 || y2 < 0)
		return 0;
	float rx1 = terrain[x1][y1].x;
	float ry1 = terrain[x1][y1].y;
	float rz1 = terrain[x1][y1].z;
	float rx2 = terrain[x2][y2].x;
	float ry2 = terrain[x2][y2].y;
	float rz2 = terrain[x2][y2].z;
	float ratio = (x - rx1) / (rx2 - rx1);
	result = rz1 + (rz2 - rz1) * ratio;
	return result;
}

void Terrain::setPos(float x, float y){
	xpos = x;
	ypos = y;
}
