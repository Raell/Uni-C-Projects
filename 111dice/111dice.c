#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>

bool paramterCheck();
bool playerCountValid();
bool dieParamsValid();
void paramterTest();
void stringToInt();
bool checkNumber();
void helpmenu();
void play();
void test();
void initialize();
void initializationTest();
void initializePlayers();
void takeTurn();
bool checkWin();
void startGame();
void AITurn();
void playerTurn();
int roll();
int rollHandler(int voidrollnum, int rerollnum, int roundscore, int playerscore);

// Circular list of players for turn order
struct player {
	
	int player;
	int currpoints;
	bool isAI;
	struct player *nextplayer;
	
};

typedef struct player player;

struct game {
	
	int target;
	int voidrollnum;
	int rerollnum;
	int scorerollnum;
	struct player *currentplayer;
	
};

typedef struct game game;

// Main function, runs tests with no parameters
int main(int n, char *argv[n]) {
	
	srand(time(NULL));
	
	if(n == 1) {
		test();
	}
	else if(n == 7 && paramterCheck(n, argv)) {
		play(n, argv);
	}
	else {
		helpmenu(argv[0]);
	}
	
}

// Set up game
void play(int n, char *argv[n]) {
	
	game *g = malloc(sizeof(game));
	
	initialize(n, argv, g);
	
	startGame(g);
	
}

// Checks when game ends and proceeds with player turn otherwise
void startGame(game *g) {
	
	bool win = false;
	
	while(!win) {
		
		takeTurn(g);	
		win = checkWin(g);
		if(!win)
			g->currentplayer = g->currentplayer->nextplayer;
		
	}
	
	printf("\nPlayer %d has won!", g->currentplayer->player);
	
}

// Function for handling player turns
void takeTurn(game *g) {
	
	player *player = g->currentplayer;
	printf("\nPlayer %d's turn. (Score: %d)\n", player->player, player->currpoints);
	if(player->isAI)
		AITurn(player, g->voidrollnum, g->rerollnum, g->scorerollnum, g->target);
	else
		playerTurn(player, g->voidrollnum, g->rerollnum, g->scorerollnum, g->target);
	
	printf("\nPress enter to continue...");
	getchar();
	
}

// Human player turn function
void playerTurn(player *player, int voidrollnum, int rerollnum, int scorerollnum, int target) {
	
	bool reroll = true;
	int roundscore = 0;
	
	do {	
		int roll = rollHandler(voidrollnum, rerollnum, roundscore, player->currpoints);
		roundscore += roll;
		
		if(roll == 0) {
			printf("\n\nPlayer %d rolled the void number, the round ends with 0 scored. (Total: %d)\n", player->player, player->currpoints);
			getchar();
			return;
		}	
		else if(roll == scorerollnum) {
			player->currpoints += roundscore;
			printf("\nPlayer %d rolled the no-reroll number, round ends with %d scored. (Total: %d)\n", player->player, roundscore, player->currpoints);
			getchar();
			return;
		}
		
		while(player->currpoints + roundscore < target) {
			printf("\nContinue rolling (Y/N)? ");
			char response;
			scanf(" %c", &response);
			
			if(response == 'y' || response == 'Y') {
				reroll = true;
				break;
			}
			
			else if (response == 'n' || response == 'N') {
				reroll = false;
				break;
			}
			
		}
		
	}while(reroll && roundscore + player->currpoints < target);
	
	player->currpoints += roundscore;
	printf("\nPlayer %d ends the round with %d scored. (Total: %d)\n", player->player, roundscore, player->currpoints);
	getchar();
	
}

// AI players turn function
void AITurn(player *AI, int voidrollnum, int rerollnum, int scorerollnum, int target) {
	
	int roundscore = 0;
	double expectednextscore = (1.0 / 6.0) * (21 - voidrollnum);
	
	while(expectednextscore > roundscore && roundscore + AI->currpoints < target) {
		
		int roll = rollHandler(voidrollnum, rerollnum, roundscore, AI->currpoints);
		roundscore += roll;
		expectednextscore = (1.0 / 6.0) * (5 * roundscore + 21 - voidrollnum);
		
		if(roll == 0) {
			printf("\n\nPlayer %d rolled the void number, the round ends with 0 scored. (Total: %d)\n", AI->player, AI->currpoints);
			return;
		}
		
		else if(roll == scorerollnum) {
			AI->currpoints += roundscore;
			printf("\nPlayer %d rolled the no-reroll number, round ends with %d scored. (Total: %d)\n", AI->player, roundscore, AI->currpoints);	
			return;
		}	
		
	}
	
	AI->currpoints += roundscore;
	printf("\nPlayer %d ends the round with %d scored. (Total: %d)\n", AI->player, roundscore, AI->currpoints);
	
}

