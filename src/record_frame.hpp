void record_frame(std::ofstream& outputfile, int agent_number,
    double time_step, double timerecord_step, double time,  int dim,
    std::vector<std::vector<double> > positions,
    std::vector<std::vector<double> > angles)
{
    
    // consider frames to use modulo operation for recording decision
    int frame_step = static_cast<int>(timerecord_step / time_step);
    int frame = static_cast<int>(time / time_step);
    
    // record if multiple of frame_step
    if (frame % frame_step == 0)
    {
        for (int agent_ind = 0; agent_ind < agent_number; agent_ind ++)
        {
            outputfile << time << "\t" 
                << agent_ind << "\t";
            
            // write all position coordinates into file
            for (int dim_ind = 0; dim_ind < dim; dim_ind++)
            {
                outputfile << positions[agent_ind][dim_ind] << "\t";
            }
            
            // write all angular components into file
            for (int dim_ind = 0; dim_ind < dim-1; dim_ind++)
            {
                outputfile << angles[agent_ind][dim_ind] << "\t";
            }
            
            // go to next line and treat next agent
            outputfile << std::endl;
        }
    }
}
