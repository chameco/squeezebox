CXX = g++
OBJECTS = src/reactor.o src/resource.o src/aabb.o src/module.o src/context.o src/entity.o src/entity_manager.o src/gui_element.o src/gui_manager.o
INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = cp -r
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(prefix)/bin
datarootdir = $(prefix)/share
datadir = $(datarootdir)
includedir = $(prefix)/include/
libdir = $(exec_prefix)/lib/
CXXFLAGS = -Iinclude -std=c++11 -g -Wall -Werror

all: shared
shared: $(OBJECTS)
	$(CXX) -std=c++11 -shared -Wl,-soname,libsqueezebox.so.1.0 -o libsqueezebox.so.1.0 -lm -lGLEW -lSDL2 -lGL -lGLU -lSDL2_image -lSDL2_mixer $(OBJECTS)
install: all
	$(INSTALL_DATA) libsqueezebox.so.1.0 $(DESTDIR)$(libdir)
	ln -sf $(DESTDIR)$(libdir)libsqueezebox.so.1.0 $(DESTDIR)$(libdir)libsqueezebox.so.1
	ln -sf $(DESTDIR)$(libdir)libsqueezebox.so.1.0 $(DESTDIR)$(libdir)libsqueezebox.so
	mkdir -p $(DESTDIR)$(includedir)squeezebox/
	$(INSTALL_DATA) include/*.hpp $(DESTDIR)$(includedir)squeezebox/
uninstall: all
	rm $(DESTDIR)$(libdir)libsqueezebox.so.1.0
	rm $(DESTDIR)$(libdir)libsqueezebox.so.1
	rm $(DESTDIR)$(libdir)libsqueezebox.so
	rm -r $(DESTDIR)$(includedir)squeezebox
clean:
	rm $(OBJECTS)
