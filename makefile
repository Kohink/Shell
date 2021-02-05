all:
	gcc -std=gnu99 -o shell *.c 

clean:
	rm -f $(OBJS) $(OUT)