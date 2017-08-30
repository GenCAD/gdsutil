#ifndef __GDSUTIL_H__
#define __GDSUTIL_H__

#define fopen64 fopen

extern int  max_line_width;
extern BOOL flag_disp_length;
extern BOOL flag_disp_all;
extern BOOL flag_disp_header;
extern BOOL flag_disp_hier;
extern BOOL flag_disp_line;
extern BOOL flag_msg_verbose;

int check_gds_header();
int read_gds_record();
long loop_stream_to_text();
long loop_text_to_stream();
unsigned int proc_strem_to_text();
int proc_text_to_stream();

int translate_gds_to_gdt();
int translate_gdt_to_gds();

#endif
