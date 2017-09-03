# GenCAD GDS UTILITY

## Purpose:
The main purpose of GenCAD GDS utility is to convert binary GDS file 
to a readable text format(GDT), so user can validate the design content,
modify or manipulate the content using simple script like tcl or perl 
without decoding the GDS binary format.

## How to build the executable

1. Compile Utility Library

  * libutil
  * libgds

	% cd libsrc
	% make clean
	% make update


2. Compile Main Program

  * gds2gdt
  * gdt2gdx
  * gds2gdx

	% cd main
	% make clean
	% make update


3. Test Prgoram

	% cd run
	% make test1
	% make test2

