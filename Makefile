PROGRAM = libcv_inpaint.so

HFILES = src/shared.h

OFILES = src/InpaintFSR.o src/InpaintNS.o src/InpaintShiftmap.o src/InpaintTelea.o src/shared.o

CFLAGS = -c -I /usr/include/vapoursynth -I /usr/local/include/vapoursynth -I /usr/include/opencv4 -I /usr/local/include/opencv4 -fPIC -g -Wall -Wextra -Wpedantic -std=c++17 -O3
LDFLAGS = -lm -shared -lopencv_core -lopencv_photo -lopencv_xphoto

%.o: %.cpp $(HFILES)
	g++ $(CFLAGS) -c $< -o $@

$(PROGRAM): $(OFILES)
	g++ $(OFILES) $(LDFLAGS) -o $@

.PHONY: clean
clean:
	-rm -f src/*.o $(program)

