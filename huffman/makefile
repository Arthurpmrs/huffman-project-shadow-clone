CC=gcc
CFLAGS=-Wall

DEPS = libs/huffman.h libs/linked_list.h libs/stack.h libs/binary_tree.h
TEST_DEPS = tests/test_binary_tree.h tests/test_linked_list.h tests/test_stack.h tests/test_huffman.h

ZIP_OBJ = zip.o libs/huffman.o libs/linked_list.o libs/stack.o
UNZIP_OBJ = unzip.o libs/binary_tree.o
TEST_OBJ = tests/tests.o libs/linked_list.o libs/binary_tree.o libs/stack.o libs/huffman.o tests/test_binary_tree.o tests/test_linked_list.o tests/test_stack.o tests/test_huffman.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

zip: $(ZIP_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

unzip: $(UNZIP_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(TEST_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tests: $(TEST_OBJ)
	$(CC) -o tests/$@ $^ $(CFLAGS) -lcunit

clean:
	-rm -f $(ZIP_OBJ)
	-rm -f $(UNZIP_OBJ)
	-rm -f $(TEST_OBJ)
	-rm -f zip
	-rm -f tmp
	-rm -f unzip
	-rm -f tests/tests