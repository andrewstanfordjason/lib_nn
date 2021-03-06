// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1


#include <stdio.h>
#include "nn_operator.h"
#include <stdint.h>
#include "xcore/hwtimer.h"

#define MAX_MODEL_SIZE_BYTES (1<<13)

__attribute__((section(".ExtMem_data")))
int8_t ddr_data[MAX_MODEL_SIZE_BYTES] __attribute__((aligned(4)));

int8_t sram_data[MAX_MODEL_SIZE_BYTES] __attribute__((aligned(4)));

//flash?

void profile_vpu_memcpy(
  int8_t * src, int8_t * dst,

  size_t src_pointer_inc, 
  size_t max_src_offset,

  size_t atom_bytes, 
  int atom_count, 
  int alignment, 
  void (*mem_cpy_func)(), 
  char * test_name){

  size_t max_dst_offset = 32;

  // printf("---------------------------\n");
  printf("%s\n", test_name);
  FILE * fp = fopen(test_name, "w+");

  size_t byte_count = MAX_MODEL_SIZE_BYTES;

  hwtimer_t t = hwtimer_alloc();

  for(size_t src_offset = 0; src_offset < max_src_offset; src_offset += src_pointer_inc){

        size_t max_cpy_atoms = (byte_count - src_offset) / atom_bytes;

        for(size_t cpy_atoms = 1; cpy_atoms < max_cpy_atoms; cpy_atoms += 1){

            uint32_t before = hwtimer_get_time(t);  
            mem_cpy_func(dst + (src_offset&3), src + src_offset, cpy_atoms);
            uint32_t after = hwtimer_get_time(t);  

            unsigned elapsed = after - before;

            fprintf(fp, "%d, %d, %d\n", src_offset, cpy_atoms*atom_bytes, elapsed);
        }
    }
  fclose(fp);
}

int main(void) {

    // profile_vpu_memcpy(sram_data, sram_data, 1, 8, 1, 16, 4, vpu_memcpy, "sram_sram_vpu_memcpy.csv");
    profile_vpu_memcpy(sram_data, sram_data, 1, 3, 1, 16, 4, vpu_memcpy_int, "sram_sram_vpu_memcpy_int.csv");
    profile_vpu_memcpy(sram_data, sram_data, 1, 3, 1, 16, 4, vpu_memcpy_ext, "sram_sram_vpu_memcpy_ext.csv");
    profile_vpu_memcpy(sram_data, sram_data, 4, 32, MEMCPY_VECT_EXT_BYTES, 8, 4, vpu_memcpy_vector_ext, "sram_sram_vpu_memcpy_vector_ext.csv");
    profile_vpu_memcpy(sram_data, sram_data, 4, 32, MEMCPY_VECT_INT_BYTES, 8, 4, vpu_memcpy_vector_int, "sram_sram_vpu_memcpy_vector_int.csv");

    // profile_vpu_memcpy(ddr_data, sram_data, 1, 8, 1, 16, 4, "ddr_sram_vpu_memcpy.csv");
    profile_vpu_memcpy(ddr_data, sram_data, 1, 3, 1, 16, 4, vpu_memcpy_int, "ddr_sram_vpu_memcpy_int.csv");
    profile_vpu_memcpy(ddr_data, sram_data, 1, 3, 1, 16, 4, vpu_memcpy_ext, "ddr_sram_vpu_memcpy_ext.csv");
    profile_vpu_memcpy(ddr_data, sram_data, 4, 32, MEMCPY_VECT_EXT_BYTES, 8, 4, vpu_memcpy_vector_ext, "ddr_sram_vpu_memcpy_vector_ext.csv");
    profile_vpu_memcpy(ddr_data, sram_data, 4, 32, MEMCPY_VECT_INT_BYTES, 8, 4, vpu_memcpy_vector_int, "ddr_sram_vpu_memcpy_vector_int.csv");

  return 0;
}
