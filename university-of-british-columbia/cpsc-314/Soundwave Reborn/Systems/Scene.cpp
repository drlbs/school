/*
Author: Charles Bihis
Website: www.charlesbihis.com
Contact: admin@charlesbihis.com


Please read this Source Code License Agreement carefully before using
the source code.

You agree to indemnify, hold harmless and defend the author from and
against any loss, damage, claims or lawsuits, including attorney's
fees that arise or result from your use or distribution of the source
code.
	
THIS SOURCE CODE IS PROVIDED "AS IS" AND "WITH ALL FAULTS", WITHOUT
ANY TECHNICAL SUPPORT OR ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. ALSO, THERE IS NO WARRANTY OF
NON-INFRINGEMENT, TITLE OR QUIET ENJOYMENT. IN NO EVENT SHALL THE AUTHOR
OR ITS SUPPLIERS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOURCE CODE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#include <stdio.h>
#include <gl\glut.h>
#include <math.h>
#include "Scene.h"
#include "../Tools/DrawFunctions.h"

#define SKYBOX_MAX_TEXTURES 6
#define SKYFRONT 0
#define SKYBACK 1
#define SKYLEFT 2
#define SKYRIGHT 3
#define SKYUP 4
#define SKYDOWN 5


namespace
{
  Scene* scene_instance = NULL;
}


Scene* Scene :: ControlInstance()
{
  if (scene_instance == NULL)			// check if it is the first call
    scene_instance = new Scene();		// create single instance

  return scene_instance;				// address of single instance
}  // ControlInstance


Scene :: Scene()
{
  current_light = GL_LIGHT1;
  spotlight1_on = false;
  spotlight2_on = false;
  spotlight3_on = false;
}  // Scene


void Scene :: Draw()
{
	glPushMatrix();
		DrawFloor();
		DrawPlatform();
		DrawCageDome();
		DrawGlassDome();
	glPopMatrix();
}  // Draw


void Scene :: DrawSpotlight()
{
	GLfloat light_ambient[] = {0.05, 0.05, 0.05, 1.0};
	GLfloat light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat light_specular[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat light_position[] = {0, 15, 0, 1.0};
	GLfloat spot_direction[] = {-350, 700, 0};

	glLightfv(current_light, GL_AMBIENT, light_ambient);
	glLightfv(current_light, GL_DIFFUSE, light_diffuse);
	glLightfv(current_light, GL_SPECULAR, light_specular);
	glLightfv(current_light, GL_POSITION, light_position);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	glLightf(current_light, GL_SPOT_CUTOFF, 45.0);
	glLightfv(current_light, GL_SPOT_DIRECTION, spot_direction);
	glLightf(current_light, GL_SPOT_EXPONENT, 2.0);

	glPushMatrix();
		TaperedCylinder(13, 3, 3, 20);
		glTranslatef(0, 10, 0);
		glRotatef(45, 0, 0, 1);
		TaperedCylinder(0, 0, 12, 30);
		TaperedCylinder(20, 12, 12, 30);
		glTranslatef(0, 20, 0);
		TaperedCylinder(0, 0, 12, 30);
	glPopMatrix();
}  // DrawSpotlight


void Scene :: DrawFloorlight()
{
	GLfloat mat_ambient[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat mat_diffuse[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	GLdouble y_clip_pane[4] = {0.0, 1.0, 0.0, 0.0};		// clip lower half (y < 0)
	GLdouble x_clip_pane[4] = {1.0, 0.0, 0.0, 0.0};		// clip left half (x < 0)

	glClipPlane(GL_CLIP_PLANE0, y_clip_pane);
	glClipPlane(GL_CLIP_PLANE1, x_clip_pane);
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);

	glPushMatrix();
		glScalef(1.5, 0.75, 1);
		glutSolidSphere(12, 20, 20);
	glPopMatrix();

	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
}  // DrawFloorlight


void Scene :: DrawFloor()
{
	GLfloat mat_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat mat_diffuse[] = {0.2, 0.2, 0.2, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	GLdouble y_clip_pane[4] = {0.0, -1.0, 0.0, 0.0};
	glClipPlane(GL_CLIP_PLANE0, y_clip_pane);
	glEnable(GL_CLIP_PLANE0);

	glPushMatrix();
		glScalef(1, 0.3, 1);
		glutSolidSphere(1000, 30, 30);
	glPopMatrix();

	glDisable(GL_CLIP_PLANE0);

	glPushMatrix();
		TaperedCylinder(0, 1000, 0, 30);
		glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			glVertex3f(-380, 0, 925);
			glVertex3f(380, 0, 925);
			glVertex3f(380, 0, 1525);
			glVertex3f(-380, 0, 1525);
		glEnd();
	glPopMatrix();
}  // DrawFloor


void Scene :: DrawCageDome()
{
	GLdouble z_clip_pane[4] = {0.0, 0.0, -1.0, 925.0};
	GLdouble y_clip_pane[4] = {0.0, 1.0, 0.0, 0.0};
	glClipPlane(GL_CLIP_PLANE0, z_clip_pane);
	glClipPlane(GL_CLIP_PLANE4, y_clip_pane);
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE4);

	glPushMatrix();
		DrawVerticalGirder();
		glRotatef(20, 0, 1, 0);
		DrawVerticalGirder();
		glRotatef(20, 0, 1, 0);
		DrawVerticalGirder();
		glRotatef(20, 0, 1, 0);
		DrawVerticalGirder();
		glRotatef(20, 0, 1, 0);
		DrawVerticalGirder();
		glRotatef(20, 0, 1, 0);
		DrawVerticalGirder();
		glRotatef(20, 0, 1, 0);
		DrawVerticalGirder();
		glRotatef(20, 0, 1, 0);
		DrawVerticalGirder();
		glRotatef(20, 0, 1, 0);
		DrawVerticalGirder();
		glRotatef(20, 0, 1, 0);
	glPopMatrix();

	glPushMatrix();
		DrawHorizontalGirder(-5);
		DrawHorizontalGirder(100);
		DrawHorizontalGirder(200);
		DrawHorizontalGirder(300);
		DrawHorizontalGirder(400);
		DrawHorizontalGirder(500);
		DrawHorizontalGirder(600);
		DrawHorizontalGirder(700);
		DrawHorizontalGirder(800);
		DrawHorizontalGirder(900);
		DrawHorizontalGirder(1000);
	glPopMatrix();

	glDisable(GL_CLIP_PLANE0);

	glPushMatrix();
		glTranslatef(0, 0, 700 + 225);
		glRotatef(90, 1, 0, 0);
		TaperedCylinder(600, 380, 380, 30);
	glPopMatrix();

	glDisable(GL_CLIP_PLANE4);
}  // DrawDome


void Scene :: DrawGlassDome()
{
	GLfloat mat_transparent[] = { 0.5, 0.5, 0.5, 0.7 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent);
	glEnable (GL_BLEND);
	glDepthMask (GL_FALSE);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);
	glDepthMask (GL_TRUE);

	GLdouble y_clip_pane[4] = {0.0, 1.0, 0.0, 0.0};
	glClipPlane(GL_CLIP_PLANE0, y_clip_pane);
	glEnable(GL_CLIP_PLANE0);

	glScalef(0.99, 0.99, 0.99);
	glutSolidSphere(1000, 30, 30);

	glDisable(GL_CLIP_PLANE0);
	glDisable (GL_BLEND);
}  // DrawGlassDome


void Scene :: DrawVerticalGirder()
{
	GLfloat mat_ambient[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat mat_diffuse[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	GLdouble y_clip_pane[4] = {0.0, 1.0, 0.0, 0.0};		// clip lower half (y < 0)
	GLdouble x_clip_pane1[4] = {1.0, 0.0, 0.0, 25.0};
	GLdouble x_clip_pane2[4] = {-1.0, 0.0, 0.0, 25.0};

	glClipPlane(GL_CLIP_PLANE1, y_clip_pane);
	glClipPlane(GL_CLIP_PLANE2, x_clip_pane1);
	glClipPlane(GL_CLIP_PLANE3, x_clip_pane2);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);

	glutSolidSphere(1000, 20, 20);

	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
}  // DrawGirder


void Scene :: DrawHorizontalGirder(double start_value)
{
	GLfloat mat_ambient[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat mat_diffuse[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	GLdouble y_clip_pane1[4] = {0.0, 1.0, 0.0, -start_value};
	GLdouble y_clip_pane2[4] = {0.0, -1.0, 0.0, start_value + 25};

	glClipPlane(GL_CLIP_PLANE1, y_clip_pane1);
	glClipPlane(GL_CLIP_PLANE2, y_clip_pane2);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_CLIP_PLANE2);

	glutSolidSphere(1000, 20, 20);

	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
}  // DrawHorizontalGirder


void Scene :: DrawPlatform()
{
	GLfloat mat_ambient[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat mat_diffuse[] = {0.4, 0.4, 0.4, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glPushMatrix();
		glTranslatef(0, 30, 0);
		TaperedCylinder(0, 185, 0, 30);
		glTranslatef(0, -6, 0);
		TaperedCylinder(6, 185, 185, 30);
		TaperedCylinder(0, 195, 0, 30);
		glTranslatef(0, -6, 0);
		TaperedCylinder(6, 195, 195, 30);
		TaperedCylinder(0, 205, 0, 30);
		glTranslatef(0, -6, 0);
		TaperedCylinder(6, 205, 205, 30);
		TaperedCylinder(0, 215, 0, 30);
		glTranslatef(0, -6, 0);
		TaperedCylinder(6, 215, 215, 30);
		TaperedCylinder(0, 225, 0, 30);
		glTranslatef(0, -6, 0);
		TaperedCylinder(6, 225, 225, 30);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-45, 0, 1, 0);
		glTranslatef(185, 30, 0);
		current_light = GL_LIGHT1;
		DrawSpotlight();						// draw front-left spotlight
	glPopMatrix();
	glPushMatrix();
		glRotatef(45, 0, 1, 0);
		glTranslatef(-185, 30, 0);
		glRotatef(180, 0, 1, 0);
		current_light = GL_LIGHT2;
		DrawSpotlight();						// draw front-right spotlight
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 30, -185);
		glRotatef(90, 0, 1, 0);
		current_light = GL_LIGHT3;
		DrawSpotlight();						// draw back spotlight
	glPopMatrix();
}  // DrawPlatform


void Scene :: ToggleLight(unsigned int light)
{
	switch(light) 
	{
		case GLUT_KEY_F5:								// toggle front-left spotlight
			if (spotlight1_on)
				glDisable(GL_LIGHT1);
			else
				glEnable(GL_LIGHT1);

			spotlight1_on = !spotlight1_on;
			break;
		case GLUT_KEY_F6:								// toggle front-right spotlight
			if (spotlight2_on)
				glDisable(GL_LIGHT2);
			else
				glEnable(GL_LIGHT2);

			spotlight2_on = !spotlight2_on;
			break;
		case GLUT_KEY_F7:								// toggle back spotlight
			if (spotlight3_on)
				glDisable(GL_LIGHT3);
			else
				glEnable(GL_LIGHT3);

			spotlight3_on = !spotlight3_on;
			break;
	}  // switch statement
}  // ToggleLight


void Scene :: DrawPath()
{
	GLfloat mat_ambient[] = {0.8, 0.25, 0.25, 1.0};
	GLfloat mat_diffuse[] = {0.8, 0.25, 0.25, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glPushMatrix();
		glTranslatef(0, -217.0, 150);
		glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			glVertex3f(-50, 0, 0);
			glVertex3f(50, 0, 0);
			glVertex3f(50, 0, 5000);
			glVertex3f(-50, 0, 5000);
		glEnd();
	glPopMatrix();
}  // DrawPath


void Scene :: DrawSkybox(float x, float y, float z, float width, float height, float length)
{
		// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

		// Draw Front side
	glBindTexture(GL_TEXTURE_2D, skybox_textures[SKYFRONT]);
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

		// Draw Back side
	glBindTexture(GL_TEXTURE_2D, skybox_textures[SKYBACK]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

		// Draw Left side
	glBindTexture(GL_TEXTURE_2D, skybox_textures[SKYLEFT]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);		
	glEnd();

		// Draw Right side
	glBindTexture(GL_TEXTURE_2D, skybox_textures[SKYRIGHT]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

		// Draw Up side
	glBindTexture(GL_TEXTURE_2D, skybox_textures[SKYUP]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

		// Draw Down side
	glBindTexture(GL_TEXTURE_2D, skybox_textures[SKYDOWN]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();
}  // Draw_Skybox
