all: main 

main: main.cc
	g++ -o main main.cc graph.cc

# graph: graph.cc
# 	g++ -o graph graph.cc

clean:
# del ../*
	rm -f *.o
	rm -f *.exe
	rm -f main