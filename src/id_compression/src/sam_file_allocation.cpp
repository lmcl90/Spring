//
//  sam_file_allocation.c
//  XC_s2fastqIO
//
//  Created by Mikel Hernaez on 11/18/14.
//  Copyright (c) 2014 Mikel Hernaez. All rights reserved.
//

#include "id_compression/include/sam_block.h"

// devuelve la longitud del PRIMER read?
// se asume que todos lso reads son de la misma longitud.

namespace spring {
namespace id_comp {

/**
 *
 */
id_block alloc_id_block() {
  uint32_t i = 0;

  id_block rtn = (id_block)calloc(1, sizeof(struct id_block_t));

  rtn->block_length = 1;

  rtn->IDs = (char **)calloc(rtn->block_length, sizeof(char *));

  // allocate the memory for each of the lines
  for (i = 0; i < rtn->block_length; i++) {
    rtn->IDs[i] = (char *)calloc(MAX_READ_LENGTH, sizeof(char));
  }

  // Allocate (and initialize) the models for the IDs
  rtn->models = alloc_id_models_t();

  return rtn;
}

void free_id_block(id_block rtn) {
  free_id_models_t(rtn->models);
  for (uint32_t i = 0; i < rtn->block_length; i++) free(rtn->IDs[i]);
  free(rtn->IDs);
  free(rtn);
}

sam_block alloc_sam_models(  // Arithmetic_stream as,
    std::string *id_array, std::ifstream *f_order, uint32_t numreads) {
  sam_block sb = (sam_block)calloc(1, sizeof(struct sam_block_t));

  //   sb->fs = fin;

  //  sb->fref = fref;

  // initialize the codebook_model
  //  uint32_t rescale = 1 << 20;
  //  sb->codebook_model = initialize_stream_model_codebook(rescale);
  sb->id_array = id_array;
  sb->f_order = f_order;
  sb->numreads = numreads;
  sb->current_read_number = 0;
  // IDs
  sb->IDs = alloc_id_block();

  return sb;
}

void free_sam_models(sam_block sb) {
  //  free_models_array(sb->codebook_model,256*4);
  free_id_block(sb->IDs);
  free(sb);
}

uint32_t load_sam_line(sam_block sb) {
  // old version
  /*
    uint32_t order;

    char *ID_line = *sb->IDs->IDs;

    // Read compulsory fields
    if (sb->current_read_number != sb->numreads) {
      sb->f_order->read((char *)&order, sizeof(uint32_t));
      strcpy(ID_line, (sb->id_array[order]).c_str());
      sb->current_read_number++;
      return 0;
    } else
      return 1;
  */

  char *ID_line = *sb->IDs->IDs;

  // Read compulsory fields
  if (sb->current_read_number != sb->numreads) {
    strcpy(ID_line, (sb->id_array[sb->current_read_number]).c_str());
    sb->current_read_number++;
    return 0;
  } else
    return 1;
}

}  // namespace id_comp
}  // namespace spring
