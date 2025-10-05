Problem 1: Polynomial ADT
Approach
I implemented the Polynomial ADT using a linked-list–based structure simulated through static maps, since the header file didn’t allow private members.
Each Polynomial object is tracked externally, and every term is stored as a (coefficient, exponent) pair.
I ensured:
Insertion combines like terms and removes zero coefficients
Addition/Multiplication return new polynomials (immutability preserved)
Derivative follows power rule
Display format matches algebraic convention (3x^4 + 2x^2 - x + 5)
This approach respects OOP principles and avoids modifying the given header file.

Challenges Faced
Couldn’t add private data members due to header constraints
Needed a global structure (std::map) to simulate internal state per object
Handling term order and formatting cleanly
Avoiding modification of original polynomials in arithmetic operations

Problem 2: Text Editor Simulation
Approach
Implemented a cursor-based text editor using two deques (left and right buffers).
Characters to the left of the cursor go in one deque
Characters to the right go in another deque
Moving the cursor or inserting/deleting updates these buffers efficiently
Since adding members to the class was not allowed, I used a global map to associate each TextEditor instance with its buffers.

Challenges Faced
Maintaining text state without private members
Managing cursor movement edge cases (cursor at start or end)
Ensuring constant-time insertions and deletions
Handling global object mapping safely

Problem 3: UNO Game Simulation
Approach
Modeled UNO as an object-oriented simulation using vectors for deck, hands, and discard pile.
Used <random> with fixed seed (1234) for reproducibility
Implemented all core actions (Skip, Reverse, Draw Two)
Maintained direction state and turn control
Created a formatted state string for output consistency
Each turn checks for playable cards (color/value/action), plays the first valid one, or draws if none available.

Challenges Faced
Managing dynamic turn order when direction reverses
Properly skipping/drawing turns with action cards
Ensuring deterministic shuffle and game flow
Output formatting to match sample output strictly

Publicly Accessible GitHub Link
https://github.com/ZahraDS/DSA-Assignment-1/tree/main
