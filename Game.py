import pygame.event
from pygame.locals import *
from pygame.display import get_surface, flip

from Entity import Entity
from Vector import Vector
from GeomComponent import PointGeomComponent, LineGeomComponent
from InputComponent import MousePosInputComponent, DragLineInputComponent

class Game(object):

    def __init__(self):

        self.entities = []

    def run(self):

        run = True

        screen = pygame.display.get_surface()

        #A point that follows the cursor
        self.addEntity( Entity( \
                        ( 50, 40 ),\
                        MousePosInputComponent(),\
                        PointGeomComponent(),\
                        None) )

        #A draggable line
        self.addEntity( Entity( \
                        ( 0, 0 ),\
                        DragLineInputComponent(),\
                        None,\
                        None) )

        #A fixed point
        self.addEntity( Entity( \
                        ( 50, 40 ),\
                        None,\
                        PointGeomComponent(),\
                        None) )
        #A fixed line
        self.addEntity( Entity( \
                        ( 50, 40 ),\
                        None,\
                        PointGeomComponent(),\
                        None) )

        while run:
            for event in pygame.event.get():
                if event.type == pygame.QUIT: run = False
                if event.type == pygame.KEYDOWN:
                    if event.key == K_ESCAPE or\
                       event.key == K_RETURN:
                        run = False

            screen.fill( 0 )

            for entity in self.entities:
                entity.update( screen )

            pygame.display.flip()


    def addEntity(self, entity):
        self.entities.append( entity )
