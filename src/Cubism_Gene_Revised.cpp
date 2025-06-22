#include "Cubism_Gene_Revised.h"

//I am going to create a rotate point function so that I can call it easier 
//https://stackoverflow.com/questions/7953316/rotate-a-point-around-a-point-with-opencv
//This above was used to help generate the function.

cv::Point rotatePoint(cv::Point pt, cv::Point centerPoint, int orientation )
{
    //Coverting it into radians 
    double rad = orientation * (CV_PI / 180);

    int rotatedPointX = ( (pt.x - centerPoint.x) * cos(rad) ) - ( (pt.y - centerPoint.y) * sin(rad) ) + centerPoint.x;
    int rotatedPointY = ( (pt.x - centerPoint.x) * sin(rad) ) + ((pt.y - centerPoint.y) * cos(rad)) + centerPoint.y;

    return cv::Point(rotatedPointX, rotatedPointY); 
}

//Now I will create a constructor that will randomly assign values as a gene 
//This randomness will allow us to get different genes and find out which one is more "fit"
//Using the img here so that it will be more transferable in the function.
//NOTE: might have issues with constantly using this img as a parameter, however we should not be creating a copy everytime due to the "&" which is the address operator

Cubism_Gene::Cubism_Gene(cv::Mat& img)
{
    //Chose to make these variables as it is easier to understand while writing canvas height and width than cols or rows
    int canvasWidth = img.cols; 
    int canvasHeight = img.rows;
 
    string shapeChoices[] = {"rectangle","triangle","circle", "square"};

    //Now creating a value that will randomly pick one of these shapes
    int shapePicker = rand() % 4; 

    shapeType = shapeChoices[shapePicker];

    //Creating the canvas area so that we can assign as size to the shapes and so that they can't be a bigger size 
    int canvasArea = canvasHeight * canvasWidth;

    //This will create a 360 degrees, to change the rotation of the object
    orientation = rand() % 361;    
        
    //The shape's area here
    //I am statically casting it to int because I want to deal with the values being int and not having to deals with decimal;
    //This is because I am going to use a rand() variable

    //I want the max size to be at most 5% of the canvas area for a single shape
    //I also don't want it to be too much because of the margins that will be included
    //This max is also just to make sure we don't get zero from rounding issues 
    int maxShapeArea = max(1, static_cast<int>(canvasArea * 0.05));
        
    //Just adding the one so that there isn't a chance for the area to be 0
    //Since why would I want it to draw nothing 
    area = rand() % (maxShapeArea) + 1; 

    //Just randomizing the colours
    colour = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);

}

Cubism_Gene::Cubism_Gene() 
{
    shapeType = "";               
    orientation = 0;              
    area = 0;                     
    colour = cv::Scalar(0, 0, 0);
    x = 0;                        
    y = 0;                        
}

bool Cubism_Gene::operator==(const Cubism_Gene& other) const 
{
    //Can judge if two things are the same just by checking if each of the variables are the same
    return (shapeType == other.shapeType && orientation == other.orientation && area == other.area && colour == other.colour);
}

