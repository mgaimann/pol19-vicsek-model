# Active Matter Simulations: The Vicsek Model

## A) Getting started

### 1. Input Parameters:
```c++
"\tArg  1: <AGNTNO>\tNumber of agents in the Vicsek model\n"
"\tArg  2: <OUTPUT>\tOutput path\n"
"\tArg  3: <VELOCY>\tAgent velocity (default: 1)\n"
"\tArg  4: <BOXSIZ>\tLength of the quadratic box (default: 100)\n"
"\tArg  5: <NOISTR>\tCoefficient of the angular alignment noise (default: 1)\n"
"\tArg  6: <RADIUS>\tNeighborhood radius around agent in which\n\t\tangular orientations are averaged (default: 1)\n"
"\tArg  7: <PERIBC>\tSet periodic boundary conditions (default: true)\n"
"\tArg  8: <TIMTOT>\tTotal time of the simulation (default: 100)\n"
"\tArg  9: <TIMSTP>\tTime step of the simulation (default: 1)\n"
"\tArg 10: <POLPRB>\tProbability of polar interactions with respect to\n\t\tnematic interactions (default: 1 (polar))\n"
```

Currently, the dimension is hard-coded to 2, and every simulated time step is recorded
(may be adapted using the `timerecord_step` parameter).


### 2. Run Instructions:

Compile with:
```bash
g++ main.cpp -std=c++11 -o "main.out" -O2
```
Run with:
```bash
./main.out [args]
```
For example:
```bash
./main.out 400 "../data/" 0.03 10 0 1 1 100 1 1
```


### 3. Debugging:

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


### 4. Parameter Sweeping

You may use the provided script `sweep.sh` for a convenient execution of the source code. Moreover, using the script you may perform parameter sweeps over
 - `agent_number`,
 - `box_size` and
 - `noise_strength`.
It can be easily adapted to sweep over further parameters, such as `polar_interact_prob`.



## B) Project structure

The central `main.cpp` file includes several `.hpp` files:
 * `io_init.hpp`: Setting default parameters, handling input parsing from the command line, creating output file and printing input parameter into output file
 * `system_init.hpp`: Initializing internal `angles` and `positions` containers
 * `record_frame.hpp`: Printing information from the current frame into the output file
 * `subspace_operations.hpp`: Deals with everything concerned with introducing the subspace grid that serves to improves computational speed. Allocating agents to subspaces, finding neighboring subspace cells taking periodic boundaries into account, determining interactions between agents.
 * `update_params.hpp`: Updating the `angles` and `positions` containers based on polar or nematic interaction rules.



## C) Source code

The `main.cpp` code works as follows:

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
 * based on the just determined interactions specified in the `interacting_neighbors` container, update the positions and angles of all agents through the integration of the stochastic differential equations
    * $$ \theta_i (t+1) = \langle \theta(t) \rangle_r + \Delta \theta_i $$ (angle) and
    * $$ x_i(t+1) = x_i(t) + v_i(t)\cdot \Delta t $$ (position),
 where $$ \langle \theta(t) \rangle_r $$ denotes the average over all agent that are within the neighborhood radius of agent $$ i $$.
