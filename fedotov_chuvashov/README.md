# Parallel trees generation

## Building

### Linux / OS X
`clang++ *.cpp -o main -std=c++17 -pthread -Werror`

## Program logic
Program generates vertices and edges. Vertices are generated first, and then additional edges.

### Vertex generation has the following logic: 
For example, if `depth = 5` and `new_vertices_count = 4`, then each vertex can generate up to 4 vertices. The probability of generating a new vertex is `100 / depth * current_depth`.
- On depth=1:
    Every vertice on this depth will attempt (4 times) to generate new vertex, probability of success generation is 100%.
- On depth=2:
    Every vertice on this depth will attempt (4 times) to generate new vertex, probability of success generation is 75%.
- ...
- On depth=5:
    Every vertice on this depth will attempt (4 times) to generate new vertex, probability of success generation is 0%.

### Additional edges generation has the following logic:
- **Green** edge: Vertex will have a self-loop with 10% probability.
- **Red** edge: Vertex will be connected with random vertex two levels deeper with 33% probability.
- **Yellow** edge: Vertex will be connected with ==one== random vertex one level deeper excluding it's descendants with with probability depending on depth:
  - Vertex on level 1 will have a yellow edge with 0% probability.
  - ...
  - Level N-1: 100% probability
### Generated graph example:
<img src="https://user-images.githubusercontent.com/31239690/219658424-d0d19f40-581d-4cdd-a056-df4876d2b561.png"  width="675" height="600">

## Usage
Program gets depth, new vertices count, graph count and thread count.

- **Depth**: specifies depth of generated trees.
- **New vertices count**: specifies *maximum* amount of vertices generated as children of its parent.
- **Graph count**: specifies amount of generated graphs.
- **Thread count**: specifies amount of threads working on graph generation.

Generated trees will be saved in `temp` directory.
