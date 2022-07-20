/*BITI 3143 EVOLUTIONARY COMPUTING
	- M I N I  P R O J E C T - 
	
Title: Optimization of Internet coverage in rural area
Group members: 1) Nurul 'Izzah Binti Muhammad Zawave  B031910128
			   2) NurAfiqah Binti Khosni
			   3) Wan Addini Aisyah Binti Wan Ismail @ Wan Roslan*/

#include<iostream>
#include<stdlib.h>
#include<ctime>
#include<stdio.h>
#include<fstream>

using namespace std;

//declare const
const int GENE = 8; //capacity distance -5000
const int TOTAL = 50;
const int DISTANCE[TOTAL] = { 2393, 2859, 1974, 1686, 3170, 4790, 863, 942, 1402, 3433, 3830, 4652, 2484, 1342, 1948, 4226, 3744, 2621, 2935, 958, 1082, 4254, 3651, 1227, 4832, 4505, 912, 951, 1385, 4820, 4612, 2580, 1513, 896, 980, 3444, 4477, 4641, 2239, 3875, 886, 2606, 2220, 3137, 949, 1403, 3235, 1364, 2429, 2484 };
const int MAX_DISTANCE = 38561; //maximum distance
const int COST[TOTAL] = { 111774, 114579, 108315, 104693, 113746, 118679, 80866, 81340, 101617, 113156, 113451, 117801, 112650, 101127, 108015, 115472, 113951, 113779, 114999, 84731, 100707, 115972, 113351, 101151, 119577, 116401, 84334, 84636, 101327, 119179, 116901, 112950, 103693, 81354, 100507, 113956, 114972, 117201, 111574, 113751, 81344, 113469, 111514, 113256, 84634, 101627, 113856, 101317, 112350, 112650 };
const int MAX_COST = 957000;  //maximum cost
const int POP_SIZE = 50; //no of chromosome for each generation
const float CROSSOVER_PROBABILITY = 0.9;
const float MUTATION_PROBABILITY = 0.2;
const int MAX_GENE = 40;

int totalgene = 0; //count to check total gene
int countGENE[TOTAL];  //count the current gene
int countCHILDREN[TOTAL]; //count the current children
int chromosome[POP_SIZE][GENE];
double fitness[POP_SIZE]; //fitness must less than 1. only 0-1 is acceptable
double fitnessC;  //fitness cost
double fitnessD;  //fitness distance
int parents[2][GENE];
int children[2][GENE];
int survivor[POP_SIZE][GENE];
int newChromo = 0;
double bestFitness = 99.9;//kna tukar
double avgFitness = 0.0;
int bestChromo[GENE];
ofstream bestFitnessFile, avgFitnessFile, bestChromosomeFile;  //create avgFitness and bestFitness textfile

void initializePop()  //chromosome representation
{
	int randNum;
	for (int c = 0; c < POP_SIZE; c++)
	{
		for (int q = 0; q < TOTAL; q++)
		{
			countGENE[q] = 0; //start count how many same number in each chromosome
		}

		for (int g = 0; g < GENE; g++)
		{
			randNum = rand() % 50;    //generate random number 
			chromosome[c][g] = randNum; //set gene equal to rand num

			countGENE[chromosome[c][g]]++; //add the counting

			for (int j = 0; j < TOTAL; j++)
			{

				if (countGENE[chromosome[c][g]] > 1) //if the count of that number more than 1
				{
					randNum = rand() % 50;  //generate new random number
					chromosome[c][g] = randNum; //set gene equal to rand num
					countGENE[chromosome[c][g]]++; //add the counting
				}
			}
			//make sure ada 1/2 je nmbr distance (1-50)
		}//gene
		//if(totalgene>2)
	}//chromosome
}

void printChormosome()
{
	for (int c = 0; c < POP_SIZE; c++)
	{
		cout << "Chromosome " << c + 1 << " : ";//print chromosome
		for (int g = 0; g < GENE; g++)
		{
			cout << chromosome[c][g] << " ";
		}//gene
		cout << endl << endl;
	}//chromosome
}

