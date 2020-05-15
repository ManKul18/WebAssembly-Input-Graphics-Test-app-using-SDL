wasm:
	emcc -o  ../output/test_keyboard.html --shell-file ./keyboard_template.html ./keyboard.cpp -s USE_SDL=2 --preload-file ./spaceship.bmp --preload-file ./missile.bmp
x86:
	g++ -o keyboard ./keyboard.cpp -lSDL2

