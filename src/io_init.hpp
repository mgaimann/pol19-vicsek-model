// command line parsing handle
int parse_input(int argc, char* argv[], int& agent_number, std::string& output_path, double& velocity, double& box_size,
        double& noise_strength, double& neighborhood_radius, bool& pbc, double& time_total, double& time_step,
        double& polar_interact_prob)

{
    if (argc < 2 || argc > 11) {
        std::cerr << "\n\t\t---ERROR---\nCheck number of input arguments.\n\n"
         "Usage:\n"
         "\tArg  1: <AGNTNO>\tNumber of agents in the Vicsek model (integer)\n"
         "\tArg  2: <OUTPUT>\tOutput path (string, default: \"../data/\")\n"
         "\tArg  3: <VELOCY>\tAgent velocity (double, default: 0.5)\n"
         "\tArg  4: <BOXSIZ>\tLength of the quadratic box (integer, default: 10)\n"
         "\tArg  5: <NOISTR>\tCoefficient of the angular alignment noise\n\t\t\t\t(double, default: 0.1)\n"
         "\tArg  6: <RADIUS>\tNeighborhood radius around agent in which angular\n\t\t\t\torientations are averaged (double, default: 1)\n"
         "\tArg  7: <PERIBC>\tSet periodic boundary conditions (boolean, default: true)\n"
         "\tArg  8: <TIMTOT>\tTotal time of the simulation (double, default: 100)\n"
         "\tArg  9: <TIMSTP>\tTime step of the simulation (double, default: 1)\n"
         "\tArg 10: <POLPRB>\tProbability of polar interactions with respect to\n\t\t\t\tnematic interactions (double, default: 1 (polar))\n"
         << std::endl;
        return 0;

    } else {
        if (argc >= 2) {
            agent_number = atoi(argv[1]);
        }
        if (argc >= 3) {
            output_path = argv[2];
        }
        if (argc >= 4) {
            velocity = atof(argv[3]);
        }
        if (argc >= 5) {
            box_size = atof(argv[4]);
        }
        if (argc >= 6) {
            noise_strength = atof(argv[5]);
        }
        if (argc >= 7) {
            neighborhood_radius = atof(argv[6]);
        }
        if (argc >= 8) {
            pbc = atoi(argv[7]);
        }
        if (argc >= 9) {
            time_total = atof(argv[8]);
        }
        if (argc == 10) {
            time_step = atof(argv[9]);
        }
        if (argc == 11) {
            polar_interact_prob = atof(argv[10]);
        }
    }
    return 1;
}


// create output file handle
void output_init(int dim, std::ofstream& outputfile, int agent_number, std::string output_path, double velocity, double box_size,
                 double noise_strength, double neighborhood_radius, bool pbc, double time_total, double time_step,
                 double polar_interact_prob){

    std::string bs = std::string(output_path)
        + "out_N_" + std::to_string(agent_number)
        + "_v_" + std::to_string(velocity)
        + "_L_" + std::to_string(box_size)
        + "_eta_" + std::to_string(noise_strength)
        + "_r_" + std::to_string(neighborhood_radius)
        + "_pbc_" + std::to_string(pbc)
        + "_totaltime_" + std::to_string(time_total)
        + "_timestep_" + std::to_string(time_step)
        + "_polar-interact-prob_" + std::to_string(polar_interact_prob)
        + ".txt";
    const char* filename = bs.c_str();

    outputfile.open(filename, std::ofstream::trunc);
    outputfile << "#parameter" << std::endl;
    outputfile << "dim=" << dim << std::endl;
    outputfile << "agent_number=" << agent_number << std::endl;
    outputfile << "velocity=" << velocity << std::endl;
    outputfile << "box_size=" << box_size << std::endl;
    outputfile << "noise_strength=" << noise_strength << std::endl;
    outputfile << "neighborhood_radius=" << neighborhood_radius << std::endl;
    outputfile << "pbc=" << pbc << std::endl;
    outputfile << "time_total=" << time_total << std::endl;
    outputfile << "time_step=" << time_step << std::endl;
    outputfile << "polar_interact_prob=" << polar_interact_prob << std::endl;
    outputfile << "\n#time\t#agent_index\t#positions (dim columns)\t#angles ((dim-1) columns)" << std::endl;
}