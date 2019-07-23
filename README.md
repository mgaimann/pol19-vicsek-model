# Active Matter Simulations: The Vicsek Model



## B) Getting started

### 1. Compilation of the C++ Code

Compile with:
```bash
g++ main.cpp -std=c++11 -o "main.out" -O2
```


### 2. Running the Compiled Code: Input parameters
Run with:
```bash
./main.out [args]
```
For example:
```bash
./main.out 400 "../data/" 0.03 10 0 1 1 100 1 1
```

The input arguments are specified in the following:
```c++
 "\tArg  1: <AGNTNO>\tNumber of agents in the Vicsek model (integer)\n"
 "\tArg  2: <OUTPUT>\tOutput path (string, default: \"../data/\")\n"
 "\tArg  3: <VELOCY>\tAgent velocity (double, default: 0.5)\n"
 "\tArg  4: <BOXSIZ>\tLength of the quadratic box (integer, default: 10)\n"
 "\tArg  5: <NOISTR>\tCoefficient of the angular alignment noise (double, default: 0.1)\n"
 "\tArg  6: <RADIUS>\tNeighborhood radius around agent in which\n\t\tangular orientations are averaged (double, default: 1)\n"
 "\tArg  7: <PERIBC>\tSet periodic boundary conditions (boolean, default: true)\n"
 "\tArg  8: <TIMTOT>\tTotal time of the simulation (double, default: 100)\n"
 "\tArg  9: <TIMSTP>\tTime step of the simulation (double, default: 1)\n"
 "\tArg 10: <POLPRB>\tProbability of polar interactions with respect to\n\t\tnematic interactions (double, default: 1 (polar))\n"
```

Currently, the dimension is hard-coded to 2, and every simulated time step is recorded
(may be adapted using the `timerecord_step` parameter).


### 3. Parameter Sweeping

You may use the provided script `sweep.sh` for a convenient execution of the source code. Moreover, using the script you may perform parameter sweeps over
 - `agent_number`,
 - `box_size` and
 - `noise_strength`.
It can be easily adapted to sweep over further parameters, such as `polar_interact_prob`.


### 4. Evaluation: Using the Jupyter Notebook

- Download FFmpeg from https://ffmpeg.org/, which is necessary for processing images to videos.
- Define the path to the `ffmpeg.exe` file on your computer as `FFmpeg1 = path` , e.g. `FFmpeg1 = "C:/Users//ffmpeg.exe"`.
- Compile and run the C++ code as described previously.
- Follow the steps in the jupyter notebook and run the `evaluation` function for the first processing step. Input parameters are:
 - `foldername`: folder with .txt files from source code output to be evaluated
 - `movie`: boolean, 1 for movie generation, 0 for no movies
 - `burnin_start`: specify a burn-in time which will not be considered in the evaluation (e.g. due to
system equilibration), if you want to "discard" the first 30 time steps, set this variable to 30
 - a video of the active matter trajectory for every output file and
 - a file containing evaluated physical quantities (such as order parameters), based on the variation of control parameters in the various files. The folder name in which your `*.txt` output files are located will be the name of this file.


### 3. Debugging

Open debugger:
```bash
gdb main.out
```
Compile in debugger:
```bash
!g++ main.cpp -std=c++11 -o "main.out" -g
```
Run in debugger:
```bash
run [args]
```

You may also set `debug=True` for a more verbose output.
The random number generator seed is fixed and may be modified for debugging purposes.


## C) Source code structure

The central `main.cpp` file includes several `.hpp` files:
 * `io_init.hpp`: Setting default parameters, handling input parsing from the command line, creating output file and printing input parameter into output file
 * `system_init.hpp`: Initializing internal `angles` and `positions` containers
 * `record_frame.hpp`: Printing information from the current frame into the output file
 * `subspace_operations.hpp`: Deals with everything concerned with introducing the subspace grid that serves to improves computational speed. Allocating agents to subspaces, finding neighboring subspace cells taking periodic boundaries into account, determining interactions between agents.
 * `update_params.hpp`: Updating the `angles` and `positions` containers based on polar or nematic interaction rules.



## D) Algorithm

The `main.cpp` code makes use of a computing scheme that introduces a subspace consisting of cells or boxes. This leads to a reduction in time complexity from O(N^2) to O(N) and allows the study of large systems.
The algorithm works as follows:

Initially, a (uniformly) random position and a (uniformly) random angle is allocated to every agent. Every agent is uniquely identified by an index. Positional and angular information is stored in `angles` and `positions` containers and may be retrieved and manipulated using this index.

Then, in every time step, we
 * record the information of the current frame
 * based on every agent's position, determine the subspace box the particle lies in and store this information in the `subspace_allocation` container   
 * based on the distance, determine the interactions with other agents. To this end, we
    * iterate through every subspace box (referred to as the "current box")
    * iterate through every agent (referred to as the "current agent") in the current box and retrieve the indices of this agent from the `subspace_allocation` container
    * iterate through every neighboring subspace box of the current box, including the current box itself (in 2D: a 3 x 3 grid) (referred to as the "neighbor box")
    * iterate through every agent in the neighbor box (referred to as the "neighbor agent") and retrieve the indices of these neighbor agents from the `subspace_allocation` container   
    * using the indices of current agent and neighbor agent, retrieve the coordinates of both from the `positions` container and compute the distance between them, taking periodic boundaries into account
    * if `distance` < `neighborhood_radius`, make an entry in the `interacting_neighbors` array for the current agent
 * based on the just determined interactions specified in the `interacting_neighbors` container, update the positions and angles of all agents through the integration of the stochastic differential equations.
