#pragma once
#include <cstdint>
#include "mips32r6_encoding.h"
#include "mips32r6_alu.h"
#include "mips_pipeline.h"
#include "mips_os.h"
#include "mips_registers.h"

void execute_instruction(){
    switch(EXE.op_code){
    case 0b000000: execute_special();
    case 0b000001: execute_regimm(); break;
    case 0b000010: j(); break;
    case 0b000011: jal(); break;
    case 0b000100: beq(); break;
    case 0b000101: bne(); break;
    case 0b000110: execute_pop06(); break;
    case 0b000111: execute_pop07(); break;

    case 0b001000: execute_pop10(); break;
    case 0b001001: addiu(); break;
    case 0b001010: slti(); break;
    case 0b001011: sltiu(); break;
    case 0b001100: andi(); break;
    case 0b001101: ori(); break;
    case 0b001110: xori(); break;
    case 0b001111: aui(); break;

    case 0b010000: execute_cop0(); break;
    case 0b010001: execute_cop1(); break;
    case 0b010010: execute_cop2(); break;
    case 0b010011: removed_r6(); break;
    case 0b010100: removed_r6(); break;
    case 0b010101: removed_r6(); break;
    case 0b010110: execute_pop26(); break;
    case 0b010111: execute_pop27(); break;

    case 0b011000: execute_pop30(); break;
    case 0b011001: reserved_B(); break;
    case 0b011010: reserved_B(); break;
    case 0b011011: reserved_B(); break;
    case 0b011100: reserved_star(); break;
    case 0b011101: reserved_no_module(); break;
    case 0b011110: reserved_no_module(); break;
    case 0b011111: execute_special3(); break;

    case 0b100000: lb(); break;
    case 0b100001: lh(); break;
    case 0b100010: removed_r6(); break;
    case 0b100011: lw(); break;
    case 0b100100: lbu(); break;
    case 0b100101: lhu(); break;
    case 0b100110: removed_r6(); break;
    case 0b100111: reserved_B(); break;

    case 0b101000: sb(); break;
    case 0b101001: sh(); break;
    case 0b101010: removed_r6(); break;
    case 0b101011: sw(); break;
    case 0b101100: reserved_B(); break;
    case 0b101101: reserved_B(); break;
    case 0b101110: removed_r6(); break;
    case 0b101111: removed_r6(); break;

    case 0b110000: removed_r6(); break;
    case 0b110001: reserved_no_module(); break;
    case 0b110010: bc(); break;
    case 0b110011: removed_r6(); break;
    case 0b110100: reserved_B(); break;
    case 0b110101: reserved_no_module(); break;
    case 0b110110: execute_pop66(); break;
    case 0b110111: reserved_B(); break;

    case 0b111000: removed_r6(); break;
    case 0b111001: reserved_no_module(); break;
    case 0b111010: balc(); break;
    case 0b111011: execute_pcrel(); break;
    case 0b111100: reserved_B(); break;
    case 0b111101: reserved_no_module(); break;
    case 0b111110: execute_pop76(); break;
    case 0b111111: reserved_B(); break;
    }
}

