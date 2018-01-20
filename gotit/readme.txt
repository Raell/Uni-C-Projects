Got It is an adding game for two players. Based on https://nrich.maths.org/1272

On every turn, each player adds a number from a specified range (e.g. 1-4) to a running total starting from 0. The objective of the game is to be the player to bring the running total to a specified goal (e.g. 23) on their turn.

The goals, range and starting player can be changed for every game. 
The program takes in 3 natural numbers as arguments x,y,z where:
	x is the goal
	y is the maximum of the range (must be less than x)
	z is the order of play (i.e. 1 = play first, 2 = play second)

The project includes an AI that always plays perfectly. The game also includes a test with 2 such AI playing with random game paramters. Assuming the players play perfectly, there is an algorithm to guarantee that for most cases that the starting player will always win. However, there are specific combinations of the goal and range where the second player will always win.

Example:
Given the goal is 32 and the 5 is the maximum of the range, the first player will win if on his turns he achieves these numbers in order regardless of what his opponent plays [2, 8, 14, 20, 26, 32].

Simulation:		     
			 Add    Total
(P1) Turn 1 : +2      2
(P2) Turn 2 : +1	  3
(P1) Turn 3 : +5      8
(P2) Turn 4 : +4      12
(P1) Turn 5 : +2      14
(P2) Turn 6 : +3	  17
(P1) Turn 7 : +3      20
(P2) Turn 8 : +4      24
(P1) Turn 9 : +2      26
(P2) Turn 10: +1	  27
(P1) Turn 11: +5      32

P1 Wins.

Explaination:
To reach 32 P1 must be within the range of 27-31 on their turn, however they must also prevent P2 from receiving this range. So P2 must be given a number outside this range, we choose 26 as it outside the range and also since P2 must add at least 1 on their turn, this forces P2 to give a number in that winning range to P1. Therefore P1 would want to give 26 to P2, so we can take 26 as a subgoal. To reach the subgoal P1 must be within the range of 21-25, and by a similar argument to earlier P2 must be given 20 as a subgoal.

This goes on until we reach the number 2, which P1 adds to on Turn 1 starting the pattern. In fact, the difference in the sequence of each subgoal is 6 or (5+1) and starts from 2 or (32 mod (5+1)) where 5 is the maximum range and 32 is the goal. We can also find the length of the sequence by ceiling(32/(5+1)). Thus, we can find the starting winning sequence and the following subgoals.

Algorithm:
This means for any x = goal, y = maximum range, we can get a winning sequence of length (ceiling(x/(y+1))) starting from (x mod (y+1)) that increments to the next number by adding (y+1) until it reaches x.

(E.g. x = 45, y = 7, Sequence = [5, 13, 21, 29, 37, 45])

This means that the first player to start the sequence and sucessfully continue it will win. This results in the starting player winning in most cases. However, the exception lies in where (x mod (y+1) == 0), as the sequence begins with 0, the starting player cannot give 0 to the next player as he must add at least 1.

(E.g. x = 36, y = 5, Sequence = [0, 6, 12, 18, 24, 30, 36])

In the case above, the second player can reach the 2nd number in the sequence, 6, and continue it to the goal. So playing perfectly, the second player will always win.

Therefore, assuming players play perfectly, the starting players will always win if (x mod (y+1) != 0) and always lose if (x mod (y+1) == 0).


