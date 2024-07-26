# ASAN = -fsanitize=address

parse_asan:
	g++ parse_test.cc -o p -Wall -ggdb3 -lgtest ${ASAN}

parse_no_asan:
	g++ parse_main.cc -o p -Wall -ggdb3 -lgtest ${ASAN}