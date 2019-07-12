
// initialisation/reset of subspace container
std::vector<std::vector<std::vector<int> > > subspace_init(
        int subspacing_number, int expected_agentnumber_per_subspace)
{
    // create empty subspace container
    std::vector < std::vector < std::vector < int > > > subspace_allocation(
            subspacing_number, std::vector < std::vector < int > > (
                    subspacing_number, std::vector<int>() //expected_agentnumber_per_subspace * 5)
            )
    );
    return subspace_allocation;
}



// allocate agent_indices to subspaces
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



// determine which subspace cells neighbor a certain cell
// first 2 dim: coord. of subspace cell, 3rd dim: neighbor index, 4th dim: array of (x,y) information of neighbors
std::vector < std::vector < std::vector < std::vector<int> > > > get_subspace_cell_neighbors(
        bool pbc, int subspacing_number, int dim, bool debug)
{
    // create empty subspace container
    std::vector < std::vector < std::vector < std::vector < int > > > > subspace_cell_neighbors(
            subspacing_number, std::vector < std::vector < std::vector < int > > >(
                    subspacing_number, std::vector< std::vector< int> >(
                            std::pow(dim+1, 2), std::vector <int>(dim)
                    )
            )
    );

    // iterate through subspace grid
    for (int subspace_xcoord = 0; subspace_xcoord < subspacing_number; subspace_xcoord++)
    {
        for (int subspace_ycoord = 0; subspace_ycoord < subspacing_number; subspace_ycoord++)
        {
            if (debug){std::cout<< "\n******CELL_NB\nx: " << subspace_xcoord << "\ty: " << subspace_ycoord << std::endl;}

            // store for each cell of subspace itself and its neighbors for neighborhood search
            // in 2D: +/-1 in x and y directions
            int neighbor_ind = 0; // we have 9 neighbors (including cell itself) in 2D

            for (int nbcell_xdim_ind = subspace_xcoord - 1;
                 nbcell_xdim_ind < subspace_xcoord + 2;
                 nbcell_xdim_ind++)
            {
                for (int nbcell_ydim_ind = subspace_ycoord - 1;
                     nbcell_ydim_ind < subspace_ycoord + 2;
                     nbcell_ydim_ind++)
                {
                    //std::cout << "was x: " << nbcell_xdim_ind <<
                    //"\ty: " << nbcell_ydim_ind << std::endl;
                    // treat periodic boundary conditions for each dimension (hardcoded!)
                    // x
                    if (nbcell_xdim_ind < 0 && pbc)
                    {
                        subspace_cell_neighbors[subspace_xcoord][subspace_ycoord][neighbor_ind][0] =
                                nbcell_xdim_ind + subspacing_number;
                    }
                    else if (nbcell_xdim_ind > subspacing_number-1 && pbc)
                    {
                        subspace_cell_neighbors[subspace_xcoord][subspace_ycoord][neighbor_ind][0] =
                                nbcell_xdim_ind - subspacing_number;
                    }
                    else if (nbcell_xdim_ind <= subspacing_number-1 && nbcell_xdim_ind >= 0)
                    {
                        subspace_cell_neighbors[subspace_xcoord][subspace_ycoord][neighbor_ind][0] =
                                nbcell_xdim_ind;
                    }
                    // y
                    if (nbcell_ydim_ind < 0 && pbc)
                    {
                        subspace_cell_neighbors[subspace_xcoord][subspace_ycoord][neighbor_ind][1] =
                                nbcell_ydim_ind + subspacing_number;
                    }
                    else if (nbcell_ydim_ind > subspacing_number-1 && pbc)
                    {
                        subspace_cell_neighbors[subspace_xcoord][subspace_ycoord][neighbor_ind][1] =
                                nbcell_ydim_ind - subspacing_number;
                    }
                    else if (nbcell_ydim_ind <= subspacing_number-1 && nbcell_ydim_ind >= 0)
                    {
                        subspace_cell_neighbors[subspace_xcoord][subspace_ycoord][neighbor_ind][1] =
                                nbcell_ydim_ind;
                    }

                    if (debug){std::cout << "nb_x: " <<
                        subspace_cell_neighbors[subspace_xcoord][subspace_ycoord][neighbor_ind][0] << "\tnb_y: " <<
                        subspace_cell_neighbors[subspace_xcoord][subspace_ycoord][neighbor_ind][1] << std::endl;}

                    neighbor_ind++;
                }
            }
        }
    }
    return subspace_cell_neighbors;
}



