#include <stdint.h>
#include <stdbool.h>

int load_program_from_disk(char* input_file, uint8_t text_dest[], int text_size, uint8_t data_dest[], int data_size);
void write_to_memory(void* bytes, int size, uint8_t memory_segment[], int memory_segment_size);

bool system_is_big_endian();
void swap_word_endianness(uint32_t* val);