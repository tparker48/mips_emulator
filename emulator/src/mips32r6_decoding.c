#include "mips32r6_decoding.h"
#include "mips_pipeline.h"
#include "mips_os.h"
#include "mips_registers.h"

enum Instruction decode_instruction(){
    switch(EXE.op_code){
    case 0b000000: return decode_special();
    case 0b000001: return decode_regimm();
    case 0b000010: return J;
    case 0b000011: return JAL;
    case 0b000100: return BEQ;
    case 0b000101: return BNE;
    case 0b000110: return decode_pop06();
    case 0b000111: return decode_pop07();

    case 0b001000: return decode_pop10();
    case 0b001001: return ADDIU;
    case 0b001010: return SLTI;
    case 0b001011: return SLTIU;
    case 0b001100: return ANDI;
    case 0b001101: return ORI;
    case 0b001110: return XORI;
    case 0b001111: return AUI;

    case 0b010000: return COP0;
    case 0b010001: return COP1;
    case 0b010010: return COP2;
    case 0b010011: return REMOVED_R6;
    case 0b010100: return REMOVED_R6;
    case 0b010101: return REMOVED_R6;
    case 0b010110: return decode_pop26();
    case 0b010111: return decode_pop27();

    case 0b011000: return decode_pop30();
    case 0b011001: return RESERVED_B;
    case 0b011010: return RESERVED_B;
    case 0b011011: return RESERVED_B;
    case 0b011100: return RESERVED_STAR;
    case 0b011101: return RESERVED_NO_MODULE;
    case 0b011110: return RESERVED_NO_MODULE;
    case 0b011111: return decode_special3();

    case 0b100000: return LB;
    case 0b100001: return LH;
    case 0b100010: return REMOVED_R6;
    case 0b100011: return LW;
    case 0b100100: return LBU;
    case 0b100101: return LHU;
    case 0b100110: return REMOVED_R6;
    case 0b100111: return RESERVED_B;

    case 0b101000: return SB;
    case 0b101001: return SH;
    case 0b101010: return REMOVED_R6;
    case 0b101011: return SW;
    case 0b101100: return RESERVED_B;
    case 0b101101: return RESERVED_B;
    case 0b101110: return REMOVED_R6;
    case 0b101111: return REMOVED_R6;

    case 0b110000: return REMOVED_R6;
    case 0b110001: return RESERVED_NO_MODULE;
    case 0b110010: return BC;
    case 0b110011: return REMOVED_R6;
    case 0b110100: return RESERVED_B;
    case 0b110101: return RESERVED_NO_MODULE;
    case 0b110110: return decode_pop66();
    case 0b110111: return RESERVED_B;

    case 0b111000: return REMOVED_R6;
    case 0b111001: return RESERVED_NO_MODULE;
    case 0b111010: return BALC;
    case 0b111011: return decode_pcrel();
    case 0b111100: return RESERVED_B;
    case 0b111101: return RESERVED_NO_MODULE;
    case 0b111110: return decode_pop76();
    case 0b111111: return RESERVED_B;

    default: return BAD_DECODE;
    }
}

