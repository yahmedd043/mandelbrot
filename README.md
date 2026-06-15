# Mandelbrot Set Viewer in C++

## Compilation Instructions (Linux)

First, install the necessary dependencies: 

```sudo apt-get install -y libsfml-dev``` (on Debian/Ubuntu)

```sudo dnf install -y SFML-devel``` (on Fedora)

Then compile:

```g++ -O3 main.cpp -o mandelbrot -lsfml-window -lsfml-graphics -lsfml-system```

## Compilation Instructions (Windows)

First, download the appropriate SDK from [here](https://www.sfml-dev.org/download/sfml/3.1.0/#windows), then compile:

```g++ -O3 main.cpp -o mandelbrot.exe -lsfml-window -lsfml-graphics -lsfml-system```

## Keybinds
- `r` resets bounds to default
- `=/-` increases/decreases precision (max iterations)

## Flags
- `-h` change window height
- `-w` change window width
- `-e` change exponent for escape-time algorithm (e.g. 2, 3)
