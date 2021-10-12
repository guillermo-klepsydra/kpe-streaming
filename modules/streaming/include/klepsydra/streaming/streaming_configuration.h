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
#include <sstream>

namespace kpsr {
namespace streaming {

const std::string POOL_SIZE = "pool_size";
const std::string NUMBER_OF_CORES = "number_of_cores";
const std::string NUMBER_OF_EVENT_LOOPS = "number_of_event_loops";
const std::string NUMBER_OF_PARALLEL_THREADS = "number_of_parallel_threads";
const std::string EVENT_LOOP_CORE_MAP = "event_loop_core_map";
const std::string LAYER_EVENT_LOOP_MAP = "layer_event_loop_map";
const std::string PARALLISED_LAYERS = "parallised_layers";
    
    
class StreamingConfiguration {
public:

    StreamingConfiguration();
    StreamingConfiguration(int poolSize,
                           size_t numberOfCores,
                           size_t numberOfEventLoops,
                           size_t nonCriticalThreadPoolSize,
                           int numberOfParallelThreads,
                           const std::vector<std::string> & parallelisedSteps);

    StreamingConfiguration(const std::string& jsonFileName);

    int poolSize;
    size_t numberOfCores;
    size_t numberOfEventLoops;
    size_t nonCriticalThreadPoolSize;
    int numberOfParallelThreads;
    std::map<size_t, std::vector<int>> eventLoopCoreMap;
    std::map<std::string, size_t> stepIDEventLoopMap;
    std::vector<std::string> parallelisedSteps;

    bool operator==(const StreamingConfiguration & rhs) const;

    void loadJsonString(const std::string& jsonString);
    std::string exportJsonString();
private:
    void loadJsonStream(std::istream& jsonStream);

    template <class Archive>
    void serialize(Archive& archive);
};
}
}

#endif // STREAMING_CONFIGURATION_H
