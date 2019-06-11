void update_positions(int agent_number, int dim, std::vector<std::vector<float> > &positions,
	std::vector<std::vector<float> > angles, float velocity, float time_step, float box_size)
{
	for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
	{
		for (int dim_ind = 0; dim_ind < dim; dim_ind++)
		{
			if (dim_ind == 0)
			{
				//differential equation
				positions[agent_ind][dim_ind] += velocity * cos(angles[agent_ind][dim_ind]) * time_step;

				//periodic boundary condition
				if (positions[agent_ind][dim_ind] > box_size)
				{
					positions[agent_ind][dim_ind] -= box_size;
				}
				if (positions[agent_ind][dim_ind] < 0)
				{
					positions[agent_ind][dim_ind] += box_size;
				}
			}
			if (dim_ind == 1)
			{
				//differential equation
				positions[agent_ind][dim_ind] += velocity * sin(angles[agent_ind][dim_ind]) * time_step;
				
				//periodic boundary condition
				if (positions[agent_ind][dim_ind] > box_size)
				{
					positions[agent_ind][dim_ind] -= box_size;
				}
				if (positions[agent_ind][dim_ind] < 0)
				{
					positions[agent_ind][dim_ind] += box_size;
				}

			}
		}
	}
}

void update_angles(int agent_number, int dim, std::vector<std::vector<float> > angles,
	float time_step, float noise_strength, std::vector<std::vector<int> > interacting_neighbors )
{
	// initialize random number generator, draw from [-pi;pi)
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-atan(1) * 4, atan(1) * 4);

	for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
	{
		for (int dim_ind = 0; dim_ind < dim - 1; dim_ind++)
		{
			if (dim_ind == 0)
			{
				//stochastic differential equation
				angles[agent_ind][dim_ind] = mean(angles) + noise_strength * dis(gen);
			}
		}
	}