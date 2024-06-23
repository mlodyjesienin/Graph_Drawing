

# Graph_Drawing

A small application for visualizing graphs and graph algorithms, developed for the C++ Course at AGH UST in 2024.

## Video presentation: 


https://github.com/mlodyjesienin/Graph_Drawing/assets/127685479/0f03a678-7c1e-4101-8c8a-e861d5cc7569


## Dependencies

To build and run the project, the SFML library is required.

### Linux Installation

```bash
sudo apt install libsfml-dev
```

## Build & Run

1. Clone the repository:

    ```bash
    git clone git@github.com:mlodyjesienin/Graph_Drawing.git
    cd Graph_Drawing
    ```

2. Create a build directory and navigate into it:

    ```bash
    mkdir build
    cd build
    ```

3. Build the project with CMake:

    ```bash
    cmake ..
    cmake --build .
    ```

4. Run the executable:

    ```bash
    ./Graph_Drawing
    ```

# UI Instructions

### Menu

Upon execution, a menu window will open. You can define your graph manually using the keyboard or paste it from the clipboard. The graph should be represented by an adjacency list enclosed in `[` braces and separated by commas `,`.

Example input: `[[1,2],[0,3,4],[0,3],[1,2,4],[1,3]]` defines a graph where:
- Vertex 0 is connected to vertices 1 and 2,
- Vertex 1 is connected to vertices 0, 3, and 4, and so on.

To draw the graph, click a button on the window or press `Enter`.

### Graph Visualization Steps

- **First Frame:** Vertices with random coordinates are displayed. The number of intersecting edges is shown at the top of the window.
  
- **Second Frame:** Vertices are repositioned using an algorithm designed to find the most efficient layout of the graph. The details of this algorithm are explained later in this README.

### Final Visualization

After the initial layout, you can manually move vertices using the mouse. Click on vertices to select them. Clicking outside the selected area deselects vertices. Hold `Shift` while clicking to select multiple vertices simultaneously.

### Algorithm Visualization - BFS

Select a source vertex and click the `BFS` button to visualize the Breadth-First Search (BFS) algorithm starting from the selected vertex. Each second, edges traversed by BFS are highlighted in green.

### Algorithm Visualization - Shortest Path

Select two vertices (source and destination), then click the `Shortest Path` button to visualize the shortest path using BFS. The path found is highlighted in red, while other vertices reached by the algorithm are marked in green.

### Algorithm Visualization - MST

Click the `MST` button to visualize the Kruskal's algorithm for finding the Minimum Spanning Tree (MST) of the graph. Edges added to the tree are marked in red, and edges considered by the algorithm but not added to the MST are marked in green.

### Error

Within the first 10 seconds of running the application, an error may occur stating that the application is not responding. You will have the option to `Force Quit` or `Wait`. In most cases, waiting for about 5 seconds resolves the issue, and the program continues to function properly. If you choose `Wait`, the program may work well for a while before encountering the same problem again.


# Algorithm Used for Finding the Optimal Graph Layout

The program utilizes the `Simulated Annealing` metaheuristic to minimize the number of intersecting edges, thereby optimizing the graph layout.

## Algorithm Steps

1. **Initialize**
   - Randomly generate coordinates for each vertex.

2. **Cost Calculation**
   - Calculate the initial cost of the positioning, where the cost is defined by the number of intersecting edges.

3. **Iterative Optimization**
   - **For 10,000 iterations:**
     - **Neighborhood Generation**
       - Create a neighboring state by:
         - With a probability of 0.7, swap the positions of two randomly selected vertices.
         - With a probability of 0.3, choose a random vertex and move it slightly.
     
     - **Cost Evaluation**
       - Calculate the cost of the new neighbor state.
     
     - **Acceptance Criterion**
       - If the cost of the neighbor state is lower than the current state, accept it as the new current state.
       - If the cost is higher, decide to accept it based on the probability described by the Kirkpatrick formula. This formula depends on the current state's cost, the neighbor state's cost, and the current temperature.
     
     - **Temperature Update**
       - Multiply the temperature parameter by a predefined cooling coefficient to control the exploration-exploitation trade-off.
   
4. **Repeat**
   - Repeat the above steps until the specified number of iterations is completed.

5. **Final Solution**
   - The best solution found after all iterations represents the final optimized positioning of the vertices.
# Example inputs:
If you are willing to see how the programm works, here are a few graphs that may help you see it:
- small graph consisting of 5 vertices:
    ```
    [[1,2],[0,3,4],[0,3],[1,2,4],[1,3]] 
    ```
- bigger graph consisting of 20 vertices:
  ```

    [[1, 2, 3], [0, 4, 5], [0, 6, 7], [0, 8, 9], [1, 10, 11], [1, 12, 13], [2, 14, 15], [2, 16, 17], [3, 18, 19], [3, 15, 17], [4, 18, 19], [4, 12, 13], [5, 11, 14], [5, 11, 19], [6, 12, 18], [6, 9, 16], [7, 15, 17], [7, 9, 16], [8, 10, 14], [8, 10, 13]]
  ```



