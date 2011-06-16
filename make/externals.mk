EXT_DIR=./externals
BOOST_LINK=${EXT_DIR}/include/boost

.PHONY: externals

externals: boost

boost:
	@mkdir -p $(dir ${BOOST_LINK})
	@ln -Tfs ${BOOST_DIR} ${BOOST_LINK}

externals_clean:
	@rm -rf ${EXT_DIR}
