CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = cpu

SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	@if [ -z "$(FILE)" ]; then echo "Please provide a .yk file. Usage: make run FILE=<filename>"; else ./$(TARGET) $(FILE); fi
