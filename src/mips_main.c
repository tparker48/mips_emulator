
#include <stdbool.h>

#include "mips_alu.h"
#include "mips_memory.h"
#include "mips_pipeline.h"

bool system_is_big_endian(){
    uint16_t dummy_halfword = 0x00FF;
    char *byte_ptr = (char *)&dummy_halfword;
    return (*byte_ptr == 0x00);
}

void swap_word_endianness(uint32_t* val)
{
    *val = ((*val << 24) & 0xFF000000) |
           ((*val << 8)  & 0x00FF0000) |
           ((*val >> 8)  & 0x0000FF00) |
           ((*val >> 24) & 0x000000FF);
}

void load_program(void *text_bytes, int len)
{
    // data should be big-endian
    if (!system_is_big_endian()){
        uint32_t* start_addr = (uint32_t*)text_bytes;
        int word_count = len/sizeof(uint32_t);
        for (int i = 0; i < word_count; i++)
        {
            swap_word_endianness((start_addr) + i);
        }
    }

    int num_bytes = sizeof(uint8_t) * len;
    if (num_bytes > TEXT_SIZE)
    {
        fprintf(stderr, "Program text length limit exceeded\n");
    }
    memset(text, 0, TEXT_SIZE);
    memcpy(text, text_bytes, num_bytes);
    pc = 0;
    pc_cap = len;
}

int load_program_from_argv(int argc, char **argv)
{
    char *buffer;
    char *input_file;
    long filelen;
    FILE *fptr;

    if (argc < 2)
    {
        printf("Usage: ./mips [file]\n");
        return 1;
    }

    // Open file
    input_file = argv[1];
    fptr = fopen(input_file, "rb");
    if (fptr == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    // Get file length
    fseek(fptr, 0, SEEK_END);
    filelen = ftell(fptr);
    rewind(fptr);
    if (filelen > TEXT_SIZE)
    {
        printf("Input file too big!.\n");
        fclose(fptr);
        return 1;
    }

    // Copy bytes
    buffer = (char *)malloc(filelen * sizeof(char));
    fread(buffer, 1, filelen, fptr);
    fclose(fptr);
    load_program(buffer, filelen);
    free(buffer);

    return 0;
}


/* Test program:
addu $v0 $a0 $a1   0b000000 00100 00101 00010 00000 100001
uint32_t program = 0b00000000100001010001000000100100;
*/
int main(int argc, char **argv)
{
    if (load_program_from_argv(argc, argv) != 0){
        return 1;
    }

    // TEMP CODE REMOVE
    registers[a1] = 5;
    registers[a2] = 10;

    int cycle = 1;
    while (!should_exit()) {
        run_cycle();
        cycle++;
    }

    // TEMP CODE REMOVE
    printf("Register $t0:%d",registers[t0]);

    return 0;
}
