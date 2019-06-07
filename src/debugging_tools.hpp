void print(std::vector<std::vector<float> > const& input, int agent_number, int dim)
{
	for (int i = 0; i < agent_number; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			std::cout << input[i][j] <<std::endl;
		}
		std::cout <<std::endl;
	}
}