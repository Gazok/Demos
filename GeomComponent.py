from pygame.draw import line as drawLine, lines as drawLines, circle as drawCircle
from abc import ABCMeta, abstractmethod
from Vector import Vector

class GeomComponent( object ):
    __metaclass__ = ABCMeta

    def __init__( self ):
        self.vertices = []
        self.color = 0xFFFFFFFF

    @abstractmethod
    def update( self, entity, surface ):
        while False:
            yield None

class PointGeomComponent( GeomComponent ):
    def update( self, entity, surface ):
        drawCircle( surface, self.color, entity.position, 2 )


class LineGeomComponent( GeomComponent ):
    def __init__( self, points ):
        super( LineGeomComponent, self ).__init__()
        self.vertices = [ points[0], points[1] ]

    def update( self, entity, surface ):
        line = ( self.vertices[0][0] - self.vertices[1][0],\
                 self.vertices[0][1] - self.vertices[1][1] )
        p1 = entity.position
        p2 = ( line[0] + p1[0], line[1] + p1[1] )
        drawLine( surface, self.color, p1, p2 )

class PolyGeomComponent( GeomComponent ):
    def __init__( self, vertices ):
        self.vertices = vertices

    def update( self, entity, surface ):
        drawLines(surface, self.color, True, vertices )

class CircleGeomComponent( GeomComponent ):
    def __init__( self, radius ):
        super( CircleGeomComponent, self ).__init__()
        self.radius = radius

    def update( self, entity, surface ):
        drawCircle( surface, self.color, entity.position, radius )
