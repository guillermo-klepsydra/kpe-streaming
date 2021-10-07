/*****************************************************************************
*                           Klepsydra Streaming Modules
*              Copyright (C) 2020-2022  Klepsydra Technologies GmbH
*                            All Rights Reserved.
*
*  This file is subject to the terms and conditions defined in
*  file 'LICENSE.md', which is part of this source code package.
*
*  NOTICE:  All information contained herein is, and remains the property of Klepsydra
*  Technologies GmbH and its suppliers, if any. The intellectual and technical concepts
*  contained herein are proprietary to Klepsydra Technologies GmbH and its suppliers and
*  may be covered by Swiss and Foreign Patents, patents in process, and are protected by
*  trade secret or copyright law. Dissemination of this information or reproduction of
*  this material is strictly forbidden unless prior written permission is obtained from
*  Klepsydra Technologies GmbH.
*
*****************************************************************************/

#ifndef STREAMING_CONFIGURATION_H
#define STREAMING_CONFIGURATION_H

#include <klepsydra/streaming/visibility.h>
#include <klepsydra/streaming/streaming_types.h>

#include <map>
#include <vector>

#include <yaml-cpp/yaml.h>

namespace kpsr {
namespace streaming {

class StreamingConfiguration {
public:

    StreamingConfiguration();
    StreamingConfiguration(int poolSize,
                           size_t numberOfCores,
                           size_t numberOfEventLoops,
                           size_t nonCriticalThreadPoolSize,
                           int numberOfParallelThreads,
                           const std::vector<std::string> & parallelisedSteps);

    int poolSize;
    size_t numberOfCores;
    size_t numberOfEventLoops;
    size_t nonCriticalThreadPoolSize;
    int numberOfParallelThreads;
    std::map<size_t, std::vector<int>> eventLoopCoreMap;
    std::map<std::string, size_t> stepIDEventLoopMap;
    std::vector<std::string> parallelisedSteps;

    bool operator==(const StreamingConfiguration & rhs) const;
};
}
}

namespace YAML {
template<>
struct convert<kpsr::streaming::StreamingConfiguration> {
    static Node encode(const kpsr::streaming::StreamingConfiguration& rhs) {
        Node node;
        node["pool_size"] = rhs.poolSize;
        node["number_of_cores"] = rhs.numberOfCores;
        node["number_of_event_loops"] = rhs.numberOfEventLoops;
        node["non_critical_thread_pool_size"] = rhs.nonCriticalThreadPoolSize;
        node["number_of_parallel_threads"] = rhs.numberOfParallelThreads;
        node["event_loop_core_map"] = rhs.eventLoopCoreMap;
        node["layer_event_loop_map"] = rhs.stepIDEventLoopMap;
        node["parallised_layers"] = rhs.parallelisedSteps;
        return node;
    }

    static bool decode(const Node& node, kpsr::streaming::StreamingConfiguration& rhs) {
        rhs.poolSize = node["pool_size"].as<int>();
        rhs.numberOfCores = node["number_of_cores"].as<size_t>();
        rhs.numberOfEventLoops = node["number_of_event_loops"].as<size_t>();
        rhs.nonCriticalThreadPoolSize = node["non_critical_thread_pool_size"].as<size_t>();
        rhs.numberOfParallelThreads = node["number_of_parallel_threads"].as<int>();
        rhs.eventLoopCoreMap = node["event_loop_core_map"].as<std::map<size_t, std::vector<int>>>();
        rhs.stepIDEventLoopMap = node["layer_event_loop_map"].as<std::map<std::string, size_t>>();
        rhs.parallelisedSteps = node["parallised_layers"].as<std::vector<std::string>>();

        return true;
    }
};
}
#endif // STREAMING_CONFIGURATION_H
