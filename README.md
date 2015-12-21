## SAT SOLVER
Final project for CPSC 554 at Yale

Bo Song, Keshan Liu
We evenly contributed to this project.

Implement Chaff SAT solver in C++.

How to run it

We have already written a makefile in this project, please type following command in the terminal

make 

(It is tested in zoo environment.)

Features:
 - Basic Davis-Putnam Backtrack Search
 - Optimized BCP
 - Variable State Independent Decaying Sum(VSIDS) Decision Heuristic

Input Format:

  CNF Format.
  For more information please visit [http://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html](http://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html) 

Reference:

 [Chaff: Engineering an Efficient SAT Solver](https://www.princeton.edu/~chaff/publication/DAC2001v56.pdf)

Time it cost

It takes us about 3 days to finish this project. One day for reading papers and two days for developing and debug it.

Biggest challenge

Debugging the project. Implement the two variable watching algorithm.