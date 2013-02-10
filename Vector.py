from math import sqrt
class Vector(object):

    def __init__ (self, x = 0.0, y = 0.0, z = 0.0):

        self.x = x
        self.y = y
        self.z = z

    def __str__ (self):

        return "(%s, %s, %s)" % (self.x, self.y, self.z)

    @staticmethod
    def dot(v1, v2):

        result = 0

        if isinstance(v1, Vector) and\
           isinstance(v2, Vector):

            result = v1.x * v2.x +\
                     v1.y * v2.y +\
                     v1.z * v2.z

        return result

    @staticmethod
    def cross(a,b):

        v = Vector()

        if isinstance(v1, Vector) and\
           isinstance(v2, Vector):

            v.x = a.y * b.z - a.z * b.y
            v.y = a.z * b.x - a.x * b.z
            v.z = a.x * b.y - a.y * b.x

        return v

    def magnitude (self):

        return sqrt(self.x**2 +\
                    self.y**2 +\
                    self.z**2)

    def versor (self):

        mag = self.magnitude()
        v = Vector()

        if mag != 0:
            v.x = self.x / mag
            v.y = self.y / mag
            v.z = self.z / mag

        return v

    def __sub__( self, v ):

        return Vector( self.x - v.x,\
                       self.y - v.y )
