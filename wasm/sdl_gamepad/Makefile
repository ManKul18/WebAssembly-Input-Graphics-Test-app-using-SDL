wasm:
	emcc -o  ../output/test_gamepad.html --shell-file ./gamepad_template.html ./gamepad.cpp -s USE_SDL=2 --preload-file ./spaceship.bmp --preload-file ./missile.bmp
x86:
	gcc -o gamepad ./joystick.c -l SDL2