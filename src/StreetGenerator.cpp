#ifdef _WIN32
#include "pch.h" // Windows
#endif
#include <cstdlib>
#include <ctime>
#include "StreetGenerator.h"


// Constructor
StreetGenerator::StreetGenerator(int blocks, float bScale, float sScale)
{
	time_t seconds;
	time(&seconds);
	// We create a seed that change each time that we run the code
	srand((unsigned)seconds);

	// Check for bad input, fix it, and assign variables
	blockScale = (bScale <= 0) ? 10.0 : bScale;
	streetScale = (sScale <= 0) ? 10.0 : sScale;
	numOfBlocks = (blocks <= 0 || blocks >= 1000) ? 1 : blocks;
	numOfStreets = numOfBlocks + 1; // streets wrap on outside

	// 2D array size using streets and blocks combined
	citySize = numOfBlocks + numOfStreets;

	// Set up the grid
	InitBlocks();
}

// Destructor
StreetGenerator::~StreetGenerator()
{

}

// Generates a data 'Block' for every index in the 2D array
void StreetGenerator::InitBlocks()
{
	// Set up vector of blocks
	for (int y = 0; y < citySize; y++)
	{
		grid.push_back(std::vector<Block>());
		for (int x = 0; x < citySize; x++)
		{
			// Generate basic block
			Block b;
			b = SetBlockScale(b, x, y, blockScale);
			// Assign it to the current row of the grid
			grid[y].push_back(b);
		}
	}
}

// Sets the scale type for a given 'Block' index
Block StreetGenerator::SetBlockScale(Block b, const int x, const int y, const float scale)
{
	Block updatedBlock = b;
	updatedBlock.startX = x * scale;
	updatedBlock.startY = y * scale;
	updatedBlock.endX = x * scale + scale;
	updatedBlock.endY = y * scale + scale;

	return updatedBlock;
}

// Generates a uniform grid of data 'Blocks' with surrounding streets
void StreetGenerator::CreateStreets_Simple()
{
	// Set up vector of blocks
	for (int y = 0; y < citySize; y++)
	{
		for (int x = 0; x < citySize; x++)
		{
			// Check if intersection
			if (y % 2 == 0 && x % 2 == 0)
			{
				grid[y][x].isStreet = true;
				grid[y][x].isIntersection = true;
				grid[y][x] = SetBlockScale(grid[y][x], x, y, streetScale);
			}
			// Else check if just a street
			else if (y % 2 == 0 || x % 2 == 0)
				grid[y][x].isStreet = true;
		}
	}
	UpdateIntersections();
}

// Generates a randomized grid of data 'Blocks' with possible surrounding streets
void StreetGenerator::CreateStreets_Complex(const int complexity)
{
	// Generate a uniform grid as a template
	CreateStreets_Simple();
	
	// Depricate specific blocks
	// Number of iterations
	for(int i = 0; i < complexity; i++)
	{
		// For each index in the 2D array:
		for (int y = 0; y < citySize; y++)
		{
			for (int x = 0; x < citySize; x++)
			{
				if (grid[y][x].isIntersection)
				{
					// Pick a random street on any of the 4 sides
					// with a 60% chance to not pick any street
					const int side = rand() % 10;
					switch (side)
					{
					case 0:
						// Checks for UP street
						if (y - 1 >= 0 && grid[y - 1][x].isStreet)
						{
							grid[y - 1][x] = SetBlockScale(grid[y - 1][x], x, y - 1, blockScale);
							grid[y - 1][x].isStreet = false;
						}
						break;
					case 1:
						// Checks for RIGHT street
						if (x + 1 < citySize && grid[y][x + 1].isStreet)
						{
							grid[y][x + 1] = SetBlockScale(grid[y][x + 1], x + 1, y, blockScale);
							grid[y][x + 1].isStreet = false;
						}
						break;
					case 2:
						// Checks for DOWN street
						if (y + 1 < citySize && grid[y + 1][x].isStreet)
						{
							grid[y + 1][x] = SetBlockScale(grid[y + 1][x], x, y + 1, blockScale);
							grid[y + 1][x].isStreet = false;
						}
						break;
					case 3:
						// Checks for LEFT street
						if (x - 1 >= 0 && grid[y][x - 1].isStreet)
						{
							grid[y][x - 1] = SetBlockScale(grid[y][x - 1], x - 1, y, blockScale);
							grid[y][x - 1].isStreet = false;
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}
	// Check the current street layout for bad intersections
	UpdateIntersections();
}

// Function returns returns true if index is an intersection
bool StreetGenerator::CheckIntersection(const int x, const int y)
{
	int sides = 0;

	if (y - 1 >= 0 && grid[y - 1][x].isStreet)
		sides++;
	if (x + 1 < citySize && grid[y][x + 1].isStreet)
		sides++;
	if (y + 1 < citySize && grid[y + 1][x].isStreet)
		sides++;
	if (x - 1 >= 0 && grid[y][x - 1].isStreet)
		sides++;

	return (sides < 3);
}

// Refactors the 2D array ensuring intersections are correct
void StreetGenerator::UpdateIntersections()
{
	for (int y = 0; y < citySize; y++)
	{
		for (int x = 0; x < citySize; x++)
		{
			if (grid[y][x].isIntersection)
				if (CheckIntersection(x, y))
					grid[y][x].isIntersection = false;
		}
	}
}

// Quick print to visualize the city layout
void StreetGenerator::PrintGridStates() 
{
	std::cout << "City Block Visualizer" << std::endl;
	for (int y = 0; y < citySize; y++)
	{
		for (int x = 0; x < citySize; x++)
		{
			if (grid[y][x].isIntersection)
				std::cout << "I" << " ";
			else if (grid[y][x].isStreet)
				std::cout << "S" << " ";
			else
				std::cout << " " << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl; // leaves an extra space at the end
}