// determine actual interactions between agent in subspace and its neighbors
std::vector<std::vector<int> > get_interacting_neighbors(
        std::vector < std::vector < std::vector < std::vector<int> > > > subspace_cell_neighbors,
        std::vector<std::vector<std::vector<int> > > subspace_allocation,
        int expected_agentnumber_per_subspace, int subspacing_number, int dim,
        float neighborhood_radius, std::vector<std::vector<float> > positions,
        int agent_number, float box_size, bool pbc, bool debug
        )
{
    // initialize interaction container
    std::vector<std::vector<int> > interacting_neighbors(agent_number,
            std::vector<int>() ); //expected_agentnumber_per_subspace * 5) );

    // iterate through subspace grid
    for (int subspace_xcoord = 0; subspace_xcoord < subspacing_number; subspace_xcoord++)
    {
        for (int subspace_ycoord = 0; subspace_ycoord < subspacing_number; subspace_ycoord++)
        {
            if (debug){std::cout << "\n=====BASE_CELL=====\nx: " << subspace_xcoord << "\ty: " << subspace_ycoord << std::endl;}

            // iterate through all agent_inds in one subspace cell
            for (int subsp_agent_ind = 0; subsp_agent_ind < subspace_allocation[subspace_xcoord][subspace_ycoord].size();
                subsp_agent_ind++)
            {
                // retrieve agent index of agent in subspace cell
                int agent_ind = subspace_allocation[subspace_xcoord][subspace_ycoord][subsp_agent_ind];

                if (debug){std::cout << "\n>>> treating subsp_agent " << subsp_agent_ind << "\tof " <<
                    subspace_allocation[subspace_xcoord][subspace_ycoord].size() <<
                    " in this cell\n with agent_ind "<< agent_ind << std::endl;}

                // iterate through all neighboring cells to this cell
                for (int nbcell_ind = 0; nbcell_ind < static_cast<int>(std::pow(dim+1, 2) ); nbcell_ind++)
                {
                    // retrieve x and y coords of current neighboring subspace cell
                    int subsp_neighbor_xcoord = subspace_cell_neighbors[subspace_xcoord][subspace_ycoord][nbcell_ind][0];
                    int subsp_neighbor_ycoord = subspace_cell_neighbors[subspace_xcoord][subspace_ycoord][nbcell_ind][1];

                    if (debug){std::cout << "\n********INTERACTING_CELL\nneighborcell_x: " << subsp_neighbor_xcoord <<
                    "\tneighborcell_y: " << subsp_neighbor_ycoord << std::endl;}


                    // access agent indices in neighboring cells
                    for (int neighbor_ind = 0;
                        neighbor_ind < subspace_allocation[subsp_neighbor_xcoord][subsp_neighbor_ycoord].size();
                        neighbor_ind++)
                    {
                        // retrieve agent index of neighboring agent
                        int nb_agent_ind = subspace_allocation[subsp_neighbor_xcoord][subsp_neighbor_ycoord][neighbor_ind];

                        if (debug){std::cout << ">>> neighbor  " << neighbor_ind << "\tof " <<
                        subspace_allocation[subsp_neighbor_xcoord][subsp_neighbor_ycoord].size() <<
                        "\t with agent_ind "<< nb_agent_ind << std::endl;}


                        // compute distance between subsp_agent and subsp_neighbor_agent (2D hardcoded)
                        float xdistance = fabs(positions[nb_agent_ind][0] - positions[agent_ind][0]);
                        float ydistance = fabs(positions[nb_agent_ind][1] - positions[agent_ind][1]);

                        // treat pbcs (hardcoded)
                        if (xdistance > (box_size/2) && pbc)
                        {
                            xdistance = box_size - xdistance;
                        }
                        if (ydistance > (box_size/2) && pbc)
                        {
                            ydistance = box_size - ydistance;
                        }

                        // compute actual distance
                        float distance = sqrt( std::pow( xdistance, 2 ) + std::pow( ydistance, 2 ) );

                        // decide on interaction between these two agents based on neighborhood_radius
                        if (distance <= neighborhood_radius)
                        {
                            interacting_neighbors[agent_ind].push_back(nb_agent_ind);

                            if (debug){std::cout << "\t dist: " << distance << "\t interact: true" << std::endl;}
                        }
                        else
                        {
                            if (debug){std::cout << "\t dist: " << distance << "\t interact: false" << std::endl;}
                        }
                    }
                    if (debug){std::cout << "********" << std::endl;}
                }
            }
        if (debug){std::cout << "===================\n" << std::endl;}
        }
    }
    return interacting_neighbors;
}




