CC= gcc 

HEADER_FILES= gills_app.h gills_extern.h gillsfileop.h gills.h gills_internal.h simplex.h testdata.h testtokens.h
SRC_FILES= gillsfileop.c mempool.c gills.c simplex.c testdata.c test.c testtokens.c
SRCS= ${HEADER_FILES} ${SRC_FILES}

OBJS= gillsfileop.o mempool.o gills.o simplex.o testdata.o test.o testtokens.o

LIBGILLS= libgills.so 

TESTS= gills_woinl_womemunl gills_inl_womemunl gills_woinl_memunl gills_inl_memunl

all: ${LIBGILLS} ${TESTS}

${LIBGILLS}: gills.c gills.h gillsfileop.c gillsfileop.h gills_internal.h
	${CC} -fPIC -shared -o libgills.so gills.c gillsfileop.c
	${CC} -c gills.c
	${CC} -c gillsfileop.c

gills_woinl_womemunl : ${SRCS}
	${CC}  -o gills_woinl_womemunl ${SRC_FILES}

gills_inl_womemunl : ${SRC_FILES}
	${CC} --define REDUCE_ACTION_INLINE=1  -o gills_inl_womemunl ${SRC_FILES}

gills_woinl_memunl: ${SRCS}
	${CC} --define GILLS_MEM_TKSTACK_UNLIM=1  -o gills_woinl_memunl ${SRC_FILES}

gills_inl_memunl: ${SRCS}
	${CC} --define REDUCE_ACTION_INLINE=1 --define GILLS_MEM_TKSTACK_UNLIM=1  -o gills_inl_memunl ${SRC_FILES}

.PHONY: testrun

testrun:
	@echo -e  "Running test without action inline and without memory no limit"
	@echo -e  "hit any key to continue"
	@read
	./gills_woinl_womemunl
	@echo -e  "Test done, hit any key to continue"
	@read
	@echo -e  "Running test with action inline and without memory no limit"
	@echo -e  "hit any key to continue"
	@read
	./gills_inl_womemunl
	@echo -e  "Test done, hit any key to continue"
	@read
	@echo -e  "Running test without action inline and with memory no limit"
	@echo -e  "hit any key to continue"
	@read
	./gills_woinl_memunl
	@echo -e  "Test done, hit any key to continue"
	@read
	@echo -e  "Running test with action inline and with memory no limit"
	@echo -e  "hit any key to continue"
	@read
	./gills_inl_memunl
	@echo -e  "Test done, hit any key to continue"
	@read

clean:
	rm -f ${LIBGILLS} ${TESTS}

