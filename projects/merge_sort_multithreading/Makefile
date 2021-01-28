PROJECT_NAME:=laboratory_one

CXX				:=g++
CXXFLAGS		:=

BUILD_DIR		:=build

SOURCE_DIR		:=src
SOURCES			:=$(wildcard $(SOURCE_DIR)/*.cpp)
OBJS			:=$(SOURCES:.cpp=.o)

INCLUDE_DIR		:=./src
INCLUDES		:=$(wildcard $(INCLUDE_DIR)/*.hpp)

.PHONY: all clean run

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
	$(CXX) $< -o $(BUILD_DIR)/$@

%.o: %.cpp $(INCLUDES)
	$(CXX) -c -I$(INCLUDE_DIR) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $<

run:
	./$(PROJECT_NAME)

clean:
	rm $(OBJS)
	rm $(PROJECT_NAME)