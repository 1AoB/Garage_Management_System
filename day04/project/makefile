#g++ main.cpp db.cpp -std=c++11 -I/usr/include/jsoncpp -lopencv_core -lopencv_imgcodecs -lopencv_videoio  -lcurl -lcrypto -ljsoncpp -lsqlite3  -o main.exe


CXX = g++
CFLAGS = -std=c++11 -I/usr/include/jsoncpp
LIBS = -lopencv_core -lopencv_imgcodecs -lopencv_videoio -lcurl -lcrypto -ljsoncpp -lsqlite3

SRCS = main.cpp db.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = main.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) $(LIBS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
