#ifndef UNO_H
#define UNO_H

#include <string>
#include <vector>

struct Card {
    
};

class UNOGame {
public:
    // Initialize game with given number of players
    UNOGame(int numPlayers);

    // Shuffle and deal cards to players
    void initialize();

    // Play one turn of the game
    void playTurn();

    // Return true if game has a winner
    bool isGameOver() const;

    // Return index of winner
    int getWinner() const;

    // Return current state as string
    std::string getState() const;
};

#endif