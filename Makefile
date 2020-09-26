CC= gcc 

HEADER_FILES= gills_app.h gills_extern.h gillsfileop.h gills.h gills_internal.h simplex.h testdata.h testtokens.h
SRCS= gillsfileop.c gills.c simplex.c testdata.c test.c testtokens.c ${HEADER_FILES}
OBJS= gillsfileop.o gills.o simplex.o testdata.o test.o testtokens.o

LIBGILLS= libgills.a libgills.so 

TESTS= test_woinl_womemunl test_inl_womemunl test_woinl_memunl test_inl_memunl

all: ${LIBGILLS} ${TESTS}

${LIBGILLS}: gills.c gills.h gillsfileop.c gillsfileop.h gills_internal.h
	${CC} -fPIC -shared -o libgills.so gills.c gillsfileop.c

test_woinl_womemunl : ${SRCS}
	${CC}  -o gills_woinl_womemunl gillsfileop.c gills.c simplex.c testdata.c test.c testtokens.c

test_inl_womemunl :
	${CC} --define REDUCE_ACTION_INLINE=1  -o gills_inl_womemunl gillsfileop.c gills.c simplex.c testdata.c test.c testtokens.c

test_woinl_memunl:
	${CC} --define GILLS_MEM_TKSTACK_UNLIM=1  -o gills_woinl_memunl gillsfileop.c gills.c simplex.c testdata.c test.c testtokens.c

test_inl_memunl:
	${CC} --define REDUCE_ACTION_INLINE=1 --define GILLS_MEM_TKSTACK_UNLIM=1  -o gills_inl_memunl gillsfileop.c gills.c simplex.c testdata.c test.c testtokens.c

.PHONY: testrun

testrun:
	@echo -e  "Running test without action inline and without memory no limit"
	@echo -e  "hit any key to continue"
	@read
	./test_woinl_womemunl
	@echo -e  "Test done, hit any key to continue"
	@read
	@echo -e  "Running test with action inline and without memory no limit"
	@echo -e  "hit any key to continue"
	@read
	./test_inl_womemunl
	@echo -e  "Test done, hit any key to continue"
	@read
	@echo -e  "Running test without action inline and with memory no limit"
	@echo -e  "hit any key to continue"
	@read
	./test_woinl_memunl
	@echo -e  "Test done, hit any key to continue"
	@read
	@echo -e  "Running test with action inline and with memory no limit"
	@echo -e  "hit any key to continue"
	@read
	./test_inl_memunl
	@echo -e  "Test done, hit any key to continue"
	@read


