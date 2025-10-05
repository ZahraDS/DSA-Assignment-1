// Implementation for UNOGame using external state mapping (no private members added).

// updated version for final submission, the initial one that i first had gave many errors since the uno.h header file wasnt in the same position as mine in the vs code and initially i was unable to figure out the issue

#include "uno.h"
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>
#include <map>

namespace {
	// Colors and internal card representation (hidden)
	enum class Color  { Red, Green, Blue, Yellow };

	struct InternalCard {
		Color color;
		std::string value; // "0"-"9", "Skip", "Reverse", "Draw Two"
		InternalCard(Color c, const std::string& v) : color(c), value(v) {}
	};

	static std::string colorToString(Color c) {
		switch (c) {
		case Color::Red:    return "Red";
		case Color::Green:  return "Green";
		case Color::Blue:   return "Blue";
		case Color::Yellow: return "Yellow";
		}
		return "Unknown";
	}

	// Build deck per assignment: 0 once; 1-9 twice; each action twice per color
	static std::vector<InternalCard> buildDeck() {
		std::vector<InternalCard> deck;
		const std::vector<Color> colors = { Color::Red, Color::Green, Color::Blue, Color::Yellow };
		for (Color c : colors) {
			deck.emplace_back(c, "0");
			for (int v = 1; v <= 9; ++v) {
				deck.emplace_back(c, std::to_string(v));
				deck.emplace_back(c, std::to_string(v));
			}
			deck.emplace_back(c, "Skip"); deck.emplace_back(c, "Skip");
			deck.emplace_back(c, "Reverse"); deck.emplace_back(c, "Reverse");
			deck.emplace_back(c, "Draw Two"); deck.emplace_back(c, "Draw Two");
		}
		return deck;
	}

	static bool playableOn(const InternalCard& a, const InternalCard& top) {
		return (a.color == top.color) || (a.value == top.value);
	}

	// selection priority: color match -> value match -> actions (Skip, Reverse, Draw Two)
	static int chooseCardIndex(const std::vector<InternalCard>& hand, const InternalCard& top) {
		for (size_t i = 0; i < hand.size(); ++i)
		if (hand[i].color == top.color) return static_cast<int>(i);
		for (size_t i = 0; i < hand.size(); ++i)
		if (hand[i].value == top.value) return static_cast<int>(i);
		const std::vector<std::string> actions = { "Skip", "Reverse", "Draw Two" };
		for (const auto &act : actions)
		for (size_t i = 0; i < hand.size(); ++i)
		if (hand[i].value == act) return static_cast<int>(i);
		return -1;
	}

	// Hidden implementation struct: stores deck, hands, direction, etc.
	struct UNOImpl {
		int numPlayers;
		std::vector<std::vector<InternalCard>> hands;
		std::vector<InternalCard> deck;
		std::vector<InternalCard> discard;
		int currentPlayer = 0;
		int direction = 1; // 1 clockwise, -1 counter-clockwise
		std::mt19937 rng;
		bool gameOver = false;
		int winnerIdx = -1;

		explicit UNOImpl(int n) : numPlayers(n), hands(n) {
			rng.seed(1234);
		}
	};

	// map storing impl per UNOGame instance
	static std::map<const UNOGame*, UNOImpl*> unoStorage;

	// safe advance helper
	static void advancePlayer(UNOImpl* g, int steps = 1) {
		if (!g || g->numPlayers <= 0) return;
		int offset = (steps * g->direction) % g->numPlayers;
		g->currentPlayer = (g->currentPlayer + offset + g->numPlayers) % g->numPlayers;
	}
} // namespace

// === UNOGame method implementations ===

UNOGame::UNOGame(int numPlayers) {
	int n = (numPlayers > 1) ? numPlayers : 2;
	UNOImpl* impl = new UNOImpl(n);
	unoStorage[this] = impl;
}

void UNOGame::initialize() {
	auto it = unoStorage.find(this);
	if (it == unoStorage.end()) return;
	UNOImpl* g = it->second;

	g->deck = buildDeck();
	g->rng.seed(1234);
	std::shuffle(g->deck.begin(), g->deck.end(), g->rng);

	for (auto &h : g->hands) h.clear();
	g->discard.clear();
	g->currentPlayer = 0;
	g->direction = 1;
	g->gameOver = false;
	g->winnerIdx = -1;

	// Deal 7 cards each
	for (int r = 0; r < 7; ++r) {
		for (int p = 0; p < g->numPlayers; ++p) {
			if (g->deck.empty()) break;
			g->hands[p].push_back(g->deck.back());
			g->deck.pop_back();
		}
	}

	// Put top card on discard if available
	if (!g->deck.empty()) {
		g->discard.push_back(g->deck.back());
		g->deck.pop_back();
	}
}

