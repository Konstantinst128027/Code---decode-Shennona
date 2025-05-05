CXX = g++
CXXFLAGS = -Werror -Wpedantic -Wall

EXECUTABLE_1 = main

$(EXECUTABLE_1): functions.o main.o 
	$(CXX) $(CXXFLAGS) functions.cpp main.cpp -o $(EXECUTABLE_1)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(EIGEN) -c $< -o $@
clean:
	rm -f *.o
cleanall:
	rm -f *.o $(EXECUTABLE_1) $(EXECUTABLE_2)
