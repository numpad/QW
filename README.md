# QuickWindow

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

##### qw_