// Returns score gained during roll
int rollHandler(int voidrollnum, int rerollnum, int roundscore, int playerscore) {
	
	int rollval = roll();
	int rollscore = rollval;
	roundscore += rollval;
	
	if(rollval == voidrollnum) {
		return 0;
	}
	
	printf(" (Current Round: %d, Total: %d)\n", roundscore, (playerscore + roundscore));
	
	if(rollval == rerollnum) {
		printf("\nRolled the mandatory re-roll number, roll again.\n");			
		int reroll = rollHandler(voidrollnum, rerollnum, roundscore, playerscore);
		if(reroll == 0)
			return 0;
		rollscore += reroll;
	}
	
	//printf("RollHandler returns %d\n", roundscore);
	return rollscore;
	
}

// Returns random number from 1-6
int roll() {
	int rollval = rand() % 6 + 1;
	printf("Rolled a %d.", rollval);
	return rollval;
}

// Returns true if a player reaches the target
bool checkWin(game *g) {
	return (g->currentplayer->currpoints >= g->target);	
}

// Game parameter initialization
void initialize(int n, char *argv[n], game *g) {
	
	int *params = malloc(n * sizeof(int));
	stringToInt(n, argv, params);
	
	g->target = params[1];
	g->voidrollnum = params[4];
	g->rerollnum = params[5];
	g->scorerollnum = params[6];
	
	initializePlayers(params[2], params[3], g);
	
}

// Players initialization
void initializePlayers(int humans, int AIs, game *g) {
	
	int playernumber = 1;
	
	player *firstplayer = malloc(sizeof(player));
	firstplayer->player = playernumber++;
	firstplayer->currpoints = 0;	
	
	if(humans == 0)
		firstplayer->isAI = true; 
	else
		firstplayer->isAI = false;
	
	player *prevplayer = firstplayer;
	
	for(int i = 1; i < humans; i++) {
		
		player *nextplayer = malloc(sizeof(player));
		nextplayer->player = playernumber++;
		nextplayer->currpoints = 0;
		nextplayer->isAI = false;
		prevplayer->nextplayer = nextplayer;
		prevplayer = nextplayer;
		
	}
	
	for(int j = (humans == 0) ? 1 : 0; j < AIs; j++) {
		
		player *nextplayer = malloc(sizeof(player));
		nextplayer->player = playernumber++;
		nextplayer->currpoints = 0;
		nextplayer->isAI = true;
		prevplayer->nextplayer = nextplayer;
		prevplayer = nextplayer;
		
	}
	
	prevplayer->nextplayer = firstplayer;
	g->currentplayer = firstplayer;	
}

// Main test function
void test() {
	paramterTest();
	initializationTest();
	printf("All tests passed sucessfully\n");
}

// Tests for initialization of game & player parameters
void initializationTest() {
	printf("Initialization Test: ");
	
	game *g = malloc(sizeof(game));
	
	char *p1[] = {"./111dice", "201", "0", "4", "4", "2", "1"};
	
	initialize(7, p1, g);
	assert(g->target == 201 && g->voidrollnum == 4 && g->rerollnum == 2 && g->scorerollnum == 1);
	player *player = g->currentplayer;
	
	for(int i = 1; i <= 4; i++) {
		
		assert(player->player == i);
		assert(player->isAI == true);
		assert(player->currpoints == 0);
		player = player->nextplayer;
		
	}
	
	char *p2[] = {"./111dice", "112", "3", "4", "1", "2", "0"};
	
	initialize(7, p2, g);
	assert(g->target == 112 && g->voidrollnum == 1 && g->rerollnum == 2 && g->scorerollnum == 0);
	player = g->currentplayer;
	
	for(int i = 1; i <= 7; i++) {
		
		assert(player->player == i);
		if(i <= 3)
			assert(player->isAI == false);
		else
			assert(player->isAI == true);
		assert(player->currpoints == 0);
		
		player = player->nextplayer;
		
	}
	
	char *p3[] = {"./111dice", "142", "10", "0", "4", "5", "6"};
	
	initialize(7, p3, g);
	assert(g->target == 142 && g->voidrollnum == 4 && g->rerollnum == 5 && g->scorerollnum == 6);
	player = g->currentplayer;
	
	for(int i = 1; i <= 10; i++) {
		
		assert(player->player == i);
		assert(player->isAI == false);
		assert(player->currpoints == 0);	
		player = player->nextplayer;
		
	}
	
	printf("Passed\n");
}

