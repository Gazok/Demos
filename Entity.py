class Entity ( object ):

    def __init__ ( self, pos, inputComp, geomComp, gfxComp ):
        self.position = pos
        self.inputComp = inputComp
        self.geomComp = geomComp
        self.gfxComp = gfxComp

    def update( self, surface ):
        if ( self.inputComp ):
            self.inputComp.update( self, self.geomComp )
        if ( self.geomComp ):
            self.geomComp.update( self, surface )
        if ( self.gfxComp ):
            self.gfxComp.update( self )
