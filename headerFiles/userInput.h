#ifndef USERINPUT_H
#define USERINPUT_H

#include "raylib.h"
#include <mutex>

void readTextInput(bool* shouldClose, std::string* currentString, std::mutex* stringLock, bool* doneTyping)
{
  stringLock->lock();
  *currentString = "";
  stringLock->unlock();
  

  while(!*shouldClose)
  {
    //std::cout << "new thread\n";
    char currentKey = GetCharPressed();

    // checking if the user is done with their input
    if(IsKeyPressed(KEY_ENTER)) 
    { 
      stringLock->lock();
      std::cout << "returning: " << *currentString << "\n";
      std::cout << "enter key pressed, done entering text!\n";
      stringLock->unlock();
      break; 
    }

    // checking if the user has typed any text
    // if the user has, then we want
    // to add the new text to the string
    stringLock->lock();
    if(((currentKey >= int('a') && currentKey <= int('z')) || (currentKey >= int('A') && currentKey <= int('Z'))) || IsKeyPressed(KEY_SPACE))
    {
      if(IsKeyPressed(KEY_SPACE))
      {
        std::cout << "Space key\n";
      }
      //stringLock->lock();
      *currentString += currentKey;
      //stringLock->unlock();
    }

    // deleting characters if the user pressed the backspace key
    if(IsKeyPressed(KEY_BACKSPACE))
    {
      //stringLock->lock();
      if(currentString->length() > 0)
      {
        currentString->pop_back();
      }
      //stringLock->unlock();
    }
    stringLock->unlock();
  }

  *doneTyping = true;
}

#endif
