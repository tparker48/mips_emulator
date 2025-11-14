#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mips_program_loading.h"

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

void write_to_memory(void* bytes, int size, uint8_t memory_segment[], int memory_segment_size)
{
    // data should be big-endian
    if (!system_is_big_endian())
    {
        uint32_t *start_addr = (uint32_t *)bytes;
        int word_count = size / sizeof(uint32_t);
        for (int i = 0; i < word_count; i++)
        {
            swap_word_endianness((start_addr) + i);
        }
    }
    if (size > memory_segment_size)
    {
        fprintf(stderr, "Program text length limit exceeded\n");
    }
    memset(memory_segment, 0, memory_segment_size);
    memcpy(memory_segment, bytes, size);
}

int load_program_from_disk(char* input_file, uint8_t text_dest[], int text_size, uint8_t data_dest[], int data_size){
    char *buffer;
    FILE *fptr;

    // Open file
    fptr = fopen(input_file, "rb");
    if (fptr == NULL)
    {
        printf("Error opening file: %s.\n", input_file);
        return 0;
    }

    // Get segment sizes in bytes
    uint32_t text_segment_size;
    uint32_t data_segment_size;
    fread(&text_segment_size, sizeof(uint32_t), 1, fptr);
    fread(&data_segment_size, sizeof(uint32_t), 1, fptr);
    if (!system_is_big_endian()){
        swap_word_endianness(&text_segment_size);
        swap_word_endianness(&data_segment_size);
    }

    if (text_segment_size > text_size){
        printf(".text segment too large! Max byte count is %d, got %d\n", text_size, text_segment_size);
        fclose(fptr);
        return 0;
    }
    if (data_segment_size > data_size){
        printf(".text segment too large! Max byte count is %d, got %d\n", data_size, data_segment_size);
        fclose(fptr);
        return 0;
    }

    // Copy .text
    buffer = (char *) malloc(text_segment_size * sizeof(char));
    fread(buffer, 1, text_segment_size, fptr);
    write_to_memory(buffer, text_segment_size, text_dest, text_size);
    free(buffer);

    // Copy .data
    buffer = (char *) malloc(data_segment_size * sizeof(char));
    fread(buffer, 1, data_segment_size, fptr);
    write_to_memory(buffer, data_segment_size, data_dest, data_size);
    free(buffer);
    
    fclose(fptr);
    return text_segment_size;
}