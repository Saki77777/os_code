hdrs = ipc.h
opts = -g -c
producer_src = producer.c ipc.c
producer_obj = producer.o ipc.o
smoker_src = smoker.c ipc.c
smoker_obj = smoker.o ipc.o
all: producer smoker
producer: $(producer_obj)
	gcc $(producer_obj) -o producer
producer.o: producer.c $(hdrs)
	gcc $(opts) producer.c
smoker: $(smoker_obj)
	gcc $(smoker_obj) -o smoker
smoker.o: smoker.c $(hdrs)
	gcc $(opts) smoker.c
clean:
	rm -f producer smoker *.o
