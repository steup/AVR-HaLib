LIB_DIR     := ${HALIB_DIR}/lib/${TARGET}
LIB_BUILD   := ${HALIB_DIR}/build/${TARGET}
LIB_NAME    := ${LIB_DIR}/libavr-halib.a
LIB_SRC_DIR := ${HALIB_DIR}/src
LIB_SRCS    := $(wildcard ${LIB_SRC_DIR}/*.[S,c,cpp])
LIB_OBJECTS := $(foreach src, ${LIB_SRCS}, ${LIB_BUILD}/$(basename $(notdir ${src})).o)
GENDIRS     += ${LIB_DIR} ${LIB_BUILD}
TO_CLEAN    += ${LIB_DIR} ${LIB_BUILD}
TO_DCLEAN   += ${HALIB_DIR}/lib ${HALIB_DIR}/build

.PHONY: lib

${LIB_BUILD}/%.o: ${LIB_SRC_DIR}/%.S | ${LIB_BUILD}
	@echo "(AS   ) $@ <- $(notdir $<)"
	@${ASM} -c ${ASMFLAGS} -o $@ $<

${LIB_BUILD}/%.o: ${LIB_SRC_DIR}/%.c | ${LIB_BUILD}
	@echo "(CC   ) $@ <- $(notdir $<)"
	@${CC} -c ${CFLAGS} -o $@ $<

${LIB_BUILD}/%.o: ${LIB_SRC_DIR}/%.cpp | ${LIB_BUILD}
	@echo "(CXX   ) $@ <- $(notdir $<)"
	@${CXX} -c ${CXXFLAGS} -o $@ $<

${LIB_NAME}: ${LIB_OBJECTS} | ${LIB_DIR}
	@echo "(AR    ) $@ <- $(notdir ${LIB_OBJECTS})"
	@$(AR) ${ARFLAGS} $@ ${LIB_OBJECTS} > /dev/null

lib: ${LIB_NAME}
