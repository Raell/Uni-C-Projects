/* 
	Game of Got It
	https://nrich.maths.org/1272
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

// Structs and definitions
enum player {A = 1, B = 2};
typedef enum player player;

struct game {
	int goal;
	int currentnumber;
	int maxincrements;
	player nextplayer;
};
typedef struct game game;

struct computer {
	player playerid;
	int *targets;
	int targetcount;
	int nexttarget;
};
typedef struct computer computer;

// Function Declarations
void helpmenu();
void test();
void gameParamtersTest();
void AIParametersTest();
bool targetTest();
void gameTest();
void preplayCheck();
player play();
bool isValidInput();
bool isValidParams();
bool checkNumber();
void initialize();
void initializeGame();
void initializeCom();
player startGame();
bool checkWin();
void takeTurn();
void comTurn();
void playerTurn();

// Runs tests with 1 arg or runs game with 4 args, spits help menu otherwise
int main(int n, char *argv[n]) {
	
	srand(time(NULL));
	
	if (n == 1) 
		test();
    else if (n == 4)
		preplayCheck(n, argv);
    else {
        helpmenu(argv[0]);
    }
    return 0;

}

// Prints help menu and exits
void helpmenu(char *progname) {
	fprintf(stderr, "Use: %s OR %s x y z\n\nWhere:\n x is any natural number\n y is any natural number < x\n z is the play order where 1 = play first and 2 = play second", progname, progname);
	exit(1);
}

// Testing function
void test() {
	gameParamtersTest();
	AIParametersTest();
	gameTest();
	printf("All tests passed.\n");
}

// Tests game parameters are set correctly
void gameParamtersTest() {
	
	game *g = malloc(sizeof(game));
	
	initializeGame(g, 23, 4, A);
	assert(g->goal == 23);
	assert(g->maxincrements == 4);
	assert(g->nextplayer == A);
	
	free(g);
	g = malloc(sizeof(game));
	
	initializeGame(g, 150, 10, B);
	assert(g->goal == 150);
	assert(g->maxincrements == 10);
	assert(g->nextplayer == B);
	
}

// Tests AI parameters are set correctly
void AIParametersTest() {
	
	game *g = malloc(sizeof(game));	
	computer *com = malloc(sizeof(computer));	
	initializeGame(g, 84, 8, A);
	initializeCom(g, com, A);
	
	assert(com->playerid == A);
	assert(com->targetcount == 10);
	assert(com->nexttarget == 0);
	int a[] = {3, 12, 21, 30, 39, 48, 57, 66, 75, 84};
	assert(targetTest(a, com->targets, com->targetcount));
	
	free(g);
	free(com);
	
	g = malloc(sizeof(game));
	com = malloc(sizeof(computer));
	initializeGame(g, 37, 5, A);
	initializeCom(g, com, B);	
	
	assert(com->playerid == B);
	assert(com->targetcount == 7);
	assert(com->nexttarget == 0);
	int b[] = {1, 7, 13, 19, 25, 31, 37};
	assert(targetTest(b, com->targets, com->targetcount));
	
	
}

// Compare AI targets with correct targets
bool targetTest(int *correctTargets, int *AITargets, int targetcount) {
	bool ret = true;
	for(int i = 0; i < targetcount; i++)
		if(correctTargets[i] != AITargets[i])
			ret = false;
		
	return ret;
}

// Tests 2 AI playing with random parameters
void gameTest() {
	
	int goal = rand() % 1000 + 1;
	int maxincrements = rand() % goal + 1;
	player winner = play(goal, maxincrements, A, 2);

	if(goal % (maxincrements + 1) != 0)
		assert(winner == A);
	else
		assert(winner == B);
	
}

// Check parameters before playing
void preplayCheck(int n, char *argv[n]) {
	
	if(!isValidParams(argv[1], argv[2], argv[3])) {
		helpmenu(argv[0]);
	}
	
	else {
		play(atoi(argv[1]), atoi(argv[2]), (atoi(argv[3]) == 1) ? A : B, 1);
	}
	
}

// Main game function, returns winner
player play(int goal, int maxincrements, player nextplayer, int comCount) {
	
	game *g = malloc(sizeof(game));
	computer *coms[comCount];	
	initialize(g, comCount, coms, goal, maxincrements, nextplayer);	
	return startGame(g, comCount, coms);
	
}

// Initialize all paramters
void initialize(game *g, int comCount, computer *coms[comCount], int goal, int maxincrements, player nextplayer) {
	
	initializeGame(g, goal, maxincrements, nextplayer);
	
	for(int i = 0; i < comCount; i++) {
		coms[i] = malloc(sizeof(computer));
		player id = (comCount == 1 || i == 1) ? B : A;
		initializeCom(g, coms[i], id);
	}
	
}

// Initialize game parameters
void initializeGame(game *g, int goal, int maxincrements, player nextplayer) {
	g->goal = goal;
	g->currentnumber = 0;
	g->maxincrements = maxincrements;
	g->nextplayer = nextplayer;
}

// Initialize AI players
void initializeCom(game *g, computer *com, player comID) {	
	
	int startvalue = g->goal % (g->maxincrements + 1);
	int increment = (g->maxincrements + 1);
	int targetcount = (int) (g->goal / (g->maxincrements + 1)) + 1;
	com->playerid = comID;
	com->targets = malloc(targetcount * sizeof(int));
	for(int h = 0, j = startvalue; h < targetcount; h++, j += increment) {
		com->targets[h] = j;
	}
	com->targetcount = targetcount;
	com->nexttarget = 0;
	
}

// Starts the game and returns winning player
player startGame(game *g, int comCount, computer *coms[comCount]) {
	
	player currentplayer = g->nextplayer;
	
	while(!checkWin(g)) {
		
		currentplayer = g->nextplayer;
		
		takeTurn(g, comCount, coms);
		
	}
	
	printf("Player %d has won!\n\n", currentplayer);
	return currentplayer;
	
}

bool checkWin(game *g) {
	return (g->currentnumber == g->goal);
}

// Runs the next turn
void takeTurn(game *g, int comCount, computer *coms[comCount]) {
	
	bool turndone = false;
	int prevnum = g->currentnumber;
	
	printf("Goal:     %d\nRange:    1 - %d\nCurrent:  %d\n\n", g->goal, g->maxincrements, g->currentnumber);
	
	for(int i = 0; i < comCount; i++) {			
		if(g->nextplayer == coms[i]->playerid) {
			comTurn(g, coms[i]);
			turndone = true;
			break;
		}			
	}
	
	if(!turndone) {
		playerTurn(g);
	}
	
	printf("\nPrevious: %d\nNew:      %d\n\n----------\n\n", prevnum, g->currentnumber);
	
	g->nextplayer = (g->nextplayer == A) ? B : A;

}

// Runs turn for AI
void comTurn(game *g, computer *com) {
		
	if(g->currentnumber >= com->nexttarget) {
		
		for(int i = 0; i < com->targetcount; i++) {
			if(g->currentnumber < com->targets[i]) {
				com->nexttarget = com->targets[i];
				break;
			}
		}
		
	}
	
	int added = 0;
	
	if(com->nexttarget - g->currentnumber <= g->maxincrements) {
		added = (com->nexttarget - g->currentnumber);
	}
	else {
		added = rand() % g->maxincrements + 1;
	}
	
	g->currentnumber += added;
	printf("Player %d added %d.\n", com->playerid, added);
	
}

// Runs turn for human player
void playerTurn(game *g) {
	
	int currentnumber = g->currentnumber;
	int added = 0;
	
	while(g->currentnumber == currentnumber) {
		printf("Please enter any number from 1 to %d: ", g->maxincrements);
		char input[20];
		fgets(input, 20, stdin);
		if(isValidInput(g, input)) {
			added = atoi(input);
			if(g->currentnumber + added <= g->goal)
				g->currentnumber += added;
			else
				printf("Invalid choice - value will exceed goal.\n\n");
		}
		else
			printf("Please enter a valid number in the range.\n\n");
	}
	
	printf("\nPlayer 1 added %d.\n", added);
	
}

// Checks if player input is within range and valid
bool isValidInput(game *g, char *input) {
	
	if(!checkNumber(input, 1))
		return false;
	
	int value = atoi(input);
	
	return (value <= g->maxincrements && value > 0);
	
}

// Checks that arguments given are valid and within range
bool isValidParams(char *goal, char *addmax, char *playorder) {
	
	bool goalvalid = checkNumber(goal, 0);
	if(goalvalid)
		goalvalid = (atoi(goal) >= 1);
	
	bool addmaxvalid = checkNumber(addmax, 0);
	if(addmaxvalid)
		addmaxvalid = (atoi(addmax) >= 1 && atoi(addmax) < atoi(goal));
	
	bool playordervalid = checkNumber(playorder, 0);
	if(playorder)
		playordervalid = (atoi(playorder) == 1 || atoi(playorder) == 2);
	
	return goalvalid && addmaxvalid && playordervalid;
	
}

// Checks if argument given is a number
bool checkNumber(char *num, int offset) {
	for(int i = 0; i < strlen(num) - offset; i++) {
		if(!isdigit(num[i]))
			return false;
	}
	return true;
}