CXX = g++
CXXFLAGS = -Werror -Wpedantic -Wall
GTEST_LIBS = -lgtest -lgtest_main -pthread

EXECUTABLE_1 = main
EXECUTABLE_2 = tests

$(EXECUTABLE_1): functions.o main.o 
	$(CXX) $(CXXFLAGS) functions.cpp main.cpp -o $(EXECUTABLE_1)
$(EXECUTABLE_2): functions.o tests.o
	$(CXX) functions.cpp tests.cpp -o $(EXECUTABLE_2) $(GTEST_LIBS)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f *.o
cleanall:
	rm -f *.o $(EXECUTABLE_1) $(EXECUTABLE_2) outfile.bin
