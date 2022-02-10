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
#include <klepsydra/mem_core/mem_env.h>
#include <klepsydra/streaming/default_thread_distribution_policy.h>
#include <klepsydra/streaming/default_thread_distribution_policy_factory_impl.h>
#include <klepsydra/streaming/json_thread_distribution_policy.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>

#include <spdlog/spdlog.h>

TEST(StreamingConfigurationManager, ConstructorWithDefaultPolicyTest)
{
    int poolSize = 0;
    size_t numberOfCores = 2;
    size_t numberOfEventLoops = numberOfCores * 1;
    size_t nonCriticalThreadPoolSize = 4;
    int numberOfParallelThreads = 2;
    std::vector<std::string> parallisedSteps = {};

    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("number_of_cores", numberOfCores);
    kpsr::streaming::DefaultThreadDistributionPolicyFactoryImpl
        defaultThreadDistributionPolicyFactoryImpl;
    std::shared_ptr<kpsr::streaming::ThreadDistributionPolicy> threadDistributionPolicy;
    ASSERT_NO_THROW(threadDistributionPolicy = defaultThreadDistributionPolicyFactoryImpl
                                                   .createThreadDistributionPolicy(&environment));

    ASSERT_NO_THROW(kpsr::streaming::StreamingConfigurationManager
                        streamingConfigurationManager(poolSize,
                                                      numberOfCores,
                                                      numberOfEventLoops,
                                                      nonCriticalThreadPoolSize,
                                                      numberOfParallelThreads,
                                                      parallisedSteps,
                                                      threadDistributionPolicy));
}

TEST(StreamingConfigurationManager, ConstructionWithDefaultPolicyWithRegularDistributionTest)
{
    int poolSize = 0;
    size_t numberOfCores = 2;
    size_t numberOfEventLoops = numberOfCores * 1;
    size_t nonCriticalThreadPoolSize = 4;
    int numberOfParallelThreads = 2;
    std::vector<std::string> parallisedSteps = {};

    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("number_of_cores", numberOfCores);
    kpsr::streaming::DefaultThreadDistributionPolicyFactoryImpl
        defaultThreadDistributionPolicyFactoryImpl;
    auto threadDistributionPolicy = defaultThreadDistributionPolicyFactoryImpl
                                        .createThreadDistributionPolicy(&environment);

    kpsr::streaming::StreamingConfigurationManager
        streamingConfigurationManager(poolSize,
                                      numberOfCores,
                                      numberOfEventLoops,
                                      nonCriticalThreadPoolSize,
                                      numberOfParallelThreads,
                                      parallisedSteps,
                                      threadDistributionPolicy);

    size_t numberOfSteps = 10;
    for (size_t i = 0; i < numberOfSteps; i++) {
        streamingConfigurationManager.addStepToEventLoop("step-" + std::to_string(i));
    }

    const std::vector<size_t> expectedIdEventLoop = {0, 1};
    const std::vector<int> expectedIdCore = {0, 1};
    int ctrPos = 0;
    for (auto eventLoopCoreMap :
         streamingConfigurationManager.getThreadDistributionPolicy()->eventLoopCoreMap) {
        ASSERT_EQ(eventLoopCoreMap.first, expectedIdEventLoop.at(ctrPos));
        ASSERT_EQ(eventLoopCoreMap.second.at(0), expectedIdCore.at(ctrPos));
        ctrPos++;
    }

    ctrPos = 0;
    for (auto stepIDEventLoopMap :
         streamingConfigurationManager.getThreadDistributionPolicy()->stepIDEventLoopMap) {
        ASSERT_EQ(stepIDEventLoopMap.first, "step-" + std::to_string(ctrPos));
        ASSERT_EQ(stepIDEventLoopMap.second, expectedIdEventLoop.at(ctrPos % 2));
        ctrPos++;
    }
}

