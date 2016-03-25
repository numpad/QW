#ifndef QUICK_GRAPHICS_H
#define QUICK_GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

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
float qw_renderer_scalex, qw_renderer_scaley;

#define qw_mousex_scaled (qw_mousex / qw_renderer_scalex)
#define qw_mousey_scaled (qw_mousey / qw_renderer_scaley)

#define QW_MOUSE_LEFT SDL_BUTTON_LEFT
#define QW_MOUSE_MIDDLE SDL_BUTTON_MIDDLE
#define QW_MOUSE_RIGHT SDL_BUTTON_RIGHT

unsigned long long qw_tick_count;
int qw_is_running;

int qw_rgba_red, qw_rgba_green, qw_rgba_blue, qw_rgba_alpha;

/* Quits the application */
void qw_quit() {
	qw_is_running = 0;
	
	SDL_DestroyRenderer(qw_renderer);
	SDL_DestroyWindow(qw_window);
	SDL_Quit();
}

/* Create scaled Window and Renderer */
int qw_screen_scaled(int width, int height, int fullscreen, const char *title, float scale_x, float scale_y) {
	qw_renderer_scalex = scale_x;
	qw_renderer_scaley = scale_y;
	qw_width = (float)width / scale_x;
	qw_height = (float)height / scale_y;
	qw_screen_rect = (SDL_Rect) { 0, 0, qw_width, qw_height };

	/* Try initializing SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
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
	
	/* Apply render scale */
	if (scale_x != 1.0 && scale_y != 1.0)
		SDL_RenderSetScale(qw_renderer, scale_x, scale_y);

	qw_is_running = 1;
	return 0;
}


/* Create Window and Renderer */
int qw_screen(int width, int height, int fullscreen, const char *title) {
	return qw_screen_scaled(width, height, fullscreen, title, 1.0, 1.0);
}

/* Reset tick_count to 0 */
void qw_resetticks() {
	qw_tick_count = 0;
}

/* Returns if a coordinate is on the screen */
int qw_onscreen(int x, int y) {
	return !(x < 0 || y < 0 || x >= qw_width || x >= qw_height);
}

/* Clears the screen with color r,g,b */
void qw_fill(int r, int g, int b) {
	SDL_SetRenderDrawColor(qw_renderer, r, g, b, 255);
	SDL_RenderFillRect(qw_renderer, &qw_screen_rect);
}

/* Redraws the renderer on the window */
void qw_redraw() {
	SDL_RenderPresent(qw_renderer);
}

/* Waits `millis` milliseconds */
void qw_delay(int millis) {
	SDL_Delay(millis);
}

/* Clears the renderer */
void qw_clear() {
	SDL_RenderClear(qw_renderer);
}

/* Returns if qw is still running and handles io etc logic */
int qw_running() {
	/* Give cpu some time to rest */
	SDL_Delay(5);
	++qw_tick_count;
	
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

/* Returns if a key is pressed. Does not respect shift+[key] */
int qw_keydown(int key) {
	return qw_keys[key];
}

/* Get mouse state */
void qw_getmouse(int *x, int *y) {
	*x = qw_mousex;
	*y = qw_mousey;
}

/* Check if mousebutton is pressd */
int qw_mousedown(int button) {
	return qw_mouse & SDL_BUTTON(button);
}

/*
 * DRAWING AND GRAPHICS FUNCTIONS
 */

/* Set draw color */
void qw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	qw_rgba_red = r;
	qw_rgba_green = g;
	qw_rgba_blue = b;
	qw_rgba_alpha = a;
	SDL_SetRenderDrawColor(qw_renderer, r, g, b, a);
}

/* Draw a pixel at x,y */
void qw_drawpixel(int x, int y) {
	SDL_RenderDrawPoint(qw_renderer, x, y);
}

/* Fills a rect at x,y with dimensions w,h */
void qw_fillrect(int x, int y, int w, int h) {
	SDL_Rect rect = {x, y, w, h};
	SDL_RenderFillRect(qw_renderer, &rect);
}

/* Draws a rect at x,y with dimensions w,h */
void qw_drawrect(int x, int y, int w, int h) {
	SDL_Rect rect = {x, y, w, h};
	SDL_RenderDrawRect(qw_renderer, &rect);
}

/* Draws a line from x1,y1 to x2,y2 */
void qw_drawline(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(qw_renderer, x1, y1, x2, y2);
}

/* Draws a circle centered at x,y with radius r */
void qw_drawcircle(int x, int y, int r) {
	circleRGBA(qw_renderer, x, y, r, qw_rgba_red, qw_rgba_green, qw_rgba_blue, qw_rgba_alpha);
}

/* Draws a circle centered at x,y with radius r */
void qw_fillcircle(int x, int y, int r) {
	filledCircleRGBA(qw_renderer, x, y, r, qw_rgba_red, qw_rgba_green, qw_rgba_blue, qw_rgba_alpha);
}

