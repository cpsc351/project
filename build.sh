g++ -o producer producer.cpp -lpthread -std=c++17
g++ -o multi-threaded-consumer consumer.cpp -lpthread -std=c++17
./producer $1
./multi-threaded-consumer $2 $3
