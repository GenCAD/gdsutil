all: $(PROG_NAME)

$(PROG_NAME): $(PROG_NAME).c $(PROG_INCS) $(PROG_OBJS)
	$(CC) $(CFLAGS) $(PROG_NAME).c $(PROG_OBJS) $(EXTR_LIBS) -o $(PROG_NAME)

update:
	$(CP) $(PROG_NAME) $(UTIL_BIN)/
	
.c.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(PROG_NAME)
