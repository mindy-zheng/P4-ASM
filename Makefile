CPP = g++ 

CPP_FLAGS = -std=c++11 -Wall

TARGET = P3 

CPP_FILES = main.cpp scanner.cpp parser.cpp testTree.cpp generator.cpp

OBJ = $(CPP_FILES:.cpp=.o) 

all: $(TARGET) 

$(TARGET): $(OBJ) 
	$(CPP) $(CPP_FLAGS) -o $(TARGET) $(OBJ) 

%.o: %.cpp 
	$(CPP) $(CPP_FLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ) $(TARGET) 
