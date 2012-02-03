.PHONY: clean distclean

clean:
	@echo "C L E A N"
	@rm -rf ${TO_CLEAN}

distclean:
	@echo "D I S T C L E A N"
	@rm -rf ${TO_DCLEAN}
