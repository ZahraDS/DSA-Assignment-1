#include <iostream>
#include "texteditor.h"
using namespace std;

int main() {
	TextEditor editor;

	editor.insertChar('H');
	editor.insertChar('e');
	editor.insertChar('y');
	editor.moveLeft();
	editor.insertChar('!');
	cout << editor.getTextWithCursor() << endl;

	return 0;
}
