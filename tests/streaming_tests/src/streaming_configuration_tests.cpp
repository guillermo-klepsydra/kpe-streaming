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
#include "config.h"

#include "gtest/gtest.h"
#include <klepsydra/streaming/streaming_configuration.h>
#include <spdlog/spdlog.h>

TEST(StreamingConfiguration, DefaultConstructionTest)
{
    ASSERT_NO_THROW(kpsr::streaming::StreamingConfiguration dummyConfig);
}

TEST(StreamingConfiguration, ConstructionTest)
{
    int poolSize = 0;
    int numberOfCores = 2;
    int nonCriticalThreadPoolSize = 4;
    int numberOfParallelThreads = 2;
    std::vector<std::string> parallisedSteps = {};

    ASSERT_NO_THROW(kpsr::streaming::StreamingConfiguration dummyConfig(poolSize,
                                                                        numberOfCores,
                                                                        numberOfCores * 1,
                                                                        nonCriticalThreadPoolSize,
                                                                        numberOfParallelThreads,
                                                                        parallisedSteps));
}