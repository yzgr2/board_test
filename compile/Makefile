all:
	gcc -c v1.c
	gcc -c v2.c
	ar crv libv1.a v1.o
	ar crv libv2.a v2.o
	gcc -o main_test_v1 main.c libv1.a libv2.a
	gcc -o main_test_v2 main.c libv2.a libv1.a

clean:
	rm *.o *.a