void execute_special(){
    switch(EXE.funct){
    case 0b000000: sll(); break;
    case 0b001000: removed_r6(); break;
    case 0b010000: clz(); break;
    case 0b011000: sop30(); break;
    case 0b100000: add(); break;
    case 0b101000: reserved_star(); break;
    case 0b110000: tge(); break;
    case 0b111000: reserved_B(); break;
    
    case 0b000001: removed_r6(); break;
    case 0b001001: jalr(); break;
    case 0b010001: clo(); break;
    case 0b011001: sop31(); break;
    case 0b100001: addu(); break;
    case 0b101001: reserved_star(); break;
    case 0b110001: tgeu(); break;
    case 0b111001: reserved_star(); break;
    
    case 0b000010: execute_srl(); break;
    case 0b001010: removed_r6(); break;
    case 0b010010: removed_r6(); break;
    case 0b011010: sop32(); break;
    case 0b100010: sub(); break;
    case 0b101010: slt(); break;
    case 0b110010: tlt(); break;
    case 0b111010: reserved_B(); break;
    
    case 0b000011: sra(); break;
    case 0b001011: removed_r6(); break;
    case 0b010011: removed_r6(); break;
    case 0b011011: sop33(); break;
    case 0b100011: subu(); break;
    case 0b101011: sltu(); break;
    case 0b110011: tltu(); break;
    case 0b111011: reserved_B(); break;
    
    case 0b000100: sllv(); break;
    case 0b001100: syscall(); break;
    case 0b010100: reserved_B(); break;
    case 0b011100: reserved_B(); break;
    case 0b100100: and(); break;
    case 0b101100: reserved_B(); break;
    case 0b110100: teq(); break;
    case 0b111100: reserved_B(); break;
    
    case 0b000101: lsa(); break;
    case 0b001101: break_(); break;
    case 0b010101: reserved_B(); break;
    case 0b011101: reserved_B();break;
    case 0b100101: or(); break;
    case 0b101101: reserved_B(); break;
    case 0b110101: seleqz(); break;
    case 0b111101: reserved_star(); break;
    
    case 0b000110: execute_srlv(); break;
    case 0b001110: sdbbp(); break;
    case 0b010110: reserved_B(); break;
    case 0b011110: reserved_B(); break;
    case 0b100110: xor(); break;
    case 0b101110: reserved_B(); break;
    case 0b110110: tne(); break;
    case 0b111110: reserved_B(); break;
    
    case 0b000111: srav(); break;
    case 0b001111: sync(); break;
    case 0b010111: reserved_B(); break;
    case 0b011111: reserved_B(); break;
    case 0b100111: nor(); break;
    case 0b101111: reserved_B(); break;
    case 0b110111: selnez(); break;
    case 0b111111: reserved_B(); break;
    }
}

void execute_regimm(){
    switch (EXE.rt){
    case 0b00000: bltz(); break;
    case 0b00001: bgez(); break;
    case 0b00010: removed_r6(); break;
    case 0b00011: removed_r6(); break;
    case 0b00100: reserved_star(); break;
    case 0b00101: reserved_star(); break;
    case 0b00110: dahi(); break;
    case 0b00111: reserved_no_module(); break;

    case 0b01000: removed_r6(); break;
    case 0b01001: removed_r6(); break;
    case 0b01010: removed_r6(); break;
    case 0b01011: removed_r6(); break;
    case 0b01100: removed_r6(); break;
    case 0b01101: reserved_star(); break;
    case 0b01110: removed_r6(); break;
    case 0b01111: reserved_star(); break;

    case 0b10000: deprecated(); break;
    case 0b10001: deprecated(); break;
    case 0b10010: removed_r6(); break;
    case 0b10011: removed_r6(); break;
    case 0b10100: reserved_star(); break;
    case 0b10101: reserved_star(); break;
    case 0b10110: reserved_star(); break;
    case 0b10111: sigrie(); break;

    case 0b11000: reserved_star(); break;
    case 0b11001: reserved_star(); break;
    case 0b11010: reserved_star(); break;
    case 0b11011: reserved_star(); break;
    case 0b11100: reserved_no_module(); break;
    case 0b11101: reserved_no_module(); break;
    case 0b11110: dati(); break;
    case 0b11111: synci(); break;
    }
}

