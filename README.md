# KSU_OS_PROJ_1
The following is my (Todd Stein) submission for my Operating Systems course at Kent State University in Spring 2022. The  project involves solving the producer/consumer problem in the C programming language using semaphores.


Designed to run on Linux systems with GNU gcc compiler.
To run type in your Linux terminal

```
./compileAndRun.sh

```
or

```
gcc consumer.c -pthread -lrt -o consumer
gcc producer.c -pthread -lrt -o producer
./producer & ./consumer

```
