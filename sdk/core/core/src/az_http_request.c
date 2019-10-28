// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#include <az_http_request.h>

#include <az_http_query.h>
#include <az_http_header.h>
#include <az_contract.h>
#include <az_span_builder.h>
#include <az_span_emitter.h>
#include <az_str.h>

#include <stdlib.h>

#include <_az_cfg.h>

#define AZ_CRLF "\r\n"

static az_span const az_crlf = AZ_CONST_STR(AZ_CRLF);

AZ_NODISCARD az_result
az_http_header_line(az_span_action const * const self, az_pair const header) {
  AZ_CONTRACT_ARG_NOT_NULL(self);

  AZ_RETURN_IF_FAILED(az_http_header_emit_span_seq(&header, *self));
  AZ_RETURN_IF_FAILED(az_span_action_do(*self, az_crlf));
  return AZ_OK;
}

AZ_ACTION_FUNC(az_http_header_line, az_span_action const, az_pair_action)

AZ_NODISCARD az_result
az_http_request_emit_span_seq(az_http_request const * const self, az_span_action const span_action) {
  AZ_CONTRACT_ARG_NOT_NULL(self);

  // a request line
  {
    AZ_RETURN_IF_FAILED(az_span_action_do(span_action, self->method));
    AZ_RETURN_IF_FAILED(az_span_action_do(span_action, AZ_STR(" ")));
    AZ_RETURN_IF_FAILED(az_span_action_do(span_action, self->path));
    AZ_RETURN_IF_FAILED(az_http_query_emit_span_seq(self->query, span_action));
    AZ_RETURN_IF_FAILED(az_span_action_do(span_action, AZ_STR(" HTTP/1.1" AZ_CRLF)));
  }

  // headers
  AZ_RETURN_IF_FAILED(az_pair_emitter_do(self->headers, az_http_header_line_action(&span_action)));

  // an empty line
  AZ_RETURN_IF_FAILED(az_span_action_do(span_action, az_crlf));

  // body.
  AZ_RETURN_IF_FAILED(az_span_action_do(span_action, self->body));

  return AZ_OK;
}

/*
AZ_NODISCARD az_result
az_build_url(az_http_request const * const p_request, az_span_append const append) {
  AZ_CONTRACT_ARG_NOT_NULL(p_request);

  // host path
  AZ_RETURN_IF_FAILED(az_span_append_do(append, p_request->path));
  // query parameters
  {
    az_query_state state = {
      .append = append,
      .separator = AZ_STR("?"),
    };
    // for each query parameter apply `az_query_to_spans`.
    AZ_RETURN_IF_FAILED(az_pair_seq_do(p_request->query, az_build_query_param_callback(&state)));
  }
  return AZ_OK;
}

AZ_CALLBACK_FUNC(az_build_url, az_http_request const *, az_span_seq)

AZ_NODISCARD az_result az_http_get_url_size(az_http_request const * const p_request, size_t * out) {
  return az_span_seq_size(az_build_url_callback(p_request), out);
}

AZ_NODISCARD az_result
az_http_url_to_new_str(az_http_request const * const p_request, char ** const out) {
  return az_span_seq_to_new_str(az_build_url_callback(p_request), out);
}
*/