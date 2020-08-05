#pragma once

#include "cluster/topic_table.h"
#include "cluster/topic_updates_dispatcher.h"
#include "raft/mux_state_machine.h"

namespace cluster {

// single instance
using controller_stm = raft::mux_state_machine<topic_updates_dispatcher>;

static constexpr ss::shard_id controller_stm_shard = 0;

} // namespace cluster