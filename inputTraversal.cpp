#include <iostream>
#include <vector>
#include <math.h>
#include "headerFiles/raylib.h"
#include "headerFiles/cell.h"
#include "headerFiles/mazeFunctions.h"
#include "headerFiles/mazeTraverser.h"
#include "headerFiles/randomNum.h"

//using namespace std;
using std::vector;
using std::stack;
using std::cout;
using std::string;
using std::to_string;





void drawCellDetails(vector<vector<cell>>& mazegrid)
{
  Vector2 squareDimensions{float(10),float(10)};
  bool fuzzyEffect = true;
  Vector2 fuzzy = fuzzyEffect ? Vector2{20,20} : squareDimensions;

  for(int i=0;i<mazegrid.size();++i)
  {
    for(int j=0;j<mazegrid[0].size();++j)
    {
      if(mazegrid[i][j].isRoot)
      {
        DrawRectangle(128 + (squareDimensions.x/4) + j*squareDimensions.x,128 + (squareDimensions.y/4) + i*squareDimensions.y,fuzzy.x,fuzzy.y,BLUE);
      }else if(mazegrid[i][j].isPlaced)
      {
        DrawRectangle(128 + (squareDimensions.x/4) + j*squareDimensions.x,128 + (squareDimensions.y/4) + i*squareDimensions.y,fuzzy.x,fuzzy.y,Fade(GREEN,.2));
      }else if(mazegrid[i][j].isTraversed)
      {
        DrawRectangle(128 + (squareDimensions.x/4) + j*squareDimensions.x,128 + (squareDimensions.y/4) + i*squareDimensions.y,fuzzy.x,fuzzy.y,RED);
      }else{
        DrawRectangle(128 + (squareDimensions.x/4) + j*squareDimensions.x,128 + (squareDimensions.y/4) + i*squareDimensions.y,fuzzy.x,fuzzy.y,Fade(YELLOW,.2));
      }
        
    }
  }
}


stack<Vector2> depthFirstSearch(vector<vector<cell>>& currentBoard,Vector2 currentCoords)
{
  stack<Vector2> shortestStack;

  // checking if the current position is out of bounds
  if(currentCoords.x < 0 || currentCoords.x >= currentBoard[0].size())
  {
    //cout << "out of bounds at x: " << currentCoords.x << "\n";
    return shortestStack;
  }
  if(currentCoords.y < 0 || currentCoords.y >= currentBoard.size())
  {
    //cout << "out of bounds at y: " << currentCoords.y << "\n";
    return shortestStack;
  }
  
  // if the current cell is already searched then return an empty stack
  if(currentBoard[currentCoords.y][currentCoords.x].searched)
  {
    return shortestStack;
  }

  if(currentBoard[currentCoords.y][currentCoords.x].goalCell)
  {
    //cout << "FOUND FINISH";
    shortestStack.push(currentCoords);
    return shortestStack;
  }

  currentBoard[currentCoords.y][currentCoords.x].searched = true;

  stack<Vector2> currentStack;

  /*
   *
   * The next four functions search and expand from the current point until they can't go further and they have found the finish location, then they will return up the shortest path
   *
   */
  if(currentBoard[currentCoords.y][currentCoords.x].connections[1])
  {
    //cout << "going right\n";
    DrawRectangle(128+currentCoords.x*40,128+currentCoords.y*40,40,40,YELLOW);
    currentStack = depthFirstSearch(currentBoard,{currentCoords.x+1,currentCoords.y});
    if((currentStack.size() < shortestStack.size() && currentStack.size() != 0) || shortestStack.size() == 0)
    {
      shortestStack = currentStack;
    }
  }
  if(currentBoard[currentCoords.y][currentCoords.x].connections[0])
  {
    //cout << "going up\n";
    DrawRectangle(128+currentCoords.x*40,128+currentCoords.y*40,40,40,YELLOW);
    currentStack = depthFirstSearch(currentBoard,{currentCoords.x,currentCoords.y-1});

    if((currentStack.size() < shortestStack.size() && currentStack.size() != 0) || shortestStack.size() == 0)
    {
      shortestStack = currentStack;
    }
  }
  if(currentBoard[currentCoords.y][currentCoords.x].connections[2])
  {
    //cout << "going down\n";
    DrawRectangle(128+currentCoords.x*40,128+currentCoords.y*40,40,40,YELLOW);
    currentStack = depthFirstSearch(currentBoard,{currentCoords.x,currentCoords.y+1});
    if((currentStack.size() < shortestStack.size() && currentStack.size() != 0) || shortestStack.size() == 0)
    {
      shortestStack = currentStack;
    }
  }
  if(currentBoard[currentCoords.y][currentCoords.x].connections[3])
  {
    //cout << "going left\n";
    DrawRectangle(128+currentCoords.x*40,128+currentCoords.y*40,40,40,YELLOW);
    currentStack = depthFirstSearch(currentBoard,{currentCoords.x-1,currentCoords.y});
    if((currentStack.size() < shortestStack.size() && currentStack.size() != 0) || shortestStack.size() == 0)
    {
      shortestStack = currentStack;
    }
  }

  // add my current coordinates to the shortest stack that is not 0
  if(shortestStack.size() != 0)
  {
    shortestStack.push(currentCoords);
  }

  return shortestStack; 
}

