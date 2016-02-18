#============================================================================
# Run: 
#    xtclsh voltmeter_ise.tcl  - creates XILINX ISE project file
#    ise voltmeter_project.ise - opens the project
#============================================================================
source "../../../../../base/xilinxise.tcl"

project_new "voltmeter_project"
project_set_props
puts "Adding source files"
xfile add "../../../../../fpga/units/clkgen/clkgen_config.vhd"
xfile add "voltmeter_config.vhd"
xfile add "../../../../../fpga/units/clkgen/clkgen.vhd"
xfile add "../../../../../fpga/units/math/math_pack.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc_entity.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_adc_autoincr.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_reg.vhd"
xfile add "../../../../../fpga/ctrls/spi/spi_ctrl.vhd"
xfile add "../../../../../fpga/chips/fpga_xc3s50.vhd"
xfile add "../../../../../fpga/chips/architecture_pc/arch_pc_ifc.vhd"
xfile add "../../../../../fpga/chips/architecture_pc/tlv_pc_ifc.vhd"
xfile add "../../../../../fpga/ctrls/vga/vga_config.vhd"
xfile add "../../../../../fpga/ctrls/vga/vga_ctrl.vhd"
xfile add "../../../../../fpga/ctrls/sdram/sdram_config.vhd"
xfile add "../../../../../fpga/ctrls/sdram/sdram_raw_ctrl.vhd"
xfile add "../../../../../fpga/ctrls/sdram/sdram_ctrl.vhd"
xfile add "../../../../../fpga/units/rasterizer/line/abssub.vhd"
xfile add "../../../../../fpga/units/rasterizer/line/rasterizer.vhd"
xfile add "../../../../../fpga/units/fifo/fifo36.vhd"
xfile add "../../fpga/top_level.vhd"
puts "Libraries"
project_set_top "fpga"
project_close
