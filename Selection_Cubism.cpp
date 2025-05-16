#include "Selection_Cubism.h"

vector<Cubism_Gene> Selection_Cubism::tournamentSelection(vector<vector<Cubism_Gene>>& population, int sampleSize, cv::Mat& originalImg)
{

    double bestFitness = -(DBL_MAX); 
    
        //Creating the randomly selected sample population
        //https://medium.com/@c4lv1nmcg0wan/genetic-algorithms-tournament-selection-b150bc76f0d8 
        //That above code suggests to use 20% of your population so that what the key value should be 

        vector<vector<Cubism_Gene>> randomlySelectedPopulation; 

        for (int i = 0; i < sampleSize; i++)
        {
            int randomChromosomeValue = rand() % population.size();
            randomlySelectedPopulation.push_back(population[randomChromosomeValue]);
        }

        //Now with this amount we just need to use our fitness function to compare both.
        // Initialize the Fitness_Cubism class to evaluate the fitness of the images
         Fitness_Cubism fitnessEvaluator;

        //Now we will take the first part of the random population to compare it after
        vector<Cubism_Gene> bestSelectedChromosome;
     
        for (int j = 0; j < randomlySelectedPopulation.size(); j++)
        {
            cv::Mat currentImageTransformed = renderCubismTransformation(originalImg,randomlySelectedPopulation[j]);  
            
            double comparableFitness = fitnessEvaluator.fitnessTest(originalImg,currentImageTransformed);

            if (comparableFitness > bestFitness)
            {
                bestFitness = comparableFitness;
                bestSelectedChromosome = randomlySelectedPopulation[j];
            }

        }

        //Now we just need to delete the element by searching through it and deleting it
        for (int k = 0; k < population.size(); k++)
        {
            //This piece of code is why we need to override (==)
            if (bestSelectedChromosome == population[k])
            {
                //Find where it matches and delete it
                //We are doing this for elitism, check main function to see descripition
                population.erase(population.begin() + k);
                //Just exiting the loop since we no longer need to search through the vector
                break;
            }
        }

        //DEBUGGING
        cout << "THE BEST FITNESS IS " << bestFitness << endl; 

        //Now we shall finally return the best selected chromosome after the tournamnet selection. 
        return bestSelectedChromosome;
}

    
    
