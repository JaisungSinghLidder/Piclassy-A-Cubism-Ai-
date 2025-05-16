#include "Mutation_Cubism.h"



    vector<Cubism_Gene> Mutation_Cubism::uniformCrossover(vector<Cubism_Gene> parentOne, vector<Cubism_Gene> parentTwo)
    {
        //Okay now creating the child who will then have his genes set.
        //Basically with uniform crossover, it shall do a coin flip on each gene and assign that gene from the respective parent
        //For example if parent one was "Heads", and it flipped to heads for the shape, then the child would have the shape gene 

        vector<Cubism_Gene> childChoromsome(parentOne.size());

        //Since these are two vectors of gene, we have to set each one to their respective loop and assign each each gene to the loop 
        
        for (int i = 0; i < childChoromsome.size(); i++)
        {
            //Flipping the coins for their respective variables 
            //Orginally coded it as one coinflip lets it take from one parent but this way could bring more diversity
            int coinFlipShape = rand() % 2 + 1;
            //int coinFlipPosition = rand() % 2 + 1;
            int coinFlipArea = rand() % 2 + 1;
            int coinFlipColour = rand() % 2 + 1;
            int coinFlipOrientation = rand() % 2 + 1;

            
            int coinFlipX = rand() % 2 + 1;
            int coinFlipY = rand() % 2 + 1;
            
            //Note: 1 for parentOne, 2 for parentTwo

            //Now intializing the variables to the child choromsome
            
            if (coinFlipShape == 1 )
            {
                childChoromsome[i].shapeType = parentOne[i].shapeType;
            }
            else if (coinFlipShape == 2)
            {
                childChoromsome[i].shapeType = parentTwo[i].shapeType; 
            }

            if (coinFlipArea == 1 )
            {
                childChoromsome[i].area = parentOne[i].area;
            }
            else if (coinFlipArea == 2)
            {
                childChoromsome[i].area = parentTwo[i].area;
            }

            if (coinFlipColour == 1)
            {
                childChoromsome[i].colour = parentOne[i].colour;
            }
            else if (coinFlipColour == 2)
            {
                childChoromsome[i].colour = parentTwo[i].colour;
            }

            if (coinFlipOrientation == 1)
            {
                childChoromsome[i].orientation = parentOne[i].orientation;
            }
            else if (coinFlipOrientation == 2)
            {
                childChoromsome[i].orientation = parentTwo[i].orientation;
            }
        
            if (coinFlipX == 1)
            {
                childChoromsome[i].x = parentOne[i].x;
            }
            else if (coinFlipX == 2)
            {
                childChoromsome[i].x = parentTwo[i].x;
            }

            if (coinFlipY == 1 )
            {
                childChoromsome[i].y = parentOne[i].y;
            }
            else if (coinFlipY == 2)
            {
                childChoromsome[i].y = parentTwo[i].y; 
            } 

        }

        return childChoromsome;
        
    }

    //Now we need to mutate the chromosome to bring some entropy and diversity into the code     
    //I may need to also pass the original image to this as well to allow for it to randomize the area 
    //I am going with the implementation of having the original image there for now 

    //I am hoping by taking the mutation rate, I will be able to mimic a simulated annealing affect where the mutation slows down as we approach the end nearer
    //Sort of like how simulated annealing is losing heat, I want this to encourage more diversity at the start and then allow it to focus in near the end. 
    void Mutation_Cubism::mutation(vector<Cubism_Gene>& preMutatedChromosome, cv::Mat& ogImg, double mutationRate)
    {

        //I am taking the original image so that we can do those area calculations.
        int canvasArea = ogImg.cols * ogImg.rows;
        //Chance for mutation to occur
        double mutationChance;
        //This will be 
        int mutationCount;
        int mutateChoice;

        //Times this by 10 since the values are 0.10, 0.3, 0.5
        //Which will then equal 1, 3, 5

        mutationCount = static_cast<int>(mutationRate*10); 

        //Just creating the shapes so that I don't have to repeatable create this area 
        string shapeChoices[] = {"rectangle","triangle","circle", "square"};
            
        for (int i = 0; i < preMutatedChromosome.size(); i++)
        {

            //Creating a mutation chance that goes from 0.1 to 1.0
            mutationChance = (rand() % 101)/100.0;

            for (int j = 0; j < mutationCount; j++)
            {
                //If the mutation chance is smaller than the mutation rate, 
                //Then just skip it, as the mutation rate lowers through the generations
                //It should become more focus in it local neighbourhood
                if (mutationChance >= mutationRate)
                {
                    continue;
                }
                else
                {
                    //We are now going to randomize and randomly pick one thing to mutate 
                    mutateChoice = rand() % 4 + 1;
                    //Quick Guide:
                    //1:Shape 
                    //2:Colour
                    //3:Orientation
                    //4:Area

                    if (mutateChoice == 1)
                    {  
                        int shapePicker = rand() % 4; 
                        string newShapeType = shapeChoices[shapePicker];
                        preMutatedChromosome[i].shapeType = newShapeType;
                    }
                    else if (mutateChoice == 2)
                    {  
                        preMutatedChromosome[i].colour = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
                    }
                    else if (mutateChoice == 3) 
                    {  
                        preMutatedChromosome[i].orientation = rand() % 361;
                    }
                    else if (mutateChoice == 4) 
                    {     
                   
                        int minShapeArea = static_cast<int>(canvasArea * 0.01);

                        if (minShapeArea < 1) 
                        {
                            minShapeArea = 1;
                        }

                        int maxShapeArea = static_cast<int>(canvasArea * 0.05);
                        
                        preMutatedChromosome[i].area = rand() % (maxShapeArea - minShapeArea + 1) + minShapeArea;

                    }   

                }
            }
        }
    }
    








