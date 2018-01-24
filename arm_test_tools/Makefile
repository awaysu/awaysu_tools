
all: build_bonnie build_iperf build_cpuloop build_memtester build_zlib build_sparse
	@rm -Rf out
	@mkdir out/bin/ out/lib/ -p
	@cp -a bonnie++-1.03e/bonnie++ \
	cpuloop/cpuloop \
	iperf-2.0.5/src/iperf \
	memtester-4.3.0/memtester \
	sparse_utils/img2simg \
	sparse_utils/simg2img \
	sparse_utils/simg2simg \
	out/bin/
	@cp -a zlib-1.2.11/libz.so* out/lib/
	@echo "===================================================================================="
	@ls out/bin/ out/lib/
	@echo
	
build_bonnie:
	cd bonnie++-1.03e; \
	./configure --host=arm-linux CC=arm-linux-gcc ; \
	make

build_iperf:
	cd iperf-2.0.5; \
	./configure --host=arm-linux CC=arm-linux-gcc ; \
	make
	
build_cpuloop:
	cd cpuloop/; \
	make

build_memtester:
	cd memtester-4.3.0; \
	make
	
build_zlib:
	cd zlib-1.2.11 ;\
	CC=arm-linux-gcc ./configure ;\
	make
	
build_sparse:
	cd sparse_utils ;\
	make
	
clean:
	make -C bonnie++-1.03e clean
	make -C iperf-2.0.5 clean
	make -C cpuloop clean
	make -C memtester-4.3.0 clean
	make -C zlib-1.2.11 clean
	make -C sparse_utils clean
	rm -Rf out
