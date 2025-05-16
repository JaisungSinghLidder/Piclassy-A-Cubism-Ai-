#include "Fitness_Cubism.h"

double Fitness_Cubism::fitnessTest(cv::Mat ogImg, cv::Mat transformedImg)
{
    double fitness = 0.0; 

    //We are transforming this image by making it grey and applying the blur to make it better for applying that Canny Edge 
    //Same with our transformed image
    //I used this resource to help me understand these processess and Canny Edge detection
    //https://learnopencv.com/edge-detection-using-opencv/ 
    cv::Mat ogGray, transGray;
    cv::cvtColor(ogImg, ogGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(transformedImg, transGray, cv::COLOR_BGR2GRAY);

    cv::Mat ogBlur, transBlur;
    cv::GaussianBlur(ogGray, ogBlur, cv::Size(5, 5), 0);
    cv::GaussianBlur(transGray, transBlur, cv::Size(5, 5), 0);

    //Now we are doing the canny edge detection
    //That numerical values at the end are the thresholds, basically checking the brightness to see the boundaries
    cv::Mat edgesOriginal, edgesTransformed;
    cv::Canny(ogBlur, edgesOriginal, 50, 150);
    //The difference in threshold is because with the 0 & 100 values, then it picks up more shapes from the transformed image to make it better to judge the edge difference
    cv::Canny(transBlur, edgesTransformed, 50, 150);

    //This is where the threshold is being applied to mask the areas that aren't important to the original image
    cv::Mat validRegion;
    cv::threshold(edgesOriginal, validRegion, 1, 255, cv::THRESH_BINARY);

    //Putting the edges onto the "valid region"
    cv::Mat maskedEdgesOriginal, maskedEdgesTransformed;
    edgesOriginal.copyTo(maskedEdgesOriginal, validRegion);
    edgesTransformed.copyTo(maskedEdgesTransformed, validRegion);

    //This is counting for the difference between them now 
    cv::Mat diffEdge;
    cv::absdiff(maskedEdgesOriginal, maskedEdgesTransformed, diffEdge);
    double diffSumEdges = cv::sum(diffEdge)[0];

    //So I wanted to reward it versus penalize it
    //That's why I put it as the denominator 
    //The smaller it is, the bigger it becomes 
    double edgeFitness;
    if (diffSumEdges != 0)
    {
        //The values is quite small, so I times it by a huge value to make it more revelant
        edgeFitness = (1.0 / diffSumEdges) * 10000000;
    }
    //If the difference is 0, somehow it a perfect match so I just give it a huge value as a reward
    else
    {
        edgeFitness = 10000000;
    }

    //This is just checking the difference between the two grayscale images 
    cv::Mat diffPixel;
    cv::absdiff(ogGray, transGray, diffPixel);
    double diffSumPixels = cv::sum(diffPixel)[0];
    
    //Same principal as the last one 
    double similarFitness;
    if (diffSumPixels != 0)
    {
        similarFitness = (1.0/ diffSumPixels) *10000000;
    }
    else 
    {
        similarFitness = 10000000;
    }

    //This is where we use the bitwise_and to reverse the mask and then count the values
    cv::Mat falseEdges;
    cv::bitwise_and(edgesTransformed, ~edgesOriginal, falseEdges); 
    //Used countNonZero instead of sum,
    //Sum was causing issues 
    //However countNonZero will work better with masks
    int strayEdgeCount = cv::countNonZero(falseEdges);

    //Same process
    double edgeFitnessPenalty;
    if (strayEdgeCount != 0)
    {

        edgeFitnessPenalty = -(strayEdgeCount * 10.0); 
    }
    else
    {
        edgeFitnessPenalty = 0.0; 
    }

    vector<vector<cv::Point>> transformedContours;
    vector<cv::Vec4i> transformedHierarchy;
    cv::findContours(edgesTransformed, transformedContours, transformedHierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    //We are going to count all the shapes that it can recognize and then reward certain shapes more
    int totalShapeCounter = 0;
    int triangleShapeCounter = 0;
    int rectangleShapeCounter = 0; 
    int squareShapeCounter = 0; 
    int circleShapeCounter = 0; 
    int noiseCount = 0;
    int cornerCount = 0;


    int imgWidth = ogImg.cols;
    int imgHeight = ogImg.rows;


    for (int i = 0; i < transformedContours.size(); i++)
    {
        vector<cv::Point> approx;
        //It requries this epsilon value, basically checking how simplified should it be 
        double epsilon = 0.02 * cv::arcLength(transformedContours[i], true);
        cv::approxPolyDP(transformedContours[i], approx, epsilon, true);

        int vertices = static_cast<int>(approx.size());
        double area = cv::contourArea(transformedContours[i]);

        if (area < 100.0) {
            noiseCount++;
            continue;
        }

        if (vertices == 3) {
            triangleShapeCounter++;
            totalShapeCounter++;
        }
        //If the bounding box can find 4 vertices 
        else if (vertices == 4) {
            totalShapeCounter++;
            cv::Rect boundingBox = cv::boundingRect(approx);
            double aspectRatio = static_cast<double>(boundingBox.width) / boundingBox.height;
            if (aspectRatio > 0.9 && aspectRatio < 1.1)
            {
                squareShapeCounter++;
            }
            else
            {
                rectangleShapeCounter++;
            }
        }
        else if (vertices > 4) {
            totalShapeCounter++;
            circleShapeCounter++;
        }
    }

    double shapeWeight = (((5.0) * squareShapeCounter) + ((0.8) * circleShapeCounter) + (1.2)*triangleShapeCounter + ((2.0) * rectangleShapeCounter));

    double noiseWeight = noiseCount * -1.0;
    edgeFitnessPenalty *= 0.01;

    //Manually checking weights
    shapeWeight *= 10;
    edgeFitness *= 25.5;
    edgeFitnessPenalty *= 100;
    similarFitness *= 100;
    similarFitness *= 8.2;
    
    fitness = shapeWeight + edgeFitness + noiseWeight + (edgeFitnessPenalty*0.01) + similarFitness;


    //DEBUGGING BLOCK: 

    /*std::cout << "FITNESS: " << fitness << std::endl
              << "SHAPE: " << shapeWeight << std::endl
              << "EDGE FITNESS: " << (edgeFitness) << std::endl
              << "EDGE FITNESS MINUS: " << (edgeFitnessPenalty*0.01) << std::endl
              << "Similar: " << similarFitness << std::endl
              << "NOISE WEIGHT: " << noiseWeight << std::endl; */


    return fitness;

}
