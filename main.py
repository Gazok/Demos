#!/usr/bin/env python

# Program    : Demos
# Author     : Jamie Bayne
# Description: A program designed to teach me more about geometry, collision
#              detection, and python


###############################################
# Let the wise hear and increase in learning, #
# and the one who understands obtain guidance #
#                                             #
#                           Proverbs 1:5      #
#                                             #
###############################################

import pygame.display

from Game import Game
from Vector import Vector

###Entry Point###
#################

def main():

    ###Set up game###
    #################

    error = False

    print("Starting Game")
    try:
        pygame.display.init()
    except pygame.error, e:
        print "Could not start pygame display module:"
        print e
        error = True
    
    try:
        screen = pygame.display.set_mode( ( 320, 240 ) )
    except pygame.error, e:
        print "Failed to create display: "
        print e
        error = True

    ###Testing###
    #############

    ###Start the game###
    #################### 

    if not error: 
        game = Game()
        game.run()

    ###End the game###
    ##################

    print("Shutting Down")
    pygame.quit()

if __name__ == '__main__' : main()
