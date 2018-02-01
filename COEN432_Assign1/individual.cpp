//
//  individual.cpp
//  COEN432_Assign1
//
//  Created by felix Adrian lucaciu on 2018-01-23.
//  Copyright © 2018 Felix. All rights reserved.
//

#include "individual.hpp"
#include <iostream>
using namespace std;
Individual::Individual(){
    
}

Individual::Individual(int NumGuests, int NumTables, int TableSize){
    int AssignedTable;
    int AssignedSeat;
    std::vector<int> GuestLocation; //Will be used to store the Location of a guest; i.e. Guest#1 -> Table 3: Seat 2
    
    std::vector<std::vector<int>> AssignedGuestPosition; //Will contain the assigned location of each guest
    
    //Randomly generation a location for the first guest
    AssignedTable = rand() % NumTables + 1;
    AssignedSeat  = rand() % TableSize + 1;
    GuestLocation.push_back(AssignedTable);
    GuestLocation.push_back(AssignedSeat);
    
    AssignedGuestPosition.push_back(GuestLocation);
    
    //Randomly generating the location for all the other guests
    for (int i=1; i<NumGuests; i++){
        bool NoDuplicates = false; // Used to make sure that no two guests have the same spot
        
        while (NoDuplicates==false){
            AssignedTable = rand() % NumTables + 1;//Generate random table
            AssignedSeat  = rand() % TableSize + 1;//Generate random seat on a table
            
            for (int j=0;j<i;j++){
                if (AssignedGuestPosition[j][0] == AssignedTable){
                    if (AssignedGuestPosition[j][1] == AssignedSeat){
                        NoDuplicates = false;
                        break;
                    }
                }
                else
                    NoDuplicates = true;
            }
        }
        //Storing the location of each guests
        GuestLocation[0] = AssignedTable;
        GuestLocation[1] = AssignedSeat;
        AssignedGuestPosition.push_back(GuestLocation);
    }
    
    //----------------------------------------------------------------//
    //Ordering the AssignedGuestPosition vector neatly into a solution
    /*  i.e. Solution = {{Table1: Seat#1=Guest1, Seat#2=Guest2...}
                    {Table2: Seat#1=Guest5, Seat#2=Guest3...}
                    ...
                }*/
    
        for (int i=1; i<=NumTables; i++){
            std::vector<int> Table(TableSize); //Will be used to store which guests are seated at a current table;

            for (int j=1; j<=TableSize; j++){
                for (int k=0; k<NumGuests; k++){
                    if(AssignedGuestPosition[k][0]==i){
                        if(AssignedGuestPosition[k][1]==j){
                            Table[j-1]=k+1;
                            break;
                        }
                        
                    }
                }
            }
            Solution.push_back(Table);
        }
    
}


void Individual::FitnessEvaluation(std::vector<std::vector<int>> PreferenceTable, int NumGuests, int NumTables, int TableSize){
    int Penality        = 0;
    int Preference      = 0;
    int guest1, guest2  = 0;
    bool Guest1SittingNextToGuest2  = false;
    bool Guest1SittingNearGuest2    = false;
    
    //Determine the penality for each two different guests
    for (int i=1; i<=NumGuests; i++){
        guest1 = i;
        for (int j=1; j<=NumGuests; j++){
            //Resetting Boolean values for a new check
            Guest1SittingNextToGuest2 = false;
            Guest1SittingNearGuest2   = false;
            
            guest2 = j;
            if (guest1!=guest2){//So we don't compare a guest to himself
                Preference = PreferenceTable[i][j];
//                cout << endl;
//                cout << "comparing guests " << guest1 << " and " << guest2 << endl;
//                cout << "The preferene for these two guests is:" << Preference << endl;
                
                //Determine whether guest1 is sitting next to guest2 or not and if he's sitting near guest2 or not
                for (int m=0; m<NumTables; m++){
                    for (int n=0; n<TableSize; n++){
                        if (Solution[m][n] == guest1){
                            if (n==0){//Corner Case guest1 is sitting on seat 1
                                if(Solution[m][TableSize-1] == guest2 or Solution[m][n+1] == guest2){
                                    Guest1SittingNextToGuest2 = true;
                                    Guest1SittingNearGuest2 = true;
                                    break;
                                }
                            }
                            else if (n==(TableSize-1)){// Corner Case guest1 is sitting on the last seat
                                if (Solution[m][n-1] == guest2 or Solution[m][0] == guest2){
                                    Guest1SittingNextToGuest2 = true;
                                    Guest1SittingNearGuest2 = true;
                                    break;
                                }
                            }
                            else{// General validation if Guest1 is seating next to guest2
                                if (Solution[m][n-1] == guest2 or Solution[m][n+1] == guest2){
                                    Guest1SittingNextToGuest2 = true;
                                    Guest1SittingNearGuest2 = true;
                                    break;
                                }
                            }
                            
                            // Checks if guest2 is sitting on the same table as guest1 (near guest1)
                            for (int k=0; k<TableSize; k++){
                                if (Solution[m][k] == guest2){
                                    Guest1SittingNearGuest2 = true;
                                }
                            }
                        }
                    }
                }
//                cout << "Guest1SittingNextToGuest2 is " << Guest1SittingNextToGuest2 << endl;
//                cout << "Guest1SittingNearGuest2 is " << Guest1SittingNearGuest2 << endl;
                //Determine the Penality based on the preference Table
                switch (Preference){
                    case 1:{
                        if (Guest1SittingNextToGuest2 == true)
                            Penality += 15;
                        else if (Guest1SittingNearGuest2 == true)
                            Penality += 10;
                        break;
                    }
                    case 2:{
                        if (Guest1SittingNextToGuest2 == true)
                            Penality += 15;
                        break;
                    }
                    case 3:{
//                        cout << "went through case 3 - No penalty was added" << endl;
                        break;
                    }
                    case 4:{
                        if (Guest1SittingNearGuest2 == false){
                            Penality += 10;
                            //cout << "went through case 4 - Penalty of 10" << endl;
                            break;
                        }
                        //cout << "went through case 4 - No penalty" << endl;
                        break;
                    }
                    case 5:{
                        if (Guest1SittingNextToGuest2 == false and Guest1SittingNearGuest2 == true){
                            Penality += 15;
//                            cout << "went through if of case 5 - Penalty of 15" << endl;
                        }
                        else if (Guest1SittingNextToGuest2 == false and Guest1SittingNearGuest2 == false){
                            Penality += 20;
//                            cout << "went through elseif of case 5 - Penalty of 20" << endl;
                        }
                        else
//                            cout << "Went through Case 5, but no penality was added" << endl;
                        break;
                    }
                        
                }
            }
            //cout << endl;
        }
    }
    
    IndividualFitness = Penality;
}

int Individual::GetFitness(){
    return IndividualFitness;
}

void Individual::SetFitness(int EvaluatedFitness){
    IndividualFitness = EvaluatedFitness;
}



std::vector <std::vector<int>> Individual::GetIndividual(){
    return Solution;
}


void Individual::SetIndividual(std::vector <std::vector<int>> ComputedSolution){
    Solution = ComputedSolution;
}


Individual::~Individual(){}
