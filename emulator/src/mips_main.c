
#include <stdbool.h>
#include <string.h>

#include "mips_alu.h"
#include "mips_memory.h"
#include "mips_pipeline.h"
#include "mips_program_loading.h"


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: ./mips [file]\n");
        return 1;
    }

    int program_length = load_program_from_disk(argv[1], text, TEXT_SIZE, static_data, STATIC_DATA_SIZE);
    if (program_length == 0)
    {
        return 1;
    }

    pc = TEXT_START;
    pc_cap = pc+program_length;

    int cycle = 1;
    while (!exited()) {
        run_cycle();
        cycle++;
    }
    
    printf("Exited with code: %d\n", get_exit_code());
    return 0;
}
