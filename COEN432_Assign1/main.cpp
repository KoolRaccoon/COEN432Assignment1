//  main.cpp
//  COEN432_Assign1

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "individual.hpp"

using namespace std;

/*Steps:
        -> Read and extract data from the .txt file
                - K: Size of the table
                - N: The number of guests in the wedding
        -> Make K arrays of size determined by N/K.
        -> Extract preference of every guest and put into a vector of (N+1)x(N+1)
        -> Find a way to display an individual possibility
        -> Generate a population of individuals
        -> Evaluate fitness of individuals
        -> Select Parents for crossover
        -> Implement Crossover and mutation and
        -> Evaluate Fitness of children
        -> Select New Parents from children
        -> Repeat Implementing Crossover and mutation Fitness = 0;
        -> Display best solution.
 
 
 

 */
//todo: fix case when there are empty seats in a solution

vector<Individual> ParentSelection(vector<Individual>, int);
Individual Crossover(Individual, Individual, vector<vector<int>>, int, int, int);
Individual Mutation(Individual, vector<vector<int>>, int, int, int);
vector<Individual> SurvivorSelection(vector<Individual>, int, int, int, int);

int main(int argc, const char * argv[]) {
//========================================================================================================//
    //This first part is to extract data out of the input files
    srand(time(NULL));
    ifstream SettingsFile;
    ifstream GuestsFile("/Users/Felix/school/University/Winter_2018/COEN432/COEN432Assignment1/COEN432_Assign1/preferences1.csv");
    int NumGuests = 0; // Number of guests
    int TableSize = 0; // Size of the tables
    int NumTables = 0; // Number of tables
    vector <vector<int>> GuestPrefTable; // A table that will contain the preferences of each guest -> the Preference Table
    
    //Get settings information from the text file
    SettingsFile.open("/Users/Felix/school/University/Winter_2018/COEN432/COEN432Assignment1/COEN432_Assign1/settings.txt");
    SettingsFile >> TableSize >> NumGuests;
    SettingsFile.close();
    
    //Determine Number of tables
    if (NumGuests%TableSize == 0)
        NumTables = NumGuests/TableSize;
    else
        NumTables = NumGuests/TableSize + 1;
    //Displays Settings Information
    cout << "Table Size is: " << TableSize << endl;
    cout << "Number of guests are: " << NumGuests << endl;
    cout << "Number of tables are: " << NumTables << endl;
    cout << endl;
    
    string Value;
    string Line;
    vector <int> row;
    vector <int> ConvertToInt;
    
    // Retrieving information from the CSV file
    while (GuestsFile.good()){
        // Retrieving rows from the CSV file
        for (int i=0; i<NumGuests; i++){
            getline(GuestsFile, Value, ',');
                if (Value == "")
                    row.push_back(0);
                else
                    row.push_back(stoi(Value));
        }
        getline(GuestsFile, Value, '\r');
        if (Value == "")
            row.push_back(0);
        else
            row.push_back(stoi(Value));
        getline(GuestsFile, Value, '\n');

        // Moving each retrieved row into the Preference Table
        GuestPrefTable.push_back(row);
        for (int i=0; i<=NumGuests;i++)
            row.pop_back();
    }
    
    //Displaying the Preference Table
    cout << "Displaying the Preference Table:" << endl;
    for (int i=0; i<=NumGuests;i++){
        for (int j=0; j<=NumGuests;j++){
            cout << GuestPrefTable[i][j];
        }
        cout << endl;
    }
    cout << endl;
//========================================================================================================//
// In this second part, the intial population is generated
    
//    vector<vector<int>> TestSolution;
//    vector<int> TestTable1, TestTable2, TestTable3;
//    //15, 0, 13, 14, 0, 12, 9, 10, 11, 0, 7, 8, 2, 4, 3, 5, 6, 1
//    TestTable1.push_back(3);
//    TestTable1.push_back(1);
//    TestTable1.push_back(5);
//    TestTable1.push_back(4);
//    TestTable1.push_back(2);
//    TestTable1.push_back(0);
//    TestSolution.push_back(TestTable1);
//    TestTable2.push_back(9);
//    TestTable2.push_back(10);
//    TestTable2.push_back(8);
//    TestTable2.push_back(11);
//    TestTable2.push_back(7);
//    TestTable2.push_back(0);
//    TestSolution.push_back(TestTable2);
//    TestTable3.push_back(12);
//    TestTable3.push_back(6);
//    TestTable3.push_back(13);
//    TestTable3.push_back(14);
//    TestTable3.push_back(15);
//    TestTable3.push_back(0);
//    TestSolution.push_back(TestTable3);
//    Individual TestIndividual;
//    TestIndividual.SetIndividual(TestSolution);
//    TestIndividual.FitnessEvaluation(GuestPrefTable, NumGuests, NumTables,TableSize);
//    cout << TestIndividual.GetFitness();
//    Mutation(TestIndividual, GuestPrefTable, NumGuests, NumTables, TableSize);
//    cout << endl;
    
    vector<vector<int>> Solution;// Will be used to contain a solution of an individual
    vector<Individual> InitialPopulation;// Will contain the whole population of individuals
    vector<Individual> BestOfEachGeneration;
    int InitialPopulationSize = 500;
    int GenerationNum = 0;
    
    for(;GenerationNum<400;GenerationNum++){
        // Generating initial population and evaluation their fitness
        for (int i=0; i<InitialPopulationSize; i++){
        Individual individual(NumGuests, NumTables, TableSize);
            individual.FitnessEvaluation(GuestPrefTable, NumGuests, NumTables, TableSize);
            InitialPopulation.push_back(individual);
            //cout << i << endl;
        }
        
        //Displaying some Solutions
//        for (int i=0; i<InitialPopulationSize;i++){
//            Solution = InitialPopulation[i].GetIndividual();
//            cout << "Displaying Solution " << i << endl;
//            for (int j=0; j<Solution.size();j++){
//                cout << "Table " << (j+1) << " Guests are -->";
//                for (int k=0; k<Solution[j].size();k++){
//                    cout << Solution[j][k] << ", ";
//                }
//                cout << endl;
//            }
//            cout << "Individual's Fitness: " << InitialPopulation[i].GetFitness() << endl;
//            cout << endl;
//        }
        
//========================================================================================================//
// This is the beginning of the Genetic Algorithm
        
        //---------------Parents Selection process---------------//
        vector<Individual> Parent_Pool;
        int ParentPoolSize = 0.3*InitialPopulationSize;
        
        Parent_Pool = ParentSelection(InitialPopulation, ParentPoolSize);
        //Displaying Selected parents for Crossover
//        cout << "<===========================================================>" << endl;
//        cout << "<===========================================================>" << endl;
//        cout << "<===========================================================>" << endl;
//        cout << "Displaying Selected parents for Crossover" << endl << endl;
//        
//        for (int i=0; i<ParentPoolSize; i++){
//            Solution = Parent_Pool[i].GetIndividual();
//            cout << "Displaying Parent " << i << endl;
//            for (int j=0; j<Solution.size();j++){
//                cout << "Table " << (j+1) << " Guests are -->";
//                for (int k=0; k<Solution[j].size();k++){
//                    cout << Solution[j][k] << ", ";
//                }
//                cout << endl;
//            }
//            cout << "Individual's Fitness: " << Parent_Pool[i].GetFitness() << endl;
//            cout << endl;
//        }
    
        //---------------Children Generation Process---------------//
        vector<Individual> NewPopulation;

        NewPopulation = Parent_Pool;
        // NewPopulation = Crossover(Parent1, Parent2, GuestPrefTable, NumGuests, NumTables, TableSize, InitialPopulationSize);
        // Generating an amount of children equivalent to the Population size
        for (int i=0; i<InitialPopulationSize; i++){
            
            Individual Child;
            Individual Child2;// Used to make a second child in Crossover phase
            int Percentage      = rand() % 100 + 1;// Will be used to determine if individuals crossover, mutate or passover
            int Parent1Pos      = rand() % Parent_Pool.size();
            Individual Parent1  = Parent_Pool[Parent1Pos];

            
            if (Percentage<=50){// There's 50% chance for Crossover
                Individual Parent2;
                int Parent2Pos = rand() % Parent_Pool.size();
                Parent2 = Parent_Pool[Parent2Pos];
                Child   = Crossover(Parent1, Parent2, GuestPrefTable, NumGuests, NumTables, TableSize);
                Child2  = Crossover(Parent1, Parent2, GuestPrefTable, NumGuests, NumTables, TableSize);
                NewPopulation.push_back(Child);
                NewPopulation.push_back(Child2);
            }
            else if (Percentage>50 && Percentage<=90){// There's 30% chance the individual will mutate
                Child = Mutation(Parent1, GuestPrefTable, NumGuests, NumTables, TableSize);
                NewPopulation.push_back(Child);
            }
            else// There's 20% chance that the individual will passover
                NewPopulation.push_back(Parent1);
        }
        
        
        //Displaying New Population resulting from children and parents
//        cout << "<===========================================================>" << endl;
//        cout << "<===========================================================>" << endl;
//        cout << "<===========================================================>" << endl;
//        cout << "Displaying New Population resulting from children and parents" << endl << endl;
//        
//        for (int i=0; i<NewPopulation.size(); i++){
//            Solution = NewPopulation[i].GetIndividual();
//            cout << "Displaying Individual " << i << endl;
//            for (int j=0; j<Solution.size();j++){
//                cout << "Table " << (j+1) << " Guests are -->";
//                for (int k=0; k<Solution[j].size();k++){
//                    cout << Solution[j][k] << ", ";
//                }
//                cout << endl;
//            }
//            cout << "Individual's Fitness: " << NewPopulation[i].GetFitness() << endl;
//            cout << endl;
//        }
    
        //---------------Survivor Selection---------------//
        vector<Individual> SurvivingPopulation;
        SurvivingPopulation = SurvivorSelection(NewPopulation, NumGuests, NumTables, TableSize, InitialPopulationSize);
        InitialPopulation = SurvivingPopulation;
        
        
//        //Displaying Surviving Population after Survivor Selection
//        cout << "<===========================================================>" << endl;
//        cout << "<===========================================================>" << endl;
//        cout << "<===========================================================>" << endl;
//        cout << "Displaying Surviving Population after Survivor Selection" << endl << endl;
//        cout << "Displaying Generation number " << NumGenerations << endl;
//        
//        
//        for (int i=0; i<SurvivingPopulation.size(); i++){
//            Solution = SurvivingPopulation[i].GetIndividual();
//            cout << "Displaying Individual " << i << endl;
//-----------------------------------------Outputs----------------------------------------------//
// This 4th part takes care of displaying outputs
        
        // Searching for the best individual of each generation
        Individual BestIndividual = SurvivingPopulation[0];
        for (int x=1; x<InitialPopulationSize; x++){
            if(SurvivingPopulation[x].GetFitness()<BestIndividual.GetFitness())
                BestIndividual = SurvivingPopulation[x];
        }
        BestOfEachGeneration.push_back(BestIndividual);
        
        cout << "Displaying best individual of Generation " << GenerationNum << endl;
        Solution = BestIndividual.GetIndividual();
            for (int j=0; j<Solution.size();j++){
                cout << "Table " << (j+1) << " Guests are -> ";
                for (int k=0; k<Solution[j].size();k++){
                    cout << Solution[j][k] << ", ";
                }
                cout << endl;
            }
            cout << "Individual's Fitness: " << BestIndividual.GetFitness() << endl;
            cout << endl;
        
        if (BestIndividual.GetFitness() == 0){
            cout << "FOUND BEST FITNESS!" << endl;
            break;
        }
    }
    // Keeping track of the best individual of each generation
    Individual BestIndividualOfAllGen = BestOfEachGeneration[0];
    for (int x=1; x<InitialPopulationSize; x++){
        if(BestOfEachGeneration[x].GetFitness()<BestIndividualOfAllGen.GetFitness())
            BestIndividualOfAllGen = BestOfEachGeneration[x];
    }
    // Displaying the best possible solution found after 400 generations
    cout << "Displaying best individual of all generations " << endl;
    Solution = BestIndividualOfAllGen.GetIndividual();
    for (int j=0; j<Solution.size();j++){
        cout << "Table " << (j+1) << " Guests are -> ";
        for (int k=0; k<Solution[j].size();k++){
            cout << Solution[j][k] << ", ";
        }
        cout << endl;
    }
    cout << "Individual's Fitness: " << BestIndividualOfAllGen.GetFitness() << endl;
    cout << endl;
    
    // Outputing best solution of all generations to the output.csv file
    ofstream OutputFile;
    OutputFile.open ("/Users/Felix/school/University/Winter_2018/COEN432/COEN432Assignment1/COEN432_Assign1/output.csv");
    vector<vector<int>> FinalSolutionVector = BestIndividualOfAllGen.GetIndividual();
    OutputFile << "Displaying Solution of Generation " << GenerationNum << '\n';
    OutputFile << "Guest, Table#, Seat#\n";
    for (int Table = 0; Table<NumTables; Table++){
        for (int Seat = 0; Seat<TableSize; Seat++){
            if (FinalSolutionVector[Table][Seat] != 0){
                OutputFile << FinalSolutionVector[Table][Seat] << ',' << Table << ',' << Seat << '\n';
            }
        }
    }
    OutputFile << "Fitness of this solution is " << BestIndividualOfAllGen.GetFitness() << '\n';
    OutputFile.close();
    return 0;
}

