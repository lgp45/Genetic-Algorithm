#include "ItemGA.h"
#include <time.h>

bool sortbysecdesc(const pair<int, double>& a,
	const pair<int, double>& b)
{
	return a.second > b.second;
}

//Main Public Methods
ItemGA::ItemGA()
{

}

void ItemGA::AddItemInfo(vector<int> itemScoreSet, vector<double> itemWeightSet)
{
	CreateNewPopulation(itemScoreSet, itemWeightSet);
}


void ItemGA::Epoch()
{
	srand((unsigned)time(NULL));
	UpdateFitness();

	int children = 0;
	vector<ItemGenome> childGenomes;

	//ELITISM
	childGenomes.push_back(m_Genomes[m_FittestGenome]);

	while (childGenomes.size() < m_PopulationSize)
	{
		//Parents
		ItemGenome p1 = RouletteSelection();
		ItemGenome p2 = RouletteSelection();

		//Childrne
		ItemGenome c1, c2;

		//Cross
		//OnePointCrossover(p1, p2, c1, c2);
		TwoPointCrossover(p1, p2, c1, c2);
		//Mutate
		Mutation(c1.itemStates);
		Mutation(c2.itemStates);
		//push into vector
		childGenomes.push_back(c1);
		children += 1;
		childGenomes.push_back(c2);
		children += 1;
	}
	//fill genome list with new gen
	m_Genomes = childGenomes;

	//push weight check onto new gen, double check for weight overages
	for (int i = 0; i < m_Genomes.size(); i++)
	{
		double wT = 0;
		//grab each held items weight and add into total
		for (int fi = 0; fi < m_ChromoLength; fi++)
		{
			if (m_Genomes[i].itemStates[fi] == 1)
			{
				wT += m_Genomes[i].itemWeights[fi];
			}
		}
		//if we exceed total, reroll, else continue 
		if (wT > INVENTORY_CAPACITY)
		{
			m_Genomes[i].RerollGenome();
		}
	}
	++m_CurrentGeneration;
}

int ItemGA::CurrentGeneration()
{
	return m_CurrentGeneration;
}

int ItemGA::GetFittestGenome()
{
	return m_FittestGenome;
}

bool ItemGA::hasStarted()
{
	return m_Running;
}

void ItemGA::StopGA()
{

}

//Private Decs
void ItemGA::Mutation(vector<int>& itemStates)
{
	for (int item = 0; item < itemStates.size(); item++)
	{
		if (RandFloat() < m_MutationRate)
		{
			//flip the int over
			if (itemStates[item] == 0)
			{
				itemStates[item] = 1;
			}
			else if (itemStates[item] == 1)
			{
				itemStates[item] = 0;
			}
		}
	}
}

void ItemGA::SinglePointCrossover(ItemGenome& p1, ItemGenome& p2, ItemGenome& c1, ItemGenome& c2)
{
	if (RandFloat() > m_CrossoverRate || (p1.itemStates == p2.itemStates))
	{
		c1 = p1;
		c2 = p2;
	}
	c1.itemScores = p1.itemScores;
	c2.itemScores = p2.itemScores;
	c1.itemWeights = p1.itemWeights;
	c2.itemWeights = p2.itemWeights;
	int cutPoint = RandInt(0, m_ChromoLength - 1);

	//swap the sections
	for (int i = 0; i < cutPoint; ++i)
	{
		c1.itemStates.push_back(p1.itemStates[i]);
		c2.itemStates.push_back(p2.itemStates[i]);
	}
	for (int i = cutPoint; i < m_ChromoLength; ++i)
	{
		c1.itemStates.push_back(p2.itemStates[i]);
		c2.itemStates.push_back(p1.itemStates[i]);
	}
}
void ItemGA::TwoPointCrossover(ItemGenome& p1, ItemGenome& p2, ItemGenome& c1, ItemGenome& c2)
{
	if (RandFloat() > m_CrossoverRate || (p1.itemStates == p2.itemStates))
	{
		c1 = p1;
		c2 = p2;
	}
	c1.itemScores = p1.itemScores;
	c2.itemScores = p2.itemScores;
	c1.itemWeights = p1.itemWeights;
	c2.itemWeights = p2.itemWeights;
	int cutPoint = RandInt(0, m_ChromoLength - 1);
	int cutPoint2 = RandInt(0, m_ChromoLength - 1);
	if (cutPoint2 == cutPoint)
	{
		cutPoint2 = RandInt(0, m_ChromoLength - 1);
	}

	//swap the sections
	for (int i = 0; i < cutPoint; ++i)
	{
		c1.itemStates.push_back(p1.itemStates[i]);
		c2.itemStates.push_back(p2.itemStates[i]);
	}
	for (int i = cutPoint; i < m_ChromoLength; ++i)
	{
		c1.itemStates.push_back(p2.itemStates[i]);
		c2.itemStates.push_back(p1.itemStates[i]);
	}
	for (int i = 0; i < cutPoint2; ++i)
	{
		c1.itemStates.push_back(p1.itemStates[i]);
		c2.itemStates.push_back(p2.itemStates[i]);
	}
	for (int i = cutPoint2; i < m_ChromoLength; ++i)
	{
		c1.itemStates.push_back(p2.itemStates[i]);
		c2.itemStates.push_back(p1.itemStates[i]);
	}
}

