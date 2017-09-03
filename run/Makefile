

BINDIR  = ../bin

GDS2GDT = $(BINDIR)/gds2gdt 
GDT2GDX = $(BINDIR)/gdt2gdx
GDS2GDX = $(BINDIR)/gds2gdx

TEST_SUITE = TEST1 TEST2 TEST3

all : $(TEST_SUITE)


include ../etc/make/test.make

clean: clean_test_result 
	rm -fr output.* *.log *.gdt *.gdx TEST.*

#
# TEST1:
#
# muxdelay.gds => muxdelay.gdt
# muxdelay.gdt => muxdelay.gdx 
#
# compare muxdelay.gdx with muxdelay.gds
# 
TEST1.title  = GDS2GDT/GDT2GDX conversion
TEST1.target = muxdelay.gdx
TEST1.golden = muxdelay.gds

muxdelay.gdt: muxdelay.gds
	$(GDS2GDT) +verbose muxdelay.gds -logfile gds2gdt_muxdelay.log -o muxdelay.gdt

muxdelay.gdx: muxdelay.gdt 
	$(GDT2GDX) +verbose muxdelay.gdt -logfile gdt2gdx_muxdelay.log -o muxdelay.gdx

#
# TEST2:
#
# muxdelay.gdx => output.gdt
# compare output.gdt with muxdelay.gdt
#
TEST2.title  = GDT2GDX/GDX2GDT conversion
TEST2.target = output.gdt
TEST2.golden = muxdelay.gdt
TEST2.check  = diff $(TEST2.target) $(TEST2.golden)

output.gdt: muxdelay.gdx 
	$(GDS2GDT) +verbose muxdelay.gdx -logfile gds2gdt_output.log -o output.gdt  
	
#
# TEST3:
#
# muxdelay.gds => output.gdx
# compare output.gdx with muxdelay.gds
#
TEST3.title  = GDS2GDX conversion
TEST3.target = output.gdx
TEST3.golden = muxdelay.gds
TEST3.check  = cmp -l $(TEST3.target) $(TEST3.golden)

output.gdx: muxdelay.gds 
	$(GDS2GDX) +verbose muxdelay.gds -logfile gds2gdx_output.log -o output.gdx   

#
# TEST4:
#
# muxdelay.gds => layer.gdt
# compare output.gdx with muxdelay.gds
#
TEST4.title  = GDS2GDT with layer
TEST4.target = layer.gdt
TEST4.golden = muxdelay.gdt
TEST4.check  = diff $(TEST4.target) $(TEST4.golden)

layer.gdt: $(GD2GDT) muxdelay.gds
	$(GDS2GDT) -layer 3 muxdelay.gds -logfile gds2gdt_layer.log -o layer.gdt
	
