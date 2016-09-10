/**
 * @file DebugState.h
 *
 * @date Sep 10, 2016
 * @author edward
 */

#pragma once

#include <spdlog/logger.h>
#include <memory>

#include "../simulation/SimParameters.h"

namespace pddm {

namespace util {

struct DebugState {
        int num_finished_shuffle;
        int num_finished_echo;
        int num_finished_agreement;
        int num_finished_aggregate;
};

DebugState& debug_state();

inline void init_debug_state() {
    debug_state().num_finished_shuffle = 0;
    debug_state().num_finished_echo = 0;
    debug_state().num_finished_agreement = 0;
    debug_state().num_finished_aggregate = 0;
}

inline void print_shuffle_status(const std::shared_ptr<spdlog::logger>& logger, const int num_meters) {
    if(debug_state().num_finished_shuffle == num_meters - simulation::METER_FAILURES_PER_QUERY) {
        logger->debug("All meters are finished with Shuffle");
    }
}

inline void print_echo_status(const std::shared_ptr<spdlog::logger>& logger, const int meter_id, const int num_meters) {
    if(debug_state().num_finished_shuffle < num_meters - simulation::METER_FAILURES_PER_QUERY) {
        logger->warn("Meter {} finished with Echo, but {} meters are still in Shuffle phase!", meter_id, num_meters - debug_state().num_finished_shuffle - simulation::METER_FAILURES_PER_QUERY);
    }
    if(debug_state().num_finished_echo == num_meters - simulation::METER_FAILURES_PER_QUERY) {
        logger->debug("All meters are finished with Echo");
    }
}

inline void print_agreement_status(const std::shared_ptr<spdlog::logger>& logger, const int meter_id, const int num_meters) {
    if(debug_state().num_finished_shuffle < num_meters - simulation::METER_FAILURES_PER_QUERY) {
        logger->warn("Meter {} finished with Agreement, but {} meters are still in Shuffle phase!", meter_id, num_meters - debug_state().num_finished_shuffle - simulation::METER_FAILURES_PER_QUERY);
    }
    if(debug_state().num_finished_agreement == num_meters - simulation::METER_FAILURES_PER_QUERY) {
        logger->debug("All meters are finished with Agreement");
    }
}

inline void print_aggregate_status(const std::shared_ptr<spdlog::logger>& logger, const int num_meters) {
    if(debug_state().num_finished_aggregate == num_meters - simulation::METER_FAILURES_PER_QUERY) {
        logger->debug("All meters are finished with Aggregation");
    }
}


}
}

