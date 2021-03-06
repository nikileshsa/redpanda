/*
 * Copyright 2020 Vectorized, Inc.
 *
 * Use of this software is governed by the Business Source License
 * included in the file licenses/BSL.md
 *
 * As of the Change Date specified in that file, in accordance with
 * the Business Source License, use of this software will be governed
 * by the Apache License, Version 2.0
 */

#pragma once

#include "bytes/iobuf.h"
#include "seastarx.h"

#include <seastar/core/file.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/temporary_buffer.hh>

static inline ss::future<ss::temporary_buffer<char>>
read_fully_tmpbuf(ss::sstring name) {
    return ss::open_file_dma(std::move(name), ss::open_flags::ro)
      .then([](ss::file f) {
          return f.size()
            .then([f](uint64_t size) mutable {
                return f.dma_read_bulk<char>(0, size);
            })
            .then([f](ss::temporary_buffer<char> buf) mutable {
                return f.close().then([f, buf = std::move(buf)]() mutable {
                    return std::move(buf);
                });
            });
      });
}
static inline ss::future<iobuf> read_fully(ss::sstring name) {
    return read_fully_tmpbuf(std::move(name))
      .then([](ss::temporary_buffer<char> buf) {
          iobuf iob;
          iob.append(std::move(buf));
          return iob;
      });
}
