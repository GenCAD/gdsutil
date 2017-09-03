#ifndef __GDSMAIN_H__
#define __GDSMAIN_H__

#define fopen64 fopen

extern int  max_line_width;
extern BOOL flag_disp_length;
extern BOOL flag_disp_all;
extern BOOL flag_disp_header;
extern BOOL flag_disp_hier;
extern BOOL flag_disp_line;
extern BOOL flag_msg_verbose;

int translate_gds_to_gdt();
int translate_gdt_to_gds();
int translate_gds_to_gdx();

#endif
