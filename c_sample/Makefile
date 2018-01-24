CC = gcc
ROOTDIR = $(shell pwd)
CFLAG = -Wall
LIB_CFLAG = -DMAKE_LIBRARY_SAMPLE -shared -Wl,-soname,
INCLUDE = -I$(ROOTDIR)/inc
SRC_PATH = $(ROOTDIR)/src/
APP_PATH = $(ROOTDIR)/bin/
OBJ_PATH = $(ROOTDIR)/obj/
LIBNAME = $(ROOTDIR)/bin/libsample.so
LIBEXE = $(ROOTDIR)/bin/libsample
LIBEXE_SRC = $(ROOTDIR)/src/lib_sample.c

EXEC_FILE = copyfile_sample \
			fileexist_sample \
			fork_sample \
			load_file_sample \
			save_file_sample \
			config_sample \
			check_program_sample \
			strtonum_sample \
			get_mount_info_sample \
			get_file_list_sample \
			time_sample \
			get_mem_info_sample \
			signal_sample \
			debug_sample \
			parameter_sample \
			network_ip_mac_sample \
			load_line_sample \
			host_to_ip_sample \
			igmpv3_sample \
			errno_sample \
			analysis_sample \
			mtdinfo_sample \
			get_process_id_sample \
			download_html_sample \
			network_connect_sample \
			socket_server_sample \
			socket_client_sample

main : clean
	@mkdir $(APP_PATH)
	@for f in $(EXEC_FILE); do \
		echo "[Compile... $$f]"  ; \
		$(CC) -o $(APP_PATH)$$f  $(CFLAG) $(INCLUDE) $(SRC_PATH)$$f.c  ; \
	done	
	
lib: clean
	@mkdir $(APP_PATH)
	@mkdir $(OBJ_PATH)
	@for f in $(EXEC_FILE); do \
		cd $(OBJ_PATH); \
		echo "[Compile... $$f]"  ; \
		$(CC) $(INCLUDE) $(LIB_CFLAG) -c $(SRC_PATH)$$f.c  ; \
	done
	@$(CC) $(LIB_CFLAG) -o $(LIBNAME) $(OBJ_PATH)/*.o
	@$(CC) -o $(LIBEXE) $(CFLAG) $(INCLUDE) $(LIBNAME) $(LIBEXE_SRC)

clean:
	@rm $(APP_PATH) $(OBJ_PATH) -Rf