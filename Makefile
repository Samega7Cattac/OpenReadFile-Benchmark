OUTPUT= ./benchmark
OBJS= main.o

all: $(OBJS)
	$(CXX) $(OBJS) -o $(OUTPUT) $(CXXFLASGS)

%.o: %.cpp %.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

debug: CXXFLAGS += -g
debug: all

clean:
	rm -f $(OBJS) $(OUTPUT)
