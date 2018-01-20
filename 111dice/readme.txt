list.c

2 sentinel nodes are used on the front and back of the linked list, with NULL on the back pointer of the front sentinel node and next pointer of the rear sentinel node

struct list points to the element behind the current position.

Front and end of the list can be found by checking for the null pointers in the sentinel nodes.

For handling errors, assert is used to check if in front or back of the list.


111dice.c

This is a dice game with rules in: https://www.reddit.com/r/ProgrammingPrompts/comments/2mwokc/easybase_task_to_difficult_challengedice_game_111/

The idea is for players to take a round rolling die and adding each roll to their score. Each player is allowed as many rolls as they want. However, the round ends when a player rolls a selected number (e.g. 1) and all points in the round is void. The player can at any time choose to save their points and end their round. The winner is the player that reaches a set number of points.

Additional optional rules include:
A mandatory re-roll rule, where players rolling a certain number (e.g. 2) will have to reroll another die.
A no re-roll rule, where players rolling a certain number (e.g. 3) will end their round and score their points.

AI players can be included, which plays by calculating the expectation of a roll (i.e. the mean score after a given roll) where the AI would not reroll if the expectation of the next roll is lower than the current round score.

The formula is given by:
X is the current round score
v is the void die number

The expectation of a roll E is 5/6(X) + 1/6(21-v). Where there is a 5/6 chance to keep your current score X + the expected value of the next roll (21/6 minus the void die value).

If X >= E, the AI will score the points. Otherwise, reroll.

Other parameters are listed below.

Use: %s OR %s a b c d e f
Where:
a is the target score
b is the number of human players (max: 10 players total)
c is the number of AI players (max: 10 players total)
d is the die number that ends and voids the points in that round
e is the die number that forces a player to do a mandatory re-roll (0 = rule not used) 
f is the die number that ends and scores the points in that round (0 = rule not used)
