//g++ main.cpp $(pkg-config --cflags --libs sdl2) -lglut -lGL -lGLEW -g
#include "initShaders.h"
#include <cstdlib>
using namespace std;

void rotate(GLuint locate);

GLuint vaoID,vboID,eboID, vbo_texcoords;
GLuint program, texture_id;
GLint uniform_mytexture;
GLint attribute_texcoord;

GLfloat pit,yaw,scalar=1;
glm::vec3 cubeTran;


GLfloat size=10;

GLfloat texcoords[2*4*6] = {
-size, -size,
size, -size,
size, size,
-size, size,
};

GLfloat vertexarray[]={
-size, -size, 0,
size, -size, 0,
size, size, 0,
-size, size, 0,
};

GLubyte elems[] = {
0, 1, 2, 3
};


void init(){

//Generate some weird texture data
/*GLubyte imageData[]={
0x00,0x00,0x00,	0x00,0x00,0x00,
0x00,0x00,0x00,	0x00,0x00,0x00
};*/
GLubyte image[64][64][3];
// Create a 64 x 64 checkerboard pattern
for (int i= 0; i < 64; i++ ) {
for (int j = 0; j < 64; j++ ) {
GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
image[i][j][0] = 0;
image[i][j][1] = c;
image[i][j][2] = 255-c;
}
}

glEnable(GL_DEPTH_TEST);  
glViewport(0, 0, 600, 600);
  
glGenTextures(1, &texture_id);
glBindTexture(GL_TEXTURE_2D, texture_id);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D,0, 3,64,64,0,GL_RGB,GL_UNSIGNED_BYTE,image);

glGenVertexArrays(1,&vaoID);
glBindVertexArray(vaoID);

glGenBuffers(2, &vboID);
glBindBuffer(GL_ARRAY_BUFFER,vboID);
glBufferData(GL_ARRAY_BUFFER,sizeof(vertexarray),vertexarray,GL_STATIC_DRAW);
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

glGenBuffers(1,&eboID);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboID);
glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elems),elems,GL_STATIC_DRAW);

glGenBuffers(1, &vbo_texcoords);
glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords);
glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

ShaderInfo shaders[]={
    { GL_VERTEX_SHADER , "vertexshader.glsl"},
    { GL_FRAGMENT_SHADER , "fragmentshader.glsl"},
    { GL_NONE , NULL}
};

program=initShaders(shaders);
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);

attribute_texcoord = glGetAttribLocation(program, "texcoord");
glEnableVertexAttribArray(attribute_texcoord);
glVertexAttribPointer(attribute_texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


void display(SDL_Window* screen){
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

glm::mat4 trans;

trans=glm::translate(trans,cubeTran);//translate the cube
trans=glm::rotate(trans,pit,glm::vec3(1,0,0));//rotate the cube around the x axis
trans=glm::rotate(trans,yaw,glm::vec3(0,1,0));//rotate the cube arround the y axis
trans=glm::scale(trans,glm::vec3(scalar));//scaling the cube
    
GLint tempLoc = glGetUniformLocation(program,"modelMatrix");//Matrix that handle the transformations
glUniformMatrix4fv(tempLoc,1,GL_FALSE,&trans[0][0]);

glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture_id);
uniform_mytexture = glGetUniformLocation(program, "texture");
glUniform1i(uniform_mytexture, 0);

glDrawElements(GL_QUADS,4,GL_UNSIGNED_BYTE,NULL);
glFlush();
SDL_GL_SwapWindow(screen);
}

void input(SDL_Window* screen){

SDL_Event event;

while (SDL_PollEvent(&event)){//Handling the keyboard
    switch (event.type){
    case SDL_QUIT:exit(0);break;
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym){
      case SDLK_ESCAPE:exit(0);
      case SDLK_w:cubeTran.y+=2;break;
      case SDLK_s:cubeTran.y-=2;break;
      case SDLK_a:cubeTran.x-=2;break;
      case SDLK_d:cubeTran.x+=2;break;
      case SDLK_e:scalar+=.1f;break;
      case SDLK_q:scalar-=.1f;break;
      case SDLK_i:pit+=.2;break;
      case SDLK_k:pit-=.2;break;
      case SDLK_j:yaw+=.2;break;
      case SDLK_l:yaw-=.2;break;
      }
    }
  }
}


int main(int argc, char **argv){

SDL_Window *window;

if(SDL_Init(SDL_INIT_VIDEO)<0){//initilizes the SDL video subsystem
    fprintf(stderr,"Unable to create window: %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);//die on error
}

window = SDL_CreateWindow(
                            "Example", //Window title
                            SDL_WINDOWPOS_UNDEFINED, //initial x position
                            SDL_WINDOWPOS_UNDEFINED, //initial y position
                            500,	//width, in pixels
                            500,	//height, in pixels
                            SDL_WINDOW_OPENGL	//flags to be had
                            );

if(window==NULL){
    fprintf(stderr,"Unable to create window: %s\n",SDL_GetError());
}

SDL_GLContext glcontext=SDL_GL_CreateContext(window);

glewExperimental=GL_TRUE;
if(glewInit()){
    fprintf(stderr, "Unable to initalize GLEW");
    exit(EXIT_FAILURE);
}
  
init();

while(true){
    input(window);//keyboard controls
    display(window);//displaying
}

SDL_GL_DeleteContext(glcontext);
SDL_DestroyWindow(window);
SDL_Quit();
 
return 0;
}
