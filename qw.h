/* QUICK GRAPHICS - Christian Schäl 2016 */
#ifndef QUICK_GRAPHICS_H
#define QUICK_GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define QW_KEY(a) SDL_SCANCODE_##a

#define QW_RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

/* Width and Height of the current window */
int qw_width, qw_height;
SDL_Window *qw_window = NULL;
SDL_Renderer *qw_renderer = NULL;
SDL_Rect qw_screen_rect;
const Uint8 *qw_keys;
Uint32 qw_mouse;
int qw_mousex, qw_mousey, qw_mousex_last, qw_mousey_last;
int qw_mousex_next, qw_mousey_next;

int qw_is_running;

/*
 * Quits the application
 */
void qw_quit() {
	qw_is_running = 0;

	SDL_DestroyRenderer(qw_renderer);
	SDL_DestroyWindow(qw_window);
	SDL_Quit();
}

/*
 * Create Window and Renderer
 */
int qw_screen(int width, int height, int fullscreen, const char *title) {
	qw_width = width;
	qw_height = height;
	qw_screen_rect = (SDL_Rect) {0, 0, qw_width, qw_height};

	/* Try initializing SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
		printf("SDL_Init failed! %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	
	/* Quit SDL when application exits */
	atexit(qw_quit);
	
	Uint32 windowflags = SDL_WINDOW_SHOWN;
	if (fullscreen) {
		windowflags |= SDL_WINDOW_FULLSCREEN;
	}

	/* Create window */
	qw_window = SDL_CreateWindow(title,
								 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								 width, height,
								 windowflags);
	if (qw_window == NULL) {
		printf("SDL_CreateWindow failed! %s\n", SDL_GetError());
		return 2;
	}

	/* Create renderer */
	qw_renderer = SDL_CreateRenderer(qw_window, -1, QW_RENDERER_FLAGS);
	if (qw_renderer == NULL) {
		printf("SDL_CreateRenderer failed! %s\n", SDL_GetError());
		return 2;
	}

	qw_is_running = 1;
	return 0;
}

/*
 * Returns if a coordinate is on the screen
 */
int qw_onscreen(int x, int y) {
	return !(x < 0 || y < 0 || x >= qw_width || x >= qw_height);
}

/*
 * Clears the screen with color r,g,b
 */
void qw_fill(int r, int g, int b) {
	SDL_SetRenderDrawColor(qw_renderer, r, g, b, 255);
	SDL_RenderFillRect(qw_renderer, &qw_screen_rect);
}

/*
 * Redraws the renderer on the window
 */
void qw_redraw() {
	SDL_RenderPresent(qw_renderer);
}

/*
 * Waits `millis` milliseconds
 */
void qw_delay(int millis) {
	SDL_Delay(millis);
}

/*
 * Clears the renderer
 */
void qw_clear() {
	SDL_RenderClear(qw_renderer);
}

/*
 * Returns if qw is still running and handles io etc logic
 */
int qw_running() {
	/* Give cpu some time to rest */
	SDL_Delay(5);
	
	/* Handle events */
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			qw_is_running = 0;
	}
	
	/* Read keys */
	qw_mousex_last = qw_mousex;
	qw_mousey_last = qw_mousey;
	qw_mouse = SDL_GetMouseState(&qw_mousex, &qw_mousey);
	qw_mousex_next = qw_mousex_last - qw_mousex;
	qw_mousey_next = qw_mousey_last - qw_mousey;

	qw_keys = SDL_GetKeyboardState(NULL);
	return qw_is_running;
}

/*
 * Returns if a key is pressed. Does not respect shift+[key]
 */
int qw_keydown(int key) {
	return qw_keys[key];
}

/*
 * Get mouse state
 */
void qw_getmouse(int *x, int *y) {
	*x = qw_mousex;
	*y = qw_mousey;
}

/*
 * Check if mousebutton is pressd
 */
int qw_mousedown(int button) {
	return qw_mouse & SDL_BUTTON(button);
}


/**********************************\
 * DRAWING AND GRAPHICS FUNCTIONS *
\**********************************/

/*
 * Set draw color
 */
void qw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_SetRenderDrawColor(qw_renderer, r, g, b, a);
}

/*
 * Draw a pixel at x,y
 */
void qw_drawpixel(int x, int y) {
	SDL_RenderDrawPoint(qw_renderer, x, y);
}

/*
 * Fills a rect at x,y with dimensions w,h
 */
void qw_fillrect(int x, int y, int w, int h) {
	SDL_Rect rect = {x, y, w, h};
	SDL_RenderFillRect(qw_renderer, &rect);
}

