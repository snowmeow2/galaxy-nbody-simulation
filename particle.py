import numpy as np

G = 4.47e-6  # [kpc^3 / (Msun * Gyr^2)]


class Hernquist:
    """The Hernquist halo model. See Hernquist (1990), ApJ, 356, 359."""

    def __init__(self, a, M, r_max=None):
        self.a = a
        self.M = M
        self.r_max = a * 5 if r_max is None else r_max

    def density(self, r):
        """Density distribution of the model."""
        return self.M * self.a / (2 * np.pi * r * (r + self.a) ** 3)

    def mass_enclosed(self, r):
        """Mass enclosed within a radius r."""
        return (r / (r + self.a)) ** 2

    def mass_enclosed_r(self, M_fac):
        """Inverse of self.mass_enclosed(r)."""
        return self.a / (1 / (M_fac**0.5) - 1)

    def escape_velocity(self, r):
        """Escape velocity at a radius r."""
        return np.sqrt(2 * G * self.M / (r + self.a))

    def potential(self, r):
        """Potential at a radius r."""
        return -G * self.M / (r + self.a)

    def distribution(self, r, v):
        """Energy distribution of the model."""
        total_energy = self.potential(r) + v**2 / 2

        q = np.sqrt(-self.a * total_energy / (G * self.M))
        vg = np.sqrt(G * self.M / self.a)

        return (
            self.M
            / (np.sqrt(2) * (2 * np.pi * self.a * vg) ** 3 * (1 - q**2) ** (5 / 2))
            * (3 * np.arcsin(q) + q * np.sqrt(1 - q**2) * (1 - 2 * q**2) * (8 * q**4 - 8 * q**2 - 3))
        )

    def random_positions(self, N):
        """Generate N random positions in the halo."""
        positions = np.zeros(N)
        for i in range(N):
            # generate a radius based on a random mass fraction
            while True:
                M_fac = np.random.random()
                r = self.mass_enclosed_r(M_fac)

                if r < self.r_max:
                    positions[i] = r
                    break

        return positions

    def random_velocity(self, r):
        """Generate random velocities for the given radius."""
        # get the maximum distribution value to speed up the generation
        velocities_range = np.linspace(0, self.escape_velocity(r), 1000)
        max_dist = max(self.distribution(r, velocities_range) * velocities_range**2)

        # generate random velocities bewteen 0 and escape velocity at r
        # until a random value is accepted
        while True:
            v = np.random.random() * self.escape_velocity(r)
            dist = self.distribution(r, v) * v**2

            if np.random.random() * max_dist < dist:
                return v

    def generate_particles(self, N):
        """Generate N particles in the halo."""
        # set random positions and velocities
        positions = self.random_positions(N)
        velocities = np.array([self.random_velocity(r) for r in positions])

        # get 4 random numbers for the angles
        theta1 = np.arccos(np.random.random(N) * 2 - 1)
        theta2 = np.arccos(np.random.random(N) * 2 - 1)
        phi1 = np.random.random(N) * 2 * np.pi
        phi2 = np.random.random(N) * 2 * np.pi

        x = positions * np.sin(theta1) * np.cos(phi1)
        y = positions * np.sin(theta1) * np.sin(phi1)
        z = positions * np.cos(theta1)

        vx = velocities * np.sin(theta2) * np.cos(phi2)
        vy = velocities * np.sin(theta2) * np.sin(phi2)
        vz = velocities * np.cos(theta2)

        return np.array([x, y, z, vx, vy, vz]).T


# read the generation parameters from the config file
with open("config.ini") as f:
    lines = f.readlines()
    lines = [line for line in lines if line != "\n" and line[0] != "#"]

    a = float(lines[0].split("=")[1].split("#")[0])
    M = float(lines[1].split("=")[1].split("#")[0])
    r_max = float(lines[2].split("=")[1].split("#")[0])
    N = int(lines[3].split("=")[1].split("#")[0])

    collision = lines[4].split("=")[1].split("#")[0]
    if collision.lower() == "true":
        collision = True
    else:
        collision = False

    if collision:
        separation = float(lines[5].split("=")[1].split("#")[0])
        rel_vel = float(lines[6].split("=")[1].split("#")[0])

# generate the halo
halo = Hernquist(a=a, M=M, r_max=r_max)
data = halo.generate_particles(N)

if collision:
    halo2 = Hernquist(a=a, M=M, r_max=r_max)
    data2 = halo2.generate_particles(N)

    # translate x by separation of X kpc
    data[:, 0] += separation / 2
    data2[:, 0] -= separation / 2

    # translate vx by X km/s
    data[:, 3] -= rel_vel / 2
    data2[:, 3] += rel_vel / 2

    data = np.concatenate((data, data2))

# save the array to txt
np.savetxt("halo.txt", data, header=str(len(data)), comments="")
