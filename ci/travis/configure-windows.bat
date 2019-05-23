call RefreshEnv.cmd
set PATH=C:\Program Files\CMake\bin;%PATH%
cmake -S . -B ./build -G Ninja -DCMAKE_BUILD_TYPE=%1 -DTARGET_CPU=%2