// Tests for command line parameters
void paramterTest() {
	printf("Parameter Test: ");
	
	char *p1[] = {"./111dice", "156", "1", "5", "1", "0", "0"};
	assert(paramterCheck(7, p1) == true);
	
	char *p2[] = {"./111dice", "142", "0", "0", "2", "0", "0"};
	assert(paramterCheck(7, p2) == false);
	
	char *p3[] = {"./111dice", "142", "1", "0", "3", "0", "0"};
	assert(paramterCheck(7, p3) == false);
	
	char *p4[] = {"./111dice", "142", "1", "1", "4", "0", "0"};
	assert(paramterCheck(7, p4) == true);
	
	char *p5[] = {"./111dice", "100", "1", "5", "7", "0", "0"};
	assert(paramterCheck(7, p5) == false);
	
	char *p6[] = {"./111dice", "187", "8", "5", "3", "3", "0"};
	assert(paramterCheck(7, p6) == false);
	
	char *p7[] = {"./111dice", "112", "3", "4", "1", "2", "0"};
	assert(paramterCheck(7, p7) == true);
	
	char *p8[] = {"./111dice", "142", "10", "0", "4", "5", "6"};
	assert(paramterCheck(7, p8) == true);
	
	char *p9[] = {"./111dice", "172", "50", "1", "4", "5", "6"};
	assert(paramterCheck(7, p9) == false);
	
	char *p10[] = {"./111dice", "182", "2", "5", "10", "1", "6"};
	assert(paramterCheck(7, p10) == false);
	
	char *p11[] = {"./111dice", "bvuv", "2", "5", "10", "1", "6"};
	assert(paramterCheck(7, p11) == false);
	
	printf("Passed\n");
}

// Check all command line parameters are in valid ranges and format
bool paramterCheck(int n, char *argv[n]) {
	
	bool allValid = true;
	for(int i = 1; i < n && allValid; i++) {
		
		allValid = checkNumber(argv[i]);
		
	}
	
	int *params = malloc(n * sizeof(int));
	stringToInt(n, argv, params);
	
	allValid = allValid && playerCountValid(params[2], params[3]) && dieParamsValid(params[4], params[5], params[6]);
	
	return allValid;
	
}

// Converts array of strings to array of ints
void stringToInt(int n, char *strings[n], int *ints) {	
	for(int i = 0; i < n; i++)
		ints[i] = atoi(strings[i]);
}

// Checks for valid number of players
bool playerCountValid(int humans, int AIs) {
	return ((humans + AIs) >= 2 && (humans + AIs) <= 10);
}

// Check no die parameter is repeated and in valid ranges
bool dieParamsValid(int voidNum, int rerollNum, int scoreNum) {
	bool voidNumValid = (voidNum >= 1 && voidNum <= 6);
	bool rerollNumValid = (rerollNum >= 0 && rerollNum <= 6);
	bool scoreNumValid = (scoreNum >= 0 && scoreNum <= 6);
	
	bool notrepeated = (voidNum != rerollNum) && (voidNum != scoreNum) && (rerollNum != scoreNum || rerollNum == 0);
	
	bool valid = voidNumValid && rerollNumValid && scoreNumValid && notrepeated;
	
	return valid;
}

// Display helpmenu
void helpmenu(char *progname) {
	fprintf(stderr, "Use: %s OR %s a b c d e f\n\nWhere:\n a is the target score\n b is the number of human players (max: 10 players total)\n c is the number of AI players (max: 10 players total)\n d is the die number that ends and voids the points in that round\n e is the die number that forces a player to do a mandatory re-roll (0 = rule not used)\n f is the die number that ends and scores the points in that round (0 = rule not used)", progname, progname);
	exit(1);
}

// Checks if string is a number
bool checkNumber(char *num) {
	for(int i = 0; i < strlen(num); i++) {
		if(!isdigit(num[i]))
			return false;
	}
	return true;
}





