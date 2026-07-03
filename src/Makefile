CXX = g++
CXXFLAGS = -std=c++20 -Wall -I../include -I. -lsqlite3

TARGET = school

SRC = main.cpp User.cpp Admin.cpp Teacher.cpp Student.cpp bcrypt.cpp blowfish.cpp Database.cpp

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./school

clean:
	rm -f *.o school