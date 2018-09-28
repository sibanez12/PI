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

#ifndef PISDNET_TABLES_H_
#define PISDNET_TABLES_H_

#include <exception>

#include "PI/pi.h"

#include "common.h"
#include "cam.h"
#include "bcam.h"
#include "stcam.h"
#include "tcam.h"

typedef struct bcam bcam_t;
typedef struct stcam stcam_t;
typedef struct tcam tcam_t;

namespace pisdnet {

  //! Baseline SDNet table
  class Table {
    protected:
      // static attributes
      static uint32_t MAX_FORMAT_LEN = 100; // characters in fmat string
      static uint32_t MAX_ENTRIES = 1024;
      static uint16_t CORE_MHZ = 600;
      static uint16_t LOOKUP_MHZ = 150;

      //! latest p4info
      const pi_p4info_t* p4info;

      //! format string for the table
      char* format;

      uint16_t response_width;
      uint16_t key_width;

      //! FIFO structure to track available entry handles
      std::queue<pi_entry_handle_t> entry_handle_free_list;

      std::map<pi_entry_handle_t, >

    public:
      //! Base table constructor
      Table(const pi_p4info_t* p4info);

      //! Base table destructor
      ~Table();

      //! Common table seup operations
      void setup_table(cam_arg_t* cam_arg);

      //! Initialize the key data
      void init_key_data();

      //! Initialize the response data
      void init_response_data();

      //! Initialize entry handle free list
      void init_entry_handle_free_list();

      //! Build the key that will be passed to Memobit API 
      void set_key(const pi_match_key_t *match_key, uint8_t *key);

      //! Build the response value that will be passed to Memobit API
      void set_response(const pi_table_entry_t *table_entry, uint8_t *response);

      //! Get an available entry_handle
      pi_entry_handle_t get_free_entry_handle();

      //! overridden by specific add_entry for table type
      virtual pi_entry_handle_t add_entry(const pi_match_key_t *match_key,
                                          const pi_table_entry_t *table_entry);
  }

   //! SDNet exact match table
  class BcamTable : public Table {
    protected:
      //! pointer to Memobit bcam table struct
      bcam_t* bcam;

      //! The format of a table entry
      typedef struct {
        uint8_t *key;
        uint8_t *response;
      } bcam_entry_t;

      // The software instance of the table: map entry_handle to entry
      std::map<pi_entry_handle_t, bcam_entry_t> bcam_table;

    public:
      //! Bcam table constructor
      BcamTable(const pi_p4info_t* p4info);
      //! Bcam table destructor
      ~BcamTable();
      pi_entry_handle_t add_entry(const pi_match_key_t *match_key,
                                  const pi_table_entry_t *table_entry);

      //! Allocate a bcam table entry
      bcam_entry_t create_entry(const pi_match_key_t *match_key,
                                const pi_table_entry_t *table_entry);
      //! Destroy bcam table entry
      void destroy_entry(bcam_entry_t e);

  }

  //! SDNet LPM table
  class StcamTable;

  //! SDNet ternary table
  class TcamTable;

  //! Exception class for table operations
  class InvalidTableOperation: public exception {
    public:
      status_t code;

      // TODO: move implementation to cpp file?
      virtual const char* what() const throw() {
        return "My exception happened";
      }
  };

}


#endif  // PISDNET_TABLES_H_
