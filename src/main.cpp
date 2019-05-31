#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <chrono>
#include"record_frame.hpp"
#include"check_neighborhood.hpp"
#include"update_params.hpp"

// Main
int main(int argc, char* argv[])
{

	// Parameter potentially modified by parsing
    int agent_number;
    std::string output_path = "";
    float velocity = 1;
    float boxsize = 100;
    float noise_strength = 1;
    float neighborhood_radius = 1;
    bool pbc = true;
    float time_step = 0.5;
    float time_total = 10;


	// Command line parsing handle
	if (argc < 2 || argc > 7)
	{
		std::cerr << "\n\t\t---ERROR---\nCheck number of input arguments.\n\n"
		"Usage:\n"
		"\tArg 1: <NUMBER>\tNumber of agents in the Vicsek model\n"
		"\tArg 2: <OUTPUT>\tOutput path\n"
		"\tArg 3: <VELOCY>\tAgent velocity (default: 1)\n"
		"\tArg 4: <BOXSIZ>\tLength of the quadratic box (default: 100)\n"
		"\tArg 5: <NOISEC>\tCoefficient of the angular alignment noise (default: 1)\n"
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
			output_path = argv[1];
		}
		if (argc >= 4)
		{
			velocity = atof(argv[2]);
		}
		if (argc >= 5)
		{
			boxsize = atof(argv[3]);
		}
		if (argc >= 6)
		{
            noise_strength = atof(argv[4]);
		}
		if (argc >= 7)
		{
			neighborhood_radius = atof(argv[5]);
		}
        if (argc == 8)
		{
			pbc = argv[6];
		}
	}


    // initialize internal parameters and arrays
    float time = 0;
    int agent_ind = 0;
    std::vector<int> neighbor_indices(0);

/*
    steps = (time_total / time_step) + 1;
    std::vector<float> time_series(steps);
    std::vector<std::vector<float> > angle_series(steps, std::vector<float>(agent_number));
    std::vector<std::vector<std::vector<float> > > pos_series(steps, std::vector<std::vector<float> >(agent_number, std::vector<float>(2)));
*/

    // allocate random positions and angles
    system_init()

    // loop over time interval
    while time < time_total
    {
        // loop over all agents in every time step
        for agent_ind < agent_number
        {
            // determine agents within neighborhood radius
            neighbor_indices = check_neighborhood();

            // update direction, velocity and position
            update_params();

            // record frame if condition is met
            record_frame();
        }
    }


    // create output file handle
    std::ofstream outputfile;
	  outputfile.open("out.txt", std::ofstream::trunc);

    outputfile << agent_number << std::endl;

    outputfile.close();

    printf("Moin");
}
