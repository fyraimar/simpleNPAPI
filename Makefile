


CFLAGS = -Wall -DXP_UNIX=1 -DMOZ_X11=1 -fPIC -g

simpleNPAPIplugin : plugin_main.o
	cc $(CFLAGS) -shared plugin_main.o -o simpleNPAPIplugin.so

plugin_main.o : plugin_main.c plugin_main.h
	cc `pkg-config --cflags gtk+-2.0` $(CFLAGS) -c plugin_main.c

clean :
	rm simpleNPAPIplugin.so plugin_main.o