ItemGenome& ItemGA::RouletteSelection()
{
	double fitSlice = RandFloat() * m_TotalFitness;

	double curFitTotal = 0.0;

	int	Genome = 0;

	for (int i = 0; i < m_PopulationSize; ++i)
	{
		curFitTotal += m_Genomes[i].fitnessRating;

		if (curFitTotal > fitSlice)
		{
			Genome = i;
			break;
		}
	}
	return m_Genomes[Genome];
}


void ItemGA::UpdateFitness()
{
	m_TotalFitness = 0;
	vector<pair<int, double>> a;

	//foreach chromo
	for (int i = 0; i < m_PopulationSize; i++)
	{
		m_Genomes[i].totalScore = 0;
		m_Genomes[i].totalWeight = 0;
		for (int f = 0; f < m_ChromoLength; f++)
		{
			if (m_Genomes[i].itemStates[f] == 1)
			{
				m_Genomes[i].totalScore += m_Genomes[i].itemScores[f];
				m_Genomes[i].totalWeight += m_Genomes[i].itemWeights[f];
			}
		}
		//if the chromo weightTotal exceeds the limitation
		if (m_Genomes[i].totalWeight > 50)
		{
			//derank this - this is a bad genome!
			m_Genomes[i].fitnessRating = -1;
		}
		else
		{
			m_Genomes[i].fitnessRating = m_Genomes[i].totalScore;
		}
		m_TotalFitness += m_Genomes[i].fitnessRating;
	}

	for (int i = 0; i < m_PopulationSize; i++)
	{
		a.push_back(make_pair(i, m_Genomes[i].fitnessRating));
	}

	//create a vector pair of the chromo and its score
	//sort(a.begin(), a.end());
	sort(a.begin(), a.end(), sortbysecdesc);
	for (int i = 0; i < a.size(); i++)
	{
		cout << "\nGenome #" + to_string(a[i].first);
		cout << " Fitness: " + to_string(a[i].second);
		cout << "\t ITEMSTATES: ";
		vector<int> temp;
		for (int f = 0; f < m_ChromoLength; f++)
		{
			cout << to_string(m_Genomes[a[i].first].itemStates[f]);
			if (m_Genomes[a[i].first].itemStates[f] == 1)
			{
				temp.push_back(f);
			}
		}
		cout << "\tItems Held: ";
		for (int i = 0; i < temp.size(); i++)
		{
			cout << "#" + to_string(temp[i]) + " ";
		}
	}

	if (m_Genomes[m_FittestGenome].fitnessRating <= a[0].second)
	{
		m_BestFitness = a[0].second;
		m_FittestGenome = a[0].first;
	}

	int removeFromBot = 0;
	for (int i = 0; i < m_PopulationSize; i++)
	{
		if (m_Genomes[i].fitnessRating <= -1)
		{
			removeFromBot += 1;
		}
	}

	m_WorstFitness = a[a.size() - (1 + removeFromBot)].second;
	//hold over
	cout << endl;
	SetTextColor(7, 2);
	cout << "\nBEST GENOME:  Genome #" + to_string(a[0].first) + " Rating: " + to_string(a[0].second) + "\tItemStates: ";
	

	//Best Genomes item output
	vector<int> temp;
	for (int f = 0; f < m_ChromoLength; f++)
	{
		cout << to_string(m_Genomes[a[0].first].itemStates[f]);
		if (m_Genomes[a[0].first].itemStates[f] == 1)
		{
			temp.push_back(f);
		}
	}
	cout << "\tItems Held: ";
	for (int i = 0; i < temp.size(); i++)
	{
		cout << "#" + to_string(temp[i]) + " ";
	}
	SetTextColor(7, 0);
	cout << "\nBest Inventory Items";
	for (int i = 0; i < m_ChromoLength; i++)
	{
		if (m_Genomes[a[0].first].itemStates[i] == 1)
		{
			cout << "\nItem #" + to_string(i) + ": " + items[i];
		}
	}
	cout << endl;
	SetTextColor(7, 4);
	cout << "\nWORST (within limit) GENOME: Genome #" + to_string(a[a.size() - (1 + removeFromBot)].first) + " Rating: " + to_string(a[a.size() - (1 + removeFromBot)].second);
	SetTextColor(7, 0);
	int avgCalc = m_PopulationSize - removeFromBot;
	cout << "\n\nCALCULATING AVERAGE -- ";
	SetTextColor(4, 0);
	cout << "Removing genomes exceeding INVENTORY CAPACITY...(IF ANY PRESENT)";
	SetTextColor(7, 1);
	cout << "\nAVERAGE RATING: " + to_string(m_TotalFitness / avgCalc);
	SetTextColor(7, 0);


}