void execute_special3() {
    switch(EXE.funct) {
    case 0b000000: ext(); break;
    case 0b000001: reserved_B(); break;
    case 0b000010: reserved_B(); break;
    case 0b000011: reserved_B(); break;
    case 0b000100: ins(); break;
    case 0b000101: reserved_B(); break;
    case 0b000110: reserved_B(); break;
    case 0b000111: reserved_B(); break;

    case 0b001000: reserved_no_module(); break;
    case 0b001001: reserved_no_module(); break;
    case 0b001010: reserved_no_module(); break;
    case 0b001011: reserved_star(); break;
    case 0b001100: reserved_no_module(); break;
    case 0b001101: reserved_no_module(); break;
    case 0b001110: reserved_star(); break;
    case 0b001111: reserved_star(); break;

    case 0b010000: reserved_no_module(); break;
    case 0b010001: reserved_no_module(); break;
    case 0b010010: reserved_no_module(); break;
    case 0b010011: reserved_no_module(); break;
    case 0b010100: reserved_no_module(); break;
    case 0b010101: reserved_no_module(); break;
    case 0b010110: reserved_no_module(); break;
    case 0b010111: reserved_no_module(); break;

    case 0b011000: reserved_no_module(); break;
    case 0b011001: removed_r6(); break;
    case 0b011010: removed_r6(); break;
    case 0b011011: reserved_no_module(); break;
    case 0b011100: reserved_no_module(); break;
    case 0b011101: reserved_no_module(); break;
    case 0b011110: reserved_no_module(); break;
    case 0b011111: reserved_no_module(); break;

    case 0b100000: execute_bshfl(); break;
    case 0b100001: removed_r6(); break;
    case 0b100010: removed_r6(); break;
    case 0b100011: reserved_no_module(); break;
    case 0b100100: reserved_B(); break;
    case 0b100101: reserved_no_module(); break;
    case 0b100110: reserved_no_module(); break;
    case 0b100111: reserved_B(); break;

    case 0b101000: reserved_no_module(); break;
    case 0b101001: reserved_no_module(); break;
    case 0b101010: reserved_star(); break;
    case 0b101011: reserved_star(); break;
    case 0b101100: reserved_no_module(); break;
    case 0b101101: reserved_no_module(); break;
    case 0b101110: reserved_no_module(); break;
    case 0b101111: reserved_no_module(); break;

    case 0b110000: reserved_no_module(); break;
    case 0b110001: reserved_no_module(); break;
    case 0b110010: reserved_star(); break;
    case 0b110011: reserved_star(); break;
    case 0b110100: reserved_no_module(); break;
    case 0b110101: pref(); break;
    case 0b110110: ll(); break;
    case 0b110111: reserved_B(); break;

    case 0b111000: reserved_no_module(); break;
    case 0b111001: reserved_star(); break;
    case 0b111010: reserved_star(); break;
    case 0b111011: reserved_no_module(); break;
    case 0b111100: reserved_no_module(); break;
    case 0b111101: reserved_star(); break;
    case 0b111110: reserved_star(); break;
    case 0b111111: reserved_star(); break;
    }
}

void execute_pcrel(){
    switch (EXE.op_code)
    {
    case 0b00000:
    case 0b00001:
    case 0b00010:
    case 0b00011:
    case 0b00100:
    case 0b00101:
    case 0b00110:
    case 0b00111: addiupc(); break;
    case 0b01000:
    case 0b01001:
    case 0b01010:
    case 0b01011:
    case 0b01100:
    case 0b01101:
    case 0b01110:
    case 0b01111: lwpc(); break;
    case 0b10000:
    case 0b10001:
    case 0b10010:
    case 0b10011:
    case 0b10100:
    case 0b10101:
    case 0b10110:
    case 0b10111: lwupc(); break;
    case 0b11000:
    case 0b11001:
    case 0b11010:
    case 0b11011: ldpc(); break;
    case 0b11110: auipc(); break;
    case 0b11111: aluipc(); break;
    }
}

