cd "`dirname "$0"`"
cmake -S . -B ./build
cmake --build ./build --config Release --target all