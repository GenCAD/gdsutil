/*
 *  Convert GDS to GDT (Text format)
 *  --------------------------------
 *  Author: Albert Li
 *  Date  : 2017/09/01
 *
 */
#include "showgds.h"

static const char* BANNER="\
#############################################################\n\
##               Layout GDS Reader (SHOWGDS)               ##\n\
##                 Version: %-10s                     ##\n\
#############################################################\n\
";

static char* HELP_DOC="\
Usage: %s [options] <gdsfile:cell> ...\n\
	+verbose	display detail message\n\
\n";


int main(argc,argv)
int argc;
char *argv[];
{
  int  i,j;
  char *opt,*ptr,*arg;
  char *cell,*file;
  char logfile[MAX_FILENAME_LEN]=SYSLOG;
  char gdsin[MAX_FILENAME_LEN];
  char gdtout[MAX_FILENAME_LEN];
  char destdir[MAX_FILENAME_LEN];
  static char *destination = NULL;
  static char *filelist    = NULL;
  
  BOOL flag_search_all    = TRUE;
  BOOL flag_stdout   = FALSE;
  BOOL flag_stdin    = FALSE;
  
  if (argc<2) { 
     printf(BANNER,VERSION);
     printf(HELP_DOC,PROGRAM);
     return -1;
  }
  i=1;
  while(i<argc) {
     opt=argv[i++];
     if (strncasecmp(opt,"-logfile",strlen(opt))==0) {
       if (((arg=argv[i++])==NULL)||(i>argc)) {
          io_error("Too few argument for '%s'\n",opt);
          arg="";
       } else {
          strcpy(logfile,arg);
       }
     }
  }
  set_syslog_file(SYSLOG);
  set_sysout_fp(stdout);  
  open_syslog_file();
  io_printf(BANNER,VERSION);
  io_printf("## DATE      : %s\n",get_time());
  copy_str(&filelist,NULL);
  i=1;
  io_printf("## COMMAND   : %s\n",argv[0]);
  while(i<argc) {
     opt=argv[i++];
     if (*opt=='+') {
       io_printf("##               %s\n",opt);
       opt++;
       if (strcasecmp(opt,"verbose")==0) {
          flag_msg_verbose = TRUE;
       } else {
          io_warning("Unknown options '+%s'\n",opt);
       }
     } else if (*opt=='-') {
       if (((arg=argv[i++])==NULL)||(i>argc)) {
          io_error("Too few argument for '%s'\n",opt);
          arg="";
       }
       io_printf("##               %s %s\n",opt,arg);
       opt++;
       if (strncasecmp(opt,"logfile",strlen(opt))==0) {
          strcpy(logfile,arg);
       } else if (strncasecmp(opt,"destination",strlen(opt))==0) {
          copy_str(&destination,arg);
       } else if (strncasecmp(opt,"linewidth",strlen(opt))==0) {
          flag_disp_line=FALSE;
          sscanf(arg,"%d",&max_line_width);
       } else {
          io_error("Unknown options '-%s'\n",opt);
       }
     } else {
       string_enqueue_sep(filelist,opt,",");
     }
  }
  if (destination) {
     io_printf("## OUTPUT    : %s\n",destination);
     if ((fclose(fopen(destination,"r"))!=0) && (mkdir(destination,0755)!=0)) {
        io_error("Can not create directory '%s'.\n",destination);
     }
  }
  if (io_error_count()==0) {
    if (filelist) {
       string_for_all_sep(filelist,opt,","){
         file=strtok(opt,":");
         cell=strtok(NULL,":");
         strcpy(gdsin,file);
         while(ptr=strchr(file,'/')) file=ptr+1;
         if (destination==NULL) {
            strcpy(destdir,"");
         } else {
            sprintf(destdir,"%s/",destination);
         }
         if (cell==NULL) cell=strtok(file,".");
         sprintf(gdtout,"%s%s.gdt",destdir,cell);
         translate_gds_to_gdt(gdsin,(flag_stdout)?0:destination?gdtout:0);
       } string_for_all_end;
    } else {
      translate_gds_to_gdt(0,0);
    }
    io_printf("##========================================\n");
  }
  io_summary();
  close_syslog_file();
  return 0;
}