void execute_bshfl() {
    switch(EXE.shamt){
    case 0b00000: bitswap(); break;
    case 0b00001: reserved_star(); break;
    case 0b00010: wsbh(); break;
    case 0b00011: reserved_star(); break;
    case 0b00100: reserved_star(); break;
    case 0b00101: reserved_star(); break;
    case 0b00110: reserved_star(); break;
    case 0b00111: reserved_star(); break;

    case 0b01000:
    case 0b01001:
    case 0b01010:
    case 0b01011: align(); break;
    case 0b01100: reserved_star(); break;
    case 0b01101: reserved_star(); break;
    case 0b01110: reserved_star(); break;
    case 0b01111: reserved_star(); break;

    case 0b10000: seb(); break;
    case 0b10001: reserved_star(); break;
    case 0b10010: reserved_star(); break;
    case 0b10011: reserved_star(); break;
    case 0b10100: reserved_star(); break;
    case 0b10101: reserved_star(); break;
    case 0b10110: reserved_star(); break;
    case 0b10111: reserved_star(); break;

    case 0b11000: seh(); break;
    case 0b11001: reserved_star(); break;
    case 0b11010: reserved_star(); break;
    case 0b11011: reserved_star(); break;
    case 0b11100: reserved_star(); break;
    case 0b11101: reserved_star(); break;
    case 0b11110: reserved_star(); break;
    case 0b11111: reserved_star(); break;
    }
}

void execute_pop06() {
    reserved_no_module();
}
void execute_pop07() {
    reserved_no_module();
}
void execute_pop10() {
    if (EXE.rs < EXE.rt && EXE.rs != 0 && EXE.rt != 0) beqc();
    else if (EXE.rs >= EXE.rt) bovc();
    else reserved_no_module();
}
void execute_pop26() {
    reserved_no_module();
}
void execute_pop27() {
    reserved_no_module();
}
void execute_pop30() {
    if (EXE.rs < EXE.rt && EXE.rs != 0 && EXE.rt != 0) bnec();
    else if (EXE.rs >= EXE.rt) bnvc();
    else reserved_no_module();
}
void execute_pop66() {
    if (EXE.rs == 0) jic();
    else reserved_no_module();
}
void execute_pop76() {
    if (EXE.rs == 0) jialc();
    else reserved_no_module(); 
}

void sop30(){
    switch(EXE.shamt){
    case 0b00010: mul(); break;
    case 0b00011: muh(); break;
    default: reserved_no_module();
    }
}
void sop31(){
    switch(EXE.shamt){
    case 0b00010: mulu(); break;
    case 0b00011: muhu(); break;
    default: reserved_no_module();
    }
}
void sop32(){
    switch(EXE.shamt){
    case 0b00010: div(); break;
    case 0b00011: mod(); break;
    default: reserved_no_module();
    }
}
void sop33(){
    switch(EXE.shamt){
    case 0b00010: divu(); break;
    case 0b00011: modu(); break;
    default: reserved_no_module();
    }
}

void execute_srl() {
    switch(EXE.r){
        case 0: srl(); break;
        case 1: rotr(); break;
    }
}
void execute_srlv() {
    switch(EXE.r){
        case 0: srlv(); break;
        case 1: rotrv(); break;
    }
}

void reserved_B(){
    trigger_trap(pc, EXCEPT_RESERVED_INSTRUCTION);
}
void reserved_star(){
    trigger_trap(pc, EXCEPT_RESERVED_INSTRUCTION);
}
void reserved_no_module() {
    trigger_trap(pc, EXCEPT_RESERVED_INSTRUCTION);

}
void removed_r6() {
    trigger_trap(pc, EXCEPT_RESERVED_INSTRUCTION);
}
void deprecated() {
    trigger_trap(pc, EXCEPT_RESERVED_INSTRUCTION);

}
void execute_cop0() {
    trigger_trap(pc, EXCEPT_RESERVED_INSTRUCTION);

}
void execute_cop1() {
    trigger_trap(pc, EXCEPT_RESERVED_INSTRUCTION);

}
void execute_cop2() {
    trigger_trap(pc, EXCEPT_RESERVED_INSTRUCTION);

}
