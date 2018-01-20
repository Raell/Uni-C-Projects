/*
	Display the arithmetic and geometric progression terms and sum if given pararmeters
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

// Function Declarations
void selectType();
void arithmeticProg();
void APResults();
void printAPFormula();
void printAPSet();
void GPResults();
void geometricProg();
void printGPFormula();
void printGPSet();
int processInputInt();
double processInputDouble();
double checkFractions();
double processFractions();
bool isValid();
void test();
void testAP();
void testGP();
double APNTerm();
double APSum();
double GPNTerm();
double GPSum();
double GPInfSum();

// Main function
int main(int argc, char **argv) {
	
	selectType();
	return 0;
	
}

// Run tests
void test() {
	testAP();
	testGP();
	printf(" All tests passed.\n");
}

// Run tests for AP
void testAP() {
	assert(APNTerm(10.0, 7, 5.0) == 40.0 && APSum(10.0, 7, 5.0) == 175.0);
	assert(APNTerm(5.0, 8, 4.0) == 33.0 && APSum(5.0, 8, 4.0) == 152.0);
	assert(APNTerm(43.0, 100, 65.0) == 6478.0 && APSum(43.0, 100, 65.0) == 326050.0);
	assert(APNTerm(-547.0, 23, -23.0) == -1053.0 && APSum(-547.0, 23, -23.0) == -18400.0);
	assert(APNTerm(0.5, 6, 0.25) == 1.75 && APSum(0.5, 6, 0.25) == 6.75);
	assert(APNTerm(24.0, 34, -9.0) == -273.0 && APSum(24.0, 34, -9.0) == -4233.0);
	assert(APNTerm(0.0, 1, 100.0) == 0.0 && APSum(0.0, 1, 100.0) == 0.0);
	assert(APNTerm(32.0, 1, 0.0) == 32.0 && APSum(32.0, 1, 0.0) == 32.0);
	assert(APNTerm(0.0, 100, 0.0) == 0.0 && APSum(0.0, 100, 0.0) == 0.0);
	assert(APNTerm(100.0, 10, 0.0) == 100.0 && APSum(100.0, 10, 0.0) == 1000.0);
}

// Run tests for GP
void testGP() {
	assert(GPNTerm(7.0, 3, 4.0) == 112 && GPSum(7.0, 3, 4.0) == 147);
	assert(GPNTerm(16.0, 6, 1.5) == 121.5 && GPSum(16.0, 6, 1.5) == 332.5);
	assert(GPNTerm(-10.0, 4, 0.5) == -1.25 && GPSum(-10.0, 4, 0.5) == -18.75 && GPInfSum(-10.0, 4, 0.5) == -20);
	assert(GPNTerm(16.0, 2, 0.5) == 8.0 && GPSum(16.0, 2, 0.5) == 24.0 && GPInfSum(16.0, 2, 0.5) == 32);
	assert(GPNTerm(55.0, 3, 10.0) == 5500 && GPSum(55.0, 3, 10.0) == 6105);
	assert(GPNTerm(80.0, 1, 1.6) == 80.0 && GPSum(80.0, 1, 1.6) == 80.0);
	assert(GPNTerm(-3.5, 4, -1.0) == 3.5 && GPSum(-3.5, 4, -1.0) == 0.0);
	assert(GPNTerm(-1.0, 4, 0.0) == 0.0 && GPSum(-1.0, 4, 0.0) == -1.0 && GPInfSum(-1.0, 4, 0.0) == -1.0);
	assert(GPNTerm(0.0, 100, 0.0) == 0.0 && GPSum(0.0, 100, 0.0) == 0.0 && GPInfSum(0.0, 100, 0.0) == 0.0);
	assert(GPNTerm(0.0, 5, -9) == 0.0 && GPSum(0.0, 5, -9) == 0.0);
}

// Choose either AP or GP
void selectType() {
	printf("Please select either \n 1. Arimetic Progression\n 2. Geometric Progression\n 3. Testing");
	
	int type;
	
	while(true) {
		type = processInputInt("\n Enter selection: ");
		
		if(type == 1 || type == 2 || type == 3)
			break;
		
		printf(" Please enter a valid selection.\n");
	}
	
	if(type == 1)
		arithmeticProg();
	
	else if(type == 2)
		geometricProg();
	
	else
		test();
}

// Processes int inputs, ensure > 0
int processInputInt(const char *prefix) {
	int y;
	while(true) {
		char *input = malloc(sizeof(int));
		printf("\n%s", prefix);
		scanf("%s", input);
		y = atoi(input);
		if(y <= 0)
			printf(" Please enter a valid number.\n");
		else
			break;
	} 
	return y;
}

// Processes double inputs, allow negatives, 0 and fractions
double processInputDouble(const char *prefix) {
	char *input = malloc(sizeof(double));
	double output;
	while(true) {
		printf("\n%s", prefix);
		scanf("%s", input);
		
		output = checkFractions(input);
		bool valid = isValid(input, output);			
		
		if(!valid) {
			printf(" Please enter a valid number.\n");
			continue;
		}
		else
			break;
	}
	
	return output;
}

// Check if input is a fraction
double checkFractions(char *input) {
	int fracpos = 0;
	for(; fracpos < strlen(input); fracpos++) {
		if(input[fracpos] == '/')
			break;
	}
	
	if(fracpos != strlen(input))
		return processFractions(input, fracpos);
	
	else
		return atof(input);
	
}

// Returns double value of fraction
double processFractions(char *input, int fracpos) {
	char numerator[fracpos + 1];
	strncpy(numerator, input, fracpos);
	numerator[fracpos] = '\0';
	char *denominator = &input[strlen(input) - (strlen(input) - fracpos - 1)];
	if(atof(denominator) == 0) {
		return 0;
	}
	return (atof(numerator) / atof(denominator));
}

// Return true if number is valid
bool isValid(const char *x, double y) {
	return !(strcmp(x, "0") != 0 && y == 0);
}

// Function to process AP
void arithmeticProg() {
	printAPFormula();
	printf("To generate an AP please provide the following values:");
	int n;
	double a, d;
	a = processInputDouble(" a = ");
	d = processInputDouble(" d = ");
	n = processInputInt(" n = ");
	
	APResults(a, n, d);
	
}

// Return value of n-th term of AP
double APNTerm(double a, int n, double d) {
	return a + (d * (n - 1));
}

// Return sum of values up to n-th term of AP
double APSum(double a, int n, double d) {
	return (n * ((2 * a) + d * (n - 1)) / 2);
}

// Prints results of AP
void APResults(double a, int n, double d) {
	printf("\nResults:\n\nFor a = %.6g, d = %.6g, n = %d,\n\n", a, d, n);
	double nterm = APNTerm(a, n, d);
	double Sn = APSum(a, n, d);
	printf(" An = %.6g + %.6g(%d - 1)\n    = %.6g\n\n", a, d, n, nterm);
	printf(" Sn = (%d / 2) * ((2 * %.6g) + %.6g(%d - 1))\n    = %.6g\n\n", n, a, d, n, Sn);
	printf(" Set of AP: [");
	printAPSet(a, n, d);
	printf("]\n");
}

// Recursively print each element in AP
void printAPSet(double a, int n, double d) {
	printf("%.6g", a);
	n -= 1;
	if(n != 0) {
		printf(", ");
		printAPSet(a + d, n, d);
	}
	
}

// Print info for AP
void printAPFormula() {
	printf("\nAritmetic Progression (AP) is a sequence of numbers such that the difference of any two sucessive members is a constant.\n\nNotations:\n d is the common difference\n a is the first term of the arithemetic progression\n An is the n-th term of an arithmetic progression\n Sn is the sum of the first n elements of an arithemetic series");
	printf("\n\nFormulas:\n An = a + d(n - 1)\n Sn = (n/2) * (2a + d(n - 1))\n\n");
}

// Function to process GP
void geometricProg() {
	printGPFormula();
	printf("To generate a GP please provide the following values:\n");
	int n;
	double a, r;
	a = processInputDouble(" a = ");
	r = processInputDouble(" r = ");
	n = processInputInt(" n = ");
	
	GPResults(a, n, r);
}

// Return value of n-th term of GP
double GPNTerm(double a, int n, double r) {
	return (a * pow(r, (n - 1)));
}

// Return sum of values up to n-th term of GP
double GPSum(double a, int n, double r) {
	if(r == 1) {
		return a * n;
	}
	else {
		return (a * ((pow(r, n) - 1) / (r - 1)));
	}
}

// Return limit of the sum of the infinite series
double GPInfSum(double a, int n, double r) {
	return (a / (1 - r));
}

// Prints results of GP, includes limit of infinite series if |r| < 1
void GPResults(double a, int n, double r) {
	printf("\nResults:\n\nFor a = %.6g, r = %.6g, n = %d,\n\n", a, r, n);
	double nterm = GPNTerm(a, n, r);
	double sn = GPSum(a, n, r);
	
	printf(" An = %.6g * %.6g^(%d - 1)\n    = %.6g\n\n", a, r, n, nterm);
	
	if(r == 1) {
		printf(" Sn = %.6g * %d\n    = %.6g\n\n", a, n, sn);
	}
	else {
		printf(" Sn = %.6g * ((%.6g^%d - 1) / (%.6g - 1)\n    = %.6g\n\n", a, r, n, r, sn);
	}
	
	if(fabs(r) < 1) {
		double si = GPInfSum(a, n, r);
		printf(" Si = %.6g / (1 - %.6g)\n    = %.6g\n\n", a, r, si);
	}
	
	printf(" Set of GP: [");
	printGPSet(a, n, r);
	printf("]\n");
}

// Recursively print each element in GP
void printGPSet(double a, int n, double r) {
	printf("%.6g", a);
	n -= 1;
	if(n != 0) {
		printf(", ");
		printGPSet(a * r, n, r);
	}
	
}

// Prints info for GP
void printGPFormula() {
	printf("\nGeometric Progression (GP) is a sequence of numbers where each term after the first is found by multiplying the previous one by a common ratio.\n\nNotations:\n r is the common ratio\n a is the first term of the geometric progression\n An is the n-th term of an geometric progression\n Sn is the sum of the first n elements of an geometric series\n Si is the sum of the infinite geometric series if |r| < 1");
	printf("\n\nFormulas:\n An = a * r^(n - 1)\n Sn = a * ((r^n - 1) / (r - 1), when r =/= 1, else Sn = a * n\n Si = a / (1 - r)\n\n");
	
}

