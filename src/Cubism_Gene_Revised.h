#ifndef CUBISM_GENE_REVISED_H
#define CUBISM_GENE_REVISED_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdlib>  
#include <ctime> 
#include <cmath>
#include <algorithm>


using namespace std;

// Declaration of the global rotatePoint function
cv::Point rotatePoint(const cv::Point pt, const cv::Point center, int angle);

struct Cubism_Gene 
{

    //Now I will create a constructor that will randomly assign values as a gene 
    //This randomness will allow us to get different genes and find out which one is more "fit"
    //Using the img here so that it will be more transferable in the function.
    //NOTE: might have issues with constantly using this img as a parameter, however we should not be creating a copy everytime due to the "&" which is the address operator

    // Types of shape: rectangle, triangle, circle 
    string shapeType;   
    int area; 
    cv::Scalar colour;   
    int orientation;  
    int x, y; 

    // Constructor to initialize a Cubism_Gene object with random values based on the provided image
    Cubism_Gene(cv::Mat& img);

    //Default constructor 
    Cubism_Gene();

    //I need to overload this operator since I compare two genes together later on
    bool operator==(const Cubism_Gene& other) const;
};

//This global function here is so to actually apply the chromosome's genes to the image to transform it 
//I made it into a global function because I didn't want it tied to an object and it made further implementation easier 
cv::Mat renderCubismTransformation(cv::Mat& img, vector<Cubism_Gene>& cubismChromosome);

#endif // CUBISM_GENE_REVISED_H


