#ifndef CELL_H
#define CELL_H

#include "raylib.h"

struct cell
{
  bool isPlaced = false;
  bool isTraversed = false;
  bool isRoot = false;
  bool connections[4]{false};
  bool searched = false; // used for the solving algorithm
  bool goalCell = false;

  Vector2 currentPos;  

  int textureIndex = 0;
  int textureRotation = 0;
 
  cell()
  {
  }
  
  cell(Vector2 newPos)
    : currentPos(newPos)
  {
  }

  cell(int textInd,int rotInd)
    : textureIndex(textInd),textureRotation(rotInd)
  {
    for(int i=0;i<4;++i)
    {
      if(textInd == 1)
      {
        connections[i] = rotInd == i;
      }else if(textInd == 2)
      {
        connections[i] = i == (rotInd % 4) || i == ((rotInd + 1) % 4);
      }else if(textInd == 3)
      {
        connections[i] = i == ((rotInd + 3) % 4);

      }else if(textInd == 4)
      {
        connections[i] = true;
      }else
      {
        connections[i] = i == rotInd || i == rotInd+2;
      }
    }
  }

  void updateTexture()
  {
    int textureCount(0);
    for(int i=0;i<4;++i)
    {
      if(connections[i])
      {
        textureCount++;
      }
    }
    if(textureCount == 0) // if we have not connections, then we don't need to set a rotation
    {
      return;
    }

    if(textureCount != 2)
    {
      textureIndex = textureCount;

      switch(textureIndex)
      {
        case(1):
          for(int i=0;i<4;++i)
          {
            if(connections[i])
            {
              textureRotation = i;
            }
          }

          break;
        case(3):
          for(int i=0;i<4;++i)
          {
            if(!connections[i])
            {
              textureRotation = (i+1)%4;
            }
          }
          break;
      }
      return;
    }

    textureIndex = 2;
    if((connections[0] && connections [2]) || (connections[1] && connections[3]))
    {
      textureIndex =  5;
      textureRotation = connections[0] ? 0 : 1;
    }else
    {
      for(int i=0;i<4;++i)
      {
        if(connections[i] && connections[(i+1)%4])
        {
          textureRotation = i;
        }
      }

    }
  }

};

#endif
