clean :
	rm ./test
build :
	gcc test.c -o test
	
run  :
	./test