//====================================Function Definitions=======================================//

// This function serves to select parents for the mating phase
vector<Individual> ParentSelection (vector<Individual> population, int MatingPoolSize){
    int Parent1;
    int Parent2;
    int Parent3;
    
    vector<Individual> Population = population;
    vector<Individual> Mating_Pool; // Will contain the individuals that can mate from the previous population
    
    for (int i=0; i<MatingPoolSize; i++){// Selecting parents based on a tournament of 3 without replacement
        //Selecting 3 parents randomly from the remaining population
        Parent1 = rand() % (Population.size() -1);
        Parent2 = rand() % (Population.size() -1);
        Parent3 = rand() % (Population.size() -1);
        
        //Selecting which parent is better of the 3 and adding it to the mating pool
        if (Population[Parent1].GetFitness() < Population[Parent2].GetFitness()){
            if (Population[Parent1].GetFitness() < Population[Parent3].GetFitness())
                Mating_Pool.push_back(Population[Parent1]);
            else
                Mating_Pool.push_back(Population[Parent3]);
        }
            else{
                if (Population[Parent2].GetFitness() < Population[Parent3].GetFitness())
                    Mating_Pool.push_back(Population[Parent2]);
                else
                    Mating_Pool.push_back(Population[Parent3]);
            }
        //Removing selected parents from the population
        Population.erase(Population.begin() + Parent1);
    }
    return Mating_Pool;
}