TEST(StreamingConfigurationManager, ConstructionWithDefaultPolicyWithIrregularDistributionTest)
{
    int poolSize = 0;
    int numberOfCores = 3;
    size_t numberOfEventLoops = numberOfCores * 1;
    int nonCriticalThreadPoolSize = 4;
    int numberOfParallelThreads = 2;
    std::vector<std::string> parallisedSteps = {};

    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("number_of_cores", numberOfCores);
    kpsr::streaming::DefaultThreadDistributionPolicyFactoryImpl
        defaultThreadDistributionPolicyFactoryImpl;
    auto threadDistributionPolicy = defaultThreadDistributionPolicyFactoryImpl
                                        .createThreadDistributionPolicy(&environment);

    kpsr::streaming::StreamingConfigurationManager
        streamingConfigurationManager(poolSize,
                                      numberOfCores,
                                      numberOfEventLoops,
                                      nonCriticalThreadPoolSize,
                                      numberOfParallelThreads,
                                      parallisedSteps,
                                      threadDistributionPolicy);

    size_t numberOfSteps = 8;
    for (size_t i = 0; i < numberOfSteps; i++) {
        streamingConfigurationManager.addStepToEventLoop("step-" + std::to_string(i));
    }

    const std::vector<size_t> expectedIdEventLoop = {0, 1, 2, 0, 1, 2, 0, 1};
    const std::vector<int> expectedIdCore = {0, 1, 2};
    int ctrPos = 0;
    for (auto eventLoopCoreMap :
         streamingConfigurationManager.getThreadDistributionPolicy()->eventLoopCoreMap) {
        ASSERT_EQ(eventLoopCoreMap.first, expectedIdEventLoop.at(ctrPos));
        ASSERT_EQ(eventLoopCoreMap.second.at(0), expectedIdCore.at(ctrPos));
        ctrPos++;
    }

    ctrPos = 0;
    for (auto stepIDEventLoopMap :
         streamingConfigurationManager.getThreadDistributionPolicy()->stepIDEventLoopMap) {
        ASSERT_EQ(stepIDEventLoopMap.first, "step-" + std::to_string(ctrPos));
        ASSERT_EQ(stepIDEventLoopMap.second, expectedIdEventLoop.at(ctrPos));
        ctrPos++;
    }
}

TEST(StreamingConfigurationManager, JsonExportTest)
{
    int poolSize = 0;
    size_t numberOfCores = 2;
    size_t numberOfEventLoops = numberOfCores * 1;
    size_t nonCriticalThreadPoolSize = 4;
    int numberOfParallelThreads = 2;
    std::vector<std::string> parallisedSteps = {};

    auto dummyThreadDistributionPolicy =
        std::make_shared<kpsr::streaming::JsonThreadDistributionPolicy>();

    kpsr::streaming::StreamingConfigurationManager
        dummyStreamingConfigurationManager(poolSize,
                                           numberOfCores,
                                           numberOfEventLoops,
                                           nonCriticalThreadPoolSize,
                                           numberOfParallelThreads,
                                           parallisedSteps,
                                           dummyThreadDistributionPolicy);

    std::string jsonExport;
    ASSERT_NO_THROW(jsonExport = dummyStreamingConfigurationManager.exportJsonString());
    ASSERT_FALSE(jsonExport.empty());
}

TEST(StreamingConfigurationManager, JsonExportWithMapsTest)
{
    int poolSize = 0;
    size_t numberOfCores = 2;
    size_t numberOfEventLoops = numberOfCores * 1;
    size_t nonCriticalThreadPoolSize = 4;
    int numberOfParallelThreads = 2;
    std::vector<std::string> parallisedSteps = {};

    auto dummyThreadDistributionPolicy =
        std::make_shared<kpsr::streaming::JsonThreadDistributionPolicy>();

    kpsr::streaming::StreamingConfigurationManager
        dummyStreamingConfigurationManager(poolSize,
                                           numberOfCores,
                                           numberOfEventLoops,
                                           nonCriticalThreadPoolSize,
                                           numberOfParallelThreads,
                                           parallisedSteps,
                                           dummyThreadDistributionPolicy);

    dummyStreamingConfigurationManager.getThreadDistributionPolicy()->eventLoopCoreMap[0] =
        std::vector<int>{0};
    dummyStreamingConfigurationManager.getThreadDistributionPolicy()->eventLoopCoreMap[1] =
        std::vector<int>{1};

    dummyStreamingConfigurationManager.getThreadDistributionPolicy()->stepIDEventLoopMap["test0"] = 0;
    dummyStreamingConfigurationManager.getThreadDistributionPolicy()->stepIDEventLoopMap["test1"] = 1;
    dummyStreamingConfigurationManager.getThreadDistributionPolicy()->stepIDEventLoopMap["test2"] = 0;

    std::string jsonExport;
    ASSERT_NO_THROW(jsonExport = dummyStreamingConfigurationManager.exportJsonString());
    ASSERT_FALSE(jsonExport.empty());
}

