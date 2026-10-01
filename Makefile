CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -O0

app: main.cpp threadfuncs.cpp threadfuncs.h
	$(CXX) $(CXXFLAGS) $^ -o $@

run: app
	./app

clean:
	rm -f app output.log trace.log

.PHONY: run clean