// This function is used during the crossover phase. The Order-1 Algorithm is used
Individual Crossover (Individual Parent1, Individual Parent2, vector<vector<int>> PreferenceTable, int NumbGuests, int NumTables, int TableSize){

    Individual Child;// Will be used to contain the solution of a child
    vector<int> ChildSolution (NumTables*TableSize);// Will be used to store a concatenated value of the solution of a child
    vector<int> Parent1Sol;// Will be used to store the solution of the parent: concatenating all tables into 1 vector
    vector<int> Parent2Sol;
    vector<vector<int>> FinalChildSolution;
    
    int SwathPosition1 = rand() % ChildSolution.size() + 1;// Used to determine the beginning and end of the swath
    int SwathPosition2 = rand() % ChildSolution.size() + 1;
    
    
    while (SwathPosition1 == SwathPosition2)// Making sure that the swath positions are not equal
        SwathPosition2 = rand() % ChildSolution.size() + 1;
    
    // Making sure that the Swath Position 1 is smaller than Swath Position 2
    // If they're not, we flip them
    if (SwathPosition1 > SwathPosition2){
        int tmp         = SwathPosition1;
        SwathPosition1  = SwathPosition2;
        SwathPosition2  = tmp;
    }
    //Storing the solution of each chosen parent into a 1D vectors
    for (int Table=0; Table<NumTables; Table++){
        for (int Seat=0; Seat<TableSize; Seat++){
            Parent1Sol.push_back(Parent1.GetIndividual()[Table][Seat]);
            Parent2Sol.push_back(Parent2.GetIndividual()[Table][Seat]);
        }
    }
    
    // Fill child solution with swath of Parent1
    for (int a=SwathPosition1; a<=SwathPosition2; a++) // todo should be a<= SwathPosition2 - fixed
        ChildSolution[a] = Parent1Sol[a];
    
    // Fill child solution with remaining values of Parent2
    bool GuestIsAlreadySeated = false;
    for (int j=0; j<ChildSolution.size(); j++){
        for (int k=0; k<ChildSolution.size(); k++){
            if(Parent2Sol[j]==ChildSolution[k] && Parent2Sol[j] != 0)// Check if a guest is already in the new solution
                GuestIsAlreadySeated = true;
        }
        
        for (int m=0; m<ChildSolution.size(); m++){// Add the guest from parent2 to the first empty seat it finds not part of the swath
            if ((ChildSolution[m] == 0) && GuestIsAlreadySeated==false){
                if (Parent2Sol[j] == 0){// If parent2[j] is an empty seat, then add it to the solution directly
                    ChildSolution[m] = Parent2Sol[j];
                    GuestIsAlreadySeated=true;// In this case, the guest will be an empty seat
                }
                else{
                ChildSolution[m] = Parent2Sol[j];
                GuestIsAlreadySeated = true;
                }
            }
            // Once we reach the Swath Position 1, start checking from the swath Position 2 and onward instead.
            if (m==SwathPosition1)
                m=SwathPosition2;
        }
        GuestIsAlreadySeated = false;
    }

    //Store the child solution into it's final form
    int Counter = 0;
    for (int Table=0; Table<NumTables; Table++){
        vector<int> IndividualTable;
        for (int Seat=0; Seat<TableSize; Seat++){
            IndividualTable.push_back(ChildSolution[Counter]);
            Counter++;
        }
        FinalChildSolution.push_back(IndividualTable);
    }
    //Attributing the child his solution and his calculated fitness
    Child.SetIndividual(FinalChildSolution);
    Child.FitnessEvaluation(PreferenceTable, NumbGuests, NumTables, TableSize);

return Child;
}

