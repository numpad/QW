#ifndef QUICK_GRAPHICS_H
#define QUICK_GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

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


/*
 * DRAWING AND GRAPHICS FUNCTIONS
 */

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

#endif
