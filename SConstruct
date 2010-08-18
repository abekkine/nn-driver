env = Environment()

clean_files = []
clean_files.append( 'core' )
clean_files.append( '.sconsign.dblite' )
env.Clean( 'default', clean_files )

files = Glob( '*.c' )

env.Append( LIBS = [ 'config' ] )

env.Program( target = 'nndriver', source = files )

