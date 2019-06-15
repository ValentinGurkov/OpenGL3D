#include "Game.h"

int main() {
	Game game("OpenGL 3D", 1680, 1050, 4, 6, false);

	while (!game.getWindowShouldClose()) {
		game.update();
		game.render();
	}

	return 0;
}