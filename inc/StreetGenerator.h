#pragma once
#include <stdlib.h>  // Useful for the following includes.
#include <stdio.h>    
#include <string.h>  // For spring operations.
#include <stdbool.h>

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.

#include <iostream>
#include <vector>



// Represents one block in the grid system
struct Block 
{
   // Represents xz coordinates for placement in OpenGL
   float startX, startY;      // Start Corner
   float endX, endY;	      // End Corner
   // Flags for block type
   bool isStreet;
   bool isIntersection;

   // Block initializer
   Block(float sx = 0.0, float sy = 0.0, 
	 float ex = 0.0, float ey = 0.0, 
	 bool isStr = false, bool isInter = false)
   {
      startX = sx;
      startY = sy;
      endX = ex; 
      endY = ey;
      isStreet = isStr;
      isIntersection = isInter;
   }
};



// Class that represents a 2D array of structs that make up a city grid
class StreetGenerator
{
  private:
   // Number of 'Block' respectively
   int numOfBlocks;
   int numOfStreets;
   int citySize;
   // Scale of the 'Block' types respectively
   float blockScale;
   float streetScale;

   // HELPER FUNCTIONS
   //
   // Generates a grid of data 'Block's
   void InitBlocks();
   // Function returns returns true if index is an intersection
   bool CheckIntersection(const int x, const int y);
   // Refactors the 2D array ensuring intersections are correct
   void UpdateIntersections();
   // Sets the scale type for a given 'Block' index
   Block SetBlockScale(Block b, const int x, const int y, const float scale);



  public:
   // Constructor
   StreetGenerator(int blocks = 20, float bScale = 0.5, float sScale = 0.5);
   // Destructor
   ~StreetGenerator();

   // Generates a simple grid of streets
   void CreateStreets_Simple();
   // Generates a simple grid of streets
   void CreateStreets_Complex(const int complexity);
   // Outputs the current state of each block
   void PrintGridStates();

   int CitySize();

   // Represents the 2D grid of data 'Block's
   std::vector<std::vector<Block> > grid;
};
