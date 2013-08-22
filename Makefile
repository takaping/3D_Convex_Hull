VPATH = src
BLDDIR = bin

TARGET = ch3d
OBJS = main.o \
	MainWindow.o \
	GraphicsModel.o \
	GraphicsView.o \
	Subject.o \
	Observer.o \
	ConvexHull.o \
	GeoGraph.o \
	Vector3d.o \
	Edge.o \
	Face.o \
	Line.o \
	Triangle.o \
	Memory.o \
	trackball.o
DEPS = $(OBJS:%.o=%.d)
RESRCS = MainWindow.glade my_logo.jpg

CXX = g++
CC = gcc
CXXFLAGS = -std=c++11 -Wall -O3 -MMD -MP -MF $(@:%.o=%.d) `pkg-config --cflags gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2`
CFLAGS = -Wall -O3 -MMD -MP -MF $(@:%.o=%.d)
LDFLAGS = -lglut -lGLU -lGL -lm `pkg-config --libs gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2`

all: $(BLDDIR)/$(TARGET) $(patsubst %, $(BLDDIR)/%, $(RESRCS))

-include $(DEPS)

$(BLDDIR)/$(TARGET): $(patsubst %, $(BLDDIR)/%, $(OBJS))
	$(CXX) $(LDFLAGS) -o $@ $^

$(BLDDIR)/%.o: %.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BLDDIR)/%.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -c -o $@ $<

$(BLDDIR)/%.glade: %.glade
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	@cp $< $@

$(BLDDIR)/%.jpg: %.jpg
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	@cp $< $@

.PHONY: clean
clean:
	@rm -rf $(BLDDIR)

