include makeconf.mk
APPLYED_TARGETS=$(addprefix $(APPLYED_DIR)/,$(MANAGED_EXTERNALS))

LISTOFAPPLYED=$(wildcard $(APPLYED_DIR)/*)

help:
	@echo "use make getall to get all managed externals"
	@echo
	@echo "if you want to use an other (local) version please provide a symlink to that version"
	@echo "and remove its name from the MANAGED_EXTERNALS-List in makeconf.mk"
	@echo
	@echo "at the moment " $(MANAGED_EXTERNALS) " are managed by this Makefile"

getall:$(addprefix checkremove,$(LISTOFAPPLYED)) $(APPLYED_TARGETS)
	@echo "Checked" $(subst $(APPLYED_DIR)/,,$?)

logging-cpp: logging-cpp-svn
	ln -s -f $< $@

logging-cpp-svn:
	rm -rf $@.part~
	svn co https://logging-cpp.svn.sourceforge.net/svnroot/logging-cpp logging-cpp-svn.part~
	mv $@.part~ $@

$(addprefix checkremove,$(LISTOFAPPLYED)):
	test -e $(subst checkremove$(APPLYED_DIR)/,,$@) || rm -f $(subst checkremove,,$@)

$(APPLYED_DIR)/%:
	@echo =========== Getting $? ===========
	rm -rf $(subst $(APPLYED_DIR)/,,$@)
	make $(subst $(APPLYED_DIR)/,,$@)
	make $(APPLYED_DIR)
	touch $@

$(APPLYED_DIR):
	mkdir $(APPLYED_DIR)
