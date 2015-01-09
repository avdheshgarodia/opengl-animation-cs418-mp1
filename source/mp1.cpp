#include <GLUT/GLUT.h>
#include <iostream>
#include <math.h>


/*
	MP1-Avdhesh Garodia
	CS418-Fall 2014
*/


void render(void);
void keyboard(unsigned char key, int x, int y);
void tick(int m);

//Time variable used for the sin calculations
static float t = 0.0; 

//The X and Y coordinates for the Vertices of the I
static float xCo[12] = {-0.37,-0.37,0.37,0.37,0.15,0.15,0.37,0.37,-0.37,-0.37,-0.15,-0.15};
static float yCo[12] = {0.5,0.8,0.8,0.5,0.5,-0.5,-0.5,-0.8,-0.8,-0.5,-0.5,0.5};

//The order in which the Vertices are to be called for a Triangle Strip
static int fillorder[16] = {0,11,1,4,2,3,4,11,5,10,9,10,8,5,7,6};

/*The X and Y offsets for the animation, 
12 rows for the 12 vertices in the I and 10 columns
for the 10 dance keyframes/positions. Theese will
be linearly interpolated in the animation
*/
static float xKey[12][10]={
/*0*/ 	{0.0, -0.1, -0.2 ,-0.1,0.0 ,-0.1,-0.1 ,-0.1 ,-0.1,-0.1},
/*1*/	{0.0, 0.1 , 0.0  ,0.1 ,0.2 ,0.1 ,0.0  ,-0.1 ,0.1 ,0.1 },
/*2*/	{0.0, -0.1, -0.2 ,-0.1,0.0 ,-0.1,0.1  ,0.1  ,-0.1,-0.1},
/*3*/	{0.0, 0.1 , 0.0  ,0.1 ,0.2 ,0.1 ,-0.1 ,0.1  ,0.01,0.1 },
/*4*/	{0.0, 0.0 , -0.1 ,0.0 ,0.0 ,0.0 ,0.0  ,0.1  ,0.0 ,0.0 },
/*5*/	{0.0, 0.0 , 0.1  ,0.0 ,-0.1,0.0 ,0.1  ,-0.05 ,0.0 ,0.0 },					
/*6*/	{0.0, 0.1 , 0.2  ,0.1 ,0.0 ,0.1 ,0.0  ,0.1  ,0.1 ,0.1 },
/*7*/	{0.0, -0.1, 0.0  ,-0.1,-0.2,-0.1,-0.1 ,0.2  ,-0.1,-0.1},
/*8*/	{0.0, 0.1 , 0.2  ,0.1 ,0.0 ,0.1 ,0.0  ,-0.1 ,0.1 ,0.1 },
/*9*/	{0.0, -0.1, 0.0  ,-0.1,-0.2,-0.1,0.1  ,-0.1 ,-0.1,-0.1},
/*10*/	{0.0, 0.0 , 0.1  ,0.0 ,-0.1,0.0 ,0.0  ,-0.05,0.1 ,0.0 },
/*11*/	{0.0, 0.0 , -0.1 ,0.0 ,0.0 ,0.0 ,-0.1 ,0.1  ,-0.1 ,0.0}};			

static float yKey[12][10]={
/*0*/ 	{0.0,0.1,0.0 ,0.0,0.0 ,0.0, -0.2 ,-0.1,0.0,0.0 },
/*1*/	{0.0,0.0,0.0 ,0.0,-0.1,0.0, -0.2 ,0.0 ,0.0,0.0 },
/*2*/	{0.0,-0.1,0.0 ,0.0,-0.1,0.0, -0.2 ,0.1 ,0.0,0.0 },
/*3*/	{0.0,0.0,0.0 ,0.0,0.1 ,0.0, -0.2 ,0.0 ,0.0,0.0 },
/*4*/	{0.0,-0.1,-0.1,0.0,0.0 ,0.0, -0.1 ,0.0 ,0.0,-0.3},
/*5*/	{0.0,0.1,0.1 ,0.0,-0.1,0.0, 0.1  ,0.0 ,0.0,0.3 },					
/*6*/	{0.0,0.1,0.0 ,0.1,0.0 ,0.0, 0.2  ,-0.1,0.0,0.0 },
/*7*/	{0.0,-0.2,0.0 ,0.1,0.1 ,0.0, 0.2  ,0.1 ,0.0,0.0 },
/*8*/	{0.0,-0.1,0.0 ,0.1,0.0 ,0.0, 0.2  ,0.1 ,0.0,0.0 },
/*9*/	{0.0,0.1,0.0 ,0.1,0.1 ,0.0, 0.2  ,-0.1,0.0,0.0 },
/*10*/	{0.0,0.0,0.1 ,0.0,0.0 ,0.0, 0.1  ,-0.1,0.0,0.3 },
/*11*/	{0.0,-0.1,-0.1,0.0,0.0 ,0.0, -0.1 ,0.0 ,0.0,-0.3}};		

//Boolean that is true when animation is running. False when animation is paused	
static bool running=true;
//Boolean for wireframe On/Off
static bool wireframe=false;
//current and previous frame of the animation
static int animstepprev = 0;
static int animstep = 1;
//The current frame 
static int currframe = 0;
//The amount of frames each step will use. Dance will be slower in this is higher
static int framesinstep =11;

//Dimensions of window
const static int WIDTH = 640;
const static int HEIGHT = 480;
	
int main(int argc, char** argv){

	
	//initialize the window
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(WIDTH,HEIGHT);
	
	//set title of window
	glutCreateWindow("MP1: Dancing I");
		
	
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(33,tick,0);
	
	//Enable line smoothing 
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POLYGON_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//set the line width	
	glLineWidth(2.7);
	
	//set the background color to WHITE
	glClearColor(1.0,1.0,1.0,1.0);
	
	//Start the main loop			
	glutMainLoop();
		
}

