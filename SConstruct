env = Environment()

clean_files = []
clean_files.append( 'core' )
clean_files.append( '.sconsign.dblite' )
env.Clean( 'default', clean_files )

env.ParseConfig( 'sdl-config --cflags --libs' )

files = Glob( '*.c' )

env.Append( CPPPATH = [ '/usr/include/freetype2' ] )
env.Append( CPPFLAGS = [ '-Wall' ] )
env.Append( CPPFLAGS = [ '-g' ] )
env.Append( LIBS = [ 'GL' ] )
env.Append( LIBS = [ 'ftgl' ] )
env.Append( LIBS = [ 'config' ] )

env.Program( target = 'nndriver', source = files )

