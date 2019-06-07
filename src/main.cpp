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
//#include"check_neighborhood.hpp"
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
	float velocity = 5;
	float box_size = 100;
	float noise_strength = 1;
	float neighborhood_radius = 1;
	bool pbc = true; // sets periodic boundary conditions
	float time_step = 1; // smallest timestep for integration of ODEs
	float timerecord_step = 1; // timestep for recording frames
	float time_total = 10; // total runtime of the simulation
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

	outputfile << "#params: dim=" << dim
		<< "; agent_number=" << agent_number
		<< "; velocity=" << velocity
		<< "; box_size=" << box_size
		<< "; noise_strength=" << noise_strength
		<< "; neighborhood_radius=" << neighborhood_radius
		<< "; pbc=" << pbc
		<< "\n#time\t#agent_index\t#positions (dim columns)\t#angles ((dim-1) columns)"
		<< std::endl;


	// allocate random positions and angles
	std::vector<std::vector<float> > positions = positions_init(
		agent_number, box_size, dim);
	std::vector<std::vector<float> > angles = angles_init(
		agent_number, box_size, dim);

	// print elements of positions_init


	/*print(positions, agent_number, dim);*/



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

			// update direction, velocity and position
			update_positions(agent_number, dim, positions, angles, velocity, time_step, box_size);
	}


		outputfile.close();
		printf("Moin");
}
