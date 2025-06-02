cmake -B build -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles"
cmake --build build --config Release
start /d "Release/" CS202GameProject_Release.exe