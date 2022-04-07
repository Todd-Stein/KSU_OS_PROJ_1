gcc consumer.c -pthread -lrt -o consumer
gcc producer.c -pthread -lrt -o producer
./producer & ./consumer
