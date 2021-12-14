# delaunay_3d
Uses some basic linear algebra for viewing points with 3D coordinates and navigating around them. An algorithm that takes advantage of the relationship between Delaunay tetrahedralizations and Voronoi diagrams approximates the tetrahedralization with O(n log n) complexity on the number of points.

Running make main compiles a program for viewing a set of points representing a dragon, taken from the Stanford University Computer Graphics Laboratory (http://graphics.stanford.edu/data/3Dscanrep/). Only every tenth point is included. The way the main file is currently written, the up arrow moves the screen forward, the down arrow backwards, and wasd tilt the screen in different directions.
