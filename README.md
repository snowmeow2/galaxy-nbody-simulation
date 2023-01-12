# Galaxy N-body Simulation
Simulate and visualise the collision of galaxies which includes N-body gravitational force, using the Barnes-Hut algorithm. It models a spherical galaxy with a Hernquist profile.

This is the product of final project for the course "Computational Astrophysics", ASTR660 in NTHU.
The main simulation is written in C++ and the visualisation is done using three.js. 

## Requirements
- Python 3, numpy
- C++11 compiler, OpenMP
## Usage
1. Clone the repository
    ```
    git clone https://github.com/snowmeow2/galaxy-nbody-simulation.git
    ```
2. Compile the C++ code
    ```
    cd galaxy-nbody-simulation
    make
    ```
3. Prepare the initial conditions
    ```
    python3 particle.py
    ``` 
    This will generate a file called `halo.txt`.
4. Run the simulation
    ```
    ./simuation
    ```
    A directory called `out` will be created and the simulation output `halo_NUM.txt` will be saved in it.
5. Visualise the simulation (optional)
   
   First set up a local server on the repository directory:
   ```
    python3 -m http.server
    ``` 
   Then open the browser and go to `localhost:8000` to view the simulation.

## Configuration
The simulation can be configured by changing the parameters in `config.ini`, `parameter.h` and `visual.js`.

### `config.ini`
Parameters in `config.ini` are used to generate the initial positions and velocities of the particles. The parameters are:
- `a`: the scale radius of the galaxy [kpc]
- `M`: the total mass of the galaxy [$M_\odot$]
- `r_max`: the maximum radius of the galaxy [kpc]
- `N`: the number of particles in the galaxy
- `collision`: whether to generate an additional galaxy for collision
- `separation`: the separation between the two galaxies [kpc]
- `rel_vel`: the relative velocity between the two galaxies [km/s]

### `parameter.h`
Parameters in `parameter.h` are used to configure the simulation. The parameters are:
- `Mass`: the mass of each particle [$M_\odot$]
- `dt`: the time step of the simulation [Gyr]
- `softening`: the softening length of the gravitational force [kpc]
- `N`: the total iteration of the simulation
- `save_iter`: how often to save the simulation data [iteration]

### `visual.js`
Parameters in `visual.js` are used to configure the animation. The parameters are:
- `particles_pos_file_prefix`: the prefix of the files containing the simulation data. The files should be named as `particles_pos_file_prefix + frame_number + ".txt"`.
- `frames`: the number of frames, 1 for a static image
- `rate`: the `frame_number` of files (it reads `halo_NUM.txt` as index) that displayed, roughly the frame rate of the animation

## Acknowledgement
This work used high-performance computing facilities operated by the
Center for Informatics and Computation in Astronomy (CICA) at National
Tsing Hua University. This equipment was funded by the Ministry of
Education of Taiwan, the Ministry of Science and Technology of Taiwan,
and National Tsing Hua University.
