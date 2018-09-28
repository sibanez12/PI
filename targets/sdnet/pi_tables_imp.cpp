/* Copyright 2013-present Barefoot Networks, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Stephen Ibanez (sibanez@stanford.edu)
 */

#include <PI/int/pi_int.h>
#include <PI/int/serialize.h>
#include <PI/p4info.h>
#include <PI/pi.h>

#include <iostream>
#include <string>

#include <cstring>

#include "common.h"
#include "tables.h"

extern "C" {

// ------------- PI table API functions --------------- //

pi_status_t _pi_table_entry_add(pi_session_handle_t session_handle,
                                pi_dev_tgt_t dev_tgt,
                                pi_p4_id_t table_id,
                                const pi_match_key_t *match_key,
                                const pi_table_entry_t *table_entry,
                                int overwrite,
                                pi_entry_handle_t *entry_handle) {
  (void) overwrite;      // unused 
  (void) session_handle; // unused

  pisdnet::Device *dev = pisdnet::get_device(dev_tgt.dev_id);
  pisdnet::Table *table = dev->get_table(table_id);
  const pi_p4info_t *p4info = dev->get_p4info();

  try {
    switch (table_entry->entry_type) {
      case PI_ACTION_ENTRY_TYPE_DATA:
        // add table entry and set entry handle
        *entry_handle = table->add_entry(p4info, match_key, table_entry)
        break;
      // TODO: support indirect table entries?
      default:
        assert(0);
    }
  } catch (pisdnet::InvalidTableOperation &ito) {
    std::cout << "pisdnet: Invalid table operation: "
              << ito.what() << std::endl;
    return static_cast<pi_status_t>(PI_STATUS_TARGET_ERROR + ito.code);
  }

  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_table_default_action_set(pi_session_handle_t session_handle,
                                         pi_dev_tgt_t dev_tgt,
                                         pi_p4_id_t table_id,
                                         const pi_table_entry_t *table_entry) {
  (void)session_handle;
  (void)dev_tgt;
  (void)table_id;
  (void)table_entry;
  func_counter_increment(__func__);
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_table_default_action_reset(pi_session_handle_t session_handle,
                                           pi_dev_tgt_t dev_tgt,
                                           pi_p4_id_t table_id) {
  (void)session_handle;
  (void)dev_tgt;
  (void)table_id;
  func_counter_increment(__func__);
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_table_default_action_get(pi_session_handle_t session_handle,
                                         pi_dev_id_t dev_id,
                                         pi_p4_id_t table_id,
                                         pi_table_entry_t *table_entry) {
  (void)session_handle;
  (void)dev_id;
  (void)table_id;
  (void)table_entry;
  func_counter_increment(__func__);
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_table_default_action_done(pi_session_handle_t session_handle,
                                          pi_table_entry_t *table_entry) {
  (void)session_handle;
  (void)table_entry;
  func_counter_increment(__func__);
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_table_entry_delete(pi_session_handle_t session_handle,
                                   pi_dev_id_t dev_id,
                                   pi_p4_id_t table_id,
                                   pi_entry_handle_t entry_handle) {
  (void)session_handle;
  (void)dev_id;
  (void)table_id;
  (void)entry_handle;
  func_counter_increment(__func__);
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_table_entry_delete_wkey(pi_session_handle_t session_handle,
                                        pi_dev_id_t dev_id, pi_p4_id_t table_id,
                                        const pi_match_key_t *match_key) {
  (void)session_handle;
  (void)dev_id;
  (void)table_id;
  (void)match_key;
  func_counter_increment(__func__);
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_table_entry_modify(pi_session_handle_t session_handle,
                                   pi_dev_id_t dev_id,
                                   pi_p4_id_t table_id,
                                   pi_entry_handle_t entry_handle,
                                   const pi_table_entry_t *table_entry) {
  (void)session_handle;
  (void)dev_id;
  (void)table_id;
  (void)entry_handle;
  (void)table_entry;
  func_counter_increment(__func__);
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_table_entry_modify_wkey(pi_session_handle_t session_handle,
                                        pi_dev_id_t dev_id, pi_p4_id_t table_id,
                                        const pi_match_key_t *match_key,
                                        const pi_table_entry_t *table_entry) {
  (void)session_handle;
  (void)dev_id;
  (void)table_id;
  (void)match_key;
  (void)table_entry;
  func_counter_increment(__func__);
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_table_entries_fetch(pi_session_handle_t session_handle,
                                    pi_dev_id_t dev_id,
                                    pi_p4_id_t table_id,
                                    pi_table_fetch_res_t *res) {
  (void)session_handle;
  (void)dev_id;
  (void)table_id;
  (void)res;
  func_counter_increment(__func__);
  return PI_STATUS_SUCCESS;
}

pi_status_t _pi_table_entries_fetch_done(pi_session_handle_t session_handle,
                                         pi_table_fetch_res_t *res) {
  (void)session_handle;
  (void)res;
  func_counter_increment(__func__);
  return PI_STATUS_SUCCESS;
}

}
