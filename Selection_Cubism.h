#ifndef SELECTION_CUBISM_H
#define SELECTION_CUBISM_H

#include <opencv2/opencv.hpp>
#include "Fitness_Cubism.h"
#include "Cubism_Gene_Revised.h"
#include <vector>
#include <cstdlib>  // For rand()
#include <ctime>    // For time() 
#include <cfloat>   // For DBL_MAX


using namespace std;

class Selection_Cubism 
{
    public:

    //I will use the cubism chromosome and apply the tournmanet selection to get more fit chromosomes.
    //I will take the fitness_cubism.cpp and use it to create the cubism.

    vector<Cubism_Gene> tournamentSelection(vector<vector<Cubism_Gene>>& population, int sampleSize, cv::Mat& originalImg);


};

#endif // SELECION_CUBISM_H