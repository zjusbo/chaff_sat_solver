## Chaff SAT Solver
Final project for CPSC 454/554 Software Analytics & Varification at Yale, 2015 Fall

It is a C++ implementation of Chaff SAT solver.

## How to run it

A makefile is contained in this project, please type following command in the terminal to compile it.

 make 

(It is tested in yale zoo environment.)

## Features
 - Basic Davis-Putnam Backtrack Search
 - Optimized BCP
 - Variable State Independent Decaying Sum(VSIDS) Decision Heuristic

## Input Format
  CNF format
  
  For more information about CNF format please visit [http://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html](http://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html) 

## Reference
 [Chaff: Engineering an Efficient SAT Solver](https://www.princeton.edu/~chaff/publication/DAC2001v56.pdf)

## Author
Bo Song, Keshan Liu