/*The Tick function called every 
33 milliseconds
*/
void tick(int m){
	
		
			
	//Redraw the display
	glutPostRedisplay();
	
	if(running){
		
		//increment the time;
		t=t+.2;	
		
		//if the current frame reaches the frame limit swicth to next position
		if(currframe==framesinstep){
			//reset current frame
			currframe=0;
			//set animstepprev to current animstep and increment animstep
			animstepprev=animstep;
			animstep=(animstep+1)%10;
		}
		//increment current frame
		currframe=currframe+1;
	}
	/*recall timer func in 33 milliseconds
	redraw every 33 milliseconds which is 
	approximatly=1000/33=30 frames per seconds
	*/
	glutTimerFunc(33,tick,0);
	
}


void keyboard(unsigned char key, int x, int y){
	
	//if key is 27 esacpe is pressed 	
	if(key==27){
		//if escape is pressed exit the program
		exit(0);
	}
	//p key toggles the pause
	else if(key=='p'){
		
		running=!running;
	}
	//o key toggles the wireframe
	else if(key=='o'){
			
		wireframe=!wireframe;

	}else if(key=='w'){
		
		framesinstep++;
	}
	else if(key=='s'){
			
		framesinstep--;

	}
	
}


void render(void){
	//x and y offsets for the vertices causes oscillations
	float x= 0.03 * sin(t-1.5);
	float y = 0.03 * sin(t - 0.5);	
	
	//clears the sreen so that new stuff can be drawn
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//if wireframe is on set Polygon mode to Lines
	if(wireframe==true){
		glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
	}
	//if wireframe is on set Polygon mode to  FIll
	if(wireframe==false){
		glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
	}
	//Start the triangle strip		
	glBegin(GL_TRIANGLE_STRIP);
	
	//Draw the first 6 vertices
	for(int i=0;i<6;i++){
		
		//Set colors of the vertices
		if(fillorder[i]==0 || fillorder[i]==3){
			glColor3f(0.31,0.01,0.01);
		}
		else{
			glColor3f(0.64,0.01,0.01);
		} 
		//compute to x and y offsets caused by linear interpolation
		//xOff = (x2 - x1)* (currframe/framesneeded)			
	   float xOff = ((xKey[fillorder[i]][animstep] - xKey[fillorder[i]][animstepprev]) * (float)(((float)currframe) / ((float)framesinstep))) + xKey[fillorder[i]][animstepprev];						
	   float yOff = ((yKey[fillorder[i]][animstep] - yKey[fillorder[i]][animstepprev]) * (float)(((float)currframe) / ((float)framesinstep))) + yKey[fillorder[i]][animstepprev];
		
	   //The actual vertex calls
	   glVertex2f(xCo[fillorder[i]]+x+xOff,yCo[fillorder[i]]+y+yOff);
	}
	//end the gl call
	glEnd();
	
	glBegin(GL_TRIANGLE_STRIP);
	
	//Draw the next 4 vertices
	for(int i=6;i<10;i++){
		
		glColor3f(0.64,0.01,0.01);
		
	    float xOff = ((xKey[fillorder[i]][animstep] - xKey[fillorder[i]][animstepprev]) * (float)(((float)currframe) / ((float)framesinstep))) + xKey[fillorder[i]][animstepprev];
			
			
	   float yOff = ((yKey[fillorder[i]][animstep] - yKey[fillorder[i]][animstepprev]) * (float)(((float)currframe) / ((float)framesinstep))) + yKey[fillorder[i]][animstepprev];
		
		
		
		glVertex2f(xCo[fillorder[i]]+x+xOff,yCo[fillorder[i]]+y+yOff);
		
	}
	
	glEnd();
	
	
	glBegin(GL_TRIANGLE_STRIP);
	
	//Draw the next 6 vertices
	for(int i=10;i<16;i++){
		
		if(fillorder[i]==5 || fillorder[i]==10){
			glColor3f(0.64,0.01,0.01);
		}
		else{
			glColor3f(0.22,0.09,0.18);
		} 
		
	    float xOff = ((xKey[fillorder[i]][animstep] - xKey[fillorder[i]][animstepprev]) * (float)(((float)currframe) / ((float)framesinstep))) + xKey[fillorder[i]][animstepprev];
			
			
	   float yOff = ((yKey[fillorder[i]][animstep] - yKey[fillorder[i]][animstepprev]) * (float)(((float)currframe) / ((float)framesinstep))) + yKey[fillorder[i]][animstepprev];
		
		
		
		glVertex2f(xCo[fillorder[i]]+x+xOff,yCo[fillorder[i]]+y+yOff);
		
	}
	
	glEnd();
	
	/*if the wireframe is false draw in 
	a line loop around all the vertices
	*/
	
	if(wireframe==false){
		glBegin(GL_LINE_LOOP);
		
		//set line color to black
		glColor3f(0.0,0.0,0.0);
		//loop through all 12 vertices
		for(int i=0;i<12;i++){
			
		   float xOff = ((xKey[i][animstep] - xKey[i][animstepprev]) * (float)(((float)currframe) / ((float)framesinstep))) + xKey[i][animstepprev];
		   float yOff = ((yKey[i][animstep] - yKey[i][animstepprev]) * (float)(((float)currframe) / ((float)framesinstep))) + yKey[i][animstepprev];
	
		    glVertex2f(xCo[i]+x+xOff,yCo[i]+y+yOff);
		}
		glEnd();
	}
	
	//swap the buffers so the animation does not flicker
	glutSwapBuffers();	
}