void UNOGame::playTurn() {
	auto it = unoStorage.find(this);
	if (it == unoStorage.end()) return;
	UNOImpl* g = it->second;
	if (!g || g->gameOver) return;

	// Ensure we have a top card to compare against
	if (g->discard.empty() && !g->deck.empty()) {
		g->discard.push_back(g->deck.back());
		g->deck.pop_back();
	}
	if (g->discard.empty()) return; // stalemate/no cards

	int p = g->currentPlayer;

	// If player already has 0 cards -> game over
	if (g->hands[p].empty()) {
		g->gameOver = true;
		g->winnerIdx = p;
		return;
	}

	InternalCard top = g->discard.back();
	int idx = chooseCardIndex(g->hands[p], top);

	if (idx != -1) {
		InternalCard played = g->hands[p][idx];
		g->hands[p].erase(g->hands[p].begin() + idx);
		g->discard.push_back(played);

		if (played.value == "Skip") {
			advancePlayer(g, 2);
		}
		else if (played.value == "Reverse") {
			g->direction *= -1;
			advancePlayer(g, 1);
		}
		else if (played.value == "Draw Two") {
			int next = (g->currentPlayer + g->direction + g->numPlayers) % g->numPlayers;
			for (int d = 0; d < 2 && !g->deck.empty(); ++d) {
				g->hands[next].push_back(g->deck.back());
				g->deck.pop_back();
			}
			// skip that player
			int afterNext = (next + g->direction + g->numPlayers) % g->numPlayers;
			g->currentPlayer = afterNext;
		}
		else {
			advancePlayer(g, 1);
		}
	}
	else {
		// draw one if possible
		if (!g->deck.empty()) {
			InternalCard drawn = g->deck.back();
			g->deck.pop_back();
			if (playableOn(drawn, top)) {
				g->discard.push_back(drawn);
				if (drawn.value == "Skip") advancePlayer(g, 2);
				else if (drawn.value == "Reverse") { g->direction *= -1; advancePlayer(g, 1); }
				else if (drawn.value == "Draw Two") {
					int next = (g->currentPlayer + g->direction + g->numPlayers) % g->numPlayers;
					for (int d = 0; d < 2 && !g->deck.empty(); ++d) {
						g->hands[next].push_back(g->deck.back());
						g->deck.pop_back();
					}
					int afterNext = (next + g->direction + g->numPlayers) % g->numPlayers;
					g->currentPlayer = afterNext;
				}
				else advancePlayer(g, 1);
			}
			else {
				g->hands[p].push_back(drawn);
				advancePlayer(g, 1);
			}
		}
		else {
			// deck empty & no playable -> advance to avoid freeze
			advancePlayer(g, 1);
		}
	}

	// check for winner
	for (int i = 0; i < g->numPlayers; ++i) {
		if (g->hands[i].empty()) {
			g->gameOver = true;
			g->winnerIdx = i;
			break;
		}
	}
}

bool UNOGame::isGameOver() const {
	auto it = unoStorage.find(this);
	if (it == unoStorage.end()) return false;
	return it->second->gameOver;
}

int UNOGame::getWinner() const {
	auto it = unoStorage.find(this);
	if (it == unoStorage.end()) return -1;
	return it->second->gameOver ? it->second->winnerIdx : -1;
}

std::string UNOGame::getState() const {
	auto it = unoStorage.find(this);
	if (it == unoStorage.end()) return std::string();

	UNOImpl* g = it->second;
	std::ostringstream out;
	std::string dirStr = (g->direction == 1) ? "Clockwise" : "Counter-clockwise";

	std::string topStr = "None";
	if (!g->discard.empty()) {
		const InternalCard &top = g->discard.back();
		topStr = colorToString(top.color) + " " + top.value;
	}

	out << "Player " << g->currentPlayer << "'s turn, Direction: " << dirStr
		<< ", Top: " << topStr << ", Players cards: ";

	for (int i = 0; i < g->numPlayers; ++i) {
		out << "P" << i << ":" << g->hands[i].size();
		if (i < g->numPlayers - 1) out << ", ";
	}
	return out.str();
}


