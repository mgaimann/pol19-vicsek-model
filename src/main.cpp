#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstring>
#include <random>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <complex>
#include"record_frame.hpp"
#include"update_params.hpp"
#include"system_init.hpp"
#include"debugging_tools.hpp"
#include"subspace_operations.hpp"
#include"io_init.hpp"

// Main
int main(int argc, char* argv[])
{

    // arameters which may be modified through parsing
    int agent_number;
    std::string output_path = "../data/";
    double velocity = 0.03;
    double box_size = 10;
    double noise_strength = 0.1;
    double neighborhood_radius = 1;
    bool pbc = true; // sets periodic boundary conditions
    double time_total = 100; // total runtime of the simulation
    double time_step = 1; // smallest timestep for integration of ODEs
    double polar_interact_prob = 1;

    // not parsable parameters
    double timerecord_step = 1;
    int dim = 2;

    // debugging options
    bool debug = false;
    int seed = 1996;


    // input / output handling
    parse_input(argc, argv, agent_number, output_path, velocity, box_size, noise_strength, neighborhood_radius,
            pbc, time_total, time_step, polar_interact_prob);
    std::ofstream outputfile;
    output_init(dim, outputfile, agent_number, output_path, velocity, box_size, noise_strength, neighborhood_radius,
                pbc, time_total, time_step, polar_interact_prob);


    // initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(seed);


    // define angle interval, here: [-pi, pi)
    // changing this might cause issues in the update_angles function
    double angle_interval_low = - atan(1) * 4;
    double angle_interval_high = atan(1) * 4;


	// allocate random positions and angles
	std::vector<std::vector<double> > positions = positions_init(
		agent_number, box_size, dim, gen);
	std::vector<std::vector<double> > angles = angles_init(
		agent_number, box_size, dim, angle_interval_low, angle_interval_high, gen);


    // create subspace with M x M fields
    // M is the box size divided by the specified neighborhood_radius
    int subspacing_number = static_cast<int>(floor(box_size / neighborhood_radius)); //per dimension

    // pre-allocated space for innermost vector (number of agent-indices):
    int expected_agentnumber_per_subspace =
            static_cast<int>( ceil(agent_number / std::pow(subspacing_number, 2) ) );

    // determine neighboring cells in subspace
    std::vector < std::vector < std::vector < std::vector<int> > > > subspace_cell_neighbors =
            get_subspace_cell_neighbors(pbc, subspacing_number, dim, debug);


    // loop over time interval
	for (double time = 0; time < time_total; time += time_step)
	{
		// record frame if condition is met  
		record_frame(outputfile, agent_number, time_step,
			timerecord_step, time, dim, positions, angles);

		// initialize/reset subspace
        std::vector<std::vector<std::vector<int> > > subspace_allocation = subspace_init(
                subspacing_number, expected_agentnumber_per_subspace);

        // allocate agents to subspaces
        allocate_to_subspace(
            subspace_allocation, neighborhood_radius, agent_number, positions, debug);


        // determine which agents interact with each other
        std::vector<std::vector<int> > interacting_neighbors = get_interacting_neighbors(
                subspace_cell_neighbors, subspace_allocation,
                expected_agentnumber_per_subspace, subspacing_number, dim,
                neighborhood_radius, positions, agent_number, box_size, pbc, debug);


        // update direction, velocity and position
        angles = update_angles(agent_number, dim, angles, noise_strength, interacting_neighbors,
                               angle_interval_low, angle_interval_high, gen, polar_interact_prob);

        update_positions(agent_number, dim, positions, angles, velocity, time_step, box_size);

        //std::cout << "time: " << time << std::endl;
    }

    // record last frame
    record_frame(outputfile, agent_number, time_step,
                 timerecord_step, time_total, dim, positions, angles);

    outputfile.close();
}