enum Instruction decode_special(){
    switch(EXE.funct){
    case 0b000000: return SLL;
    case 0b001000: return REMOVED_R6;
    case 0b010000: return CLZ;
    case 0b011000: return decode_sop30();
    case 0b100000: return ADD;
    case 0b101000: return RESERVED_STAR;
    case 0b110000: return TGE;
    case 0b111000: return RESERVED_B;
    
    case 0b000001: return REMOVED_R6;
    case 0b001001: return JALR;
    case 0b010001: return CLO;
    case 0b011001: return decode_sop31();
    case 0b100001: return ADDU;
    case 0b101001: return RESERVED_STAR;
    case 0b110001: return TGEU;
    case 0b111001: return RESERVED_STAR;
    
    case 0b000010: return decode_srl();
    case 0b001010: return REMOVED_R6;
    case 0b010010: return REMOVED_R6;
    case 0b011010: return decode_sop32();
    case 0b100010: return SUB;
    case 0b101010: return SLT;
    case 0b110010: return TLT;
    case 0b111010: return RESERVED_B;
    
    case 0b000011: return SRA;
    case 0b001011: return REMOVED_R6;
    case 0b010011: return REMOVED_R6;
    case 0b011011: return decode_sop33();
    case 0b100011: return SUBU;
    case 0b101011: return SLTU;
    case 0b110011: return TLTU;
    case 0b111011: return RESERVED_B;
    
    case 0b000100: return SLLV;
    case 0b001100: return SYSCALL;
    case 0b010100: return RESERVED_B;
    case 0b011100: return RESERVED_B;
    case 0b100100: return AND;
    case 0b101100: return RESERVED_B;
    case 0b110100: return TEQ;
    case 0b111100: return RESERVED_B;
    
    case 0b000101: return LSA;
    case 0b001101: return BREAK;
    case 0b010101: return RESERVED_B;
    case 0b011101: return RESERVED_B;
    case 0b100101: return OR;
    case 0b101101: return RESERVED_B;
    case 0b110101: return SELEQZ;
    case 0b111101: return RESERVED_STAR;
    
    case 0b000110: return decode_srlv();
    case 0b001110: return SDBBP;
    case 0b010110: return RESERVED_B;
    case 0b011110: return RESERVED_B;
    case 0b100110: return XOR;
    case 0b101110: return RESERVED_B;
    case 0b110110: return TNE;
    case 0b111110: return RESERVED_B;
    
    case 0b000111: return SRAV;
    case 0b001111: return SYNC;
    case 0b010111: return RESERVED_B;
    case 0b011111: return RESERVED_B;
    case 0b100111: return NOR;
    case 0b101111: return RESERVED_B;
    case 0b110111: return SELNEZ;
    case 0b111111: return RESERVED_B;

    default: return BAD_DECODE;
    }
}

enum Instruction decode_regimm(){
    switch (EXE.rt){
    case 0b00000: return BLTZ;
    case 0b00001: return BGEZ;
    case 0b00010: return REMOVED_R6;
    case 0b00011: return REMOVED_R6;
    case 0b00100: return RESERVED_STAR;
    case 0b00101: return RESERVED_STAR;
    case 0b00110: return RESERVED_NO_MODULE;
    case 0b00111: return RESERVED_NO_MODULE;

    case 0b01000: return REMOVED_R6;
    case 0b01001: return REMOVED_R6;
    case 0b01010: return REMOVED_R6;
    case 0b01011: return REMOVED_R6;
    case 0b01100: return REMOVED_R6;
    case 0b01101: return RESERVED_STAR;
    case 0b01110: return REMOVED_R6;
    case 0b01111: return RESERVED_STAR;

    case 0b10000: return DEPRECATED;
    case 0b10001: return DEPRECATED;
    case 0b10010: return REMOVED_R6;
    case 0b10011: return REMOVED_R6;
    case 0b10100: return RESERVED_STAR;
    case 0b10101: return RESERVED_STAR;
    case 0b10110: return RESERVED_STAR;
    case 0b10111: return SIGRIE;

    case 0b11000: return RESERVED_STAR;
    case 0b11001: return RESERVED_STAR;
    case 0b11010: return RESERVED_STAR;
    case 0b11011: return RESERVED_STAR;
    case 0b11100: return RESERVED_NO_MODULE;
    case 0b11101: return RESERVED_NO_MODULE;
    case 0b11110: return RESERVED_NO_MODULE;
    case 0b11111: return SYNCI;

    default: return BAD_DECODE;
    }
}

