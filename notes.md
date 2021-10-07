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
