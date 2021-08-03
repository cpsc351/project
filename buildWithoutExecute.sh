# g++ -c consumer.hpp multi-threaded-consumer.o #-lpthread
# g++ -c producer.cpp producer.o #-lpthread
# g++ -o multi-threaded-consumer.o producer.o producer -lpthread
# g++ -o multi-threaded-consumer.o multi-threaded-consumer -lpthread
g++ -o producer producer.cpp -lpthread -std=c++17
g++ -o consumer multi-threaded-consumer.cpp -lpthread -std=c++17
#./producer $1
#./consumer $2 $3
#./multi-threaded-consumer $1
