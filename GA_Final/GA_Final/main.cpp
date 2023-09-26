#include "ItemGA.h"
#include <time.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	//First Code Run Through
//	based on the intial testing on my PC with the values it generated on first random, I saw a strong maximum at 273 total item score as BEST GENOME.
//	through various other testing means, consistent results were yielded to hitting a relative maximum consistently. 
//	Occasional undershoot possible due to generation crossover and high mutation rate.
//	Create and fill the item vectors with weight and gearScore
//
//	//Second Code Run Through
//	secondary testing has yielded a consistent result of 200 total genome score for best genome overwall.  
//
//	//Notes
//	//Fitness rating is =  total item score of the inventory
//	//GA limits tested -> breakages at too high length -> xmemory overflow
//	
//////////////////////////////////////////////////////////////////////////////////////////////

//Global GA variable declaration
ItemGA ga;

//place into a separate function for readability and main func cleanliness
vector<string> items()
{
	vector<string> items;
	vector<string> s1;
	vector<string> s2;
	vector<string> s3;

	//prefs
	s1.push_back("Potion");
	s1.push_back("Sword");
	s1.push_back("Helm");
	s1.push_back("Gloves");
	s1.push_back("Mace");
	s1.push_back("Boots");
	s1.push_back("Tome");
	s1.push_back("Scroll");

	//combs
	s2.push_back(" of Potent ");
	s2.push_back(" of Greater ");
	s2.push_back(" of Mighty ");
	//ads
	s3.push_back("Water");
	s3.push_back("Earth");
	s3.push_back("Rage");
	s3.push_back("Holy");
	s3.push_back("Unholy");
	s3.push_back("Luck");
	s3.push_back("Fire");
	s3.push_back("Frost");
	s3.push_back("Haste");
	s3.push_back("Vigor");
	s3.push_back("Speed");

	for (int i = 0; i < CHROMO_LENGTH; i++)
	{
		items.push_back(s1[RandInt(0, s1.size() - 1)] + s2[RandInt(0, s2.size() - 1)] + s3[RandInt(0, s3.size() - 1)]);
	}
	return items;
}

int main()
{
	//basic control operator for user
	int rep = -1;
	//Items Setup FOR GA
	vector<double> gearWeight;
	vector<int> gearScore;
	//Some basic ID
	vector<string> itemNames;


	//Uncomment this srand and comment the other for newly randomized items names, weights and values for each session.
	//srand((unsigned)time(NULL));	
	
	
	
	itemNames = items();	//run the basic name gen and place these names in itemNames so we can print during the run;
	for (int i = 0; i < CHROMO_LENGTH; i++)	//filling of item weight and score vector lists
	{
		double t = (RandFloat() * 15) + 1;
		//float vZ = ceil(t * 100) / 100;
		gearWeight.push_back(t);

		gearScore.push_back(RandInt(5, 50));
	}


	srand((unsigned)time(NULL));		//start grabbing a new random seed for gens now that we have a stable item set


	cout << "Session Loot Table\n";		//Create and Display the current loot table being utilized.
	for (int i = 0; i < CHROMO_LENGTH; i++)
	{
		cout << "\nItem " + to_string(i + 1);
		cout << ": " + itemNames[i] + "\t";
		cout << "\t|Score: " + to_string(gearScore[i]);
		string temp = to_string(gearWeight[i]);
		string sub = temp.substr(0, 4);
		cout << "\tWeight: " + sub;
	}

	ga.items = itemNames;	//pushing of itemNames into a basic variable for easy use -> Separate from genome struc, within main GA
	ga.AddItemInfo(gearScore, gearWeight);	//push all of the weights and create the GA starter population.

	//Main Control Loop
	//while within max gen limits
	while (ga.CurrentGeneration() < MAX_GENERATIONS)
	{
		int maxGens = MAX_GENERATIONS;

		//set text color gold
		SetTextColor(6, 0);
		cout << "\nClicking within the console screen area should pause the program, press enter to resume.";
		cout << "\nEnter 1 to run a single generation, 2 for 10 generations, 3 continuous til max (" + to_string(maxGens) + ")-- \nChoice: ";
		cin >> rep;

		//set text white
		SetTextColor(7, 0);
		//if run single generation
		if (rep == 1)
		{
			system("cls");
			//print loot table
			cout << "Session Loot Table";
			for (int i = 0; i < CHROMO_LENGTH; i++)
			{
				cout << "\nItem " + to_string(i + 1);
				cout << ": " + itemNames[i] + "\t";
				cout << "\t|Score: " + to_string(gearScore[i]);
				string temp = to_string(gearWeight[i]);
				string sub = temp.substr(0, 4);
				cout << "\tWeight: " + sub;
			}
			GotoXY(0, 14);

			//print gen info and begin epoch cycle
			cout << "Current Generation: " + to_string(ga.CurrentGeneration());
			ga.Epoch();

		}
		//if run 10 generations
		if (rep == 2)
		{
			system("cls");
			//print loot table
			cout << "Session Loot Table";
			for (int i = 0; i < CHROMO_LENGTH; i++)
			{
				cout << "\nItem " + to_string(i + 1);
				cout << ": " + itemNames[i] + "\t";
				cout << "\t|Score: " + to_string(gearScore[i]);
				string temp = to_string(gearWeight[i]);
				string sub = temp.substr(0, 4);
				cout << "\tWeight: " + sub;
			}
			//run 10 generations of the epoch func
			for (int i = 0; i < 10; ++i)
			{
				GotoXY(0, 14);
				cout << "Current Generation: " + to_string(ga.CurrentGeneration());
				ga.Epoch();
			}

		}
		//if run until max gens are hit
		if (rep == 3)
		{
			system("cls");
			//print loot table
			cout << "Session Loot Table";
			for (int i = 0; i < CHROMO_LENGTH; i++)
			{
				cout << "\nItem " + to_string(i + 1);
				cout << ": " + itemNames[i] + "\t";
				cout << "\t|Score: " + to_string(gearScore[i]);
				string temp = to_string(gearWeight[i]);
				string sub = temp.substr(0, 4);
				cout << "\tWeight: " + sub;
			}
			//continue epoch until max gen is hit.
			while (ga.CurrentGeneration() < MAX_GENERATIONS)
			{
				GotoXY(0, 14);
				cout << "Current Generation: " + to_string(ga.CurrentGeneration());
				ga.Epoch();
			}

		}
	}

}