//Using the Insertion method for Mutation
Individual Mutation(Individual MutatedInd, vector<vector<int>> GuestPreferenceTable, int NumGuests, int NumTables, int TableSize){
    
    vector<vector<int>> MutationSolution;
    //Generate two positions to swap them
    int TablePosition1  = rand() % NumTables;
    int SeatPosition1   = rand() % TableSize;
    int TablePosition2  = rand() % NumTables;
    int SeatPosition2   = rand() % TableSize;
    int TempValue       = 0;
    
    MutationSolution = MutatedInd.GetIndividual();
    
    while (TablePosition1 == TablePosition2)
        TablePosition2 = rand() % NumTables;
    
    while (SeatPosition1 == SeatPosition2)
        SeatPosition2 = rand() % TableSize;
        
    // Swapping two guests in the solution vector
    TempValue = MutationSolution[TablePosition1][SeatPosition1];
    MutationSolution[TablePosition1][SeatPosition1] = MutationSolution[TablePosition2][SeatPosition2];;
    MutationSolution[TablePosition2][SeatPosition2] = TempValue;
    
    MutatedInd.SetIndividual(MutationSolution);
    MutatedInd.FitnessEvaluation(GuestPreferenceTable, NumGuests, NumTables, TableSize);
    
    return MutatedInd;
    
}

//Using Nu + Lambda approach for Survivor Selection
vector<Individual> SurvivorSelection(vector<Individual> NewPopulation, int NumbGuests, int NumTables, int TableSize, int PopSize){
    vector<Individual> SurvivingPopulation;
    int pos_min = 0;
    Individual TempValue;

    //Ordering the NewPopulation based on fitness from least fit to most fit
    for (int i=0; i<NewPopulation.size()-1; i++){
        pos_min = i;
        for (int j=i+1; j<NewPopulation.size(); j++){
            if (NewPopulation[j].GetFitness() < NewPopulation[pos_min].GetFitness()){
                pos_min = j;
            }
            if (pos_min != i){
                TempValue = NewPopulation[i];
                NewPopulation[i] = NewPopulation[pos_min];
                NewPopulation[pos_min] = TempValue;
            }
        }
    }
    
    //The surviving population results in the PopSize worth of individuals with the best fitness
    for (int k =PopSize; k>0; k--)
        SurvivingPopulation.push_back(NewPopulation[k]);
    
    return SurvivingPopulation;
}

