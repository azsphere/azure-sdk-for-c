// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#include <az_curl_adapter.h>

#include <az_action.h>
#include <az_http_header.h>

#include <_az_cfg.h>

/**
 * Adds a header string to the given CURL list.
 */
AZ_NODISCARD az_result
az_http_header_str_to_curl(struct curl_slist ** const pp_list, char const * str) {
  AZ_CONTRACT_ARG_NOT_NULL(pp_list);
  AZ_CONTRACT_ARG_NOT_NULL(str);

  struct curl_slist * const p_list = curl_slist_append(*pp_list, str);
  if (p_list == NULL) {
    return AZ_ERROR_HTTP_UNKNOWN;
  }
  *pp_list = p_list;
  return AZ_OK;
}

AZ_ACTION_FUNC(az_http_header_str_to_curl, struct curl_slist *, az_str_action)

/**
 * Adds a header to the given CURL list.
 */
AZ_NODISCARD az_result az_http_header_to_curl(struct curl_slist ** pp_list, az_pair const header) {
  AZ_CONTRACT_ARG_NOT_NULL(pp_list);

  return az_span_emitter_to_tmp_str(
      az_http_header_emit_span_seq_action(&header), az_http_header_str_to_curl_action(pp_list));
}