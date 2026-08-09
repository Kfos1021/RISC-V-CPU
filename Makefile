RTL := \
	rtl/core/cpu.sv \
	rtl/core/pc.sv \
	rtl/core/imem.sv \
	rtl/core/decoder.sv \
	rtl/core/control.sv \
	rtl/core/imm_gen.sv \
	rtl/core/regfile.sv \
	rtl/core/alu.sv \
	rtl/core/dmem.sv

VERILATOR = verilator -Wall --cc

CXXFLAGS=-std=c++17

.PHONY: software compiled cpu clean alu regfile decoder control imm_gen pc imem dmem

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

dmem:
	verilator -Wall --cc rtl/core/dmem.sv --exe sim/tb_dmem.cpp -CFLAGS "$(CXXFLAGS)"
	make -C obj_dir -f Vdmem.mk Vdmem
	./obj_dir/Vdmem

cpu:
	$(VERILATOR) \
		$(RTL) \
		--exe sim/tb_cpu.cpp \
		-CFLAGS "$(CXXFLAGS)"

	make -C obj_dir -f Vcpu.mk Vcpu
	./obj_dir/Vcpu

compiled: software
	$(VERILATOR) \
		-GIMEM_FILE=\"programs/simple.hex\" \
		$(RTL) \
		--exe sim/tb_cpu_compiled.cpp \
		-CFLAGS "$(CXXFLAGS)"

	make -C obj_dir -f Vcpu.mk Vcpu
	./obj_dir/Vcpu

software:
	riscv64-unknown-elf-gcc \
		-march=rv32i \
		-mabi=ilp32 \
		-nostdlib \
		-nostartfiles \
		-ffreestanding \
		-O0 \
		-T software/linker.ld \
		software/start.S \
		software/simple.c \
		-o software/simple.elf

	riscv64-unknown-elf-objdump \
		-d software/simple.elf \
		> software/simple.dump

	riscv64-unknown-elf-objcopy \
		-O binary \
		software/simple.elf \
		software/simple.bin

	python3 software/bin_to_hex.py

clean:
	rm -rf obj_dir
