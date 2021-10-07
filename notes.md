# SDL

- Dynamically linked library
    - header files *.h
    - library files *.a
    - binary files *.so

- library files contain addresses of all functions, maintained in Import Address Table
- library binaries are imported at runtime by the OS

# Makfile

- after describing the target and dependencies, specify command
    - MUST BE INDENTED WITH A TAB CHARACTER!

# SDL windows

- Create a window
- Window has a surface that we draw to
- after drawing to window, then update the window for it to actually render

# SDL Surface

- an image data type
- contains all the pixels and stuff needed to render them
- SDL surfaces use software rendering--CPU does the rendering
- "blitting"
    - take a source surface and stamp a copy of it onto the destination surface
    - drawing happens to back buffer, need to SDL_UpdateWindowSurface to swap back/front buffers
