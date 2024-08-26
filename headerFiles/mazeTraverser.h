
#ifndef MAZETRAVERSER_H
#define MAZETRAVERSER_H

#include <stack>
#include <vector>
#include "cell.h"
#include "raylib.h"
#include <iostream>

using std::cout;
using std::stack;
using std::vector;

struct traveler
{
  /*
   * these 2 positions below are indexed based on the current board, not pixel based
   */
  Vector2 currentPosition;
  Vector2 previousPosition; 
  Vector2 currentRoot;

  vector<vector<cell>> currentBoard;
  stack<Vector2> rootStack; 
  stack<Vector2> currentPath;
  bool mazeDone = false;

  traveler(vector<vector<cell>>& inputBoard)
  {
    currentBoard = inputBoard;
    rootStack = generateRandomCoordList({(float)inputBoard[0].size(),(float)inputBoard.size()});
    goToNextRoot();
    currentBoard[rootStack.top().y][rootStack.top().x].isPlaced = true;
    rootStack.pop();
  }


  /*
   * The 4 below functions are for movement control of the wandering path in relation to what direction it should move
   *
   */
  void goUp()
  {
    if(currentPosition.y == 0) { return; }
    --currentPosition.y;
    checkCollisions();
  }
  void goDown()
  {
    if(currentPosition.y == currentBoard.size()-1) { return; }
    ++currentPosition.y;
    checkCollisions();
  }
  void goLeft()
  {
    if(currentPosition.x == 0) { return; }
    --currentPosition.x;
    checkCollisions();
  }
  void goRight()
  {
    if(currentPosition.x == currentBoard[0].size()-1) { return; }
    ++currentPosition.x;
    checkCollisions();
  }

  void checkCollisions() 
  {
    if(currentBoard[currentRoot.y][currentRoot.x].isPlaced) { return; }

    if(currentRoot.y == currentPosition.y && currentRoot.x == currentPosition.x)
    {
      while(!currentPath.empty())
      {
        currentPosition = currentPath.top();
        currentBoard[currentPosition.y][currentPosition.x].isTraversed = false;
        currentPath.pop();
      }
      currentPosition = currentRoot;
    }else if(collidedMaze()) // if we have collided with the maze then we place the wandering path and attach it to teh contacted point
    {
      placePath();
    }else if(collidedPath()) // if we have collided with the wandering path then we want to destroy the wandering path till down to the contact point
    {
      shrinkTo();
    }else{
      addToPath(); // add to the wandering path
    }
  }

  bool collidedMaze()
  {
    return currentBoard[currentPosition.y][currentPosition.x].isPlaced;
  }

  bool collidedPath() // checks if the wandering path has hit its own path
  {
    return currentBoard[currentPosition.y][currentPosition.x].isTraversed;
  }

  void shrinkTo() // shrinking the wandering path to the contact point
  {
    Vector2 endOfPath = currentRoot;
    do{
      if(!currentPath.empty())
      {
        endOfPath = currentPath.top();
        currentPath.pop();
      }else
      {
        currentBoard[endOfPath.y][endOfPath.x].isTraversed = false;
        return;
      }

      currentBoard[endOfPath.y][endOfPath.x].isTraversed = false;
    }while((endOfPath.x != currentPosition.x) || (endOfPath.y != currentPosition.y));

    currentBoard[endOfPath.y][endOfPath.x].isTraversed = true;
    currentPath.push(endOfPath);
  }

  void addToPath() // adds the current position to the path stack and marks the current tile as wandered/traversed
  {
    currentPath.push(currentPosition);
    currentBoard[currentPosition.y][currentPosition.x].isTraversed = true;
  }

  void placePath()
  {
    if(mazeDone) { exit(1); }

    currentBoard[currentPosition.y][currentPosition.x].isPlaced = true;
    bool doneUpdating(false);
    int iteration(0);
    do
    {
      if(currentPath.empty()) // this means that we are at the current root
      {
        currentBoard[currentRoot.y][currentRoot.x].isPlaced = true;
        currentBoard[currentRoot.y][currentRoot.x].isRoot = false;
        doneUpdating = true;
        if(iteration != 0)
        {
          currentPosition = previousPosition;
        }
        previousPosition = currentRoot;
      }else
      {
        
        if(iteration != 0)
        {
          currentPosition = previousPosition;
        }
        previousPosition = currentPath.top();
        currentPath.pop();
      }

      int xShift,yShift;
      xShift = currentPosition.x-previousPosition.x;
      yShift = currentPosition.y-previousPosition.y;

      if(xShift != 0)
      {
        if(xShift == 1)
        {
          currentBoard[currentPosition.y][currentPosition.x].connections[3] = true;
          currentBoard[previousPosition.y][previousPosition.x].connections[1] = true;
        }else if(xShift == -1)
        {
          currentBoard[currentPosition.y][currentPosition.x].connections[1] = true;
          currentBoard[previousPosition.y][previousPosition.x].connections[3] = true;
        }
      }else if(yShift != 0)
      {
        if(yShift == 1)
        {
          currentBoard[currentPosition.y][currentPosition.x].connections[0] = true;
          currentBoard[previousPosition.y][previousPosition.x].connections[2] = true;
        }else if(yShift == -1)
        {
          currentBoard[currentPosition.y][currentPosition.x].connections[2] = true;
          currentBoard[previousPosition.y][previousPosition.x].connections[0] = true;
        }

      }
      currentBoard[previousPosition.y][previousPosition.x].isPlaced = true;
      iteration++;
    }
    while(!doneUpdating);
    
    goToNextRoot(); 

  }

  bool goToNextRoot()
  {
    if(mazeDone) { return false; }

    if(rootStack.empty())
    {
      //cout << "maze finished!\n";
      mazeDone = true;
      return false;
    }

    do
    {
      currentRoot = rootStack.top();
      rootStack.pop();
      if(!currentBoard[currentRoot.y][currentRoot.x].isPlaced)
      {
        currentPosition = currentRoot;
        currentBoard[currentRoot.y][currentRoot.x].isRoot = true;
        break;
      }
    }
    while(!rootStack.empty());

    if(rootStack.empty() && currentBoard[currentRoot.y][currentRoot.x].isPlaced)
    {
      //cout << "maze finished!\n";
      mazeDone = true;
      return false;
    }

    return true;
  }
};

#endif
