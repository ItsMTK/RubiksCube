/*
===============================================================

	Coded By Mohammad Hamdan ( M . T . K )
		Hope You Like It!!
			:)
===============================================================
*/


//#include <windows.h>		//for windows machines.

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include "vector"

using namespace std;

struct cube_rotate{
	GLfloat angle;		//the angle of the cube
	GLfloat x;		//position of the cube on x-axis
	GLfloat y;		//position of the cube on y-axis
	GLfloat z;		//position of the cube on z-axis
};

struct face_movements{
	bool* axi;
	int f_index;
	int s_index;
	GLfloat angle;
};

//in this list we'll store angless' roations of each cube
vector<cube_rotate> cube_rotations[3][3][3];



/*
=============================

	Constants

=============================
*/


const GLfloat teta = 45;
const GLfloat cube_size = 30;
const int rotate_delta = 5;
const GLfloat lining_error = 70.0f;
const int x_separator = 550;	//it depends on the screen
const int y_separator = 960;	//it depends on the screen

/*
==============================

	Movement
	Variables

==============================
*/


GLint rot_x = 0;
GLint rot_y = 0;
GLfloat fAspect;

bool x_effected_faces[3] = {false};	//faces could be selected to rotate on x-axis
bool y_effected_faces[3] = {false};	//faces could be selected to rotate on y-axis
bool z_effected_faces[3] = {false};	//faces could be selected to rotate on z-axis


int x_pointer_before = 0;
int x_pointer_after = 0;

int y_pointer_before = 0;
int y_pointer_after = 0;

face_movements movements[6][4] = {};

/*
=============================

	 Shaping
	Variables

=============================
*/


GLint gap = 2;


/*===========================*/

void init_selected_faces(){
	for(int i = 0; i < 3 ; i++){
		x_effected_faces[i] = false;
		y_effected_faces[i] = false;
		z_effected_faces[i] = false;
	}
}

inline void set_cam(){
	gluLookAt(0,80,300, 0,0,0, 0,1,0);
}

void init(){
	
	//front face
	movements[0][0] = {x_effected_faces , 0 , 2 , -90.0f};	//up movement
	movements[0][1] = {x_effected_faces , 0 , 2 ,  90.0f};	//down movement
	movements[0][2] = {y_effected_faces , 2 , 0 ,  90.0f};	//right movement
	movements[0][3] = {y_effected_faces , 2 , 0 , -90.0f}; 	//left movement
	
	//back face
	movements[1][0] = {x_effected_faces , 2 , 0 ,  90.0f};	//up movement
	movements[1][1] = {x_effected_faces , 2 , 0 , -90.0f};	//down movement
	movements[1][2] = {y_effected_faces , 2 , 0 ,  90.0f};	//right movement
	movements[1][3] = {y_effected_faces , 2 , 0 , -90.0f};	//left movement
		
	//left face

	movements[2][0] = {z_effected_faces , 0 , 2 , -90.0f};	//up movement
	movements[2][1] = {z_effected_faces , 0 , 2 ,  90.0f};	//down movement
	movements[2][2] = {y_effected_faces , 2 , 0 ,  90.0f};	//right movement
	movements[2][3] = {y_effected_faces , 2 , 0 , -90.0f};	//left movement
	
	
	//right face
	movements[3][0] = {z_effected_faces , 2 , 0 ,  90.0f};	//up movement
	movements[3][1] = {z_effected_faces , 2 , 0 , -90.0f};	//down movement
	movements[3][2] = {y_effected_faces , 2 , 0 ,  90.0f};	//right movement
	movements[3][3] = {y_effected_faces , 2 , 0 , -90.0f};	//left movement
	
	//top face
	movements[4][0] = {x_effected_faces , 0 , 2 , -90.0f};	//up movement
	movements[4][1] = {x_effected_faces , 0 , 2 ,  90.0f};	//down movement
	movements[4][2] = {z_effected_faces , 0 , 2 , -90.0f};	//right movement
	movements[4][3] = {z_effected_faces , 0 , 2 ,  90.0f};	//left movement
	
	//down face
	movements[5][0] = {x_effected_faces , 0 , 2 , -90.0f};	//up movement
	movements[5][1] = {x_effected_faces , 0 , 2 ,  90.0f};	//down movement
	movements[5][2] = {z_effected_faces , 2 , 0 ,  90.0f};	//right movement
	movements[5][3] = {z_effected_faces , 2 , 0 , -90.0f};	//left movement
	
		  
  	// init lighting
  	GLfloat ambient_lighte[4]={0.2,0.2,0.2,1.0}; 
  	GLfloat diffuse_light[4]={0.7,0.7,0.7,1.0};		// color
  	GLfloat specular_light[4]={1.0, 1.0, 1.0, 1.0};	// brightness
  	GLfloat light_position[4]={0.0, 50.0, 50.0, 1.0};

  	// material brightness capacity
  	GLfloat specularity[4]={1.0,1.0,1.0,1.0}; 
 	GLint material_specularity = 60;

  	// black background
  	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  
  	// Gouraud colorization model
  	glShadeModel(GL_SMOOTH);

  	// material reflectability
  	glMaterialfv(GL_FRONT,GL_SPECULAR, specularity);
	// brightness concentration
  	glMateriali(GL_FRONT,GL_SHININESS,material_specularity);

  	// activate ambient light
  	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_lighte);

  	// define light parameters
  	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_lighte); 
  	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light );
  	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light );
  	glLightfv(GL_LIGHT0, GL_POSITION, light_position );

  	// enable changing material color
  	glEnable(GL_COLOR_MATERIAL);
  	// enable lighting
  	glEnable(GL_LIGHTING);  
  	glEnable(GL_LIGHT0);
  	// enable depth buffering
  	glEnable(GL_DEPTH_TEST);

}


