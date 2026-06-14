TOP=alu

all:
	verilator -Wall --cc rtl/core/alu.sv --exe sim/tb_alu.cpp -CFLAGS "-std=c++17"
	make -C obj_dir -f V$(TOP).mk V$(TOP)
	./obj_dir/V$(TOP)

clean:
	rm -rf obj_dir