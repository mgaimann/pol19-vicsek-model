std::vector<std::vector<std::vector<int> > > subspace_init(
  float box_size, float neighborhood_radius, int agent_number)
  {
    // create subspace with M x M fields
    // M is the box size divided by the specified neighborhood_radius
    int subspacing_number = static_cast<int>(floor(box_size / neighborhood_radius));

    // pre-allocated space for innermost vector (number of agent-indices):
    // agent_number / area * 5 (arbitrary)
    int expected_agentnumber_per_subspace =
            static_cast<int>( ceil(agent_number / std::pow(subspacing_number, 2) * 5) );

    // create empty subspace container
    std::vector < std::vector < std::vector < int > > > subspace_allocation(
            subspacing_number, std::vector < std::vector < int > > (
                    subspacing_number, std::vector<int>(expected_agentnumber_per_subspace)
            )
    );

    return subspace_allocation;
}


void allocate_to_subspace(
        std::vector<std::vector<std::vector<int> > > &subspace_allocation, // pass by ref
        float neighborhood_radius, int agent_number,
        std::vector<std::vector<float> > positions)
{
  // allocate all agents to subboxes
  for (int agent_ind = 0; agent_ind < agent_number; agent_ind++)
  {
      // subbox pos = integer division of position by subbox spacing (nbhood radius)
      // Caution: dim = 2 hard-coded here
      int subspace_xcoord = static_cast<int> (floor(positions[agent_ind][0] / neighborhood_radius) );
      int subspace_ycoord = static_cast<int> (floor(positions[agent_ind][1] / neighborhood_radius) );

      subspace_allocation[subspace_xcoord][subspace_ycoord].push_back(agent_ind);
    }
}
