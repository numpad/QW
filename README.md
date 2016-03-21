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


