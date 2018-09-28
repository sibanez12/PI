//
// Copyright (c) 2018 Stephen Ibanez
// All rights reserved.
//
// This software was developed by Stanford University and the University of Cambridge Computer Laboratory 
// under National Science Foundation under Grant No. CNS-0855268,
// the University of Cambridge Computer Laboratory under EPSRC INTERNET Project EP/H040536/1 and
// by the University of Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-11-C-0249 ("MRC2"), 
// as part of the DARPA MRC research programme.
//
// @NETFPGA_LICENSE_HEADER_START@
//
// Licensed to NetFPGA C.I.C. (NetFPGA) under one or more contributor
// license agreements.  See the NOTICE file distributed with this work for
// additional information regarding copyright ownership.  NetFPGA licenses this
// file to you under the NetFPGA Hardware-Software License, Version 1.0 (the
// "License"); you may not use this file except in compliance with the
// License.  You may obtain a copy of the License at:
//
//   http://www.netfpga-cic.org
//
// Unless required by applicable law or agreed to in writing, Work distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations under the License.
//
// @NETFPGA_LICENSE_HEADER_END@
//

/*
 * Stephen Ibanez (sibanez@stanford.edu)
 *
 */

//! @file

#include "PI/pi.h"

#include <iostream>

#include "common.h"
#include "tables.h"
#include "cam.h"
#include "bcam.h"
#include "stcam.h"
#include "tcam.h"

////////// pisdnet Table methods /////////

//! Constructor of base table class
pisdnet::Table::Table(const pi_p4info_t* p4info) {
  this->p4info = p4info;
  this->init_key_data();
  this->init_response_data();
  this->init_entry_handle_free_list();
}

//! Destructor of the base table class
pisdnet::Table::~Table() {
  delete[] this->format;
  delete[] this->key;
  delete[] this->response;
}

//! Common table setup operations
void pisdnet::Table::setup_table(cam_arg_t* cam_arg) {
  // local variables
  int rc;

  // Set mandatory arguments
  rc = cam_arg_set_format(cam_arg, this->format);
  if (rc)
      cam_print_error(rc);

  cam_arg_set_num_entries(cam_arg, pisdnet::Table::MAX_ENTRIES);
  this->set_resp_width();
  cam_arg_set_response_width(cam_arg, this->resp_width);
  cam_arg_set_engine_freq(cam_arg, pisdnet::Table::CORE_MHZ);
  cam_arg_set_engine_lookup_rate(cam_arg, pisdnet::Table::LOOKUP_MHZ);

  // TODO: Set optional arguments?
  //uint8_t prio_width = static_cast<uint8_t>(std::log2(pisdnet::Table::MAX_ENTRIES));
  //cam_arg_force_prio_width(cam_arg, prio_width);
  //cam_arg_force_mem_type(cam_arg, KM_ADDR == 2048);
  //cam_arg_force_num_units(cam_arg, UNITS);

  // HW read/write functions
  cam_arg_set_hw_write_function(cam_arg, &axi_lite_wr);
  cam_arg_set_hw_read_function(cam_arg, &axi_lite_rd);

}

//! Create format string from p4info file
void pisdnet::Table::init_key_data() {
  // allocate buffer space for format string
  this->format = new char[pisdnet::Table::MAX_FORMAT_LEN];
  // TODO: set the format string
  // TODO: set the key_width (bytes)
  this->key_width =  
}

//! set response width from p4info file, allocate response buffer
void pisdnet::Table::init_response_data() {
  // TODO: set response width from p4info file (bytes)
  this->resp_width = 
} 

//! Initialize entry_handle_free_list
void pisdnet::Table::init_entry_handle_free_list() {
  // add a handle for each possible entry in the table
  for (auto i=0; i < pisdnet::Table:MAX_ENTRIES; i++) {
    this->entry_handle_free_list.push(static_cast<pi_entry_handle_t>(i));
  }
}

//! Get available entry handle
pi_entry_handle_t pisdnet::Table::get_free_entry_handle() {
  if (!this->entry_handle_free_list.empty()) {
    return this->entry_handle_free_list.pop();
  }
  else {
    // TODO: define exception to throw
    throw Exception("No available entry handles");
  }
}

//! Set the key buffer based on the provided match_key
void pisdnet::Table::set_key(const pi_match_key_t *match_key, uint8_t *key) {

}

//! Set the response buffer based on the provided table entry
void pisdnet::Table::set_response(const pi_table_entry_t *table_entry, uint8_t *response) {

}

////////// pisdnet BcamTable methods /////////

//! Constructor of Bcam tables
pisdnet::BcamTable::BcamTable(const pi_p4info_t* p4info) : pisdnet::Table(p4info) {
  // local variables
  cam_arg_t* cam_arg;
  int rc;

  // Allocate argument structure
  rc = cam_arg_create(&cam_arg);
  if (rc) cam_print_error(rc);

  // perform table setup
  this->setup_table(cam_arg);
  // Create software bcam instance
  rc = bcam_create(cam_arg, this->bcam);
  if (rc) cam_print_error(rc);

  // Deallocate argument structure
  rc = cam_arg_destroy(cam_arg);
  if (rc) cam_print_error(rc);
}

//! BcamTable Destructor
pisdnet::BcamTable::~BcamTable() {
  // Table class destructor is invoked automatically
  int rc;
  // cleanup the bcam software instance
  rc = bcam_destroy(this->bcam);
  if (rc) cam_print_error(rc);
}

//! Bcam table add entry method
pi_entry_handle_t pisdnet::BcamTable::add_entry(const pi_match_key_t *match_key,
                                                const pi_table_entry_t *table_entry) {
  int rc;

  bcam_entry_t bcam_entry = this->create_entry(match_key, table_entry);

  // Use Memobit API functions to add table entry
  rc = bcam_insert(this->bcam, bcam_entry.key, bcam_entry.response);
  if (rc) cam_print_error(rc);

  // create entry handle (which is just a uint64_t)
  pi_entry_handle_t handle = this->get_free_entry_handle();

  // Add entry to software table
  this->bcam_table[handle] = bcam_entry;

  return handle;
}

//! Allocate bcam table entry 
bcam_entry_t pisdnet::BcamTable::create_entry(const pi_match_key_t *match_key,
                                              const pi_table_entry_t *table_entry) {
  bcam_entry_t e;
  // allocate the key and response buffers
  e.key = new uint8_t[this->key_width];
  e.response = new uint8_t[this->response_width];
  // set the key and response buffers
  this->set_key(match_key, e.key);
  this->set_response(table_entry. e.response);
  return e;
}

//! Destroy bcam table entry 
void pisdnet::BcamTable::destroy_entry(bcam_entry_t e) {
  delete[] e.key;
  delete[] e.response;
}

