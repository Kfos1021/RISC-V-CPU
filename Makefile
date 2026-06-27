CXXFLAGS=-std=c++17

alu:
	verilator -Wall --cc rtl/core/alu.sv --exe sim/tb_alu.cpp -CFLAGS "$(CXXFLAGS)"
	make -C obj_dir -f Valu.mk Valu
	./obj_dir/Valu

regfile:
	verilator -Wall --cc rtl/core/regfile.sv --exe sim/tb_regfile.cpp -CFLAGS "$(CXXFLAGS)"
	make -C obj_dir -f Vregfile.mk Vregfile
	./obj_dir/Vregfile

decoder:
	verilator -Wall --cc rtl/core/decoder.sv --exe sim/tb_decoder.cpp -CFLAGS "$(CXXFLAGS)"
	make -C obj_dir -f Vdecoder.mk Vdecoder
	./obj_dir/Vdecoder

control:
	verilator --Wall --cc rtl/core/control.sv \
		--exe sim/tb_control.cpp \
		-CFLAGS "-std=c++17"

	make -C obj_dir -f Vcontrol.mk Vcontrol
	./obj_dir/Vcontrol

imm_gen:
	verilator -Wall --cc rtl/core/imm_gen.sv --exe sim/tb_imm_gen.cpp -CFLAGS "$(CXXFLAGS)"
	make -C obj_dir -f Vimm_gen.mk Vimm_gen
	./obj_dir/Vimm_gen

pc:
	verilator -Wall --cc rtl/core/pc.sv --exe sim/tb_pc.cpp -CFLAGS "$(CXXFLAGS)"
	make -C obj_dir -f Vpc.mk Vpc
	./obj_dir/Vpc

imem:
	verilator -Wall --cc rtl/core/imem.sv --exe sim/tb_imem.cpp -CFLAGS "$(CXXFLAGS)"
	make -C obj_dir -f Vimem.mk Vimem
	./obj_dir/Vimem

clean:
	rm -rf obj_dir
