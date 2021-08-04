g++ -o producer producer.cpp -g -lpthread -std=c++17
g++ -o multi-threaded-consumer consumer.cpp -g -lpthread -std=c++17
./producer Zoomreport.txt
echo "Ran producer on zoomreport.txt"
echo "provide command line arguments after invoking run"
gdb ./multi-threaded-consumer
