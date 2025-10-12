
#include <stdbool.h>

#include "mips_alu.h"
#include "mips_memory.h"
#include "mips_pipeline.h"


void load_program(void *text_bytes, int len)
{
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

    while (!should_exit()) {
        run_cycle();
    }

    return 0;
}
