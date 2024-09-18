# Maze generator
 This project is a maze generator that visualizes the process of generating a maze using a random walking algorithm called Wilsons algorithm. After the maze has been generatied, two random points in the maze will be selected to have draggable nodes. These draggable nodes display the shortest path from one node to the other no matter the size of the maze.

 Some more details on Wilsons algorithm can be found here: https://en.wikipedia.org/wiki/Maze_generation_algorithm#Wilson's_algorithm


## <b>Maze generation process:</b>

    Starting with an empty x wide by y tall grid, we have a wandering path from at a point a. The wandering path wanders until it connects with a placed point b. In the case that it connects to a placed path, it takes its path that it took from point a to b and places that path. We then get a new point a that hasn't been occupied yet and repeat the same process. In the case that the wandering path contacts the wandering path from point a, the wandering path breaks all the way to that contact point and continues wandering from that point.


![Maze generation process clip](./resources/MazeGenSlowed.gif "Maze generation process")

    Below is an example of the same process above but at 1000x speed and on a much bigger scale! Notice that dragging the path nodes after it has been solved has a slight delay before it has figured out the shortest path!

![Maze generation process clip](./resources/MazeGeneratorSampleVideo.gif "Maze generation process")

## <b>Shortest path navigation:</b>

    Here is an example of moving around the nodes and the maze displaying the shortest path between those two nodes in real time.
 ![Maze Solving clip](./resources/MazeGeneratorSolver.gif "Maze solving process")

---
## Grid breakdown:

    The grid is split up into a grid the size of X by Y filled with 'cells'. These cells represent each piece inside the maze and holds the data we need in order to find the shortest path between our nodes which includes:
---
- isPlaced
- isTraversed  
- isRoot
- connections[4]
- searched // used for the solving algorithm
- goalCell
- Vector2 currentPos // holds the cell location in the grid
- int textureIndex  
- int textureRotation
---

    In the above variables, take note that there is a relationship between isRoot and goalCell. With having two node locations for finding the shortest path, I decided to have one of the nodes hold the relationship of isRoot, meaning where we are searching from and goalCell which is the other node that we searched through the maze for.

#### Types of cell textures:

    Regarding two of the variables that is stored in a cell called textureIndex and textureRotation. The textureIndex tells the program which texture we want going from left to right starting at 0 on the template below. The textureRotation is important because it takes the texture at the given textureIndex location and places it in the grid at the cells location matching the connections to the cells around it.
 ![Cell template](./resources/mazetemplate.png "Blank cell template")
 ---
#### Why have a template?
    I chose to have a template which represents all of the possible cell connection cases to be able to load in my own custom textures for the maze such as the one below:
 ![Colored cell template](./resources/mazebluetheme.png "Colored cell template")
---
    Program in running slowly:
![Maze Solving clip](./resources/MazeGenStepByStep.gif "Maze solving process")
---
## Photo gallery:
![Maze Solving clip](./resources/samplePhoto1.png "Maze solving process")
![Maze Solving clip](./resources/samplePhoto2.png "Maze solving process")
![Maze Solving clip](./resources/samplePhoto3.png "Maze solving process")