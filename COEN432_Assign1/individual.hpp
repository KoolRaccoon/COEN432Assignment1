//
//  individual.hpp
//  COEN432_Assign1
//
//  Created by felix Adrian lucaciu on 2018-01-23.
//  Copyright © 2018 Felix. All rights reserved.
//

#ifndef individual_hpp
#define individual_hpp

#include <stdio.h>
#include <vector>
#include <ctime>
#include <cstdlib>

#endif /* individual_hpp */

class Individual {

public:
    //Variables
    std::vector <std::vector<int>> Solution;
    int IndividualFitness;
    
    //Functions
    Individual();
    Individual(int, int, int);
    std::vector <std::vector<int>> GetIndividual();
    void SetIndividual(std::vector <std::vector<int>>);
    void FitnessEvaluation(std::vector<std::vector<int>>, int, int, int);
    void SetFitness(int);
    int GetFitness();
    ~Individual();
    
    
private:

    
    
};