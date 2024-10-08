# Maze generator

<p>
This project is a maze generator that visualizes the generation of a maze using a random walking algorithm called Wilsons algorithm. Although Wilsons algorithm is by no means fast, the result that it gives best suites the dynamics of a maze that I was after which includes a fairely even spacing of sections. After the maze has been generatied,two random points in the maze will be selected to have draggable nodes. These draggable nodes display the shortest path from one node to the other no matter the size of the maze.

Some more details on Wilsons algorithm can be found here: 
</p>

    https://en.wikipedia.org/wiki/Maze_generation_algorithm#Wilson's_algorithm


## <b>Maze generation process:</b>

<p>
    Starting with an empty x wide by y tall grid, we have a wandering path from at a point a. The wandering path wanders until it connects with a placed point b. In the case that it connects to a placed path, it takes its path that it took from point a to b and places that path. We then get a new point a that hasn't been occupied yet and repeat the same process. In the case that the wandering path contacts the wandering path from point a, the wandering path breaks all the way to that contact point and continues wandering from that point.
</p>

![Maze generation process clip](https://github.com/ZachBoggs/mazeGeneratorResources/blob/main/MazeGenSlowed.gif?raw=true "Maze generation process")

<p>
    Below is an example of the same process above but at 1000x speed and on a much larger scale! Each time that you can see the path update the random walkder has stepped one thousand times! Notice that after the maze has been generated that dragging the path nodes has a slight delay before it displays the shortest path!
<p>

![Maze generation process clip](https://github.com/ZachBoggs/mazeGeneratorResources/blob/main/MazeGeneratorSampleVideo.gif?raw=true "Maze generation process")

## <b>Shortest path navigation:</b>

Here is an example of moving around the nodes and the maze displaying the shortest path between those two nodes in real time.

 ![Maze Solving clip](https://github.com/ZachBoggs/mazeGeneratorResources/blob/main/MazeGeneratorSolver.gif?raw=true "Maze solving process")

---
## Grid breakdown:

The grid is split up into a grid the size of X by Y filled with 'cells'. These cells represent each piece inside the maze and holds the data we need in order to find theshortest path between our nodes which includes:

---
```
- isPlaced
- isTraversed
- isRoot
- connections[4]
- searched // used for the solving algorithm
- goalCell
- Vector2 currentPos // holds the cell location in the grid
- int textureIndex  
- int textureRotation
```
---

In the above variables, take note that there is a relationship between isRoot and goalCell. With having two node locations for finding the shortest path, I decided to haveone of the nodes hold the relationship of isRoot, meaning where we are searching from and goalCell which is the other node that we searched through the maze for.

#### Types of cell textures:

Regarding two of the variables that is stored in a cell called textureIndex and textureRotation. The textureIndex tells the program which texture we want going from left toright starting at 0 on the template below. The textureRotation is important because it takes the texture at the given textureIndex location and places it in the grid at thecells location matching the connections to the cells around it.

 ![Cell template](https://github.com/ZachBoggs/mazeGeneratorResources/blob/main/mazeTemplate.png?raw=true "Blank cell template")

 ---

#### Why have a template?

I chose to have a template which represents all of the possible cell connection cases to be able to load in my own custom textures for the maze such as the one below:

 ![Colored cell template](https://github.com/ZachBoggs/mazeGeneratorResources/blob/main/mazeBlueTheme.png?raw=true "Colored cell template")

---

Program running slowly:
![Maze Solving clip](https://github.com/ZachBoggs/mazeGeneratorResources/blob/main/MazeGenStepByStep.gif?raw=true "Maze solving process")
---
<br>

## Photo gallery:
![Maze Solving clip](https://github.com/ZachBoggs/mazeGeneratorResources/blob/main/samplePhoto1.png?raw=true "Maze solving process")
![Maze Solving clip](https://github.com/ZachBoggs/mazeGeneratorResources/blob/main/samplePhoto2.png?raw=true "Maze solving process")
![Maze Solving clip](https://github.com/ZachBoggs/mazeGeneratorResources/blob/main/samplePhoto3.png?raw=true "Maze solving process")
