#pragma once
#include "utility.h"
#include "Params.h"
#include <time.h>
#include <Windows.h>

using namespace std;
static void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

static void SetTextColor(int foreground, int background)
{
	int finalColor = (16 * background) + foreground;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), finalColor);
	return;
}
//unused but potential for expanding the base gene into a more robust object type
struct Item
{
	string itemName;
	int itemScore;
	int itemWeight;
	int itemLocation;
	Item();
	Item(const std::string& name, int score, int weight);
	Item(const std::string& name, int score, int weight, int location);
};

struct ItemGenome
{
	//GENES
	vector<int> itemStates;	//gene base

	//CALC METRICS
	vector<int> itemScores;
	vector<double> itemWeights;

	//TOTALS
	double fitnessRating;
	double totalWeight = 0;
	int totalScore = 0;

	//FUNCS
	void RerollGenome()
	{
		itemStates.clear();
		double totalWc = 0;
		for (int i = 0; i < CHROMO_LENGTH; ++i)
		{
			itemStates.push_back(RandInt(0, 1));
		}

		for (int f = 0; f < CHROMO_LENGTH; f++)
		{
			if (itemStates[f] == 1)
			{
				totalWc += itemWeights[f];
			}
		}

		if (totalWc < INVENTORY_CAPACITY)
		{
			//continue
		}
		else
		{
			RerollGenome();
		}

	}
	ItemGenome() :fitnessRating(0) {}
	ItemGenome(const int num_items) :fitnessRating(0)
	{
		srand((unsigned)time(NULL));
		for (int i = 0; i < num_items; ++i)
		{
			itemStates.push_back(RandInt(0, 1));
		}
	}
	ItemGenome(const int num_items, vector<int> itemScoreSet, vector<double> itemWeightSet) :fitnessRating(0)
	{
		bool reroll = false;
		for (int i = 0; i < num_items; ++i)
		{
			itemStates.push_back(RandInt(0, 1));
			//generic vectors, could also be a pair
			itemScores.push_back(itemScoreSet[i]);
			itemWeights.push_back(itemWeightSet[i]);

			//struct object
			if (itemStates[i] == 1)
			{
				totalScore += itemScores[i];
				totalWeight += itemWeights[i];
			}
		}
		if (totalWeight > INVENTORY_CAPACITY)
		{
			reroll = true;
		}
		if (reroll == true)
		{
			itemStates.clear();
			for (int i = 0; i < num_items; ++i)
			{
				itemStates.push_back(RandInt(0, 1));
				if (itemStates[i] == 1)
				{
					totalScore += itemScores[i];
					totalWeight += itemWeights[i];
				}
			}
		}
	}
};

class ItemGA
{
private:
	//GA Params
	vector<ItemGenome> m_Genomes;	//genome population
	bool m_Running;

	//Gene + Genome Stat track
	int m_Gene;
	int m_FittestGenome;
	double m_BestFitness;
	double m_WorstFitness;
	double m_TotalFitness;
	int m_CurrentGeneration;
	//Chromosome
	int m_ChromoLength = CHROMO_LENGTH;
	//population size
	int m_PopulationSize = POP_SIZE;
	double m_CrossoverRate = CROSSOVER_RATE;
	double m_MutationRate = MUTATION_RATE;
	//GA Functions and Features
	void Mutation(vector<int>& itemstates);
	void SinglePointCrossover(ItemGenome& p1, ItemGenome& p2, ItemGenome& c1, ItemGenome& c2);
	void TwoPointCrossover(ItemGenome& p1, ItemGenome& p2, ItemGenome& c1, ItemGenome& c2);	
	ItemGenome& RouletteSelection();
	void UpdateFitness();
	void CreateNewPopulation();
	void CreateNewPopulation(vector<int> itemScoreSet, vector<double> itemWeightSet);

public:

	//Constructor	//blank obj construct
	ItemGA();
	vector<string> items;	//itemName fill
	void AddItemInfo(vector<int> itemScoreSet, vector<double> itemWeightSet);
	//Main Caller
	void Epoch();
	//return of the current generation
	int CurrentGeneration();
	//return of the fittest genome in the generation
	int GetFittestGenome();
	void FittestGenomesStats();
	//operator to tell if the GA has began running
	bool hasStarted();
	//operator to stop the GA from running.
	void StopGA();
};