TEST(StreamingConfigurationManager, JsonExportLoadTest)
{
    int poolSize = 0;
    size_t numberOfCores = 2;
    size_t numberOfEventLoops = numberOfCores * 1;
    size_t nonCriticalThreadPoolSize = 4;
    int numberOfParallelThreads = 2;
    std::vector<std::string> parallisedSteps = {};

    auto dummyThreadDistributionPolicy =
        std::make_shared<kpsr::streaming::JsonThreadDistributionPolicy>();

    kpsr::streaming::StreamingConfigurationManager
        dummyStreamingConfigurationManager(poolSize,
                                           numberOfCores,
                                           numberOfEventLoops,
                                           nonCriticalThreadPoolSize,
                                           numberOfParallelThreads,
                                           parallisedSteps,
                                           dummyThreadDistributionPolicy);

    std::string jsonExport;
    ASSERT_NO_THROW(jsonExport = dummyStreamingConfigurationManager.exportJsonString());
    ASSERT_FALSE(jsonExport.empty());

    auto copyThreadDistributionPolicy =
        std::make_shared<kpsr::streaming::JsonThreadDistributionPolicy>();
    kpsr::streaming::StreamingConfigurationManager copyStreamingConfigurationManager(
        0, 0, 0, 0, 0, parallisedSteps, copyThreadDistributionPolicy);
    ASSERT_NO_THROW(copyStreamingConfigurationManager.loadJsonString(jsonExport));
    ASSERT_EQ(copyStreamingConfigurationManager, dummyStreamingConfigurationManager);
}

TEST(StreamingConfigurationManager, JsonExportLoadWithMapsTest)
{
    int poolSize = 0;
    size_t numberOfCores = 2;
    size_t numberOfEventLoops = numberOfCores * 1;
    size_t nonCriticalThreadPoolSize = 4;
    int numberOfParallelThreads = 2;
    std::vector<std::string> parallisedSteps = {};

    auto dummyThreadDistributionPolicy =
        std::make_shared<kpsr::streaming::JsonThreadDistributionPolicy>();

    kpsr::streaming::StreamingConfigurationManager
        dummyStreamingConfigurationManager(poolSize,
                                           numberOfCores,
                                           numberOfEventLoops,
                                           nonCriticalThreadPoolSize,
                                           numberOfParallelThreads,
                                           parallisedSteps,
                                           dummyThreadDistributionPolicy);

    dummyStreamingConfigurationManager.getThreadDistributionPolicy()->eventLoopCoreMap[0] =
        std::vector<int>{0};
    dummyStreamingConfigurationManager.getThreadDistributionPolicy()->eventLoopCoreMap[1] =
        std::vector<int>{1};

    dummyStreamingConfigurationManager.getThreadDistributionPolicy()->stepIDEventLoopMap["test0"] = 0;
    dummyStreamingConfigurationManager.getThreadDistributionPolicy()->stepIDEventLoopMap["test1"] = 1;
    dummyStreamingConfigurationManager.getThreadDistributionPolicy()->stepIDEventLoopMap["test2"] = 0;

    std::string jsonExport;
    ASSERT_NO_THROW(jsonExport = dummyStreamingConfigurationManager.exportJsonString());
    ASSERT_FALSE(jsonExport.empty());

    auto copyThreadDistributionPolicy =
        std::make_shared<kpsr::streaming::JsonThreadDistributionPolicy>();
    kpsr::streaming::StreamingConfigurationManager copyStreamingConfigurationManager(
        0, 0, 0, 0, 0, parallisedSteps, copyThreadDistributionPolicy);
    ASSERT_NO_THROW(copyStreamingConfigurationManager.loadJsonString(jsonExport));
    ASSERT_EQ(copyStreamingConfigurationManager, dummyStreamingConfigurationManager);

    ASSERT_EQ(0, copyStreamingConfigurationManager.addStepToEventLoop("test0"));
    ASSERT_EQ(1, copyStreamingConfigurationManager.addStepToEventLoop("test1"));
    ASSERT_EQ(0, copyStreamingConfigurationManager.addStepToEventLoop("test2"));

    const std::vector<size_t> expectedIdEventLoop = {0, 1};
    const std::vector<int> expectedIdCore = {0, 1};
    int ctrPos = 0;
    for (auto eventLoopCoreMap :
         copyStreamingConfigurationManager.getThreadDistributionPolicy()->eventLoopCoreMap) {
        ASSERT_EQ(eventLoopCoreMap.first, expectedIdEventLoop.at(ctrPos));
        ASSERT_EQ(eventLoopCoreMap.second.at(0), expectedIdCore.at(ctrPos));
        ctrPos++;
    }

    ctrPos = 0;
    for (auto stepIDEventLoopMap :
         copyStreamingConfigurationManager.getThreadDistributionPolicy()->stepIDEventLoopMap) {
        ASSERT_EQ(stepIDEventLoopMap.first, "test" + std::to_string(ctrPos));
        ASSERT_EQ(stepIDEventLoopMap.second, expectedIdEventLoop.at(ctrPos % 2));
        ctrPos++;
    }
}