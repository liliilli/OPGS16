import random
import time

# Two dimensional vector.
class Vector2():
    def __init__(self, x = None, y = None):
        self.x = float() if x is None else x
        self.y = float() if y is None else y

    def __add__(self, other):
        if isinstance(other, self.__class__):
            x = self.x + other.x
            y = self.y + other.y
            return Vector2(x, y)

    def __mul__(self, other):
        if isinstance(other, float):
            return Vector2(self.x * other, self.y * other)

# Two dimensional particle.
class Particle():
    def __init__(self):
        self.position = Vector2()
        self.velocity = Vector2()
        self.acc      = Vector2()
        self.mass     = 0;

    def RefreshAcc(self, force_vector = Vector2(), dt = float()):
        self.acc    = Vector2(force_vector.x * dt, force_vector.y * dt)

    def RefreshPosition(self, dt):
        self.velocity += self.acc
        self.position += (self.velocity * dt)


# Global Array of particles.abs
particles = list();
particles.append(Particle());
fps_interval = float()

old_time        = time.clock()
time_now        = time.clock()
elapsed_time    = time_now - old_time
delta_time      = 0


def PrintParticleInformation() :
    ''' Print each particle's information of container '''
    for particle, index in zip(particles, range(0, len(particles))):
        print("particles[" + str(index) + "]", particle.position.x, particle.position.y)


def InitializeParticle():
    ''' Initialize particle internal value '''
    for particle in particles:
        # Set x, y position.
        x = random.random() * 50
        y = random.random() * 50
        particle.position = Vector2(x, y)
        particle.velocity = Vector2()
        # Set weight.
        weight = random.random() * 5
        particle.mass = weight


def RefreshTime():
    ''' Refresh Time interval, delta_time, etc '''
    # Global declaration
    global time_now, old_time, elapsed_time, delta_time
    # Body
    time_now    = time.clock()
    delta_time  = time_now - old_time
    old_time    = time_now
    elapsed_time += delta_time


def RunSimulation():
    ''' Simulate particle movement '''
    global elapsed_time
    # Body, Initialize.
    InitializeParticle()
    PrintParticleInformation()
    # Looping
    while True:
        RefreshTime()
        # Count thick
        thick_count = int()
        try: thick_count = int(elapsed_time / fps_interval)
        except: exit()
        # Update simulation
        while thick_count > 0:
            Simulate()
            # At final, decrease thick_count
            thick_count     -= 1
            elapsed_time    -= fps_interval


def Simulate():
    ''' Simulate something '''
    for particle in particles:
        particle.RefreshAcc(Vector2(0, -9.8), fps_interval)
        particle.RefreshPosition(fps_interval)
        PrintParticleInformation()


def SetFps(fps = None):
    ''' Set thick time '''
    global fps_interval
    fps_thick = float(60 if fps is None else fps)
    fps_interval = 1 / fps_thick


def Main():
    ''' Particle moving '''
    SetFps(60)
    RunSimulation()


if __name__ == "__main__":
    Main();