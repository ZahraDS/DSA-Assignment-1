#include "uno.h"
#include <iostream>
using namespace std;

int main() {
	UNOGame game(2); // Create a 2-player UNO game
	game.initialize();

	while (!game.isGameOver()) {
		game.playTurn();
		cout << game.getState() << endl;
	}

	cout << "Winner is Player " << game.getWinner() + 1 << "!" << endl;
	return 0;
}