cv::Mat renderCubismTransformation(cv::Mat& img, vector<Cubism_Gene>& cubismChromosome)
{
    //Now we need to create a blank canvas in which it will draw on.
    cv::Mat transformedImg = cv::Mat::zeros(img.size(), img.type());
    
    //Need to know the dimensions of the canvas so that we can check the bounds
    int canvasWidth = img.cols; 
    int canvasHeight = img.rows;

    //Going to include margins, why? 
    //This is because when if the shapes rotates, sometimes it doesn't stay in the previous bounds anymore
    //By adding a margin, we should be able to make sure it doesn't rotate off screen 
    //Margins will have to be calculated in the loop though because we need to know the gene's area
    int marginX, marginY;

    //We are also going to need to know the width and the height
    int width,height;

    //Just for circle case 
    int radius; 

    //Just for triangle case 
    //Could use width but it gets confusing for me to look at 
    int base; 

    //We are going to need the average of x and y
    //When we rotate the shapes, we need to know the center of the shape
    int xAverage, yAverage;

    //I would also like to note most of the general logic will be in the rectangle case as it is the first
    //The rest will only have comments taking about special logic cases about itself

    //This is where it starts drawing the shapes
    for (int j = 0; j < cubismChromosome.size(); j++ )
    {
        if (cubismChromosome[j].shapeType == "rectangle")
        {
            //Formula Rectangle: Length times Width
            //But one has to be shorter than the other

            //Since rectangle widths are smaller that the lengths 
            //This is to prevent the case where the width equals to 0 after being casted
            int maxWidth = std::max(1, static_cast<int>(cubismChromosome[j].area * 0.40));

            width = rand() % maxWidth + 1;

            height = static_cast<int>(cubismChromosome[j].area / static_cast<double>(width));
            //height = cubismChromosome[j].area / width;
            
            //Creating margins to prevent the rotation from taking up too much space
            marginX = static_cast<int>(width * 1.5);
            marginY = static_cast<int>(height * 1.5);

            //Okay for this random calculations of x and y
            //When calculating it, we times it by two to create that buffer space around it so we are only left with the center space 
            //Then we add the marginX after to get one of the margin side's back
            cubismChromosome[j].x = rand() % (canvasWidth - 2 * marginX + 1) + marginX;
            cubismChromosome[j].y = rand() % (canvasHeight - 2 * marginY + 1) + marginY;


            

            cv::Point pt1(cubismChromosome[j].x, cubismChromosome[j].y);                     // top-left
            cv::Point pt2(cubismChromosome[j].x + width, cubismChromosome[j].y);             // top-right
            cv::Point pt3(cubismChromosome[j].x + width, cubismChromosome[j].y + height);    // bottom-right
            cv::Point pt4(cubismChromosome[j].x, cubismChromosome[j].y + height);            


            //Now that we have created the shape we now want to rotate the shape and apply the rotation to the image 
            //We need to get the center of the rectangle though to appply the rotation matrix
            //We can use this simple calculation for simple centre 
            //Using (x1,y1) and (x2,y2) verticies
            //The center is the ((x1+x2)/2,(y1+y2)/2)

            xAverage = ((pt1.x + pt2.x)/2); 
            yAverage = ((pt1.y + pt2.y)/2);

            cv::Point center(xAverage,yAverage);

            //Okay now all we need to do now is rotate both the points by the new orientation to give us.
            cv::Point pt1Rotated = rotatePoint(pt1,center,cubismChromosome[j].orientation);
            cv::Point pt2Rotated = rotatePoint(pt2,center,cubismChromosome[j].orientation);
            cv::Point pt3Rotated = rotatePoint(pt3, center,cubismChromosome[j].orientation);
            cv::Point pt4Rotated = rotatePoint(pt4,center, cubismChromosome[j].orientation);
                

            //NEW IMPELMENTATION: 
            //I originally used the cv::rectangle for this but found out through research that you can use this to draw the rotated shape
            //https://answers.opencv.org/question/14807/fill-an-image-with-the-content-of-rotatedrect/
            //Found this information using the first answer in the form above 

            vector<cv::Point> points = { pt1Rotated, pt2Rotated, pt3Rotated, pt4Rotated };
            cv::fillConvexPoly(transformedImg, points, cubismChromosome[j].colour);                
        }
        else if (cubismChromosome[j].shapeType == "square")
        {
            //Now because this is a square. Width and height will simply be square root
            //Square calculation: side x side = area
            //Everything else is the same as the rectangle

            // Width and height for square = sqrt(area), with a minimum of 1
            int maxWidth = std::max(1, static_cast<int>(sqrt(cubismChromosome[j].area)));

            width = maxWidth;
            height = width;

            width = maxWidth;
    
            height = width; 

            marginX = static_cast<int>(width * 1.5);
            marginY = static_cast<int>(height * 1.5);

            cubismChromosome[j].x = rand() % (canvasWidth - 2 * marginX + 1) + marginX;
            cubismChromosome[j].y = rand() % (canvasHeight - 2 * marginY + 1) + marginY;

            cv::Point pt1(cubismChromosome[j].x, cubismChromosome[j].y);                     // top-left
            cv::Point pt2(cubismChromosome[j].x + width, cubismChromosome[j].y);             // top-right
            cv::Point pt3(cubismChromosome[j].x + width, cubismChromosome[j].y + height);    // bottom-right
            cv::Point pt4(cubismChromosome[j].x, cubismChromosome[j].y + height);            // bottom-left

            xAverage = ((pt1.x + pt2.x)/2);
            yAverage = ((pt1.y + pt2.y)/2);

            cv::Point center (xAverage, yAverage);

            cv::Point pt1Rotated = rotatePoint(pt1,center,cubismChromosome[j].orientation);
            cv::Point pt2Rotated = rotatePoint(pt2,center,cubismChromosome[j].orientation);
            cv::Point pt3Rotated = rotatePoint(pt3,center,cubismChromosome[j].orientation);
            cv::Point pt4Rotated = rotatePoint(pt4,center,cubismChromosome[j].orientation);

            vector<cv::Point> squarePoints = { pt1Rotated, pt2Rotated, pt3Rotated, pt4Rotated };

            cv::fillConvexPoly(transformedImg, squarePoints, cubismChromosome[j].colour);

            }
            else if (cubismChromosome[j].shapeType == "circle")
            {
                //Now we have to create a circle. 
                //I am going to re arrange the equation for a circle to find the area.
                //Circle area forumla: area = pi(r)^2

                radius = static_cast<int>(std::max(1.0, sqrt(cubismChromosome[j].area / CV_PI)));


                //Checking if the radius is too small 
                /*if (radius < 1) 
                {
                    radius = 1;
                }*/ 
                
                //Now why dont't I have margins here, well that's because I don't need them
                //Why? Well tried to rotate a circle and see the difference, there is no difference
                //So we don't need margins, but we use the radius here in place of the margins with the same idea
                //so that we can make sure it doesn't go out of bounds

                cubismChromosome[j].x = rand() % (canvasWidth - 2 * radius + 1) + radius;
                cubismChromosome[j].y = rand() % (canvasHeight - 2 * radius + 1) + radius;


                cv::Point center(cubismChromosome[j].x, cubismChromosome[j].y);

                //Since we don't need rotation
                //Just using the circle function 
                cv::circle(transformedImg, center, radius, cubismChromosome[j].colour, -1);
            }
            else if (cubismChromosome[j].shapeType == "triangle")
            {
                //Now we have to create the triangle 
                //I have to create the equation. 
                //Triangle equation (1/2)Base x Height = Area

                int maxHeight = std::max(1, static_cast<int>(sqrt(2 * cubismChromosome[j].area)));
                height = std::max(1, rand() % maxHeight + 1);

                // Avoid division-by-zero
                base = std::max(1, (2 * cubismChromosome[j].area) / height);

                // Calculate safe x and y so triangle stays on canvas, add margin for rotation
                marginX = static_cast<int>(base * 1.5);
                marginY = static_cast<int>(height * 1.5);

                cubismChromosome[j].x = rand() % (canvasWidth - 2 * marginX + 1) + marginX;
                cubismChromosome[j].y = rand() % (canvasHeight - 2 * marginY + 1) + marginY;

                vector<cv::Point> trianglePoints;

                //Here we are creating a right angle triangle by doing the before mention creating a rectangle and then cutting it in half.
                //This is the bottom left point
                cv::Point pt1(cubismChromosome[j].x, cubismChromosome[j].y);
                //This is the bottom right point
                cv::Point pt2(cubismChromosome[j].x + base, cubismChromosome[j].y);
                //This is the top right corner 
                cv::Point pt3(cubismChromosome[j].x + base, cubismChromosome[j].y - height);

                int xAverage = ((pt1.x + pt2.x + pt3.x)/3);
                int yAverage = ((pt1.y + pt2.y + pt3.y)/3);
                
                cv::Point center(xAverage, yAverage);

                cv::Point pt1Rotated = rotatePoint(pt1,center,cubismChromosome[j].orientation);
                cv::Point pt2Rotated = rotatePoint(pt2,center,cubismChromosome[j].orientation);
                cv::Point pt3Rotated = rotatePoint(pt3,center,cubismChromosome[j].orientation);

                trianglePoints.push_back(pt1Rotated);
                trianglePoints.push_back(pt2Rotated);
                trianglePoints.push_back(pt3Rotated);

                cv::fillConvexPoly(transformedImg, trianglePoints, cubismChromosome[j].colour);
            }   
    }

    return transformedImg; 
}
        