// goes through the next iteration of where the path should be going, 
// it also updates the textures for the current tiles that it crosses
void movePath(int direction, traveler& mazePath)
{
  switch(direction)
  {
    case 0:
      mazePath.goUp();
      updateTextures(mazePath.currentBoard);
      break;
    case 1:

      mazePath.goRight();
      updateTextures(mazePath.currentBoard);
      break;
    case 2:
      mazePath.goLeft();
      updateTextures(mazePath.currentBoard);
      break;
    case 3:
      mazePath.goDown();
      updateTextures(mazePath.currentBoard);
      break;
    case -1:
      break;
  }
}

int main()
{
  /*
   * This is my maze traversal program that generates a maze based off of the current seed and dimensions that the user has specified
   *
   */

  int currentSeed(2);
  Vector2 windowSize = {1900,1000};
  InitWindow(windowSize.x,windowSize.y,"Maze Generator & Solver");
  SetTargetFPS(60);
  while(!IsWindowReady()){}
  
  //Texture atlas = LoadTexture("randompatternpng.png");
  Texture atlas = LoadTexture("textures/mazebluetheme.png");
  //Texture atlas = LoadTexture("textures/shrunken.png");
  //Texture atlas = LoadTexture("mazetemplate.png");
  //Rectangle spriteRect = {0, 0, 128, 128 };

  vector<vector<cell>> mazegrid;

  //mazegrid = {{cell(1,2),cell(2,1),cell(3,2),cell(1,2)},{cell(2,0),cell(3,2),cell(2,0),cell(3,2)},{cell(5,1),cell(3,3),cell(5,1),cell(2,3)}};
  //mazegrid = {{cell(1,2),cell(2,1),cell(3,2),cell(1,1),cell(2,2)},{cell(2,0),cell(3,2),cell(2,0),cell(2,2),cell(5,0)},{cell(2,1),cell(3,3),cell(3,1),cell(3,3),cell(3,2)},{cell(1,0),cell(2,1),cell(2,3),cell(1,1),cell(2,3)}};

  Vector2 mazeOffset  = {50,50};   // the shift in pixels from the top and left side of the screen
  Vector2 mazeSize    = {8,10};    // dimensions in squares / grid size
  Vector2 newMazeSize = mazeSize;
  Vector2 inputScale  = {128,128}; // the size of each sprite of the maze
  Vector2 outputScale = {(int)(windowSize.x-mazeOffset.x)/mazeSize.x,(int)(windowSize.y-mazeOffset.y)/mazeSize.y};

  // squaring off the ratios
  if(outputScale.x > outputScale.y)
  {
    outputScale.x = outputScale.y;
  }else
  {
    outputScale.y = outputScale.x;
  }

  Rectangle spriteRect = {0, 0, inputScale.x, inputScale.y};

  mazegrid = blankMaze(mazeSize.x,mazeSize.y);
  updateTextures(mazegrid);
  srand(currentSeed);
  traveler mazePath(mazegrid);
  //mazePath.rootStack = generateRandomCoordList(mazeSize);
  //cout << "stack size after generation: " << mazePath.rootStack.size() << "\n";
  //mazePath.goToNextRoot();
  cout << "stack size after first take: " << mazePath.rootStack.size() << "\n";
  //mazePath.currentBoard[mazePath.rootStack.top().y][mazePath.rootStack.top().x].isPlaced = true;
  //mazePath.rootStack.pop();
  updateTextures(mazePath.currentBoard);
  
  bool debugMode = true; // displays debug information
  bool paused = false;
  bool showBackend = true; // displays where the wandering segment is and its current path
  // allows the user to control how the wandering segment moves
  bool manualMode = false;

// if the user is dragging the starting or end point, should the shortest path be updating while it is dragging or wait till the user releases the mouse button
  bool smoothDrag = true; 
  vector<Vector2> shortestPath;
  int startingY;
  int endingY;
  int startingX;
  int endingX;
  bool solvedMaze = false;
  Vector2 markerOrigin; // holds the location where the marker was before it was being dragged

  bool adjustingMarkers(false);
  bool draggingMarker[2]{false,false}; // represents the movement state of each marker in the end after the maze has been generated and it finds the shortest path between two markers

  
  while(!WindowShouldClose())
  {
    
    // checking for user input
    if(IsKeyPressed(KEY_P)) { paused      = !paused;      }
    if(IsKeyPressed(KEY_B)) { showBackend = !showBackend; }
    if(IsKeyPressed(KEY_D)) { debugMode   = !debugMode;   }

    if(IsKeyPressed(KEY_R))
    {
      mazeSize = newMazeSize;
      outputScale = {(int)(windowSize.x-mazeOffset.x)/mazeSize.x,(int)(windowSize.y-mazeOffset.y)/mazeSize.y};
      if(outputScale.x > outputScale.y) // finding which axis is the smallest that fits in the viewport and scaling it to that
      {
        outputScale.x = outputScale.y;
      }else
      {
        outputScale.y = outputScale.x;
      }
      solvedMaze = false;
      shortestPath = vector<Vector2>(); // empty the shortest path variable so it doesn't show previous solutions
      if(!IsKeyDown(KEY_LEFT_SHIFT)) // if the shift key is down then we make a random maze, but if it isn't down then we generate based off of the seed
      {
        //srand(currentSeed);
      }

      // emptying the wandering segment to the current movement path stack
      while(!mazePath.currentPath.empty()) { mazePath.currentPath.pop(); }

      mazegrid = blankMaze(mazeSize.x,mazeSize.y);
      //updateTextures(mazegrid);
      srand(currentSeed);
      mazePath = traveler(mazegrid);
      //mazePath.rootStack = generateRandomCoordList(mazeSize);
      //mazePath.goToNextRoot();
      //mazePath.currentBoard[mazePath.rootStack.top().y][mazePath.rootStack.top().x].isPlaced = true;
      //mazePath.rootStack.pop();
      updateTextures(mazePath.currentBoard);
      //mazePath.mazeDone = false;
      /*
      // solving for the shortest path in between frames
      while(!mazePath.currentPath.empty())
      {
        mazePath.currentPath.pop();
      }
      */
    }

    if(IsKeyPressed(KEY_M))
    {
      manualMode = !manualMode;
    }
    if(IsKeyPressed(KEY_J))
    {
      currentSeed--;
    }else if(IsKeyPressed(KEY_K))
    {
      currentSeed++;
    }

    if(!manualMode)
    {
      if(IsKeyPressed(KEY_UP) || (IsKeyDown(KEY_UP) && IsKeyDown(KEY_LEFT_SHIFT)))
      {
        newMazeSize.y++;
      }else if(IsKeyPressed(KEY_DOWN) || (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT_SHIFT)))
      {
        if(newMazeSize.y > 2)
        {
          newMazeSize.y--;
        }
      }
      if(IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_LEFT_SHIFT)))
      {
        newMazeSize.x++;
      }else if(IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)))
      {
        if(newMazeSize.x > 2)
        {
          newMazeSize.x--;
        }
      }
    }

    if(!paused)
    {
        int direction = -1;
        if(!manualMode && !mazePath.mazeDone)
        {
          for(int m=0;m<1000;++m)
          {
            if(!mazePath.mazeDone)
            {
              direction = randomNumber(0,4);

              movePath(direction, mazePath);
            }
          }
        }else
        {
          // if the maze has not been generated yet, then we set the direction and update the grid
          if(!mazePath.mazeDone)
          {
            if(IsKeyPressed(KEY_UP) || (IsKeyDown(KEY_UP) && IsKeyDown(KEY_LEFT_SHIFT))) 
            {
              //cout << "^\n";
              direction = 0;
              
            }else if(IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_LEFT_SHIFT)))
            {
              //cout << ">\n";
              direction = 1;
              
            }else if(IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)))
            {
              //cout << "<\n";
              direction = 2;
              
            }else if(IsKeyPressed(KEY_DOWN) || (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT_SHIFT)))
            {
              //cout << "\\/\n";
              direction = 3;
              
            }else if(IsKeyPressed(KEY_S))
            {
              cout << "placing path because s was pressed!\n";
              cout << "path empty: " << mazePath.currentPath.empty() << "\n";
              cout << "path size: " << mazePath.currentPath.size() << "\n";
              mazePath.placePath();
              updateTextures(mazePath.currentBoard);
            }

            movePath(direction, mazePath);
          }
        }
    }else
    {
      DrawText("Paused",windowSize.x-200,50,50,BLACK);

    }
    
    BeginDrawing();
    ClearBackground(RAYWHITE);

    /*
    for(int i=0;i<atlas.width/128;++i)
    {
      DrawTexturePro(atlas,(Rectangle){i*128,0,128,128},(Rectangle){(i%4)*170,((i/4)*170),128,128},(Vector2){0,0},0,WHITE);
    }
    */

    drawMaze(atlas,mazePath.currentBoard,mazeOffset,inputScale,outputScale);
    if(showBackend)
    {
      if(!mazePath.mazeDone)
      {
        drawCellDetails(mazePath.currentBoard);
        DrawRectangle(mazePath.currentPosition.x*outputScale.x + mazeOffset.x,mazePath.currentPosition.y*outputScale.y + mazeOffset.y,outputScale.x,outputScale.y,RED);
        DrawRectangle(mazePath.currentRoot.x*outputScale.x + mazeOffset.x,mazePath.currentRoot.y*outputScale.y + mazeOffset.y,outputScale.x,outputScale.y,BLUE);
      }
    }

    if(mazePath.mazeDone)
    {
      if(shortestPath.size() == 0)
      {
        if(!solvedMaze)
        {
          //randomely generate coordinates for a starting point and an end point
          startingY = randomNumber(0,mazePath.currentBoard.size()-1);
          startingX = randomNumber(0,mazePath.currentBoard[0].size()-1);

          endingY = randomNumber(0,mazePath.currentBoard.size()-1);
          endingX = randomNumber(0,mazePath.currentBoard[0].size()-1);


          //mazePath.currentBoard[endingY][mazePath.currentBoard[0].size()-1].goalCell = true;
          // marking which cell is going to be our end goal
          mazePath.currentBoard[endingY][endingX].goalCell = true;

          // finding the shortest path from the start to the finish
          stack<Vector2> shortestStack = depthFirstSearch(mazePath.currentBoard,{(float)startingX,(float)startingY});

          // resetting the shortest path and adding the path information to it
          shortestPath = {};
         
          while(!shortestStack.empty())
          {
            shortestPath.push_back(shortestStack.top());
            shortestStack.pop();
          }

          //cout << "Shortest PATH length: " << shortestPath.size() << "\n";
          solvedMaze = true;
        }
      }else
      {
        // drawing the path that was given in scale of the maze
        for(int i=0;i<shortestPath.size()-1;++i)
        {
          //DrawRectangle(shortestPath[i].x*40+128,shortestPath[i].y*40+128,40,40,Fade(PINK,.4));
          DrawLineEx({shortestPath[i].x*outputScale.x+mazeOffset.x+((float)outputScale.x/2),shortestPath[i].y*outputScale.y+mazeOffset.y+((float)outputScale.y/2)},{shortestPath[i+1].x*outputScale.x+mazeOffset.x+((float)outputScale.x/2),shortestPath[i+1].y*outputScale.y+mazeOffset.y+((float)outputScale.y/2)},5,PINK);
        }

        // mouse position markers
        Vector2 mousePos = GetMousePosition();

        if(debugMode)
        {
          DrawRectangle(490,0,200,200,GRAY);
          // printing the output scale to the screen so we can debug easier
          string scaleX = to_string(outputScale.x);
          string scaleY = to_string(outputScale.y);

          DrawText("scale x:",500,0,20,BLACK);
          DrawText("scale y:",500,20,20,BLACK);

          DrawText(scaleX.c_str(),600,0,20,BLACK);
          DrawText(scaleY.c_str(),600,20,20,BLACK);


          // printing out debug stats for the markers
          string draggingStatus0 = "marker[0]: ";
          string draggingStatus1 = "marker[1]: ";

          draggingStatus0 += draggingMarker[0] ? "true" : "false";
          draggingStatus1 += draggingMarker[1] ? "true" : "false";

          DrawText(draggingStatus0.c_str(),500,40,20,BLACK);
          DrawText(draggingStatus1.c_str(),500,60,20,BLACK);

          
          // displaying the position for the markers
          // displaying the x axis of the markers
          string markerPos0 = "marker[0] x:";
          string markerPos1 = "marker[1] x:";

          DrawText(markerPos0.c_str(),500,80,20,BLACK);
          DrawText(markerPos1.c_str(),500,120,20,BLACK);

          markerPos0 = to_string(startingX);
          markerPos1 = to_string(endingX);

          DrawText(markerPos0.c_str(),650,80,20,BLACK);
          DrawText(markerPos1.c_str(),650,120,20,BLACK);

          // displaying the y axis of the markers
          markerPos0 = "marker[0] y:";
          markerPos1 = "marker[1] y:";

          DrawText(markerPos0.c_str(),500,100,20,BLACK);
          DrawText(markerPos1.c_str(),500,140,20,BLACK);

          markerPos0 = to_string(startingY);
          markerPos1 = to_string(endingY);

          DrawText(markerPos0.c_str(),650,100,20,BLACK);
          DrawText(markerPos1.c_str(),650,140,20,BLACK);


          // marking where the mouse is on each axis
          DrawRectangle(mousePos.x,0,10,10,RED);
          DrawRectangle(0,mousePos.y,10,10,RED);
        }


        // if the mouse is farther over then the offset, then we recognize the mouse at the offset of that axis
        if(mousePos.x < mazeOffset.x) mousePos.x = mazeOffset.x;
        if(mousePos.y < mazeOffset.y) mousePos.y = mazeOffset.y;

        mousePos.x -= mazeOffset.x;
        mousePos.y -= mazeOffset.y;

        float remainderX = mousePos.x;
        float remainderY = mousePos.y;

        while(remainderX > outputScale.x ) { remainderX -= outputScale.x; };
        while(remainderY > outputScale.y ) { remainderY -= outputScale.y; };

        mousePos.x -= remainderX;
        mousePos.y -= remainderY;

        DrawRectangle(mousePos.x + mazeOffset.x,mousePos.y + mazeOffset.y,outputScale.x,outputScale.y,BLACK);

        Vector2 mousePosCell{mousePos.x / outputScale.x,mousePos.y / outputScale.y};

        if(mousePosCell.x > mazeSize.x - 1)
        {
          mousePosCell.x = mazeSize.x - 1;
          mousePos.x = (mazeSize.x - 1) * outputScale.x;
        }

        if(mousePosCell.y > mazeSize.y - 1)
        {
          mousePosCell.y = mazeSize.y - 1;
          mousePos.y = (mazeSize.y - 1) * outputScale.y;
        }

        mousePosCell.x += .5;
        mousePosCell.y += .5;

        mousePosCell.x = (int)mousePosCell.x;
        mousePosCell.y = (int)mousePosCell.y;

        //cout << "mouse cell x: " << mousePosCell.x << ", y: " << mousePosCell.y << "\n";

        if(debugMode)
        {
          string mouseX = "mouse x: ";
          string mouseY = "mouse y: ";

          DrawText(mouseX.c_str(),500,160,20,BLACK);
          DrawText(mouseY.c_str(),500,180,20,BLACK);

          mouseX = to_string(mousePosCell.x);
          mouseY = to_string(mousePosCell.y) ;

          DrawText(mouseX.c_str(),650,160,20,BLACK);
          DrawText(mouseY.c_str(),650,180,20,BLACK);


          // displaying guides for where the cursor is at
          DrawRectangle(mazeOffset.x + mousePos.x,0,outputScale.x/6,mazeOffset.y + mazeSize.y * outputScale.y,GREEN);
          DrawRectangle(0,mazeOffset.y + mousePos.y,mazeOffset.x + mazeSize.x * outputScale.x,outputScale.y/6,GREEN);
        }

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
        {
          // drawing a ghost block where the cursor is at
          DrawRectangle(mazeOffset.x + mousePos.x,mazeOffset.y + mousePos.y,outputScale.x,outputScale.y,YELLOW);
          DrawRectangle(mazeOffset.x + mousePos.x + outputScale.x/4,mazeOffset.y + mousePos.y + outputScale.y/4,outputScale.x/2,outputScale.y/2,PURPLE);

          // making sure nothing is being dragged already before we set a status
          // so we check both the front and end node locations in comparison to the mouse location
          if(!draggingMarker[0] && !draggingMarker[1])
          {
            // if we are grabbing the 'starting' node
            if(startingX == (int)mousePosCell.x && startingY == (int)mousePosCell.y)
            {
              draggingMarker[0] = true;
              markerOrigin.x = startingX;
              markerOrigin.y = startingY;
            }

            // if we are grabbing the 'ending' node
            if(endingX == ((int)mousePosCell.x) && endingY == ((int)mousePosCell.y))
            {
              draggingMarker[1] = true;
              markerOrigin.x = endingX;
              markerOrigin.y = endingY;
            }

          }else if(draggingMarker[0])
          {
            DrawRectangle(mazeOffset.x + startingX * outputScale.x,mazeOffset.y + startingY * outputScale.y,outputScale.x,outputScale.y,RED);
          }else if(draggingMarker[1])
          {
            DrawRectangle(mazeOffset.x + endingX * outputScale.x,mazeOffset.y + endingY * outputScale.y,outputScale.x,outputScale.y,RED);
          }
          // setting initial click position

          // calculate out which marker we are currently trying to select if there is one underneath where the cursor is currently at
          // if there is a marker where we are at, then we set dragging marker to true
          // if dragging marker is true, then the purple square will be drawn where the mouse is at
          // if dragging square for either one is true, then we will NOT be trying to find the 
          // shortest path between the markers
        }else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
          // if we are dragging a marker, then we want to make sure that we didn't just set the marker down on top of the other marker



  //Vector2 markerOrigin; // holds the location where the marker was before it was being dragged
  /* this is how we draw the rectangle at where the cursor loation is at
          }else if(draggingMarker[0])
          {
            DrawRectangle(mazeOffset.x + startingX * outputScale.x,mazeOffset.y + startingY * outputScale.y,outputScale.x,outputScale.y,RED);
          }else if(draggingMarker[1])
          {
            DrawRectangle(mazeOffset.x + endingX * outputScale.x,mazeOffset.y + endingY * outputScale.y,outputScale.x,outputScale.y,RED);
            */


          /*
            have an array of size 2 or a vector of 2 that you get from a functiont that returns the coordinates of what cell the mouse is currently at
            this function would also accomodate for if the mouse is off the screen to the right, left, above and or below
            
            have another function that can take the cell coordinate and give you the pixel location of where that would be at in the maze

            update the stored location for the marker
            have it so if a marker is being dragged, that is where the path generation is coming from, (the current location the mouse is at ) as long as it does not cover another marker on that tile

            check if the position is valid, if it is, then update the origin and draw from the current location
*/

            // updating the markers since we are no longer dragging them
          if(draggingMarker[0])
          {
            if(!(mousePosCell.x == endingX && mousePosCell.y == endingY))
            {
              
              //mazePath.mazeDone = false;
              startingX = mousePosCell.x;
              startingY = mousePosCell.y;
              shortestPath = vector<Vector2>(); // empty the shortest path variable so it doesn't show previous solutions
              //solvedMaze = false;

              //we may need to reset all of the cells to remove the old path
              clearTraversal(mazePath.currentBoard);
              mazePath.currentBoard[endingY][endingX].goalCell = true;

              // finding the shortest path from the start to the finish
              stack<Vector2> shortestStack = depthFirstSearch(mazePath.currentBoard,{(float)startingX,(float)startingY});

              cout << "shortestStack size: " << shortestStack.size() << "\n";

              // resetting the shortest path and adding the path information to it
              shortestPath = {};
             
              while(!shortestStack.empty())
              {
                shortestPath.push_back(shortestStack.top());
                shortestStack.pop();
              }
              cout << "shortest path found!\n";
              solvedMaze = true;
            }

            //DrawRectangle(mazeOffset.x + startingX * outputScale.x,mazeOffset.y + startingY * outputScale.y,outputScale.x,outputScale.y,ORANGE);
            draggingMarker[0] = false;
          }else if(draggingMarker[1])
          {
            if(!(mousePosCell.x == startingX && mousePosCell.y == startingY))
            {
              endingX = mousePosCell.x;
              endingY = mousePosCell.y;
              
              //shortestPath = vector<Vector2>(); // empty the shortest path variable so it doesn't show previous solutions
              solvedMaze = false;
            }
            //DrawRectangle(mazeOffset.x + endingX * outputScale.x,mazeOffset.y + endingY * outputScale.y,outputScale.x,outputScale.y,ORANGE);
            draggingMarker[1] = false;
          }
          // check if we need to update the marker location, or if we should leave the marker where it is already at

        }
        //DrawRectangle(mazeOffset.x + ((mousePos.x - mazeOffset.x)/ ((int)mazeSize.x * outputScale.x)),0,20,20,GREEN);
        //bool draggingMarker[2]{false,false}; // represents the movement state of each marker in the end after the maze has been generated and it finds the shortest path between two markers
      }
      DrawRectangle(mazeOffset.x+(float)outputScale.x /4 + outputScale.x * startingX,startingY*outputScale.y+mazeOffset.y+((float)outputScale.y/4),(float)outputScale.x/2,(float)outputScale.y/2,PURPLE);
      DrawRectangle(mazeOffset.x+endingX*outputScale.x+((float)outputScale.x/4),endingY*outputScale.y+mazeOffset.y+((float)outputScale.y/4),(float)outputScale.x/2,(float)outputScale.y/2,PURPLE);

    }
    DrawText(("Seed: " + to_string(currentSeed)).c_str(),0,0,60,BLACK);
    DrawText(("Increase: k/K"),0,60,20,BLACK);
    DrawText(("Decrease: j/J"),0,80,20,BLACK);
    DrawText("Size: ",0,100,30,BLACK);
    DrawText(("x: " + to_string((int)newMazeSize.x)).c_str(),0,130,20,BLACK);
    DrawText(("y: " + to_string((int)newMazeSize.y)).c_str(),0,150,20,BLACK);
    EndDrawing();

  }

  CloseWindow();

  return 0;
}


