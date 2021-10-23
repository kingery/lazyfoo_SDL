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

# SDL Textures

- SDL can render a image as a texture
- requires SDL_Renderer

gotta make sure these concepts are clear:
- window
- surface
- renderer
- texture

# Geometry rendering

- Render basic geometric shapes without additional graphics
- SDL_SetRenderDrawColor(R, G, B, Alpha -- "opacity")
- origin of rendering is upper-left corner of screen
    - x increases to the right
    - y increases to the bottom
- steps to render a rect
    - declare the Rect (SDL_Rect)
    - set the renderer color/opacity (SDL_SetRenderDrawColor)
    - apply the shape to the renderer (SDL_RenderFillRect, SDL_RenderDrawRect)
        - the color being drawn is a property of the SDL_Renderer object! 