void apply_rotation(GLfloat angle){
	vector<cube_rotate> face[3][3];
	int axi = 0;
	int index = 0;
	cube_rotate rotation = {0 , 0.0 , 0.0 , 0.0};
	
	//selecting rotation axi

	for(int i = 0; i < 3 ; i++)
		if(x_effected_faces[i]){
			axi = 0;
			index = i;
			break;
		}
		else if(y_effected_faces[i]){
			axi = 1;
			index = i;
			break;
		}
		else if(z_effected_faces[i]){
			axi = 2;
			index = i;
			break;
		}

	int k;		
	//saving rotaions in their new places
	for(int i = 0; i < 3 ; i++)
		for(int j = 0; j < 3 ; j++){
			k = 2 - i % 3;
			if(axi == 0){					
				rotation = {angle , 1.0 , 0.0 , 0.0};
				face[i][j] = cube_rotations[index][k][j];
			}
			else if(axi == 1){					
				rotation = {angle , 0.0 , 1.0 , 0.0};
				face[i][j] = cube_rotations[j][index][k];	
			}
			else if(axi == 2){
				rotation = {angle , 0.0 , 0.0 , 1.0};
				face[i][j] = cube_rotations[j][k][index];	
			}
			
			face[i][j].push_back(rotation);
		}

	//stupid code to make rubik's cube able to solve!! Sorry :( 

	for(int i = 0; i < 3 ; i++){
		int j = 2 - i%3;
		if(axi == 0){
			if(angle < 0){
				cube_rotations[index][0][j] = face[j][0];
				cube_rotations[index][i][0] = face[0][i];
				cube_rotations[index][2][i] = face[i][2];
				cube_rotations[index][j][2] = face[2][j];
			}
			
			else{
				cube_rotations[index][0][i] = face[j][2];
				cube_rotations[index][i][2] = face[0][j];
				cube_rotations[index][2][j] = face[i][0];
				cube_rotations[index][j][0] = face[2][i];
			}
		}
		
		else if(axi == 1){
			if(angle < 0){
				cube_rotations[j][index][0] = face[j][0];
				cube_rotations[0][index][i] = face[0][i];
				cube_rotations[i][index][2] = face[i][2];
				cube_rotations[2][index][j] = face[2][j];
			}
			
			else{
				cube_rotations[i][index][0] = face[j][2];
				cube_rotations[2][index][i] = face[0][j];
				cube_rotations[j][index][2] = face[i][0];
				cube_rotations[0][index][j] = face[2][i];
			}
		}
		
		else if(axi == 2){
			if(angle > 0){
				cube_rotations[j][0][index] = face[j][0];
				cube_rotations[0][i][index] = face[0][i];
				cube_rotations[i][2][index] = face[i][2];
				cube_rotations[2][j][index] = face[2][j];
			}
			
			else{
				cube_rotations[i][0][index] = face[j][2];
				cube_rotations[2][i][index] = face[0][j];
				cube_rotations[j][2][index] = face[i][0];
				cube_rotations[0][j][index] = face[2][i];
			}	
		}	
	}

}		