enum Instruction decode_special3() {
    switch(EXE.funct) {
    case 0b000000: return EXT;
    case 0b000001: return RESERVED_B;
    case 0b000010: return RESERVED_B;
    case 0b000011: return RESERVED_B;
    case 0b000100: return INS;
    case 0b000101: return RESERVED_B;
    case 0b000110: return RESERVED_B;
    case 0b000111: return RESERVED_B;

    case 0b001000: return RESERVED_NO_MODULE;
    case 0b001001: return RESERVED_NO_MODULE;
    case 0b001010: return RESERVED_NO_MODULE;
    case 0b001011: return RESERVED_STAR;
    case 0b001100: return RESERVED_NO_MODULE;
    case 0b001101: return RESERVED_NO_MODULE;
    case 0b001110: return RESERVED_STAR;
    case 0b001111: return RESERVED_STAR;

    case 0b010000: return RESERVED_NO_MODULE;
    case 0b010001: return RESERVED_NO_MODULE;
    case 0b010010: return RESERVED_NO_MODULE;
    case 0b010011: return RESERVED_NO_MODULE;
    case 0b010100: return RESERVED_NO_MODULE;
    case 0b010101: return RESERVED_NO_MODULE;
    case 0b010110: return RESERVED_NO_MODULE;
    case 0b010111: return RESERVED_NO_MODULE;

    case 0b011000: return RESERVED_NO_MODULE;
    case 0b011001: return REMOVED_R6;
    case 0b011010: return REMOVED_R6;
    case 0b011011: return RESERVED_NO_MODULE;
    case 0b011100: return RESERVED_NO_MODULE;
    case 0b011101: return RESERVED_NO_MODULE;
    case 0b011110: return RESERVED_NO_MODULE;
    case 0b011111: return RESERVED_NO_MODULE;

    case 0b100000: return decode_bshfl();
    case 0b100001: return REMOVED_R6;
    case 0b100010: return REMOVED_R6;
    case 0b100011: return RESERVED_NO_MODULE;
    case 0b100100: return RESERVED_B;
    case 0b100101: return RESERVED_NO_MODULE;
    case 0b100110: return RESERVED_NO_MODULE;
    case 0b100111: return RESERVED_B;

    case 0b101000: return RESERVED_NO_MODULE;
    case 0b101001: return RESERVED_NO_MODULE;
    case 0b101010: return RESERVED_STAR;
    case 0b101011: return RESERVED_STAR;
    case 0b101100: return RESERVED_NO_MODULE;
    case 0b101101: return RESERVED_NO_MODULE;
    case 0b101110: return RESERVED_NO_MODULE;
    case 0b101111: return RESERVED_NO_MODULE;

    case 0b110000: return RESERVED_NO_MODULE;
    case 0b110001: return RESERVED_NO_MODULE;
    case 0b110010: return RESERVED_STAR;
    case 0b110011: return RESERVED_STAR;
    case 0b110100: return RESERVED_NO_MODULE;
    case 0b110101: return PREF;
    case 0b110110: return LL;
    case 0b110111: return RESERVED_B;

    case 0b111000: return RESERVED_NO_MODULE;
    case 0b111001: return RESERVED_STAR;
    case 0b111010: return RESERVED_STAR;
    case 0b111011: return RESERVED_NO_MODULE;
    case 0b111100: return RESERVED_NO_MODULE;
    case 0b111101: return RESERVED_STAR;
    case 0b111110: return RESERVED_STAR;
    case 0b111111: return RESERVED_STAR;

    default: return BAD_DECODE;
    }
}

enum Instruction decode_pcrel(){
    switch (EXE.op_code)
    {
    case 0b00000:
    case 0b00001:
    case 0b00010:
    case 0b00011:
    case 0b00100:
    case 0b00101:
    case 0b00110:
    case 0b00111: return ADDIUPC;
    case 0b01000:
    case 0b01001:
    case 0b01010:
    case 0b01011:
    case 0b01100:
    case 0b01101:
    case 0b01110:
    case 0b01111: return LWPC;
    case 0b10000:
    case 0b10001:
    case 0b10010:
    case 0b10011:
    case 0b10100:
    case 0b10101:
    case 0b10110:
    case 0b10111: return RESERVED_NO_MODULE;
    case 0b11000:
    case 0b11001:
    case 0b11010:
    case 0b11011: return RESERVED_NO_MODULE;
    case 0b11110: return AUIPC;
    case 0b11111: return ALUIPC;

    default: return BAD_DECODE;
    }
}

