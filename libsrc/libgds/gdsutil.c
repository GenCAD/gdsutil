/*
 * $Id $
 *
 *
 */
#include "libutil.h"  
#include "gdsdata.h"
#include "gdsutil.h"

int  max_line_width      = 0;
int  gds_record_num      = 0;
int  gdt_line_num        = 0;
BOOL flag_msg_verbose    = FALSE;
BOOL flag_disp_line      = FALSE;
BOOL flag_disp_header	 = TRUE;
BOOL flag_disp_length	 = FALSE;
BOOL flag_disp_hier	 = FALSE;


int check_gds_header(gdsfp)
FILE *gdsfp;
{
  unsigned char header[GDSHEADERLENGTH];
  fseek(gdsfp,-fread(header,1,GDSHEADERLENGTH,gdsfp),1);
  return  ((GDSRECID(header)==GDS_HEADER) && (GDSRECTYPE(header)==GDS_I2));
}

int read_gds_record(gdsfp,gdsbuf,bufsize)
FILE *gdsfp;
unsigned char *gdsbuf;
int  bufsize;
{
    unsigned int len;
    if (fread(gdsbuf,1,GDSHEADERLENGTH,gdsfp)==GDSHEADERLENGTH) {
       gds_record_num++;	     
       len = GDSRECLEN(gdsbuf);
       if (len==0) {
           return 0;
       } else if (len<GDSHEADERLENGTH) {
           io_error("line #%04d, Record length(%d) is not correct around <%ld>\n",gds_record_num,len,ftell(gdsfp)); 
       } else if (len>bufsize) { 
           io_error("line #%04d, Record length(%d) is too large(>%d) around <%ld>\n",gds_record_num,len,bufsize,ftell(gdsfp)); 
       } else if (fread(gdsbuf+GDSHEADERLENGTH,1,len-GDSHEADERLENGTH,gdsfp)!=len-GDSHEADERLENGTH) {
           io_error("line #%04d, Data length(%d) is not correct around <%ld>\n",gds_record_num,len,ftell(gdsfp));
       } else {
           return len;
       }
    } else if (!feof(gdsfp)) {
       io_error("line #%04d, Data header is not correct around <%ld>\n",gds_record_num,ftell(gdsfp));
    }
    return -1;
}


long loop_stream_to_text(inpfp,outfp,proc_func)
FILE *inpfp;
FILE *outfp;
int (*proc_func)();
{
  long outbuf_size;
  int reclen,i,q;
  char *ptr;
  static unsigned char gdsinpbuf[MAX_GDSBUF_SIZE];
  static char *gdtoutbuf=NULL;

  while((reclen=read_gds_record(inpfp,gdsinpbuf,MAX_GDSBUF_SIZE))>0) {
     outbuf_size+=(*proc_func)(reclen,gdsinpbuf,&gdtoutbuf);
     switch(GDSRECID(gdsinpbuf)) {
     case GDS_STRNAME:
	if (flag_msg_verbose)
        io_printf("## %s (%ld)\n",gdsinpbuf+GDSHEADERLENGTH,ftell(inpfp));
	else
        fprintf(stderr,"\r%60s\r## %s (%ld)\r","",gdsinpbuf+GDSHEADERLENGTH,ftell(inpfp));
        break;
     }
     if (flag_disp_line) fprintf(outfp,"[#%04d] ",gds_record_num);
     if (flag_disp_length) fprintf(outfp,"[%03d] ",reclen);

  if ((max_line_width <= 0) || (strlen(gdtoutbuf) < max_line_width)) {
     fprintf(outfp,"%s\n",gdtoutbuf);
  } else {
     for(i=q=0,ptr=gdtoutbuf; *ptr ; ptr++,i++) {
       if (*ptr=='\t') *ptr=' ';
       fputc(*ptr,outfp);
       if ((i>max_line_width) && strchr(" ,;",*ptr)) {
          if (q) { i=0; fprintf(outfp,"\\\n");} 
          else   { i=8; fprintf(outfp,"\\\n\t"); }
       } 
       if (*ptr=='\"') q=~q;
     }
     fputc('\n',outfp);
  }
  

  }
  return outbuf_size;
}


unsigned int proc_stream_to_text(len,gdsbuf,outbufptr)
unsigned int len;
unsigned char *gdsbuf;
char **outbufptr;
{
   unsigned int  i,j;
   unsigned int  size;
   unsigned int  gdsid;
   unsigned int  gdstype;
   GDS_DEF gdsdef;

   gdsid   = GDSRECID(gdsbuf);
   gdstype = GDSRECTYPE(gdsbuf);
   gdsdef  = gds_def_array + gdsid;
   
   copy_str(outbufptr,gdsdef->name);
   size=(gdsdef->size)?abs(gdsdef->size):gds_size[gdstype];
   if (size>0) {
      for(j=0, i=GDSHEADERLENGTH; i<len; j++,i+=size) {
        if (size>len-i) size=len-i;
        append_str(outbufptr,decode_gds_data(gdstype,gdsbuf+i,size));
      }
      if (gdsdef->num<0) {
        if (j>abs(gdsdef->num))
           io_error("line #%04d, Data number(%d) of '%s' exceeds limitation(%d)!\n", gds_record_num,
             len/size,gdsdef->name,gdsdef->num);
      } else {
        if (j!=gdsdef->num)
           io_error("line #%04d, Data number(%d) of '%s' does not match(%d)!\n", gds_record_num,
             len/size,gdsdef->name,gdsdef->num);
      }
   }
   return strlen(*outbufptr);
}


