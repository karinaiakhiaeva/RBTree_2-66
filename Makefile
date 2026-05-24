CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = test_rbtree

all: $(TARGET)

$(TARGET): RBTree.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) RBTree.o -L. -lRBTreeTest

RBTree.o: RBTree.cpp BST.h
	$(CXX) $(CXXFLAGS) -c RBTree.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean run
