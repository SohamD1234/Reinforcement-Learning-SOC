# Assignment 2: Monte Carlo Programming

You will be using Monte Carlo techniques to find the optimal policies of MDPs whose dynamics are unknown.
This assignment will have three sections.

## Part I: Monte Carlo ES on an unknown MDP

- Here you are given an executable that represents the Markov Decision Process. The executable is named [```MDP```](./MDP).

- You can query the number of states and actions of the MDP with ```./MDP states``` and ```./MDP actions```. The discount factor of the MDP can be obtained with ```./MDP gamma```.

- To start interacting with the MDP, run ```./MDP <starting state>```. At every iteration, the executable will display the current state and current return of the MDP, and ask you to choose an action, after which it will report the reward obtained.

- You must implement the Monte Carlo ES algorithm (in [```part1.ipynb```](./part1.ipynb)) that learns the optimal policy of the MDP by simulating episodes with exploring starts.

- See [```part1.ipynb```](./part1.ipynb) for more details.

## Part II: On Policy Methods to solve Tic-Tac-Toe

- Tic-Tac-Toe is a simple game that can be modelled as an MDP, where you are one of the players and the environment represents your opponent.

- Tic-Tac-Toe can actually be solved explicitly using the [Minimax algorithm](../report.pdf) to get an exact solution.

- Your first subtask will be to implement the minimax algorithm to solve Tic-Tac-Toe explicitly. See [```part2a.ipynb```](./part2a.ipynb) for details about the Minimax algorithm.

- We can also solve Tic-Tac-Toe using On policy methods.

- Your second subtask will be to implement an on policy algorithm using $\varepsilon$-soft policies to solve Tic-Tac-Toe.

- See ```part2.ipynb``` for more details.

## Part III: Off Policy Methods to solve Snake

- Here you will solve the game of Snake using Weighted Importance Sampling (an off-policy method). Refer to [Sutton and Barto](../SuttonBarto.pdf) for an explanation of this algorithm.

- Again, you will make an engine that can solve the game of Snake.

- You are not required to make a GUI for this game (although bonus points for those who do!)

- The snake will be present in a 100x100 pixel grid, and will start at the center. You can choose whether it dies when hitting a wall or if it can pass through walls. Food will appear at random points on the grid. The snake gets a reward of $+1$ whenever it eats the food and it's length increases by one unit. If the snake intersects itself, it dies and the game is over. Refer [here](https://en.wikipedia.org/wiki/Snake_(video_game_genre)) for all the rules.

- You can use the "WASD" keys to represent the actions that can be taken at any point in the game.