void evaluateChromosome()
{
	int accdistance = 0, acccost = 0;
	for (int c = 0; c < POP_SIZE; c++) //pop_size 
	{                     
		for (int g = 0; g < GENE; g++) //gene
		{     
			accdistance = accdistance + DISTANCE[chromosome[c][g]]; //calculate total distance
			acccost = acccost + COST[chromosome[c][g]]; //calculate total cost
		}
		cout << "\nTotal cost for chromosome " << c + 1 << ": " << acccost;
		cout << "\nTotal distance for chromosome " << c + 1 << ": " << accdistance;
		fitnessD = abs(MAX_DISTANCE - accdistance) / (double)MAX_DISTANCE; //fitness for distance
		fitnessC = abs(MAX_COST - acccost) / (double)MAX_COST; //fitness for cost
		fitness[c] = (fitnessC * 0.5) + (fitnessD * 0.5 ); //fitness for each chromosome
		cout << "\nFitness for chromosome " << c + 1 << ": " << fitness[c] << endl;
		accdistance = 0, acccost = 0;
	
	}
}

void ParentSelection()
{
	//declare necessary variable
	int player1, player2;
	int indexParent[2];
	for (int i = 0; i < 2; i++)
	{
		player1 = rand() % POP_SIZE;
		do {
			player2 = rand() % POP_SIZE;
		} while (player1 == player2);

		cout << "\n\tPlayer 1: " << player1+1 << "\n\tPlayer 2: " << player2+1;

		if (fitness[player1] < fitness[player2])
			indexParent[i] = player1;
		else
			indexParent[i] = player2;
		cout << "\n\tWinner is: " << indexParent[i] + 1;
	}
	for (int i = 0; i < 2; i++)
	{
		cout << "\n\tParent " << i + 1 << ": ";
		for (int j = 0; j < GENE; j++)
		{
			parents[i][j] = chromosome[indexParent[i]][j];
			cout << parents[i][j] << " ";
		}
	}
}

void crossover()
{
	float randNo; //to choose rand no between 0-1
	int xpoint, a; //crossover point
	for (int r = 0; r < 2; r++)
	{
		for (int c = 0; c < GENE; c++)
		{
			children[r][c] = parents[r][c];
		}
	}//end of copy parents to children

	randNo = (rand() % 11) / 10.0; //rand no between 0 and 1
	cout << "\n\tRandNo is: " << randNo;
	if (randNo <= CROSSOVER_PROBABILITY) //if randno <= 0.9, crossover happen
	{
		int z = 0;
	change:
		xpoint = rand() % GENE;//to find point to crossover
		cout << "\n\tCrossover point: " << xpoint;
		for (int g = xpoint; g < GENE; g++)
		{
			children[0][g] = parents[1][g];
			children[1][g] = parents[0][g];
		}

		for (int r = 0; r < 2; r++)
		{//print new children
			for (int q = 0; q < TOTAL; q++)
			{
				countCHILDREN[q] = 0;
			}
			
			for (int c = 0; c < GENE; c++)
			{
				for (int j = 0; j < TOTAL; j++)
				{
					a = children[r][c];
					if (a == j) //set a equal to j
					{
						countCHILDREN[j]++; //update count 
					}

					if (countCHILDREN[j] >= 3)
					{
						z = z + 1;
						if (z <= 5) //if crossover more or equal to 5
							goto change; // go to change
						else
						{
							for (int r = 0; r < 2; r++)
							{
								for (int c = 0; c < GENE; c++)
								{
									children[r][c] = parents[r][c];
								}//end copy
								cout << "\t\nCrossover did not happen";
							}
						}
					}
				}
			}
		}
	}
	for (int r = 0; r < 2; r++)
	{
		cout << "\n\tChildren " << r + 1 << ": ";
		for (int c = 0; c < GENE; c++)
		{
			cout << children[r][c] << " ";
		}
	}
	cout << endl;

}

