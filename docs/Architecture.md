<<<<<<< HEAD
* Our library is static. We have separate headers and cpp files that should be built into an object file and then the library can be installed.
* We have Converter struct for users don't have to think about graph representation. Any supported representation will work fine. All algorithms take Converter class as an argument and extract the graph from it. The user only have to pass Converter(graph) as an argument. 
* Converter class constructs adjacency list from any graph and saves it as data member. 
* All of our algorithms use adjacency list representation.
=======
* Our library is header-only. It implements algorithms as functions.
* Our library is static. We have separate headers and cpp files that should be built into an object file and then the library can be installed.
* We have Converter struct for users don't have to think about graph representation. Any supported representation will work fine. All algorithms take Converter class as an argument and extract the graph from it. The user only have to pass Converter(graph) as an argument. 
* Converter class constructs adjacency list from any graph and saves it as data member. 
* All of our algorithms use adjacency list representation.
<<<<<<< HEAD
* Lee's algorithm takes bool matrices instead of graph. These matrices represent mazes like these:
<p align="center">
<img src="maze.png" width="250" height="175" />
</p>

>>>>>>> 4fbc71d (fix namings)
=======
>>>>>>> aa0e36e (Updated bench_report.txt + Update Architecture.md)
* If a user has edge list representation we require for him to make it using our Edge struct which is:

```
struct Edge {
    int from;
    ll w;
    int to;
};
```
    
<<<<<<< HEAD
<<<<<<< HEAD
* Below is our file structure. All implementations are in src folder, headers in include folder and tests and benchmarks are also in src folder.
=======
* Below is our file structure. All implementations are in include/spath.hpp and tests and benchmarks are in tests/ folder.
>>>>>>> 4fbc71d (fix namings)
=======
* Below is our file structure. All implementations are in src folder, headers in include folder and tests and benchmarks are also in src folder.
>>>>>>> aa0e36e (Updated bench_report.txt + Update Architecture.md)


File structure:

./

  ├── Architecture.md

  ├── CMakeLists.txt

<<<<<<< HEAD
<<<<<<< HEAD
  ├── Config.cmake.in
=======
  ├── Control_and_acceptance_procedure.md
>>>>>>> 4fbc71d (fix namings)
=======
  ├── Config.cmake.in
>>>>>>> aa0e36e (Updated bench_report.txt + Update Architecture.md)

  ├── LICENSE

  ├── README.md

<<<<<<< HEAD
<<<<<<< HEAD
  ├── build_repo_structure.sh

  ├── docs/

  ├──   ├── Architecture.md

  ├──   ├── Control_and_acceptance_procedure.md

  ├──   ├── Requirements.md

  ├──   ├── bench_report.txt

  ├──   ├── convert.py

  ├── include/

  ├──   ├── converter.hpp

  ├──   ├── edge.hpp

  ├──   ├── spath.hpp

  ├── src/

  ├──   ├── bench.cpp

  ├──   ├── converter.cpp

  ├──   ├── spath.cpp

=======
  ├── Requirements.md

=======
>>>>>>> aa0e36e (Updated bench_report.txt + Update Architecture.md)
  ├── build_repo_structure.sh

  ├── docs/

  ├──   ├── Architecture.md

  ├──   ├── Control_and_acceptance_procedure.md

  ├──   ├── Requirements.md

  ├──   ├── bench_report.txt

  ├──   ├── convert.py

  ├── include/

  ├──   ├── converter.hpp

  ├──   ├── edge.hpp

  ├──   ├── spath.hpp

<<<<<<< HEAD
  ├── maze.png

  ├── tests/

  ├──   ├── bench.cpp

>>>>>>> 4fbc71d (fix namings)
=======
  ├── src/

  ├──   ├── bench.cpp

  ├──   ├── converter.cpp

  ├──   ├── spath.cpp

>>>>>>> aa0e36e (Updated bench_report.txt + Update Architecture.md)
  ├──   ├── tests.cpp

  ├── utility/

  ├──   ├── Architecture-template.md

  ├──   ├── build_repo_structure.sh

<<<<<<< HEAD
  ├──   ├── maze.png

=======
>>>>>>> 4fbc71d (fix namings)
