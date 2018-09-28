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

#ifndef PISDNET_COMMON_H_
#define PISDNET_COMMON_H_

#include <stdbool.h>
#include <stdint.h>

//! The different status codes for pisdnet functions 
typedef enum {
  PISDNET_STATUS_SUCCESS = 0
} pisdnet_status_t;

//! container for info about a device
typedef struct {
  const pi_p4info_t *p4info;
} pisdnet_dev_info_t;


pisdnet_status_t pisdnet_get_dev_info(pi_dev_id_t dev_id,
                                      pisdnet_dev_info_t *dev_info);


#endif  // PISDNET_COMMON_H_
