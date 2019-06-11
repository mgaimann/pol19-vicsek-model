void update_positions(int agent_number, int dim, std::vector<std::vector<float> > &positions,
	std::vector<std::vector<float> > angles, float velocity, float time_step, float box_size)
{
	for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
	{
		for (int dim_ind = 0; dim_ind < dim; dim_ind++)
		{
			if (dim_ind == 0)
			{
				positions[agent_ind][dim_ind] += velocity * cos(angles[agent_ind][dim_ind]) * time_step;

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
				positions[agent_ind][dim_ind] += velocity * sin(angles[agent_ind][dim_ind]) * time_step;
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