void mutation()
{
	//declare necessary variables
	double probValue;
	int mutationBit, z = 0, a = 0, check = 0;
	//for both children
	for (int c = 0; c < 2; c++)
	{//generate random probability value

		mutationBit = rand() % GENE;
		probValue = (rand() % 11) / 10.0;
		cout << "\n\tMutation probability: " << probValue;
		check = children[c][mutationBit];

		if (probValue < MUTATION_PROBABILITY)
		{//probability happens
		happen:
			for (int q = 0; q < TOTAL; q++)
			{
				countCHILDREN[q] = 0;
			}
			for (int g = 0; g < GENE; g++)
			{
				for (int j = 0; j < TOTAL; j++)
				{
					a = children[c][g];
					if (a == j)
					{
						countCHILDREN[j]++;
					}
				}
			}

			cout << "\n\tChildren mutated at : " << mutationBit;
			children[c][mutationBit] = rand() % 50;
			cout << "\n\tRandom is : " << children[c][mutationBit];
			countCHILDREN[children[c][mutationBit]]++;

			if (children[c][mutationBit] == check)
			{
				z = z + 1;
				cout << "\n\tMutation check is: " << z;
				goto happen;
			}
			if (countCHILDREN[children[c][mutationBit]] >= 3)
			{
				z = z + 1;
				cout << "\n\tMutation is: " << z;
				if (z <= 5)
					goto happen;
				else
					goto nothappen;
			}
		}

		else
		{//probability does not happen
		nothappen:
			cout << "\n\tMutation did not happen ";
		}

		//print children after mutation
		cout << "\n\tChildren " << c + 1 << " after mutation : ";
		for (int g = 0; g < GENE; g++)
		{
			cout << children[c][g] << " ";
		}
	}
}

void survivalSelection()
{
	for (int i = 0; i < 2; i++) {
		for (int g = 0; g < GENE; g++) {
			survivor[newChromo][g] = children[i][g];
		}
		newChromo++;
	}
	for (int r = 0; r < newChromo; r++) {
		cout << "\n\tNew chromosome so far " << r + 1 << ": ";
		for (int c = 0; c < GENE; c++)
		{
			cout << survivor[r][c] << " ";
		}
	}//print new chromosome
}

void copyChromosome()
{
	for (int c = 0; c < POP_SIZE; c++) {
		for (int g = 0; g < GENE; g++) {
			chromosome[c][g] = survivor[c][g];
		}
	}
}

void recordBestFitness() {
	for (int c = 0; c < POP_SIZE; c++) {
		if (bestFitness > fitness[c]) {
			bestFitness = fitness[c];
			for (int g = 0; g < GENE; g++) {
				bestChromo[g] = chromosome[c][g];
			}
		}

	}//display in screen
	cout << "\n\t\tBest Fitness: " << bestFitness;
	cout << "\n\t\tBest Chromosome: ";
	for (int g = 0; g < GENE; g++) {
		cout << bestChromo[g] << " ";
	}
	cout << endl;

	bestFitnessFile << bestFitness << endl;
	for (int g = 0; g < GENE; g++) {
		bestChromosomeFile << bestChromo[g] << " ";
	}
	bestChromosomeFile << endl;
}

void calcAvgFitness() { //ACP
	double sum = 0;
	for (int c = 0; c < GENE; c++) {
		sum += fitness[c];
	}
	avgFitness = sum / POP_SIZE;
	cout << "\n\t\tAverage Fitness: " << avgFitness << endl;

	avgFitnessFile << avgFitness << endl;
}

int main()
{
	/*for (int i = 0; i < 100; i++)
	{
		cout << test[i] << " " << endl;
	}*/
	srand(time(0));
	bestFitnessFile.open("bestFitness.txt");
	avgFitnessFile.open("avgFitness.txt");
	bestChromosomeFile.open("bestChromosome.txt");
	cout << "\t\t\n\nGA START";
	cout << "\t\nINITIALISATION --> "<< endl;
	initializePop();
	for (int gen = 0; gen < MAX_GENE; gen++)
	{
		cout << "\n\n\t\t GENERATION: " << gen + 1 << endl << endl;
		printChormosome();
		cout << "\n\n\t\t EVALUATION: " << endl << endl;
		evaluateChromosome();
		recordBestFitness();
		calcAvgFitness();
		newChromo = 0;
		for (int i = 0; i < POP_SIZE / 2; i++) { //for generation
			cout << "\n\n\t\t PARENT SELECTION: " << endl << endl;
			ParentSelection();
			cout << "\n\n\t\t CROSSOVER: " << endl << endl;
			crossover();
			cout << "\n\n\t\t MUTATION: " << endl << endl;
			mutation();
			cout << endl;
			cout << "\n\n\t\t SURVIVAL SELECTION: " << endl << endl;
			survivalSelection();
		}
		//getchar();
		copyChromosome();
		
	}
	//parent until survival, have to do loop because once run only produce 2 children. we want more children
	bestFitnessFile.close();
	avgFitnessFile.close();
	bestChromosomeFile.close();
}