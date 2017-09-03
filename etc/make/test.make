#
#
# <parameter >  
#  TEST.target = <target data file>
#  TEST.golden = <golden data file>
#  TEST.check  = <compare script>
#
# <input file>
#  golden/TEST.golden : golden output file
#  golden/TEST.check  : golden TEST.check result (optional) 
#
# <output file>
#  TEST.runlog : output log file at executing the test command
#
#  TEST.check  : output of comparing run data with golden data
#  TEST.diff   : comparison result of TEST.check and TEST.waive
#  TEST.status : 
#        "PASS" if TEST.diff is empty
#        "FAIL" if TEST.diff is not empty

# <result>
#  TEST.PASS   : 
#  TEST.FAIL   :
#
TIME_STAMP = `date +%F/s%T`

$(TEST_SUITE):
	@rm -fr $@.runlog $@.check $@.diff $@.status $@.PASS $@.FAIL
	@echo "##########################################################"
	@echo "$@.title  : $($@.title)"
	@echo "$@.target : make $($@.target)"
	@echo "$(TIME_STAMP) RUNING" >> $@.status
	@make $($@.target) > $@.runlog
	@echo "$(TIME_STAMP) CHECKING" >> $@.status
	@echo "$@.check  : $($@.check)"
	@echo `$($@.check) 2>&1 ` > $@.check 
	@echo "$(TIME_STAMP) DIFF" >> $@.status
	@echo "" > $@.waive
	if (test -s golden/$@.check) then \
	   diff $@.check golden/$@.check > $@.diff; \
	else \
	   diff $@.check $@.waive > $@.diff ; \
	fi
	echo "$(TIME_STAMP) DONE" >> $@.status
	if (test -s $@.diff) then \
	  { echo "$(TIME_STAMP) FAIL" >> $@.status ;\
	   echo "$@.time   : $(TIME_STAMP)" ;\
	   echo "$@.status : FAIL" ;\
	   touch $@.FAIL; } \
	else \
	 {  echo "$(TIME_STAMP) PASS" >> $@.status ;\
	   echo "$@.time   : $(TIME_STAMP)" ;\
	   echo "$@.status : PASS" ;\
	   touch $@.PASS; } \
	fi

reset_test_status:
	@for test in $(TEST_SUITE); do \
	( echo "$$test"; rm -f $$test.* ); \
	done;
