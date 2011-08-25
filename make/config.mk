BOOST_DIR?=/usr/include/boost

USE_AVR_HALIB=1

ifdef USE_INT_MANAGER
	CFLAGS:=${CFLAGS} -DUSE_INT_MANAGER=1
	CXXFLAGS:=${CXXFLAGS} -DUSE_INT_MANAGER=1
	LDFLAGS:=${LDFLAGS} -Wl,-T${HALIB}/ldscripts/InterruptManager/avr5.x
else
	CFLAGS:=${CFLAGS} -DUSE_INT_MANAGER=0
	CXXFLAGS:=${CXXFLAGS} -DUSE_INT_MANAGER=0
endif

AVR_CFLAGS= -mmcu=${CHIP} -DF_CPU=${CLOCK}ULL -D__NO_STL__ \
			-DBOOST_NO_STDLIB_CONFIG -fno-strict-aliasing \
			-fno-exceptions

CFLAGS:=${CFLAGS} $(filter-out ${CFLAGS}, ${AVR_CFLAGS})

CXXFLAGS:=${CXXFLAGS} $(filter-out ${CXXFLAGS}, ${AVR_CFLAGS}) -fno-rtti -fno-threadsafe-statics

ASMFLAGS+=${CFLAGS}
LDFLAGS+=-mmcu=${CHIP}
LIBS+=avr-halib-${CHIP}
LDPATHS+=${HALIB}/lib
OBJCPFLAGS+= -j .data -j .text
ARFLAGS=rus

INCLUDES += ${HALIB}/include \
			${HALIB}/experimental/include \
			${HALIB}/externals/include/ \
			${HALIB}/externals/include/boost/compatibility/cpp_c_headers/ \
			${LOGGING_DIR}