int translate_gds_to_gdt(gdsin,gdtout)
char *gdsin;
char *gdtout;
{
  FILE *gdsinfp;
  FILE *gdtoutfp;
  char *buffer,*ptr;
  HASH_ENTRY hash_ele;
  
  io_printf("##========================================\n");
  if ((gdsin==0)||(*gdsin==0)) {
     gdsinfp=stdin;
     io_printf("## INPUT GDS FILE  : <stdin>\n");
  } else if ((gdsinfp=(FILE*)fopen64(gdsin,"rb"))==NULL) {
     io_error("Can not open gds file '%s'\n",gdsin);
     return 0;
  } else {
     io_printf("## INPUT GDS FILE  : %s\n",gdsin);
  }
  if (!check_gds_header(gdsinfp)) {
     io_error("'%s' is not gds format file\n",gdsin);
     fclose(gdsinfp);
     return 0;
  }
  if ((gdtout==0)||(*gdtout==0)) {
     gdtoutfp=stdout;
  } else if ((gdtoutfp=(FILE*)fopen64(gdtout,"w"))==NULL) {
     io_error("Can not output gdt file to '%s'\n",gdtout);
     fclose(gdsinfp);
     return 0;
  } else {
     io_printf("## OUTPUT GDT FILE : %s\n",gdtout);
  }
  gds_record_num = 0;
  io_printf("##----------------------------------------\n");
  loop_stream_to_text(gdsinfp,gdtoutfp,proc_stream_to_text);
  
  fclose(gdsinfp);
  fclose(gdtoutfp);

  return gds_record_num;
}
 


int proc_text_to_stream(len,inpbuf,outbuf)
int len;
char *inpbuf;
char *outbuf;
{
  GDS_DEF gdsdef;
  char *ptr,*token;
  char *keyword;
  int buflen;
  
  buflen=0;
  outbuf[0]=outbuf[1]=0;
  for(ptr=inpbuf;isspace(*ptr);ptr++);
  keyword=strtok(ptr," \t\n");
  while(keyword && (*keyword=='[')) {
	  keyword=strtok(NULL," \t\n");
  }
  if (keyword==NULL) return 0;
  for(gdsdef=gds_def_array; *gdsdef->name; gdsdef++) {
     if (strcasecmp(keyword,gdsdef->name)==0) {
        buflen=GDSHEADERLENGTH;
        if (gdsdef->size) {
           while(token=strtok(NULL," \t\n")) {
             buflen+=encode_gds_data(gdsdef->type,token,outbuf+buflen,gdsdef->size);
           }
        }
        outbuf[0]=buflen / 256;
        outbuf[1]=buflen % 256;
        outbuf[2]=gdsdef->id;
        outbuf[3]=gdsdef->type;
        return buflen;
     }
  }
  io_warning("Unknown keyword: %s\n",keyword);
  return buflen;
}

long loop_text_to_stream(gdtinfp,gdsoutfp,proc_func)
FILE *gdtinfp;
FILE *gdsoutfp;
int (*proc_func)();
{
  char *Buffer;
  char gds_buffer[MAX_GDSBUF_SIZE];
  long gds_buflen;
  long gds_len=0;

  while(!feof(gdtinfp)) {
    Buffer=read_lines(gdtinfp,MATCH_MULTIPLE,"\\");
    gdt_line_num++;
    if (Buffer && (*Buffer!='#') &&(*Buffer!='*')) { 
       gds_buflen=proc_func(strlen(Buffer),Buffer,gds_buffer);
     switch(GDSRECID(gds_buffer)) {
     case GDS_STRNAME:
	if (flag_msg_verbose)
        io_printf("## %s (%ld)\n",gds_buffer+GDSHEADERLENGTH,ftell(gdtinfp));
	else
        fprintf(stderr,"\r%60s\r## %s (%ld)\r","",gds_buffer+GDSHEADERLENGTH,ftell(gdtinfp));
        break;
     }
       fwrite(gds_buffer,gds_buflen,1,gdsoutfp);
       gds_len += gds_buflen;
    }
  }
  return gds_len;
}


int translate_gdt_to_gds(gdtin,gdsout)
char *gdtin;
char *gdsout;
{
  FILE *gdtinfp;
  FILE *gdsoutfp;
  char *buffer,*ptr;
  long gds_len;
  
  io_printf("##========================================\n");
  if ((gdtin==NULL)||(*gdtin==0)) {
     gdtinfp=stdin;
  } else if ((gdtinfp=(FILE*)fopen64(gdtin,"r"))==NULL) {
     io_error("Can not open gds file '%s'\n",gdtin);
     return 0;
  } else {
     io_printf("## INPUT GDT FILE  : %s\n",gdtin);
  }
  if ((gdsout==NULL)||(*gdsout==0)) {
     gdsoutfp=stdout;
  } else if ((gdsoutfp=(FILE*)fopen64(gdsout,"wb"))==NULL) {
     io_error("Can not output gdt file to '%s'\n",gdsout);
     fclose(gdtinfp);
     return 0;
  } else {
     io_printf("## OUTPUT GDS FILE : %s\n",gdsout);
  }
  gds_len = loop_text_to_stream(gdtinfp,gdsoutfp,proc_text_to_stream);
  gds_len = 2048 - (gds_len % 2048);
/*  if (gds_len>0) { fwrite("",1,gds_len,gdsoutfp); } */
  fclose(gdtinfp);
  fclose(gdsoutfp);
  return 1;
}


