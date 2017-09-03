HEADER_H = HEADER.h
PKGS_LIB = $(PKG_NAME).a
PKGS_INC = $(PKG_NAME).h

package: depend $(OUT_DIR)/$(PKGS_LIB) $(OUT_DIR)/$(PKGS_INC)

depend:
	@for dir in $(PKG_DEPS); do \
	(cd ../$$dir; echo "*** Entering [$$dir]..."; make package); \
	done;

$(HEADER_H): $(UTIL_HEAD) $(UTIL_OBJS)
	@echo "/*" > $(HEADER_H)
	@echo " *">> $(HEADER_H)
	@echo " * Package : GenCAD Utility Header" >> $(HEADER_H)
	@echo " * Date    : `date +%D`" >> $(HEADER_H)
	@echo " *">> $(HEADER_H)
	@echo " */" >> $(HEADER_H)
	$(CAT) $(UTIL_HEAD) >> $(HEADER_H)

$(OUT_DIR)/$(PKGS_INC): $(HEADER_H) $(UTIL_INCS)
	$(MKDIR) $(OUT_DIR)
	$(CAT) $(HEADER_H) $(UTIL_INCS) > $(OUT_DIR)/$(PKGS_INC)

$(OUT_DIR)/$(PKGS_LIB): $(UTIL_OBJS)
	$(MKDIR) $(OUT_DIR)
	$(AR) $(OUT_DIR)/$(PKGS_LIB) $(UTIL_OBJS)
	$(RANLIB) $(OUT_DIR)/$(PKGS_LIB)

compile: $(UTIL_OBJS)

update: package
	$(CP) $(OUT_DIR)/$(PKGS_LIB) $(LIBS_DIR)
	$(CP) $(OUT_DIR)/$(PKGS_INC) $(LIBS_INC)

clean:
	rm -f $(HEADER_H) core *.o *.log *~ 


