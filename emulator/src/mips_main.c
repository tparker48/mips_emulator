
#include <stdbool.h>
#include <string.h>

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

void write_program_to_memory(void* program_bytes, int program_size, uint8_t dest[], int dest_size)
{
    // data should be big-endian
    if (!system_is_big_endian())
    {
        uint32_t *start_addr = (uint32_t *)program_bytes;
        int word_count = program_size / sizeof(uint32_t);
        for (int i = 0; i < word_count; i++)
        {
            swap_word_endianness((start_addr) + i);
        }
    }
    if (program_size > dest_size)
    {
        fprintf(stderr, "Program text length limit exceeded\n");
    }
    memset(dest, 0, dest_size);
    memcpy(dest, program_bytes, program_size);
}

int load_program_from_disk(char* input_file, uint8_t dest[], int dest_size){
    char *buffer;
    long filelen;
    FILE *fptr;

    // Open file
    fptr = fopen(input_file, "rb");
    if (fptr == NULL)
    {
        printf("Error opening file: %s.\n", input_file);
        return 0;
    }

    // Get file length
    fseek(fptr, 0, SEEK_END);
    filelen = ftell(fptr);
    rewind(fptr);
    if (filelen > TEXT_SIZE)
    {
        printf("Input file too big!.\n");
        fclose(fptr);
        return 0;
    }

    // Copy bytes
    buffer = (char *)malloc(filelen * sizeof(char));
    fread(buffer, 1, filelen, fptr);
    fclose(fptr);
    write_program_to_memory(buffer, filelen, dest, dest_size);
    free(buffer);

    return filelen;
}

int parse_kernel_symbol_map(char* mapfile){
    char *buffer;
    FILE *fptr;
    int filelen;

    fptr = fopen(mapfile, "r");
    if (fptr == NULL){
        printf("Error loading kernel symbol map %s", mapfile);
        return 0;
    }

    char line[128];
    char name[64];
    uint32_t addr;
    kernel_start_addr = 0;
    kernel_exception_vector_addr = 0;
    while (fscanf(fptr, "%63s = %x", name, &addr) == 2)
    {
        if (strcmp(name, "_start") == 0){
            kernel_start_addr = addr;
        }
        else if (strcmp(name, "exception_vector") == 0)
        {
            kernel_exception_vector_addr = addr;
        }
    }
    fclose(fptr);

    if (kernel_start_addr == 0 || kernel_exception_vector_addr == 0)
    {
        if (kernel_start_addr == 0)
            printf("%s did not contain required symbol: _start\n", mapfile);
        if (kernel_exception_vector_addr == 0)
            printf("%s did not contain required symbol: exception_vector\n", mapfile);
        return 0;
    }

    return 1;
}


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: ./mips [file]\n");
        return 1;
    }

    if (load_program_from_disk("kernel/kernel.bin", kernel, KERNEL_SIZE) == 0)
    {
        return 1;
    }

    if(parse_kernel_symbol_map("kernel/kernel.map") == 0)
    {
        return 1;
    }

    int program_length = load_program_from_disk(argv[1], text, TEXT_SIZE);
    if (program_length == 0)
    {
        return 1;
    }

    pc = TEXT_START;
    pc_cap = program_length;

    int cycle = 1;
    while (!exited()) {
        run_cycle();
        cycle++;
    }
    printf("Exited with code: %d", get_exit_code());

    return 0;
}