void draw_cube(int x_step , int y_step , int z_step ){

	//rotation value of the wanted cube!!
	vector<cube_rotate> lrot = cube_rotations[x_step][y_step][z_step];

	glPushMatrix();
	
	glTranslatef(
		(x_step - 1) * cube_size + (x_step) * gap,
		(y_step - 1) * cube_size + (y_step) * gap,
		(z_step - 1) * cube_size + (z_step) * gap
	);
	
  	for(int i = lrot.size() - 1; i >= 0; --i)
    		glRotatef(lrot[i].angle, lrot[i].x, lrot[i].y, lrot[i].z);

	//drawing the cube
	glColor3f(0.950f, 0.0846f, 0.0846f);	//red
  	glBegin(GL_QUADS);  // front
    		glNormal3f(0.0, 0.0, 1.0);  // to the front
    		glVertex3f(cube_size/2, cube_size/2, cube_size/2);
    		glVertex3f(-cube_size/2, cube_size/2, cube_size/2);
    		glVertex3f(-cube_size/2, -cube_size/2, cube_size/2);
    		glVertex3f(cube_size/2, -cube_size/2, cube_size/2);
  	glEnd();

  	glColor3f(0.970f, 0.337f, 0.0873f);	//orange
  	glBegin(GL_QUADS);  // back
    		glNormal3f(0.0, 0.0, -1.0);  // to the back
    		glVertex3f(cube_size/2, cube_size/2, -cube_size/2);
    		glVertex3f(cube_size/2, -cube_size/2, -cube_size/2);
    		glVertex3f(-cube_size/2, -cube_size/2, -cube_size/2);
    		glVertex3f(-cube_size/2, cube_size/2, -cube_size/2);
  	glEnd();

  	glColor3f(0.230f, 0.557f, 0.720f);	//blue
  	glBegin(GL_QUADS);  // left
    		glNormal3f(-1.0, 0.0, 0.0);  // to the left
    		glVertex3f(-cube_size/2, cube_size/2, cube_size/2);
    		glVertex3f(-cube_size/2, cube_size/2, -cube_size/2);
    		glVertex3f(-cube_size/2, -cube_size/2, -cube_size/2);
    		glVertex3f(-cube_size/2, -cube_size/2, cube_size/2);
  	glEnd();
  
  	glColor3f(0.239f, 0.720f, 0.230f);	//green
  	glBegin(GL_QUADS);  // right
    		glNormal3f(1.0, 0.0, 0.0);  // to the right
    		glVertex3f(cube_size/2, cube_size/2, cube_size/2);
    		glVertex3f(cube_size/2, -cube_size/2, cube_size/2);
    		glVertex3f(cube_size/2, -cube_size/2, -cube_size/2);
    		glVertex3f(cube_size/2, cube_size/2, -cube_size/2);
  	glEnd();

  	glColor3f(0.923f, 0.920f, 0.920f);	//white
  	glBegin(GL_QUADS);  // top
    		glNormal3f(0.0, 1.0, 0.0);  // to the up
    		glVertex3f(-cube_size/2, cube_size/2, -cube_size/2);
   	 	glVertex3f(-cube_size/2, cube_size/2, cube_size/2);
   		glVertex3f(cube_size/2, cube_size/2, cube_size/2);
    		glVertex3f(cube_size/2, cube_size/2, -cube_size/2);
  	glEnd();

  	glColor3f(0.970f, 0.803f, 0.1582f);	//yellow
 	glBegin(GL_QUADS);  // bottom
    		glNormal3f(0.0, -1.0, 0.0);  // to the down
    		glVertex3f(-cube_size/2, -cube_size/2, -cube_size/2);
    		glVertex3f(cube_size/2, -cube_size/2, -cube_size/2);
    		glVertex3f(cube_size/2, -cube_size/2, cube_size/2);
    		glVertex3f(-cube_size/2, -cube_size/2, cube_size/2);
  	glEnd();
  	glPopMatrix();


}


//display function

void draw_func(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	set_cam();
	
	glRotatef(rot_x , 1.0 , 0.0 , 0.0);
	glRotatef(rot_y , 0.0 , 1.0 , 0.0);
	
	for(int i = 0 ; i < 3 ; i++)
		for(int j = 0 ; j < 3 ; j++)
			for(int k = 0 ; k < 3 ; k++)
				draw_cube(i , j , k);
				
	glutSwapBuffers();
}

//reshape function 

