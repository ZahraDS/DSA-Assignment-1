// texteditor.cpp
#include "texteditor.h"
#include <deque>
#include <map>
#include <string>
#include <sstream>

// i stored each TextEditor object's state externally (can't add members to header)
static std::map<const TextEditor*, std::pair<std::deque<char>, std::deque<char>>> editorStorage;

// Helper to get references to left/right deques for this object
static std::pair<std::deque<char>&, std::deque<char>&> getBuffers(const TextEditor* obj) {
    // Ensure an entry exists (default-constructs empty deques if absent)
    auto &entry = editorStorage[obj];
    return { std::ref(entry.first), std::ref(entry.second) };
}

void TextEditor::insertChar(char c) {
    // Insert character at cursor -> push to left buffer
    auto buffers = getBuffers(this);
    std::deque<char> &left = buffers.first;
    left.push_back(c);
}

void TextEditor::deleteChar() {
    // Delete character before cursor (backspace-like)
    auto buffers = getBuffers(this);
    std::deque<char> &left = buffers.first;
    if (!left.empty()) left.pop_back();
}

void TextEditor::moveLeft() {
    // Move cursor left if possible (transfer last of left -> front of right)
    auto buffers = getBuffers(this);
    std::deque<char> &left = buffers.first;
    std::deque<char> &right = buffers.second;
    if (!left.empty()) {
        char ch = left.back();
        left.pop_back();
        right.push_front(ch);
    }
}

void TextEditor::moveRight() {
    // Move cursor right if possible (transfer front of right -> end of left)
    auto buffers = getBuffers(this);
    std::deque<char> &left = buffers.first;
    std::deque<char> &right = buffers.second;
    if (!right.empty()) {
        char ch = right.front();
        right.pop_front();
        left.push_back(ch);
    }
}

std::string TextEditor::getTextWithCursor() const {
    auto it = editorStorage.find(this);
    // If not found, empty editor -> cursor at position 0
    if (it == editorStorage.end()) return std::string("|");

    const std::deque<char> &left = it->second.first;
    const std::deque<char> &right = it->second.second;

    std::ostringstream out;
    for (char c : left) out << c;
    out << '|';
    for (char c : right) out << c;
    return out.str();
}

