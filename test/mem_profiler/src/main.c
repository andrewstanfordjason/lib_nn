// Copyright 2020 XMOS LIMITED. This Software is subject to the terms of the 
// XMOS Public License: Version 1


#include <stdio.h>
#include "nn_operator.h"

#define MAX_MODEL_SIZE_BYTES 8192

__attribute__((section(".ExtMem_data")))
unsigned char model_data[MAX_MODEL_SIZE_BYTES] __attribute__((aligned(4)));


int main(void) {
  int ret_val;

  unsigned char sram[MAX_MODEL_SIZE_BYTES] __attribute__((aligned(4)));


  vpu_memcpy(sram, model_data, MAX_MODEL_SIZE_BYTES);

  return 0;
}