//initial convert
void ItemGA::CreateNewPopulation()
{
	//clear the current population now that we are creating anew or creating first set
	m_Genomes.clear();

	for (int i = 0; i < m_PopulationSize; i++)
	{
		m_Genomes.push_back(ItemGenome(m_ChromoLength));
	}
	cout << "Creating population...";

	//clear all calc variables
	m_CurrentGeneration = 0;
	m_FittestGenome = 0;
	m_BestFitness = 0;
	m_TotalFitness = 0;
	m_WorstFitness = 0;
}

//current use
void ItemGA::CreateNewPopulation(vector<int> itemScoreSet, vector<double> itemWeightSet)
{
	//clear the current population now that we are creating anew or creating first set
	cout << "\n-- Creating population --\n";
	for (int i = 0; i < m_PopulationSize; i++)
	{
		m_Genomes.push_back(ItemGenome(m_ChromoLength, itemScoreSet, itemWeightSet));
	}
	//doubke check the genome, if it is over capacity, reroll it
	for (int i = 0; i < m_PopulationSize; i++)
	{
		float totalWc = 0;
		for (int f = 0; f < m_ChromoLength; f++)
		{
			if (m_Genomes[i].itemStates[f] == 1)
			{
				totalWc += m_Genomes[i].itemWeights[f];
			}
		}
		if (totalWc > INVENTORY_CAPACITY)
		{
			m_Genomes[i].RerollGenome();
		}
	}

	//output of creation
	for (int i = 0; i < m_PopulationSize; i++)
	{
		cout << "\nGenome #" + to_string(i);
		float totalW = 0;
		float totalS = 0;
		cout << "\tItemStates: ";
		for (int f = 0; f < m_ChromoLength; f++)
		{
			cout << to_string(m_Genomes[i].itemStates[f]);
			if (m_Genomes[i].itemStates[f] == 1)
			{
				totalW += m_Genomes[i].itemWeights[f];
				totalS += m_Genomes[i].itemScores[f];
			}
		}
		string temp = to_string(totalW);
		string sub = temp.substr(0, 4);
		string temp2 = to_string(totalS);
		string sub2 = temp2.substr(0, 6);
		cout << "\tTotal Weight: " + sub;
		cout << "\tTotal Item Score: " + sub2;
	}

	//clear all calc variables
	m_CurrentGeneration = 0;
	m_FittestGenome = 0;
	m_BestFitness = 0;
	m_WorstFitness = 0;
	m_TotalFitness = 0;
}