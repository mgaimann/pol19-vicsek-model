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
#include"record_frame.hpp"
#include"update_params.hpp"
#include"system_init.hpp"
#include"debugging_tools.hpp"
#include"subspace_operations.hpp"


// Main
int main(int argc, char* argv[])
{

	// Parameter potentially modified by parsing
      int agent_number;
      std::string output_path = "../data/";
      float velocity = 1;
      float box_size = 10;
      float noise_strength = 1;
      float neighborhood_radius = 1;
      bool pbc = true; // sets periodic boundary conditions
      float time_step = 0.01; // smallest timestep for integration of ODEs
      float timerecord_step = 0.1; // timestep for recording frames
      float time_total = 100; // total runtime of the simulation
      int dim = 2;


	// Command line parsing handle
	if (argc < 2 || argc > 7)
	{
		std::cerr << "\n\t\t---ERROR---\nCheck number of input arguments.\n\n"
			"Usage:\n"
			"\tArg 1: <AGNTNO>\tNumber of agents in the Vicsek model\n"
			"\tArg 2: <OUTPUT>\tOutput path\n"
			"\tArg 3: <VELOCY>\tAgent velocity (default: 1)\n"
			"\tArg 4: <BOXSIZ>\tLength of the quadratic box (default: 100)\n"
			"\tArg 5: <NOISTR>\tCoefficient of the angular alignment noise (default: 1)\n"
			"\tArg 6: <RADIUS>\tNeighborhood radius around agent in which\n\t\tangular orientations are averaged (default: 1)\n"
			"\tArg 7: <PERIBC>\tSet periodic boundary conditions (default: true)\n"
			<< std::endl;
		return 0;
	}
	else
	{
		if (argc >= 2)
		{
			agent_number = atoi(argv[1]);
		}
		if (argc >= 3)
		{
			output_path = argv[2];
		}
		if (argc >= 4)
		{
			velocity = atof(argv[3]);
		}
		if (argc >= 5)
		{
			box_size = atof(argv[4]);
		}
		if (argc >= 6)
		{
			noise_strength = atof(argv[5]);
		}
		if (argc >= 7)
		{
			neighborhood_radius = atof(argv[6]);
		}
		if (argc == 8)
		{
			pbc = argv[7];
		}
	}


	// create output file handle
	std::string bs = std::string(output_path)
		+ "out_agntno_" + std::to_string(agent_number)
		+ "_noistr_" + std::to_string(noise_strength)
		+ ".txt";
	const char* filename = bs.c_str();
	std::ofstream outputfile;
	outputfile.open(filename, std::ofstream::trunc);

	outputfile << "#parameter" << std::endl;
	outputfile << "dim = " << dim << std::endl;
	outputfile << "agent_number = " << agent_number << std::endl;
	outputfile << "velocity = " << velocity << std::endl;
	outputfile << "box_size = " << box_size << std::endl;
	outputfile << "noise_strength = " << noise_strength << std::endl;
	outputfile << "neighborhood_radius = " << neighborhood_radius << std::endl;
	outputfile << "pbc = " << pbc << std::endl;
	outputfile << "\n#time\t#agent_index\t#positions (dim columns)\t#angles ((dim-1) columns)"
		<< std::endl;


	// allocate random positions and angles
	std::vector<std::vector<float> > positions = positions_init(
		agent_number, box_size, dim);
	std::vector<std::vector<float> > angles = angles_init(
		agent_number, box_size, dim);


    // create subspace with M x M fields
    // M is the box size divided by the specified neighborhood_radius
    int subspacing_number = static_cast<int>(floor(box_size / neighborhood_radius)); //per dimension

    // pre-allocated space for innermost vector (number of agent-indices):
    int expected_agentnumber_per_subspace =
            static_cast<int>( ceil(agent_number / std::pow(subspacing_number, 2) ) );

    // determine neighboring cells in subspace
    std::vector < std::vector < std::vector < std::vector<int> > > > subspace_cell_neighbors =
            get_subspace_cell_neighbors(pbc, subspacing_number, dim);



	// loop over time interval
	for (float time = 0; time < time_total; time += time_step)
	{
		// record frame if condition is met  
		record_frame(outputfile, agent_number, time_step,
			timerecord_step, time, dim, positions, angles);

		// initialize/reset subspace
        std::vector<std::vector<std::vector<int> > > subspace_allocation = subspace_init(
            box_size, neighborhood_radius, agent_number);

        // allocate agents to subspaces
        allocate_to_subspace(
            subspace_allocation, neighborhood_radius, agent_number, positions);

        // determine which agents interact with each other
        std::vector<std::vector<int> > interacting_neighbors = get_interacting_neighbors(
                subspace_cell_neighbors, subspace_allocation,
                expected_agentnumber_per_subspace, subspacing_number, dim,
                neighborhood_radius, positions, agent_number);

        // update direction, velocity and position
        update_positions(agent_number, dim, positions, angles, velocity, time_step, box_size);


    }


		outputfile.close();
		printf("Moin");
}
