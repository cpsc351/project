echo "command line argument must be provided after 'run' in gdb"
g++ -o producer consumer.hpp producer.cpp -g -lpthread -std=c++17
gdb ./producer $1 $2 $3
