EXECUTABLE = save_players

CXX = g++
CXXFLAGS = -Wall -Werror -pedantic --std=c++17 -g -fsanitize=address -fsanitize=undefined

SOURCES = $(wildcard *.cpp)

save_players.exe: save_players.cpp
	$(CXX) $(CXXFLAGS) save_players.cpp -o $@

clean:
	rm -f $(EXECUTABLE)_debug $(EXECUTABLE)
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out

debug: CXXFLAGS += -g3 -DDEBUG
debug:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)_debug
