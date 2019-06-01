void record_frame(std::ofstream outputfile, int agent_number,
    float time_step, float timerecord_step, float time,  int dim,
    std::vector<std::vector<float> > positions,
    std::vector<std::vector<float> > angles)
{
    int frame_step = static_cast<int>(time_step / timerecord_step);
    int frame = static_cast<int>(time / time_step);
    
    if (frame % frame_step == 0) // record if multiple of frame_step
    {
        for (int agent_ind = 0; agent_ind < agent_number; agent_ind ++)
        {
            outputfile << time << "\t" 
                << agent_ind << "\t";
            
            // write all position coordinates into file
            for (int dim_ind = 0; dim_ind < dim; dim++)
            {
                outputfile << positions[agent_ind][dim] << "\t";
            }
            
            // write all angular components into file
            for (int dim_ind = 0; dim_ind < dim-1; dim++)
            {
                outputfile << angles[agent_ind][dim] << "\t";
            }
            
            // go to next line and treat next agent
            outputfile << std::endl;
        }
    }
}
