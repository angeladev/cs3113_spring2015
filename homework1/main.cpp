
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) { 
	SDL_Surface *surface = IMG_Load(image_path);          
	GLuint textureID;     
	glGenTextures(1, &textureID);     
	glBindTexture(GL_TEXTURE_2D, textureID);          
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);          
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);     
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);         
	SDL_FreeSurface(surface);      
	return textureID; 
}

void DrawSprite(GLint texture, float x, float y, float rotation) { 
	glEnable(GL_TEXTURE_2D);    
	glBindTexture(GL_TEXTURE_2D, texture);          
	glMatrixMode(GL_MODELVIEW);        
	glLoadIdentity();   
	glTranslatef(x, y, 0.0); 
	glRotatef(rotation, 0.0, 0.0, 1.0); 
	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);     glEnableClientState(GL_VERTEX_ARRAY);   
	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };   
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);   
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);  
	glEnable(GL_BLEND);    
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);  
	glDisable(GL_TEXTURE_2D); 
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glMatrixMode(GL_MODELVIEW);

	glClearColor(0.15f, 0.55f, 0.5f, 1.0f);
	glViewport(0, 0, 800, 600);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	//Loading Textures...
	GLuint pinkAlien = LoadTexture("pinkAlien.png");
	GLuint greenAlien = LoadTexture("greenAlien.png");
	GLuint smiley = LoadTexture("cute.gif");

	//Keeping Time....
	float lastFrameTicks = 0.0f;
	float greenAngle = 0.0f; //rotation of green Alien dude

	bool done = false;
	SDL_Event event;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		SDL_GL_SwapWindow(displayWindow);
		glClear(GL_COLOR_BUFFER_BIT);

		//3 textures:
		//// 1. on animated element
		greenAngle += elapsed;
		DrawSprite(greenAlien, 0.0, 0.0, greenAngle);
		//// 2.
		DrawSprite(pinkAlien, 0.4, 0.6, 0.14);
		//// 3.
		DrawSprite(smiley, 0.75, 0.35, 0.0);

		//vertex color element
		GLfloat colorTri[] = {	0.0, 0.5, 0.5,
								0.9, 0.15, 0.35,
								0.9, 0.15, 0.35 }; 
		glColorPointer(3, GL_FLOAT, 0, colorTri);
		glEnableClientState(GL_COLOR_ARRAY);
		GLfloat triangle[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f }; 
		glVertexPointer(2, GL_FLOAT, 0, triangle);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		

	}

	SDL_Quit();
	return 0;
}