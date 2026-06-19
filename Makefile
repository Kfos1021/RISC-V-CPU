CXXFLAGS=-std=c++17

alu:
	verilator -Wall --cc rtl/core/alu.sv --exe sim/tb_alu.cpp -CFLAGS "$(CXXFLAGS)"
	make -C obj_dir -f Valu.mk Valu
	./obj_dir/Valu

regfile:
	verilator -Wall --cc rtl/core/regfile.sv --exe sim/tb_regfile.cpp -CFLAGS "$(CXXFLAGS)"
	make -C obj_dir -f Vregfile.mk Vregfile
	./obj_dir/Vregfile

clean:
	rm -rf obj_dir