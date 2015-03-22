CXX = clang++
SRCS = $(filter-out main.cpp,$(foreach file,$(wildcard src/*),$(notdir $(file))))
TEST = squeezebox_exec
LIB_DIR = lib_$(CXX)
LIB_STATIC = libsqueezebox.a
LIB_SHARED = libsqueezebox.so
BUILD_DIR = build_$(CXX)
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

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

CXXFLAGS_g++ = -Iinclude -std=c++11 -g -Wall -Werror -fPIC
CXXFLAGS_clang++ = -Iinclude -std=c++11 -g -Wall -Werror -fPIC

CXXFLAGS = $(CXXFLAGS_$(CXX))

LINKER_FLAGS_g++ = -Wl,-z,origin '-Wl,-rpath,$$ORIGIN/$(LIB_DIR):$$ORIGIN/steam-runtime/amd64/lib/x86_64-linux-gnu:$$ORIGIN/steam-runtime/amd64/lib:$$ORIGIN/steam-runtime/amd64/usr/lib/x86_64-linux-gnu:$$ORIGIN/steam-runtime/amd64/usr/lib' '-L$(LIB_DIR)' -lm -lGLEW -lGL -lGLU -lSDL2_image -lSDL2_mixer -lSDL2 -lBox2D -llua
LINKER_FLAGS_clang++ = -Wl,-z,origin '-Wl,-rpath,$$ORIGIN/$(LIB_DIR):$$ORIGIN/steam-runtime/amd64/lib/x86_64-linux-gnu:$$ORIGIN/steam-runtime/amd64/lib:$$ORIGIN/steam-runtime/amd64/usr/lib/x86_64-linux-gnu:$$ORIGIN/steam-runtime/amd64/usr/lib' '-L$(LIB_DIR)' -lm -lGLEW -lGL -lGLU -lSDL2_image -lSDL2_mixer -lSDL2 -lBox2D -llua
LINKER_FLAGS = $(LINKER_FLAGS_$(CXX))

vpath %.cpp src

.PHONY: all directories install uninstall clean

all: directories $(LIB_DIR)/$(LIB_STATIC) $(LIB_DIR)/$(LIB_SHARED) $(TEST)

directories: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $<

$(LIB_DIR)/$(LIB_STATIC): $(OBJS)
	ar -cvq $(LIB_DIR)/$(LIB_STATIC) $^

$(LIB_DIR)/$(LIB_SHARED): $(OBJS)
	$(CXX) -shared -Wl,-soname,$(LIB_SHARED).1 $(LINKER_FLAGS) -o $(LIB_DIR)/$(LIB_SHARED).1.0 $^
	(cd $(LIB_DIR); ln -sf '$(LIB_SHARED).1.0' '$(LIB_SHARED).1'; ln -sf '$(LIB_SHARED).1.0' '$(LIB_SHARED)')

$(TEST): $(BUILD_DIR)/main.o $(LIB_DIR)/$(LIB_SHARED)
	$(CXX) $(LINKER_FLAGS) -lsqueezebox $(BUILD_DIR)/main.o -o $(TEST)

install: all
	$(INSTALL_DATA) $(LIB_DIR)/$(LIB_STATIC) $(DESTDIR)$(libdir)
	$(INSTALL_DATA) $(LIB_DIR)/$(LIB_SHARED) $(DESTDIR)$(libdir)
	$(INSTALL_DATA) include/*.hpp $(DESTDIR)$(includedir)squeezebox/

uninstall: all
	rm -r $(DESTDIR)$(includedir)squeezebox

clean:
	rm $(BUILD_DIR)/*
