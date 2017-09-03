# GenCAD GDS UTILITY

## Purpose:
The main purpose of GenCAD GDS utility is to convert binary GDS file 
to a readable text format(GDT), so user can validate the design content,
modify or manipulate the content using simple script like tcl or perl 
without decoding the GDS binary format.

## How to build the executable

1. Compile Utility Library

  * libutil - general function route, such hash, list, link ....
  * libgds  - GDS reader/writter
	
	% cd libsrc/
	
	% make clean
	
	% make update


2. Compile Main Program

  * gds2gdt - convert GDS (binary format) to GDT (text format)
  * gdt2gdx - convert GDT (text format) to GDX (binary format)
  * gds2gdx - convert GDS (binary format) to GDX (binary format)

	% cd main/
	
	% make clean
	
	% make publish


3. Test Prgoram

	% cd run/
	
	% make TEST1
	


