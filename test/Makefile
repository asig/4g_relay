SRC = \
  test.c \
  ../src/pdu.c \
  ../src/settings.c

HEADERS = \
  ../src/pdu.h \
  ../src/settings.h

test: test.c $(SRC) $(HEADERS)
	gcc -g -o test $(SRC) -I../src