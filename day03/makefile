CXX = g++
CXXFLAGS = -c -g -std=c++11
LIBS = -lsqlite3


parking.exe: parking.o db.o
	$(CXX) $^ $(LIBS)  -o $@
parking.o:parking.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

db.o:db.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY:clean
clean:
	$(RM) main.o parking.o

#g++ parking.cpp db.cpp -std=c++11 -lsqlite3  -o parking.exe

