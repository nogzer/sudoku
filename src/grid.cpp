#include "grid.hpp"

/*!
 * \file grid.cpp
 * \brief Implementation of class Grid
 * \author Matthieu Nogatchewsky
 * \version 1.0
 * \date April 2018
 */

Grid::Grid(){
  initGrid();
}

void Grid::initGrid(){
  position = 0;
  state = PROCESSING;
  for(int j = 0 ; j < 9 ; j++){
    row[j].initEntity();
    column[j].initEntity();
    square[j].initEntity();
  }
  for(int i = 0 ; i < 81 ; i++){
    cells[i%9][i/9]=0;
  }
}

bool Grid::load(Board extCells){
  initGrid();
  int value;
  for(int i = 0 ; i < 81 && (state == PROCESSING) ; i++){
    value = extCells[i%9][i/9];
    if(value >= 1 && value <= 9){
      nextStep(value);
    } else {
      nextCell();
    }
  }
  initCell();
  return (state != UNSOLVABLE);
}

void Grid::save(Board& extCells){
  for(int i = 0 ; i < 81 ; i++){
    extCells[i%9][i/9] = cells[i%9][i/9];
  }
}

void Grid::initCell(){
  position = 0;
}
void Grid::nextCell(){
  position++;
}
void Grid::prevCell(){
  position--;
}

void Grid::nextStep(int value){
  bool passed = true;
  passed &= row[theRow()].addValue(value);
  passed &= column[theColumn()].addValue(value);
  passed &= square[theSquare()].addValue(value);
  if(passed){
    cells[theRow()][theColumn()] = value;
    nextCell();
  } else {
    state = UNSOLVABLE;
  }
}
void Grid::prevStep(int value){
  prevCell();
  cells[theRow()][theColumn()] = 0;
  square[theSquare()].remValue(value);
  column[theColumn()].remValue(value);
  row[theRow()].remValue(value);
}

void Grid::processing(){
  if(position == 81){
    state = SOLVED;
  } else {
    if(cells[theRow()][theColumn()] == 0){
      List l = List();
      l.merge(row[theRow()].getList());
      l.merge(column[theColumn()].getList());
      l.merge(square[theSquare()].getList());
      if(!l.empty()){
	for(int i = 0 ; i < l.size() && (state == PROCESSING) ; i++){
	  nextStep(l.at(i));
	  processing();
	  if(state == PROCESSING) {
	    prevStep(l.at(i));
	  }
	}
      }
    } else {
      nextCell();
      processing();
      prevCell();
    }
  }
}

void Grid::solve(){
  initCell();
  state = PROCESSING;
  processing();
  if(state == PROCESSING){
    state = UNSOLVABLE;
  }
}

int Grid::theSquare(){
  return ((position%9)/3+((position/9)/3)*3);
}
int Grid::theColumn(){
  return position/9;
}
int Grid::theRow(){
  return position%9;
}