/*
 * Draws a rect at x,y with dimensions w,h
 */
void qw_drawrect(int x, int y, int w, int h) {
	SDL_Rect rect = {x, y, w, h};
	SDL_RenderDrawRect(qw_renderer, &rect);
}

/*
 * Draws a line from x1,y1 to x2,y2
 */
void qw_drawline(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(qw_renderer, x1, y1, x2, y2);
}

<<<<<<< HEAD
/**********\
 * IMAGES *
\**********/

typedef struct {
	SDL_Texture *texture;
	SDL_Rect *src,
	         *dst;
} qw_image;

/*
 * Loads qw_image from path
 */
qw_image qw_loadimage(const char *fn) {
	SDL_Surface *img_s = IMG_Load(fn);
	if (!img_s) {
		printf("IMG_Load error: %s\n", IMG_GetError());
		return (qw_image) {
			NULL, NULL, NULL
		};
	}
	
	qw_image img = {
		.texture = SDL_CreateTextureFromSurface(qw_renderer, img_s),
		.src = malloc(sizeof(SDL_Rect)),
		.dst = malloc(sizeof(SDL_Rect))
	};
	
	img.src->x = 0;
	img.src->y = 0;
	img.src->w = img_s->w;
	img.src->h = img_s->h;
	
	img.dst->x = 0;
	img.dst->y = 0;
	img.dst->w = img_s->w;
	img.dst->h = img_s->h;


	SDL_FreeSurface(img_s);

=======
/*****************************\
 * IMAGE LOADING AND DRAWING *
\*****************************/

/*
 * Image holding information:
 * - rgba texture
 * - source rect
 * - destination rect
 */
typedef struct {
	SDL_Rect *dst,
	         *src;
	
	SDL_Texture *texture;
} qw_image;

/*
 * Loads image from filename
 */
SDL_Texture *qw_loadtexture(const char *fn) {
	SDL_Surface *img_s = IMG_Load(fn);
	/* check if image loading failed */
	if (!img_s) {
		printf("IMG_Load: %s\n", IMG_GetError());
		return NULL;
	}

	SDL_Texture *img = SDL_CreateTextureFromSurface(qw_renderer, img_s);
	SDL_FreeSurface(img_s);
>>>>>>> 3243c51e36087a84b8e142402f82d60e523f709f
	return img;
}

/*
<<<<<<< HEAD
 * Frees all resources used by qw_image `img`
 */
void qw_destroyimage(qw_image img) {
	free(img.src);
	free(img.dst);
	SDL_DestroyTexture(img.texture);
}

/*
 * Draws qw_image to the screen
=======
 * Loads qw_image from filename
 */
qw_image qw_loadimage(const char *fn) {
	qw_image img = {
		.texture = qw_loadtexture(fn),
		.dst = (SDL_Rect*)malloc(sizeof(SDL_Rect)),
		.src = (SDL_Rect*)malloc(sizeof(SDL_Rect))
	};

	img.dst->x = 10;
	img.dst->y = 10;
	img.dst->w = 100;
	img.dst->h = 70;
	
	img.src = NULL;

	return img;
}

/*
 * Draws a qw_image to the screen
>>>>>>> 3243c51e36087a84b8e142402f82d60e523f709f
 */
void qw_drawimage(qw_image img) {
	SDL_RenderCopy(qw_renderer, img.texture, img.src, img.dst);
}

/*
<<<<<<< HEAD
 * Moves qw_image destination rect dx,dy pixels
 */
void qw_moveimage(qw_image img, int dx, int dy) {
	img.dst->x += dx;
	img.dst->y += dy;
}

/*
 * Moves qw_image destination rect to coordinates x,y
 */
void qw_moveimage_to(qw_image img, int x, int y) {
=======
 * Frees resources taken by texture
 */
void qw_destroyimage(qw_image img) {
	free(img.dst);
	free(img.src);
	SDL_DestroyTexture(img.texture);
}

/*
 * Set image position
 */
void qw_setimage(qw_image img, int x, int y) {
>>>>>>> 3243c51e36087a84b8e142402f82d60e523f709f
	img.dst->x = x;
	img.dst->y = y;
}

<<<<<<< HEAD
=======
/*
 * Move image position
 */
void qw_moveimage(qw_image img, int dx, int dy) {
	img.dst->x += dx;
	img.dst->y += dy;
}

>>>>>>> 3243c51e36087a84b8e142402f82d60e523f709f
#endif