enum Instruction decode_bshfl() {
    switch(EXE.shamt){
    case 0b00000: return BITSWAP;
    case 0b00001: return RESERVED_STAR;
    case 0b00010: return WSBH;
    case 0b00011: return RESERVED_STAR;
    case 0b00100: return RESERVED_STAR;
    case 0b00101: return RESERVED_STAR;
    case 0b00110: return RESERVED_STAR;
    case 0b00111: return RESERVED_STAR;

    case 0b01000:
    case 0b01001:
    case 0b01010:
    case 0b01011: return ALIGN;
    case 0b01100: return RESERVED_STAR;
    case 0b01101: return RESERVED_STAR;
    case 0b01110: return RESERVED_STAR;
    case 0b01111: return RESERVED_STAR;

    case 0b10000: return SEB;
    case 0b10001: return RESERVED_STAR;
    case 0b10010: return RESERVED_STAR;
    case 0b10011: return RESERVED_STAR;
    case 0b10100: return RESERVED_STAR;
    case 0b10101: return RESERVED_STAR;
    case 0b10110: return RESERVED_STAR;
    case 0b10111: return RESERVED_STAR;

    case 0b11000: return SEH;
    case 0b11001: return RESERVED_STAR;
    case 0b11010: return RESERVED_STAR;
    case 0b11011: return RESERVED_STAR;
    case 0b11100: return RESERVED_STAR;
    case 0b11101: return RESERVED_STAR;
    case 0b11110: return RESERVED_STAR;
    case 0b11111: return RESERVED_STAR;

    default: return BAD_DECODE;
    }
}

enum Instruction decode_pop06() {
    return RESERVED_NO_MODULE;
}
enum Instruction decode_pop07() {
    return RESERVED_NO_MODULE;
}
enum Instruction decode_pop10() {
    if (EXE.rs < EXE.rt && EXE.rs != 0 && EXE.rt != 0) return BEQC;
    else if (EXE.rs >= EXE.rt) return BOVC;
    else return RESERVED_NO_MODULE;
}
enum Instruction decode_pop26() {
    return RESERVED_NO_MODULE;
}
enum Instruction decode_pop27() {
    return RESERVED_NO_MODULE;
}
enum Instruction decode_pop30() {
    if (EXE.rs < EXE.rt && EXE.rs != 0 && EXE.rt != 0) return BNEC;
    else if (EXE.rs >= EXE.rt) return BNVC;
    else return RESERVED_NO_MODULE;
}
enum Instruction decode_pop66() {
    if (EXE.rs == 0) return JIC;
    else return RESERVED_NO_MODULE;
}
enum Instruction decode_pop76() {
    if (EXE.rs == 0) return JIALC;
    else return RESERVED_NO_MODULE; 
}

enum Instruction decode_sop30(){
    switch(EXE.shamt){
    case 0b00010: return MUL;
    case 0b00011: return MUH;
    default: return RESERVED_NO_MODULE;
    }
}
enum Instruction decode_sop31(){
    switch(EXE.shamt){
    case 0b00010: return MULU;
    case 0b00011: return MUHU;
    default: return RESERVED_NO_MODULE;
    }
}
enum Instruction decode_sop32(){
    switch(EXE.shamt){
    case 0b00010: return DIV;
    case 0b00011: return MOD;
    default: return RESERVED_NO_MODULE;
    }
}
enum Instruction decode_sop33(){
    switch(EXE.shamt){
    case 0b00010: return DIVU;
    case 0b00011: return MODU;
    default: return RESERVED_NO_MODULE;
    }
}

enum Instruction decode_srl() {
    switch(EXE.r){
        case 0: return SRL;
        case 1: return ROTR;
        default: return BAD_DECODE;
    }
}
enum Instruction decode_srlv() {
    switch(EXE.r){
        case 0: return SRLV;
        case 1: return ROTRV;
        default: return BAD_DECODE;
    }
}

bool is_cti(enum Instruction inst){
    //CTIs include all branches and jumps, NAL, ERET, ERETNC, DERET, WAIT, and PAUSE. 
    switch (inst){
        case J:
        case JAL:
        case JALR:
        case JIALC:
        case JIC:
        case BALC:
        case BC:
        case BEQ:
        case BEQC:
        case BEQZC:
        case BGEZ:
        case BGTZ:
        case BLEZ:
        case BLTZ:
        case BNE:
        case BNEC:
        case BNEZC:
        case BNVC:
        case BOVC:
        case PAUSE:
            return true;
        default:
            return false;
    }
}

int32_t get_sign_extended_offset(uint32_t x, int bits)
{
    uint32_t mask = (1u << bits) - 1;
    x &= mask;

    uint32_t sign_bit_mask = 1u << (bits - 1);
    if (x & sign_bit_mask) {
        uint32_t extend_mask = ~((1u << bits) - 1);
        x |= extend_mask;
    }

    return (int32_t)x << 2;
}