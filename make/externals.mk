BOOST_DIR    ?= /usr/include/boost
EXT_DIR      := ${HALIB_DIR}/externals
EXT_INC      := ${EXT_DIR}/include
BOOST_LINK   := ${EXT_INC}/boost
LOGGING_LINK := ${EXT_INC}/logging
GENDIRS      += ${EXT_INC}
TO_DCLEAN    += ${EXT_DIR}

BOOST_CFLAGS := -D__NO_STL__ \
			    -DBOOST_NO_STDLIB_CONFIG \
				-DBOOST_CHRONO_MAC_API \
				-DBOOST_ALL_NO_LIB \
				-DBOOST_NO_CWCHAR \
				-DBOOST_NO_INTRINSIC_WCHAR_T \
				-DBOOST_NO_LIMITS \
				-D_LITTLE_ENDIAN

AVR_CFLAGS   += ${BOOST_CFLAGS}
AVR_CXXFLAGS += ${BOOST_CFLAGS}
INCLUDES     += ${EXT_INC} \
			    ${BOOST_LINK}/compatibility/cpp_c_headers/ \
				${HALIB_DIR}/include/avr_boost_compat

.PHONY: externals boost logging

externals: boost logging

boost: | ${EXT_INC}
	${LINK} -Tfs ${BOOST_DIR} ${BOOST_LINK}

logging: | ${EXT_INC}
	${LINK} -Tfs ${LOGGING_DIR}/include/logging ${LOGGING_LINK}
