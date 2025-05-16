#include "Fitness_Cubism.h"
#include "Mutation_Cubism.h"
#include "Selection_Cubism.h"
#include "Cubism_Gene_Revised.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;


int main() {
    
    //This code should allow for randomness within the program
    srand(time(NULL)); 


    cv::Mat originalImg = cv::imread(/*INSERT IMAGE HERE THAT YOU WANT TO TRANSFORM!!!!!!!!*/);  

    // This is just a check for when the image is empty
    if (originalImg.empty()) {
        cout << "Error: IMAGE NOT FOUND" << endl;
        return -1;
    }

    
    int populationSize = 150;  
    int numGenerations = 150;
    double mutationRate = 0;


    // Outer loop creating chromosomes 
    vector<vector<Cubism_Gene>> population;
    for (int i = 0; i < populationSize; i++) {
        vector<Cubism_Gene> cubistChromosome;
        //Inner loop creating the genes that will go with the chromosome
        for (int j = 0; j < 100; j++) {  
            Cubism_Gene gene(originalImg);
            cubistChromosome.push_back(gene);
        }
        population.push_back(cubistChromosome);
    }

    // These objects will allow us to apply mutation and selection
    Mutation_Cubism mutator;
    Selection_Cubism selector;



    //Now we need to create the loop
    //Within the loop we need to evauluate the fitness
    //Use the tournament selection 
    //Then use the mutation after to introduce more diversity 

    for (int i = 0; i < numGenerations; i++)
    {
        //This will be the image that will be transformed
        //I am just adding this just to make sure the originalImg is not affected if something goes wrong
        cv::Mat transformedImg = originalImg.clone();

        //Now I didn't mention how I was going to tackle this part in my proposal so here a quick summary
        //I will use elitism to keep about a small portion (4) of the population
        //I will then replace the rest with a new generation of chromosome 

        vector<Cubism_Gene> parentOne = selector.tournamentSelection(population, 20, originalImg);
        vector<Cubism_Gene> parentTwo = selector.tournamentSelection(population, 20, originalImg);
        vector<Cubism_Gene> parentThree = selector.tournamentSelection(population, 20, originalImg);
        vector<Cubism_Gene> parentFour = selector.tournamentSelection(population, 20, originalImg);

        //Now we will create two sets of differnt dynasties/familes 
        vector<vector<Cubism_Gene>> dynastyOne;
        vector<vector<Cubism_Gene>> dynastyTwo;

        //Now before we begin doing crossover we need to here define the mutation rate
        //The reason is because as the generations go along, I want the mutation rate to lower and lower as we should be getting more fit individuals.
        //So the mutation rate should make it harder for that to happen as it goes along 
        //I want it to follow a kind of simulated annealing we learned about in class where it lowers as the "temp" lowers 


        // Adjusted mutation rates for 150 generations
        // Adjusted mutation rates for 1000 generations
        if (i < 75)
        {
            mutationRate = 0.5;
        }
        else if (i >= 75 && i < 100) 
        {
            mutationRate = 0.3;
        }
        else if (i >= 100) 
        {
            mutationRate = 0.10;
        }
        

        for (int j = 0; j < 48 ; j++)
        {

            //Generating family of the first offspring 
            vector<Cubism_Gene> childOne = mutator.uniformCrossover(parentOne,parentTwo);
            //Now the mutation will be applied (or not)
            mutator.mutation(childOne,transformedImg,mutationRate);
            //Push it into the first family
            dynastyOne.push_back(childOne);

            //Generating family of the second offspring 
            vector<Cubism_Gene> childTwo = mutator.uniformCrossover(parentThree, parentFour);
            mutator.mutation(childTwo,transformedImg,mutationRate);
            dynastyTwo.push_back(childTwo);

            //If either child is empty
            if (childOne.empty() || childTwo.empty()) {
                cout << "Error: CHILD EMPTY" << i << endl;
            }

        }

        vector<vector<Cubism_Gene>> mergedNewPopulation;
        //Now add the 4 original chromosomes/parents back into the main population
        mergedNewPopulation.push_back(parentOne);
        mergedNewPopulation.push_back(parentTwo);
        mergedNewPopulation.push_back(parentThree);
        mergedNewPopulation.push_back(parentFour);


        //Now insert both of the dynasties back into the new population

        mergedNewPopulation.insert(mergedNewPopulation.end(), dynastyOne.begin(), dynastyOne.end());

        mergedNewPopulation.insert(mergedNewPopulation.end(), dynastyTwo.begin(), dynastyTwo.end());

        //Now replace the original population with this one 

        population = mergedNewPopulation;
        
    }
    
    


    //Now just creating another image just to make sure the original image doesn't get transformed by accident 

    cv::Mat finalImage = originalImg.clone();

    vector<Cubism_Gene> finalChoromsome;

    
    //Now I will do a tournament selection but with the whole population so that it will return the most fit indivdual
    finalChoromsome = selector.tournamentSelection(population,population.size(),finalImage);

    //Putting the result into this cv::Mat
    cv::Mat result = renderCubismTransformation(originalImg,finalChoromsome);


    //Now finally displaying the transformed image 
    cv::imshow("Result", result);

    cv::waitKey(0);
    cv::destroyAllWindows(); 
    

    // BELOW IS DEBUGGING
    // I am just using this to check if the canny edge is being used correctly and playing with the blur, etc 

    /*
   
    cv::Mat img = cv::imread("c:\\Users\\jaisu\\Downloads\\test_cubism_piclassy.jpg");

    if (img.empty()) {
        cout << "Image not found!" << endl;
        return -1;
    }

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

    cv::Mat edges;
    //50, 150
    //Best for Image #3: 50, 300
    cv::Canny(blurred, edges, 50, 150); 

    cv::imshow("Canny Edges", edges);
    cv::waitKey(0);

    */

    
    
    

     
    
}
