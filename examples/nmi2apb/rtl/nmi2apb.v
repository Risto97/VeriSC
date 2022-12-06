`timescale 1ns/1ps

module nmi2apb (
    input   wire clk_i,
    input   wire rst_ni,
    
    // PicoRV32 Native Memory Interface
    input   wire           mem_valid_i,
	output  wire           mem_ready_o,
	input   wire [31:0]    mem_addr_i,
	input   wire [31:0]    mem_wdata_i,
	input   wire [ 3:0]    mem_wstrb_i,
	output  wire [31:0]    mem_rdata_o,

    // APB master port
    output  wire           psel_o,
    output  wire           penable_o,
    output  wire           pwrite_o,
    input   wire           pready_i,
    output  wire [31:0]    paddr_o,    
    output  wire [31:0]    pwdata_o,    
    output  wire [ 3:0]    pstrb_o,    
    input   wire [31:0]    prdata_i    
);

    reg psel_del;

    // 1-cycle delay
    always @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) psel_del <= 1'b0;
        else psel_del <= psel_o;
    end

    assign psel_o      = mem_valid_i;
    assign penable_o   = psel_o & psel_del;
    assign mem_ready_o = psel_o & penable_o & pready_i;

    // These are driven to 0 if PSEL is not active only as a recommandation, it's not really necessary
    assign pwrite_o    = psel_o ? |mem_wstrb_i : 1'b0;
    assign paddr_o     = psel_o ? mem_addr_i : {32{1'b0}};
    assign pstrb_o     = psel_o ? mem_wstrb_i : {4{1'b0}};
    assign pwdata_o    = psel_o ? mem_wdata_i : {32{1'b0}};
    assign mem_rdata_o = psel_o ? prdata_i : {32{1'b0}};
    
endmodule
