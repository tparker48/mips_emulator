#include "mips_instructions.h"

bool is_r_type(uint8_t op_code)
{
    return (op_code == 0x00 || op_code == 0x10);
}
bool is_j_type(uint8_t op_code)
{
    return (op_code == 0x2 || op_code == 0x3);
}
bool is_i_type(uint8_t op_code)
{
    return !(is_r_type(op_code) || is_j_type(op_code));
}
