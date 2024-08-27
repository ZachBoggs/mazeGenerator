#ifndef MAZEFUNCTIONS_H
#define MAZEFUNCTIONS_H

#include <iostream>
#include <stack>
#include <vector>
#include "raylib.h"
#include "cell.h"
#include <math.h>
#include <fstream>

using std::stack;
using std::vector;

// udpating the textures for all of the cells in the maze based on their connections
void updateTextures(vector<vector<cell>>& mazegrid)
{
  for(int i=0;i<mazegrid.size();++i)
  {
    for(int j=0;j<mazegrid[0].size();++j)
    {
      mazegrid[i][j].updateTexture();
    }
  }
}

// displays the maze using raylib from the given textures
void drawMaze(Texture& atlas,vector<vector<cell>> mazegrid,Vector2 offset,Vector2 inputScale,Vector2 outputScale)
{
  for(int i=0;i<mazegrid.size();++i)
  {
    for(int j=0;j<mazegrid[0].size();++j)
    {
      if(mazegrid[i][j].isPlaced == false)
      {
        continue;
      }
      // draw the correct texture in the current place with the correct rotation
      DrawTexturePro(atlas,(Rectangle){float(mazegrid[i][j].textureIndex)*inputScale.x,0,inputScale.x,inputScale.y},(Rectangle){offset.x + outputScale.x/2 + float(j%mazegrid[0].size())*outputScale.x,offset.y + outputScale.y/2
+ (float(i)*outputScale.y),outputScale.x,outputScale.y},(Vector2){outputScale.x/2,outputScale.y/2},float(mazegrid[i][j].textureRotation)*90,WHITE);
    }
  }
}

// generates a blank maze with the given coordinates
vector<vector<cell>> blankMaze(int width, int height)
{
  vector<vector<cell>> mazegrid;

  for(int i=0;i<height;++i)
  {
    vector<cell> currentRow;
    for(int j=0;j<width;++j)
    {
      currentRow.push_back(cell());
    }
    mazegrid.push_back(currentRow);
  }
  return mazegrid;
}

// takes in dimensions and generates all of the possible path locations
// then scrambles the generated coordinates, then pushes them onto the stack
stack<Vector2> generateRandomCoordList(const Vector2& mazeDimensions)
{
  vector<Vector2> coords;
  stack<Vector2> rootStack;
  for(int i=0;i<mazeDimensions.y;++i)
  {
    for(int j=0;j<mazeDimensions.x;++j)
    {
      coords.push_back({(float)j,(float)i});
    }
  }
  
  int randomSwaps = (mazeDimensions.x * mazeDimensions.y)/2;
  int firstCoordIndex;
  int secondCoordIndex;
  Vector2 temp;

  for(int i=0;i<randomSwaps;++i)
  {
    firstCoordIndex = rand() % coords.size();
    secondCoordIndex = rand() % coords.size();

    temp = coords[firstCoordIndex];
    coords[firstCoordIndex] = coords[secondCoordIndex];
    coords[secondCoordIndex] = temp;
  }

  while(coords.size())
  {
    rootStack.push(coords[coords.size()-1]);
    coords.pop_back();
  }

  return rootStack;
}




void clearTraversal(vector<vector<cell>>& board)
{
  for(vector<cell>& row : board)
  {
    for(cell& referenceCell : row)
    {
      referenceCell.isRoot = false;
      referenceCell.goalCell = false;
      referenceCell.searched = false;
    }
  }
}

void saveMaze(vector<vector<cell>>& board, std::string fileName,int startingX, int startingY,int endingX, int endingY)
{
  int mazeWidth  = board[0].size();
  int mazeHeight = board.size();

  std::string fileDir = "mazes/";
  fileDir += fileName;

  std::ofstream writingFile(fileDir);

  writingFile << mazeWidth << " " << mazeHeight << "\n";

  for(vector<cell>& row : board)
  {
    for(cell& currentCell : row)
    {
      writingFile << currentCell << "\n";
    }
  }
  writingFile << startingX << " " << startingY << "\n";
  writingFile << endingX   << " " << endingY   << "\n";

  writingFile.close();
}


#endif
