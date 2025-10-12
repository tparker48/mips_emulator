#include "mips_instructions.h"

bool is_r_type(uint8_t op_code)
{
    return (op_code == 0x00 || op_code == 0x10);
}
bool is_j_type(uint8_t op_code)
{
    return (op_code == 0x02 || op_code == 0x03);
}
bool is_i_type(uint8_t op_code)
{
    return !(is_r_type(op_code) || is_j_type(op_code));
}


bool reads_mem(uint8_t op_code)
{
    return (
        op_code == OP_LW || 
        op_code == OP_LH || 
        op_code == OP_LB ||
        op_code == OP_LHU || 
        op_code == OP_LBU
    );
}
bool writes_hilo(uint8_t op_code, uint8_t funct)
{
    return (
        op_code == OP_ZERO && 
        ( 
            funct == FUNC_MULT || 
            funct == FUNC_MULTU || 
            funct == FUNC_DIV || 
            funct == FUNC_DIVU
        )
    );
}

bool reads_hilo(uint8_t op_code, uint8_t funct)
{
    return (
        op_code == OP_ZERO &&
        (funct == FUNC_MFHI ||
         funct == FUNC_MFLO
        )
    );
}