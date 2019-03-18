matrix: a5.h a5_matrix.c
	gcc a5.h a5_matrix.c -o matrix

list: a5.h a5_list.c
	gcc a5.h a5_list.c -o list

memcheck: matrix list
	valgrind --leak-check=full ./matrix
	valgrind --leak-check=full ./list

run: matrix list
	./matrix
	./list

clean:
	rm matrix list
