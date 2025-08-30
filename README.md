# _Rubick's Cube Solver (CUBEISTEIN)_

The Cube Solver is constructed to Refine and Create a Hybrid of Beginner and CFOP Method of Solving a Rubick's Cubs.

Designed with 5 Stepper Motors of 4.4kg-cm each. Occuping The Right, Left, Front, Back and Lower Face for Manipulation.

Powering the Entire Circuit with a 12V 1A Adapter for the Steppers controlled by the DRV8825 Stepper Drivers and a Buck Converter to Power the ESP32 Wroom DA.

The CAD for this Project was Created from Scratch using Fusion 360.

Programmed using ESP-IDF in C.

## Description of the Build Folder

StepperMovers.c : Holds the Hardware Pin Description and the Movement Commands based on the Trigger of the Face to Move

AlgoSolveAlpha.c : This is Raw Algorithms on Steroids. There are 2 Sets of Algorithm for each Stage of the Solve. Comparing 16 Combinations of solver and Refining these using 2 Layers of Filters to Remove Repetitions and Anti-Moves to Optimize the Code. Using the least moves it executes the Commands.

main.c : Setsup the Stepper and Scrambles the cube on random, then solves the cube using the optimized Algorithm.

## OUTCOME NOTED
After over 300 Iterations the average Move count reaches about 90 Moves

The Observed Minimum as 20 and the Maximum as 130 Moves.

The Solve Time is limited by the Physical Hardware and 3D printed Frame and the Link between the Motor shaft and the Face Center for the Solver, the avergae time of Solve being about 15 seconds.

