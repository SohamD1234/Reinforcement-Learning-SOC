# Assignment 1: Solving MDPs

You will be finding optimal policies for sample MDPs in this assignment.

## Goal

Implement the algorithms you have learnt to solve MDPs (Policy Iteration, Value Iteration and Linear Programming) through the content of this part to find the solution to a given MDP.

Follow good programming practices!

Try comparing the two methods and see which seems to be faster; think of reasons why. Write them as comments in your code or maintain a separate markdown/text document.

## Test MDPs format

You are given two MDPs to test on in the [MDPs](./MDPs) folder. Both are continuing MDPs. They follow the format:

```html
states <number of states>
actions <number of actions>
tran <initial state> <action taken> <final state> <reward> <transition probability>
...all the other possible transitions...
tran <initial state> <action taken> <final state> <reward> <transition probability>
gamma  <discount rate>
```

The solutions to the MDPs are contained in the same directory. You can use these solutions to verify the output of your MDP planner. The format:

```html
<optimal value function for first state> <optimal action for first state>
...one entry for each state...
<optimal value function for last state> <optimal action for last state>
```

## A Rough Outline

- The first step would be parse through the MDP files and store them into suitable data structures in python.
- Next, you have to implement the algorithms and store their results in the code.
- Finally, you can make a function that takes the results generated and stores them in an output text file so that you can verify the solution.
- You can also write another script to automate the process: run your python file and then compare the output produced to the solutions of the test MDPs provided. But, this isn't necessary as the MDPs aren't too big in size anyways.
- For the Linear Programming section, you may use the ```pulp``` library.
