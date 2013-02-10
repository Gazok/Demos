from abc import ABCMeta, abstractmethod
import pygame.mouse
from Vector import Vector
from GeomComponent import LineGeomComponent

class InputComponent( object ):
    __metaclass__ = ABCMeta

    @abstractmethod
    def update( self, entity, geomComp ):
        while False:
            yield None

class MousePosInputComponent( InputComponent ):

    def update( self, entity, geomComp):
        entity.position = pygame.mouse.get_pos()

class DragLineInputComponent( InputComponent ):
    def __init__( self ):
        self.mouseDown = False

    def update( self, entity, geomComp ):
        mousePos = pygame.mouse.get_pos()
        mouseDown = pygame.mouse.get_pressed()[0]

        if mouseDown and self.mouseDown:
            entity.geomComp.vertices[1] = ( entity.position[0] - mousePos[0],\
                                            entity.position[1] - mousePos[1] )
        elif mouseDown and not self.mouseDown:
            entity.position = mousePos
            entity.geomComp = LineGeomComponent( ( ( 0, 0 ), ( 0, 0 ) ) )

        self.mouseDown = mouseDown
