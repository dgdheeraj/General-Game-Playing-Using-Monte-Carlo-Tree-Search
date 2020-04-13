# General-Game-Playing-Using-Monte-Carlo-Tree-Search
Implementation of  Monte Carlo Tree Search along with two zero-sum games as it's use case.<br/>
Games Implemented<br/>
1. Connect4 <br/>
2. Othello/Reversi <br/>

Monte Carlo Tree Search is a heuristic search algorithm with links to Reinforcement Learning. It consists of four stages<br/>
1. Selection
2. Expansion
3. Simulation
4. Backpropagation

# Usage
To run Connect4 <br/>
gcc mc.h monte_carlo_c4.c .\mcts_connect4.c -lm <br/>
./a.out<br/>

To run Othello/Reversi <br/>
gcc header.h .\monte_carlo.c .\myreversi.c -lm <br/>
./a.out <br/>


