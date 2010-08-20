env = Environment()

clean_files = []
clean_files.append( 'core' )
clean_files.append( '.sconsign.dblite' )
env.Clean( 'default', clean_files )

env.ParseConfig( 'sdl-config --cflags --libs' )

files = Glob( '*.c' )

env.Append( CPPPATH = [ '/usr/include/freetype2' ] )
env.Append( CPPFLAGS = [ '-Wall' ] )
env.Append( CPPFLAGS = [ '-Werror' ] )
env.Append( CPPFLAGS = [ '-g' ] )
env.Append( LINKFLAGS = [ '-g', '-Wall' ] )
env.Append( LIBS = [ 'GL' ] )
env.Append( LIBS = [ 'ftgl' ] )
env.Append( LIBS = [ 'config' ] )
env.Append( LIBS = [ 'fann' ] )
env.Append( LIBS = [ 'm' ] )

env.Program( target = 'nndriver', source = files )

