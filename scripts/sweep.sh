#!/bin/bash

# Executable file
execfile="../src/main.out"

# Set constants
agent_number="100"
output_path="../data/sweep1/"
velocity="1"
neighborhood_radius="1"
pbc="1"
time_total="100"
time_step="1"

# Interval boundaries and steps for parameter sweeps
box_size_low="5";
box_size_high="10"
box_size_step="1"

noise_strength_low="0";
noise_strength_high="1"
noise_strength_step="0.1"

# Work around floating point math
box_iterations="$( bc <<<"($box_size_high - $box_size_low) / $box_size_step" )"
noise_iterations="$( bc <<<"($noise_strength_high - $noise_strength_low) / $noise_strength_step" )"
box_iter_ind="0"
noise_iter_ind="0"


# Call executable with various parameters

# Initialize
mkdir -p $output_path
box_size=$box_size_low

# Vary box size (density) and noise strength
while [ $box_iter_ind -le $box_iterations ]; do

    # Reset noise variable
    noise_strength=$noise_strength_low
    noise_iter_ind="0"
    while [ $noise_iter_ind -le $noise_iterations ]; do

        # Run script
        echo Computing BOXSIZ $box_size NOISTR $noise_strength
        $execfile $agent_number $output_path $velocity $box_size $noise_strength $neighborhood_radius $pbc $time_total $time_step
        wait

        noise_strength="$( bc <<<"$noise_strength + $noise_strength_step" )"
        noise_iter_ind=$((noise_iter_ind+1))
    done
    echo
    box_size="$( bc <<<"$box_size + $box_size_step" )"
    box_iter_ind=$((box_iter_ind+1))
done