void reshape_func(GLsizei w , GLsizei h){

	glViewport(0 , 0 , w , h);
	fAspect = (GLfloat)w / (GLfloat)h;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(teta , fAspect , 0.4 , 500);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void keyboard_func(unsigned char key , int x , int y){

	switch(key){
	
		/*
		*=======================*
		|
		|	moving cube
		|
		*=======================*
		*/		
		
		case 'a': 	//moving left
		case 'A':	
			if(rot_x > 40 || rot_x < -40){
				break;
			}
			else		
				rot_y = (rot_y + rotate_delta) % 360;
			break;
			
		case 'd':	//moving right
		case 'D':
			if(rot_x > 40 || rot_x < -40){	
				break;
			}
			
			if(rot_y < 0)
				rot_y += 360;
			else
				rot_y = (rot_y - rotate_delta) % 360;
		
			
			
			break;
			
		case 'w':	//moving up
		case 'W':
			if(rot_x < 90 && rot_y >= -50 && rot_y < 50)
				rot_x += rotate_delta;
			break;
			
		case 's':	//moving down
		case 'S':
			if(rot_x > -90 && rot_y >= -40 && rot_y < 40)
				rot_x -= rotate_delta;
			break;
		

		/*========= Exit :( =========*/
		
		case 27:
			exit(0);	
			
	}
	
	glutPostRedisplay();

}


void select_movement(int face_index,  pair<int , int> start_point , pair<int , int> end_point){
	
	int angle = 0;
	int f_index = 0 , s_index = 0;
	int movement_index = 0;
	
	
	if(end_point.second - start_point.second <= 0 && abs(end_point.first - start_point.first) < lining_error){		//up
		movement_index = 0;
	}
					
	else if(end_point.second - start_point.second > 0 && abs(end_point.first - start_point.first) < lining_error){		//down
		movement_index = 1;	
	}
	
	else if(end_point.first - start_point.first > 0 && abs(end_point.second - start_point.second) < lining_error){		//right
		movement_index = 2;	
	}

					
	else if(end_point.first - start_point.first <= 0 && abs(end_point.second - start_point.second) < lining_error){		//left
		movement_index = 3;	
	}
	
	f_index = movements[face_index][movement_index].f_index;
	s_index = movements[face_index][movement_index].s_index;
	angle = movements[face_index][movement_index].angle;

	
	switch(movement_index){
		case 0: case 1:
			init_selected_faces();
			if(end_point.first <= y_separator)
				movements[face_index][movement_index].axi[f_index] = true;
				
			else
				movements[face_index][movement_index].axi[s_index] = true;
			break;
		
		case 2: case 3:
			init_selected_faces();
			if(end_point.second <= x_separator)
				movements[face_index][movement_index].axi[f_index] = true;
				
			else
				movements[face_index][movement_index].axi[s_index] = true;
			break;

	}		
	
	
	apply_rotation(angle);
	
}

void mouse_func(int btn , int state , int x , int y){

	switch(btn){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){
				x_pointer_before = x;
				y_pointer_before = y;
			}
			
			else if(state == GLUT_UP){
			
				x_pointer_after = x;
				y_pointer_after = y;	

				if((rot_y >= 45 && rot_y < 135) && (rot_x >= -45 && rot_x < 46)){	//left face
					
					select_movement(2 , {x_pointer_before , y_pointer_before} , {x_pointer_after , y_pointer_after});	
				}
				
				else if((rot_y >= 135 && rot_y < 225) && (rot_x >= -45 && rot_x < 46)){	//back face
					
					select_movement(1 , {x_pointer_before , y_pointer_before} , {x_pointer_after , y_pointer_after});
				}
				
				else if((rot_y >= 225 && rot_y < 315) && (rot_x >= -45 && rot_x < 46)){	//right face
					
					select_movement(3 , {x_pointer_before , y_pointer_before} , {x_pointer_after , y_pointer_after});
				}
				else if(rot_x >= -45 && rot_x < 46){	//front face
					
					select_movement(0 , {x_pointer_before , y_pointer_before} , {x_pointer_after , y_pointer_after});
				}
				else if(rot_x >= 46){	//up face
					
					select_movement(4 , {x_pointer_before , y_pointer_before} , {x_pointer_after , y_pointer_after});
				}
				
				else if(rot_x <= -46){	//down face
					
					select_movement(5 , {x_pointer_before , y_pointer_before} , {x_pointer_after , y_pointer_after});
				}
	
				

			}
			
			break;

	}
	glutPostRedisplay();
}



int main(int argc , char* argv[]){

	glutInit(&argc , argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1920 , 1080);
	glutCreateWindow("Rubic Cube!");
	glutFullScreen();

	init();


	glutDisplayFunc(draw_func);
	glutReshapeFunc(reshape_func);
	
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);

	glutMainLoop();
}
