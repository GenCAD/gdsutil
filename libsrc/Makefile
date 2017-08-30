.KEEP_STATE:

PACKAGES = libutil libgds

target: update

package update release clean:
	@for dir in $(PACKAGES); do \
	(cd $$dir; echo "*** Entering [$$dir]..."; make $@); \
	done;

$(PACKAGES):
	cd $@; make update;