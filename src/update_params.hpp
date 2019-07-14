// defines the signum function
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


// updates the positions of all agents from time t to t+1
void update_positions(int agent_number, int dim, std::vector<std::vector<float> > &positions,
	std::vector<std::vector<float> > angles, float velocity, float time_step, float box_size)
{
    // 2D hard-coded: consider only one angular dimension
    int dim_angle_ind = 0;

	for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
	{
	    // iterate over all spatial dimensions (dim_pos_ind)
		for (int dim_pos_ind = 0; dim_pos_ind < dim; dim_pos_ind++)
		{
			if (dim_pos_ind == 0)
			{
				//differential equation
				positions[agent_ind][dim_pos_ind] += velocity * cos(angles[agent_ind][dim_angle_ind]) * time_step;

				//periodic boundary conditions
				if (positions[agent_ind][dim_pos_ind] >= box_size)
				{
					positions[agent_ind][dim_pos_ind] -= box_size;
				}
				if (positions[agent_ind][dim_pos_ind] < 0)
				{
					positions[agent_ind][dim_pos_ind] += box_size;
				}
			}
			if (dim_pos_ind == 1)
			{
                //differential equation
				positions[agent_ind][dim_pos_ind] += velocity * sin(angles[agent_ind][dim_angle_ind]) * time_step;

                //periodic boundary condition
				if (positions[agent_ind][dim_pos_ind] >= box_size)
				{
					positions[agent_ind][dim_pos_ind] -= box_size;
				}
				if (positions[agent_ind][dim_pos_ind] < 0)
				{
					positions[agent_ind][dim_pos_ind] += box_size;
				}
			}
		}
	}
}


// updates the angles of all agents from time t to t+1
std::vector<std::vector<float> > update_angles(int agent_number, int dim, std::vector<std::vector<float> > angles,
	float noise_strength, std::vector<std::vector<int> > interacting_neighbors, float angle_interval_low,
	float angle_interval_high, std::mt19937& gen, float polar_interact_prob)
{

	// RNG: draw from [-pi;pi) (angular noise) and [0,1) (polar or nematic interactions)
    std::uniform_real_distribution<> dis(angle_interval_low, angle_interval_high);
    std::uniform_real_distribution<> dis2(0, 1);


	for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
	{
	    // decide on type of interaction
	    bool polar_flag = false;
	    if (dis2(gen) < polar_interact_prob)
        {
            polar_flag = true;
        }

	    // treat all angular dimensions
		for (int dim_ind = 0; dim_ind < dim - 1; dim_ind++)
		{
			if (dim_ind == 0)
			{
			    // summation of complex exponentials exp(i*theta) (take mean of circular quantity)
				const std::complex<float> i(0,1);
			    std::complex<float> exp_sum = 0;

				// integration of the stochastic differential equations
				// angular update: following the original Vicsek paper
				for (int interact_neighbor_ind = 0; interact_neighbor_ind < interacting_neighbors[agent_ind].size();
				    interact_neighbor_ind++)
				{
					int selected_agent_ind = interacting_neighbors[agent_ind][interact_neighbor_ind];
                    std::complex<float> agent_angle( angles[selected_agent_ind][dim_ind] ,0);

                    if (polar_flag) // polar interaction
					{
					    exp_sum += std::exp(i * agent_angle);
                    }
					else // nematic interaction according to arXiv:1206.3811 eq. 1
                    {
                        std::complex<float> nematic_coeff( sgn( cos (angles[agent_ind][dim_ind] -
                        angles[selected_agent_ind][dim_ind])), 0);
					    exp_sum +=  std::exp(i * nematic_coeff* agent_angle);
                    }
				}
				// obtain argument of complex exponential and add noise
                angles[agent_ind][dim_ind] = static_cast<float>(std::arg(exp_sum)) + noise_strength * dis(gen);


                // pbc, put angles into [-pi,pi) by subtracting or adding 2pi
				if (angles[agent_ind][dim_ind] > angle_interval_high)
				{ 
					angles[agent_ind][dim_ind] -= 2 * atan(1) * 4;
				}
				else if (angles[agent_ind][dim_ind] <= angle_interval_low)
				{
					angles[agent_ind][dim_ind] += 2 * atan(1) * 4;
				}
			}
		}
	}
	return angles;
}