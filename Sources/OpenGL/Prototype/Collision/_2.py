import math
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

    def __truediv__(self, other):
        if isinstance(other, float):
            return Vector2(self.x / other, self.y / other)


class Particle():
    """ Two dimensional particle. """
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


class BoxShape():
    """ @brief Contains box information. """
    def __init__(self, width = None, height = None, mass = None):
        """ Constructor.
        declare and define width, height, mass, and I scalar value.
        """
        self.width = random.randint(3, 10) if width is None else width
        self.height = random.randint(3, 10) if height is None else height
        self.mass = float(10) if mass is None else float(mass)
        # 우상단만 관성 모멘트를 구함.
        self.moment_i = self.set_box_interia()

    def set_box_interia(self):
        """ Set moment of interia of Boxshape instance input value.
        @param[in] instance BoxShape instance to set interia.
        @return Moment of interia value.
        """
        m = self.mass
        w = self.width
        h = self.height
        return m * (w**2 + h**2) / 12.0;


class RigidBody():
    """ @brief Rigidbody has effect of physical interaction """
    def __init__(self, position = None, lv = None, angle = None, av = None):
        self.position = Vector2() if position is None else position
        self.linear_velocity = Vector2() if lv is None else lv
        self.angle = float() if angle is None else angle
        self.angular_velocity = float() if av is None else av
        self.force = Vector2()
        self.torque = float()
        self.body = BoxShape()

    def calculate_torque(self, f = None):
        if f is not None: self.force = f
        # 우상단 지점에만 F 을 가함.
        r = Vector2(self.body.width / 2, self.body.height / 2)
        self.torque = r.x * self.force.y - r.y * self.force.x

    def move(self, dt): #{
        # Linear
        linear_a = (self.force / self.body.mass) * dt
        self.linear_velocity += linear_a
        self.position += self.linear_velocity * dt
        # Angular
        angular_a = self.torque / self.body.moment_i
        self.angular_velocity += angular_a * dt
        self.angle += self.angular_velocity * dt
   #}


class Main():
    """ Main class """
    def __init__(self):
        """ Initialize global time variables and variables used in app. """
        # Time variables.
        self.fps_interval = float()
        self.old_time = time.clock()
        self.time_now = self.old_time
        self.elapsed_time = self.time_now - self.old_time
        self.delta_time = 0
        # variables used in this application.
        self.rigidbodies = []
        # Run it!
        self.set_fps(15)
        self.initialize_rigidbodies()
        self.print_rigidbodies()
        self.run()

    def set_fps(self, fps = None):
        """ Set thick time """
        fps_thick = float(60 if fps is None else fps)
        self.fps_interval = 1 / fps_thick

    def initialize_rigidbodies(self, number = 1):
        """ Initialize rigidbody """
        for i in range(0, number):
            position = self.get_uniform_random_vector2(50, 50)
            angle = random.random() * 2 * math.pi
            lv = Vector2()
            # Append rigidbody
            self.rigidbodies.append(RigidBody(position, lv, angle, 0))

    def get_uniform_random_vector2(self, x, y):
        """ Get Vector2 instance with values created by uniform random value """
        x = random.random() * x
        y = random.random() * y
        return Vector2(x, y)

    def run(self):
        """ Actual running routine """
        while True:
            self.refresh_time()
            # count thick
            thick_count = int()
            try: thick_count = int(self.elapsed_time / self.fps_interval)
            except: exit()
            # update simulation
            while thick_count > 0:
                self.simulate()
                # at final, decrease thick_count
                thick_count -= 1
                self.elapsed_time -= self.fps_interval

    def refresh_time(self):
        """ Refresh Time interval, delta_time, etc """
        self.time_now    = time.clock()
        self.delta_time  = self.time_now - self.old_time
        self.old_time    = self.time_now
        self.elapsed_time += self.delta_time

    def simulate(self):
        """ Simulate simple torque simulation """
        force = Vector2(0, 100)
        for instance in self.rigidbodies:
            instance.calculate_torque(f = force)
            instance.move(self.fps_interval)
            self.print_rigidbodies()

    def print_rigidbodies(self):
        """ Print information of each rigidbody. """
        id_range = range(0, len(self.rigidbodies))
        for instance, index in zip(self.rigidbodies, id_range):
            _1 = "Body[" + str(index) + "]"
            _2 = "p = ({0:2f}, {1:2f}), a = {2:2f}".format(instance.position.x,
                                                           instance.position.y,
                                                           instance.angle)
            print(_1, _2)


def main():
    """ Simple torque and angular moving simulation """
    Main()

if __name__ == "__main__":
    main();