wasm:
	emcc -o  ../output/test_mouse.html --shell-file ./mouse_template.html ./mouse.cpp -s USE_SDL=2 --preload-file ./spaceship.bmp --preload-file ./missile.bmp
x86:
	g++ -o test_mouse ./mouse.cpp -lSDL2