/*************************\
 * IMAGE LOADING/DRAWING *
\*************************/

/*
 * Image holding information:
 * - rgba pixel data
 * - source rectangle (x,y,w,h)
 * - destination rect
 */
typedef struct {
	SDL_Rect *src,
	         *dst;
	SDL_Texture *texture;
	
	double angle;
	SDL_RendererFlip flip;
	SDL_Point center;
} qw_image;

/* Frees resources used by qw_image */
void qw_destroyimage(qw_image img) {
	free(img.src);
	free(img.dst);
	SDL_DestroyTexture(img.texture);
}

/* Load qw_image from filename */
qw_image qw_loadimage(const char *fn) {
	SDL_Surface *img_s = IMG_Load(fn);
	
	/* image loading failed? */
	if (!img_s) {
		printf("IMG_Load: %s\n", IMG_GetError());
		return (qw_image){NULL, NULL, NULL};
	}
	
	/* Allocate memory for src&dst rects and load rgba data from sdl_surface img_s */
	qw_image img = {
		.texture = SDL_CreateTextureFromSurface(qw_renderer, img_s),
		.src = malloc(sizeof(SDL_Rect)),
		.dst = malloc(sizeof(SDL_Rect))
	};
	
	/* src rect: full image by default */
	img.src->x = 0;
	img.src->y = 0;
	img.src->w = img_s->w;
	img.src->h = img_s->h;
	
	/* dst rect: full image by default */
	img.dst->x = 0;
	img.dst->y = 0;
	img.dst->w = img_s->w;
	img.dst->h = img_s->h;
	
	img.angle = 0.0;
	img.flip = SDL_FLIP_NONE;
	img.center = (SDL_Point) {
		.x = img.dst->w / 2,
		.y = img.dst->h / 2
	};

	SDL_FreeSurface(img_s);
	return img;
}

/* Draws qw_image to screen */
void qw_drawimage(qw_image img) {
	//SDL_RenderCopy(qw_renderer, img.texture, img.src, img.dst);
	SDL_RenderCopyEx(qw_renderer, img.texture, img.src, img.dst, img.angle, &img.center, img.flip);
}

/* Moves qw_image dst rect */
void qw_moveimage(qw_image img, int dx, int dy) {
	img.dst->x += dx;
	img.dst->y += dy;
}

/* Places qw_image dst rect at x,y */
void qw_placeimage(qw_image img, int x, int y) {
	img.dst->x = x;
	img.dst->y = y;
}

/* Returns image width */
int qw_imagewidth(qw_image img) {
	return img.dst->w;
}

/* Returns image height */
int qw_imageheight(qw_image img) {
	return img.dst->h;
}

/* Rotates image in degrees */
void qw_imagerotate(qw_image *img, double deg) {
	img->angle += deg;
}

/* Sets image rotation in degrees */
void qw_imagerotation(qw_image *img, double deg) {
	img->angle = deg;
}

/* Resets image rotation and flip */
void qw_resetimage(qw_image *img) {
	img->flip = SDL_FLIP_NONE;
	img->angle = 0.f;
}

/* Flips image, x and y can both be either 0 or 1 */
void qw_flipimage(qw_image *img, int x, int y) {
	if (x)
		img->flip ^= SDL_FLIP_HORIZONTAL;	
	if (y)
		img->flip ^= SDL_FLIP_VERTICAL;
}

/* Sets image flip, x and y can be either 0 (no flip) or 1 (horizontal/vertical flip) */
void qw_image_setflip(qw_image *img, int x, int y) {
	if (x && y)
		img->flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
	else if (x && !y)
		img->flip = SDL_FLIP_HORIZONTAL;
	else if (!x && y)
		img->flip = SDL_FLIP_VERTICAL;
	else if (!x && !y)
		img->flip = SDL_FLIP_NONE;
}

/* Sets center point of qw_image used for rotation */
void qw_image_setcenter(qw_image *img, int x, int y) {
	img->center.x = x;
	img->center.y = y;
}

/* Sets image source rect */
void qw_image_setsrc(qw_image img, int x, int y, int w, int h) {
	img.src->x = x;
	img.src->y = y;
	img.src->w = w;
	img.src->h = h;
}

/* Set image source rect position without modifying size */
void qw_image_srcpos(qw_image img, int x, int y) {
	img.src->x = x;
	img.src->y = y;
}

/* Sets image destination size */
void qw_image_setsize(qw_image img, int w, int h) {
	img.dst->w = w;
	img.dst->h = h;
}

/*
 * GFX PRIMITIVES
 */

/* Writes string to x,y */
void qw_write(const char *str, int x, int y) {
	stringRGBA(qw_renderer, x, y, str, qw_rgba_red, qw_rgba_green, qw_rgba_blue, qw_rgba_alpha);
}

/*
 * MATH
 */

/* Returns distance between two points */
float qw_distance(int x0, int y0, int x1, int y1) {
	return sqrt( (x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0) );
}

#endif
