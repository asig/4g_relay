.PHONY: clean test

SRC = \
	test.c \
	fakes.c \
	../src/pdu.c \
	../src/settings.c

HEADERS = \
	../src/pdu.h \
	../src/settings.h

all: run_test

clean:
	@rm -f test

test: test.c $(SRC) $(HEADERS)
	@gcc -g -o test $(SRC) \
		-DUSE_HAL_DRIVER \
		-DHAL_CRC_MODULE_ENABLED \
		-DSTM32F103x6 \
		-I../src \
		-I../src/CMSIS/Include \
		-I../src/STM32F1xx_HAL_Driver/Inc \
		-I../src/CMSIS/Device/ST/STM32F1xx/Include

run_test: test
	@./test

