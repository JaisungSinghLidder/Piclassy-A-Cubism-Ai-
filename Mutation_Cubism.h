#ifndef MUTATION_CUBISM_H
#define MUTATION_CUBISM_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "Cubism_Gene_Revised.h"
#include <cstdlib>  // For rand()


//Okay I am going to create now the crossover function so that we can inherit two of the parents gene
//Now for the Cubism gene there is a lot of indepedence between them
//As for example the shape wouldn't necessarily depend on the colour
//Therefore, I will use uniform crossover to give the greatest amount of diversity within the function 


class Mutation_Cubism
{
    public:

    vector<Cubism_Gene> uniformCrossover(vector<Cubism_Gene> parentOne, vector<Cubism_Gene> parentTwo);

    void mutation(vector<Cubism_Gene>& preMutatedChromosome, cv::Mat& ogImg, double mutationRate );

};

#endif // MUTATION_CUBISM_H