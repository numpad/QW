# QuickWindow

## 1. Using QW
 1. `#include "qw.h"` in your project
 2. Use a C99-compatible compiler
 3. Link against SDL2, SDL2_image, SDL2_gfx and libm

## 2. Documentation

### Global Variables

 Variable | Type | Description
----------|------|-------------
qw_width, qw_height | int | Size of the current window
qw_window | SDL_Window | SDL_Window managed by QW
qw_renderer | SDL_Renderer | SDL_Renderer managed by QW
qw_mousex, qw_mousey | int | Mouseposition relative to window
qw_mousex_last, qw_mousey_last | int | Mouseposition from previous frame
qw_mousex_next, qw_mousey_next | int | Direction the mouse is moving
qw_tick_count | unsigned long long | Number of frames since QW started its main loop

### Functions

##### qw_quit()
> Quits the application

##### qw_screen(int **width**, int **height**, int **fullscreen**, const char \***title**)
> Create and open a window

##### qw_resetticks()
> Reset tick_count to 0

##### qw_onscreen(int **x**, int **y**)
> Checks if x,y are on screen or not

##### qw_fill(int **r**, int **g**, int **b**)
> Clears the screen with background color **r**,**g**,**b**

##### qw_redraw()
> Draws rendered content to screen

##### qw_delay(int **millis**)
> Pauses execution for **millis** milliseconds

##### qw_clear()
> Clears the renderer

##### qw_running()
> Execute main loop, returns true if qw is running

```c
qw_screen(640, 480, 0, "Quick Window");

while (qw_running()) {
	qw_color(200, 100, 100, 255);
	qw_rect(100, 100, 100, 200);

	qw_redraw();
}
qw_quit();
```

##### qw_keydown(int **key**)
> Returns if **key** is down

```c
if (qw_keydown(QW_KEY(LEFT))) {
	// Move the player left
}
```

##### qw_getmouse(int **\*x**, int **\*y**)
> Writes mouse x and y to *&x*, *&y*
> __Deprecated! Use *qw_mousex* and *qw_mousey*__

##### qw_mousedown(int **button**)
> Returns if mouse**button** is down

##### qw_color(Uint8 **r**, Uint8 **g**, Uint8 **b**, Uint8 **a**)
> Sets color for drawing

##### qw_drawpixel(int **x**, int **y**)
> Draws pixel at **x**,**y**

##### qw_fillrect(int **x**, int **y**, int **w**, int **h**)
> Draws a filled rectangle at **x**,**y** with size **w**,**h**

##### qw_drawrect(int **x**, int **y**, int **w**, int **h**)
> Draws a rectangle at **x**,**y** with size **w**,**h**

##### qw_drawline(int **x1**, int **y1**, int **x2**, int **y2**)
> Draws a line from **x1**,**y1** to **x2**,**y2**

##### qw_drawcircle(int **x**, int **y**, int **r**)
> Draws a circle around **x**,**y** with radius **r**

##### qw_fillcircle(int **x**, int **y**, int **r**)
> Draws a filled circle around **x**,**y** with radius **r**

##### qw_loadimage(const char **\*fn**)
> Returns a *qw_image* read from filename **fn**

##### qw_destroyimage(qw_image **img**)
> Frees all resources taken by **img**

##### qw_drawimage(qw_image **img**)
> Draws **img**

##### qw_moveimage(qw_image **img**, int **dx**, int **dy**)
> Moves **img** position **dx**,**dy** pixels

##### qw_placeimage(qw_image **img**, int **x**, int **y**)
> Sets **img** position to **x**,**y**

##### qw_imagewidth(qw_image **img**)
> Returns width of **img**

##### qw_imageheight(qw_image **img**)
> Returns height of **img**

##### qw_imagerotate(qw_image **\*img**, double **deg**)
> Rotates **img** **deg** degrees clockwise

##### qw_imagerotation(qw_image **\*img**, double **deg)
> Sets **img** rotation to **deg** degrees

##### qw_resetimage(qw_image **\*img**)
> Resets **img** rotation and flip

##### qw_flipimage(qw_image **\*img**, int **x**, int **y**)
> Flips **img** horizontally if **x**, vertically if **y** is true

##### qw_image_setflip(qw_image **\*img**, int **x**, int **y**)
> Set **img** flip, resets all previous flips!

##### qw_write(const char **\*str**, int **x**, int **y**)
> Writes **str** at position **x**,**y** to screen

##### qw_distance(int **x0**, int **y0**, int **x1**, int **y1**)
> Returns distance between **x0**,**y0** and **x1**,**y1**

