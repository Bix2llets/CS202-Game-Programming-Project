cmake -B build -DCMAKE_BUILD_TYPE=Debug -G"Unix Makefiles"
cmake --build build --config Debug
start /d "build/Debug/" CS202GameProject_Debug.exe