from __future__ import with_statement
from IECore import *
from CortexOcean import OceanOp

# renders a sphere
class oceanProcedural( ParameterisedProcedural ) :
    def __init__( self ) :
        ParameterisedProcedural.__init__( self, "Renders an ocean surface." )
        self.grid = Reader.create("/home/dan/grid.obj").read()
        
    def doBound( self, args ) :
        return self.grid.bound()
        
    def doRenderState( self, renderer, args ) :
        pass

    def doRender( self, renderer, args ) :
        grid = self.grid
        OceanOp()( input=grid, copyInput=False, size=1000, shortwave=.1, waveheight=2.8, chop=True, choppiness=1.1, damp=.5 )        
        MeshNormalsOp()( input=grid, copyInput=False )
        grid.render( renderer )

# register
registerRunTimeTyped( oceanProcedural )
