# mandelbrot-Gempyre
Mandelbrot using Gempyre and Hiillos for UI

mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=Release

or for mingw

cmake ..  -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release 


cmake --build . --config Release


