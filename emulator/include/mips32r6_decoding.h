#pragma once

#include <stdbool.h>
#include <stdint.h>

enum Instruction {
    ADD,
    ADDIU,
    ADDIUPC,
    ADDU,
    ALIGN,
    ALUIPC,
    AND,
    ANDI,
    AUI,
    AUIPC,
    BALC,
    BC,
    BEQ,
    BGEZ,
    BEQC,
    BNEC,
    BEQZC,
    BNEZC,
    BGTZ,
    BITSWAP,
    BLEZ,
    BLTZ,
    BNE,
    BOVC,
    BNVC,
    BREAK,
    CLO,
    CLZ,
    DIV,
    MOD,
    DIVU,
    MODU,
    EHB,
    EXT,
    INS,
    J,
    JAL,
    JALR,
    JIALC,
    JIC,
    LB,
    LBU,
    LH,
    LHU,
    LL,
    LSA,
    LW,
    LWPC,
    MUL,
    MUH,
    MULU,
    MUHU,
    NOR,
    OR,
    ORI,
    PAUSE,
    PREF,
    ROTR,
    ROTRV,
    SB,
    SC,
    SDBBP,
    SEB,
    SEH,
    SELEQZ,
    SELNEZ,
    SH,
    SIGRIE,
    SLL,
    SLLV,
    SLT,
    SLTI,
    SLTIU,
    SLTU,
    SRA,
    SRAV,
    SRL,
    SRLV,
    SUB,
    SUBU,
    SW,
    SYNC,
    SYNCI,
    SYSCALL,
    TEQ,
    TGE,
    TGEU,
    TLT,
    TLTU,
    TNE,
    WSBH,
    XOR,
    XORI,

    // Bad Decodes
    BAD_DECODE,
    RESERVED_B,
    RESERVED_STAR,
    RESERVED_NO_MODULE,
    REMOVED_R6,
    DEPRECATED,
    COP0,
    COP1,
    COP2
};

enum Instruction decode_instruction();

enum Instruction decode_special();
enum Instruction decode_regimm();
enum Instruction decode_special3();
enum Instruction decode_pcrel();
enum Instruction decode_bshfl();
enum Instruction decode_pop06();
enum Instruction decode_pop07();
enum Instruction decode_pop10();
enum Instruction decode_pop26();
enum Instruction decode_pop27();
enum Instruction decode_pop30();
enum Instruction decode_pop66();
enum Instruction decode_pop76();
enum Instruction decode_sop30();
enum Instruction decode_sop31();
enum Instruction decode_sop32();
enum Instruction decode_sop33();
enum Instruction decode_srl();
enum Instruction decode_srlv();

bool is_cti(enum Instruction inst);
int32_t get_sign_extended_offset(uint32_t x, int bits);
