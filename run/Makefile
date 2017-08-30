BINDIR  = ../bin
GDS2GDT = $(BINDIR)/gds2gdt 
GDT2GDX = $(BINDIR)/gdt2gdx
GDS2GDX = $(BINDIR)/gds2gdx
SHOWGDS = $(BINDIR)/showgds

TESTCASE = ../data/muxdelay.gds

#
# muxdelay.gds => muxdelay.gdt
# muxdelay.gdt => muxdelay.gdx  (diff with muxdelay.gds)
# muxdelay.gdx => output.gdt    (diff with muxdelay.gdt)
# 

all : test1 test2

muxdelay.gdt: $(GDS2GDT)
	$(GDS2GDT) +verbose $(TESTCASE) -logfile muxdelay_gds2gdt.log  -destination . 

muxdelay.gdx: muxdelay.gdt $(GDT2GDX)
	$(GDT2GDX) +verbose muxdelay.gdt -logfile muxdelay_gdt2gdx.log

test1: $(TESTCASE) muxdelay.gdx
	cmp	$(TESTCASE) muxdelay.gdx


output.gdt: muxdelay.gdx $(GDS2GDT)
	$(GDS2GDT) +verbose muxdelay.gdx -gdtfile output.gdt  -logfile output_gds2gdt.log
	
test2: muxdelay.gdt output.gdt
	diff	muxdelay.gdt output.gdt

output.gdx: $(TESTCASE) $(GDS2GDX)
	$(GDS2GDX) +verbose $(TESTCASE) -gdxfile output.gdx   -logfile output_gds2gdx.log
	
test3: output.gdx
	cmp	$(TESTCASE) output.gdx
	
#
# muxdelay.gds => new2.gds (diff with muxdelay.gds)
# new2.gds     => new2.gdt (diff with muxdelay.gdt)
 
new.gdt: muxdelay.gdx
	$(SHOWGDS) +verbose muxdelay.gdx  > new.gdt -logfile output_showgds.log

clean:
	rm -fr muxdelay.* output.* *.log *.gdt *.gdx
