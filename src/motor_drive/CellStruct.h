
#ifndef CELL_STRUCT_H
#define CELL_STRUCT_H

// Defines the operation for a cell.
// Cell::start and Cell::length defines the physical
// position of the cell around the cassette, starting at zero
// for the first cell, increasing in the clockwise direction,
// in steps.
//
// Cell::duration defines how long the mixing time is, in
// seconds.
typedef struct Cell {
  int start;
  int length;
  int duration;
} Cell;

Cell makeCell(int start, int length, int duration) {
  Cell c;
  c.start = start;
  c.length = length;
  c.duration = duration;
  return c;
}

#endif
