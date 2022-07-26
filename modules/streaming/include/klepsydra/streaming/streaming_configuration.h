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

#include <klepsydra/streaming/streaming_types.h>
#include <klepsydra/streaming/visibility.h>

#include <map>
#include <sstream>
#include <vector>

namespace kpsr {
namespace streaming {

class StreamingConfiguration
{
public:
    StreamingConfiguration();
    StreamingConfiguration(int poolSize,
                           size_t numberOfCores,
                           size_t numberOfEventLoops,
                           size_t nonCriticalThreadPoolSize,
                           int numberOfParallelThreads,
                           const std::vector<std::string> &parallelisedSteps);

    int poolSize;
    size_t numberOfCores;
    size_t numberOfEventLoops;
    size_t nonCriticalThreadPoolSize;
    int numberOfParallelThreads;
    std::vector<std::string> parallelisedSteps;
};
} // namespace streaming
} // namespace kpsr

#endif // STREAMING_CONFIGURATION_H