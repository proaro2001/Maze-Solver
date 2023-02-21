//
// Created by Yusuf Pisan on 4/18/18.
//

#include "maze.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

// print maze object
ostream &operator<<(ostream &out, const Maze &maze) {
  for (int row = 0; row < maze.height; ++row) {
    for (int col = 0; col < maze.width; ++col) {
      out << maze.field[row][col];
    }
    out << endl;
  }
  out << endl;
  return out;
}

// default constructor
Maze::Maze() = default;

// Load maze file from current directory
bool Maze::load(const string &fileName) {
  ifstream inFile;
  inFile.open(fileName);
  if (!inFile.is_open()) {
    cerr << "Unable to open file: " << fileName << endl;
    // next line is just to satisfy clang-tidy
    // otherwise it complains saying function can be const
    width = height = 0;
    return false;
  }
  inFile >> width >> height;
  inFile >> exitRow >> exitColumn;
  inFile >> startRow >> startColumn;
  // ignore leftover charcaters up to newline
  inFile.ignore(INT_MAX, '\n');
  string line;
  for (int row = 0; row < height; ++row) {
    getline(inFile, line);
    field.push_back(line);
  }
  return true;
}

// // true if maze can be solved
bool Maze::solve() { return goNorth(startRow, startColumn); }

// goNorth
bool Maze::goNorth(int row, int col) {
  bool success = false;
  if (!isInside(row, col) || !isClear(row, col)) {
    return success;
  }
  markAsPath(row, col);
  path.push_back('N');

  if (atExit(row, col)) {
    return true;
  }
  success = goNorth(row - 1, col);
  if (!success) {
    success = goWest(row, col - 1);
  }
  if (!success) {
    success = goEast(row, col + 1);
  }
  if (!success) {
    markAsVisited(row, col);
    path.pop_back();
  }

  return success;
}

// goWest
bool Maze::goWest(int row, int col) {
  bool success = false;
  if (!isInside(row, col) || !isClear(row, col)) {
    return success;
  }
  markAsPath(row, col);
  path.push_back('W');

  if (atExit(row, col)) {
    return true;
  }
  success = goWest(row, col - 1);
  if (!success) {
    success = goNorth(row - 1, col);
  }
  if (!success) {
    success = goSouth(row + 1, col);
  }
  if (!success) {
    markAsVisited(row, col);
    path.pop_back();
  }

  return success;
}

// goEast
bool Maze::goEast(int row, int col) {
  bool success = false;
  if (!isInside(row, col) || !isClear(row, col)) {
    return success;
  }
  markAsPath(row, col);
  path.push_back('E');

  if (atExit(row, col)) {
    return true;
  }
  success = goEast(row, col + 1);
  if (!success) {
    success = goNorth(row - 1, col);
  }
  if (!success) {
    success = goSouth(row + 1, col);
  }
  if (!success) {
    markAsVisited(row, col);
    path.pop_back();
  }

  return success;
}

// goSouth
bool Maze::goSouth(int row, int col) {
  bool success = false;
  if (!isInside(row, col) || !isClear(row, col)) {
    return success;
  }
  markAsPath(row, col);
  path.push_back('S');

  if (atExit(row, col)) {
    return true;
  }
  success = goSouth(row + 1, col);
  if (!success) {
    success = goWest(row, col - 1);
  }
  if (!success) {
    success = goEast(row, col + 1);
  }
  if (!success) {
    markAsVisited(row, col);
    path.pop_back();
  }

  return success;
}

// path to exit
string Maze::getPath() const { return path; }

// true if row, column is the exit
bool Maze::atExit(int row, int column) const {
  return row == exitRow && column == exitColumn;
}

// true if row, column is inside the maze
bool Maze::isInside(int row, int col) const {
  return row >= 0 && col >= 0 && row < field.size() && col < field[row].size();
}

// true if row, column is clear to move
bool Maze::isClear(int row, int col) const {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  return field[row][col] == CLEAR;
}

// mark location as part of the path to exit
void Maze::markAsPath(int row, int col) {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  field[row][col] = PATH;
}

// mark location as visited, not part of the path to exit
void Maze::markAsVisited(int row, int col) {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  field[row][col] = VISITED;
}
