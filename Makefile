all:
	g++ -o producer producer.cpp -lpthread -std=c++17
	g++ -o multi-threaded-consumer consumer.cpp -lpthread -std=c++17

run_sample:
	./producer Zoomreport.txt
	./multi-threaded-consumer 3@csu.fullerton.edu 5
