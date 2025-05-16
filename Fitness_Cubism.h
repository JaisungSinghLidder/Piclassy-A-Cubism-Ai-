#ifndef FITNESS_CUBISM_H
#define FITNESS_CUBISM_H

#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <vector> 
#include "Cubism_Gene_Revised.h"

using namespace std;

class Fitness_Cubism
{
    public:
    //We will look at the transformed image and compare it to the original image 
    //Mostly looking at the difference between edges 
    //Also rewarding certain shapes 
    double fitnessTest(cv::Mat ogImg, cv::Mat transformedImg);
};



#endif // FITNESS_CUBISM_H
