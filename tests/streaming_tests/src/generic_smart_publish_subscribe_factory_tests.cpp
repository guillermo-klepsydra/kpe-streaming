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

#include <klepsydra/streaming/generic_smart_publish_subscribe_factory.h>

#include <klepsydra/mem_core/mem_env.h>
#include <klepsydra/streaming/default_thread_distribution_policy.h>
#include <klepsydra/streaming/default_thread_distribution_policy_factory_impl.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>

#include "config.h"
#include "gtest/gtest.h"
#include <spdlog/spdlog.h>

class TestThreadDistributionPolicy : public kpsr::streaming::ThreadDistributionPolicy
{
public:
    TestThreadDistributionPolicy(size_t numberOfCores,
                                 size_t numberOfEventLoops,
                                 std::vector<std::string> stepNames)
    {
        for (size_t i = 0; i < numberOfEventLoops; i++) {
            std::vector<int> cores{static_cast<int>(i % numberOfCores)};
            eventLoopCoreMap[i] = cores;
        }

        for (auto stepName : stepNames) {
            if (stepName == "testStep_EventEmitter") {
                stepIDEventLoopMap[stepName].factoryType = kpsr::streaming::FactoryType::EventEmitter;
            } else if (stepName == "testStep_EventLoop") {
                stepIDEventLoopMap[stepName].factoryType = kpsr::streaming::FactoryType::EventLoop;
            } else if (stepName == "testStep_DataMultiplexer") {
                stepIDEventLoopMap[stepName].factoryType =
                    kpsr::streaming::FactoryType::DataMultiplexer;
            } else if (stepName == "testStep_Unsupported") {
                stepIDEventLoopMap[stepName].factoryType = kpsr::streaming::FactoryType::Unsupported;
            } else {
                spdlog::error("The testStep name is not correct.");
            }
            stepIDEventLoopMap[stepName].coreId = 0;
        }
    }

    size_t addStepToEventLoop(const std::string &stepName) override
    {
        auto index = stepIDEventLoopMap.find(stepName);
        return index->second.coreId;
    }
    kpsr::streaming::FactoryType getFactoryType(const std::string &stepName) override
    {
        auto index = stepIDEventLoopMap.find(stepName);
        if (index == stepIDEventLoopMap.end()) {
            throw std::runtime_error("The stepName " + stepName +
                                     " is not registered in the stepIDEventLoopMap.");
        } else {
            return index->second.factoryType;
        }
    }
};

using TestTuple = std::tuple<kpsr::Container *, bool>;
class GenericSmartPublishSubscribeFactoryTest : public ::testing::TestWithParam<TestTuple>
{
protected:
    GenericSmartPublishSubscribeFactoryTest()
        : container(nullptr)
        , poolSize(2)
        , numberOfCores(1)
        , numberOfEventLoops(1)
        , stepNames({"testStep_EventEmitter",
                     "testStep_EventLoop",
                     "testStep_DataMultiplexer",
                     "testStep_Unsupported"})
        , nonCriticalThreadPoolSize(1)
        , numberOfParallelThreads(1)
        , parallisedStreams()
        , testThreadDistributionPolicy(
              std::make_shared<TestThreadDistributionPolicy>(numberOfCores,
                                                             numberOfEventLoops,
                                                             stepNames))
        , streamingConfigurationManager(
              std::make_unique<kpsr::streaming::StreamingConfigurationManager>(
                  poolSize,
                  numberOfCores,
                  numberOfEventLoops,
                  nonCriticalThreadPoolSize,
                  numberOfParallelThreads,
                  parallisedStreams,
                  testThreadDistributionPolicy))
        , eventEmitterFactory(
              std::make_shared<kpsr::streaming::EventEmitterFactory>(container, poolSize))
        , eventLoopFactory(
              std::make_shared<kpsr::streaming::EventLoopFactory>(container,
                                                                  streamingConfigurationManager
                                                                      .get()))
    {}

    virtual void SetUp()
    {
        container = std::get<0>(GetParam());
        useTestFactory = std::get<1>(GetParam());
    }

    virtual void TearDown() { container = nullptr; }

    kpsr::Container *container;
    bool useTestFactory;
    int poolSize;
    size_t numberOfCores;
    size_t numberOfEventLoops;
    std::vector<std::string> stepNames;
    size_t nonCriticalThreadPoolSize;
    int numberOfParallelThreads;
    std::vector<std::string> parallisedStreams;
    std::shared_ptr<TestThreadDistributionPolicy> testThreadDistributionPolicy;
    std::unique_ptr<kpsr::streaming::StreamingConfigurationManager> streamingConfigurationManager;
    std::shared_ptr<kpsr::streaming::EventEmitterFactory> eventEmitterFactory;
    std::shared_ptr<kpsr::streaming::EventLoopFactory> eventLoopFactory;
};

kpsr::mem::MemEnv genericSmartPubSubFactoryTestEnvironment;
kpsr::Container genericSmartPubSubFactoryTestContainer(&genericSmartPubSubFactoryTestEnvironment,
                                                "GenericSmartPubSubFactoryTestEnvironment");

INSTANTIATE_TEST_SUITE_P(GenericSmartPublishSubscribeFactoryTests,
                         GenericSmartPublishSubscribeFactoryTest,
                         ::testing::Combine(::testing::Values(nullptr,
                                                              &genericSmartPubSubFactoryTestContainer),
                                            ::testing::Values(true, false)));

TEST_P(GenericSmartPublishSubscribeFactoryTest, ConstructorTest)
{
    ASSERT_NO_THROW(kpsr::streaming::GenericSmartPublishSubscribeFactory<kpsr::streaming::F32AlignedVector>
                        smartFactoryFloat32(eventEmitterFactory,
                                            eventLoopFactory,
                                            streamingConfigurationManager.get(),
                                            useTestFactory));

    ASSERT_NO_THROW(kpsr::streaming::GenericSmartPublishSubscribeFactory<kpsr::streaming::I8AlignedVector>
                        smartFactoryChar(eventEmitterFactory,
                                         eventLoopFactory,
                                         streamingConfigurationManager.get(),
                                         useTestFactory));

    ASSERT_NO_THROW(kpsr::streaming::GenericSmartPublishSubscribeFactory<kpsr::streaming::UI8AlignedVector>
                        smartFactoryChar(eventEmitterFactory,
                                         eventLoopFactory,
                                         streamingConfigurationManager.get(),
                                         useTestFactory));
}


TEST_P(GenericSmartPublishSubscribeFactoryTest, getPubSubF32Test)
{
    auto smartFactoryF32 = std::make_unique<kpsr::streaming::GenericSmartPublishSubscribeFactory<
        kpsr::streaming::F32AlignedVector>>(eventEmitterFactory,
                                            eventLoopFactory,
                                            streamingConfigurationManager.get(),
                                            useTestFactory);
    ASSERT_NO_THROW(smartFactoryF32->startup());

    for (auto stepName : stepNames) {
        kpsr::Subscriber<kpsr::streaming::F32AlignedVector>
            *subscriber;
        ASSERT_NO_THROW(subscriber = smartFactoryF32->getSubscriber(stepName));

        kpsr::Publisher<kpsr::streaming::F32AlignedVector>
            *publisher;
        ASSERT_NO_THROW(publisher = smartFactoryF32->getPublisher(stepName));

        if (((stepName != "testStep_DataMultiplexer") && (stepName != "testStep_Unsupported")) || (useTestFactory)) {
            ASSERT_NE(nullptr, subscriber);
            ASSERT_NE(nullptr, publisher);
        } else {
            ASSERT_EQ(nullptr, subscriber);
            ASSERT_EQ(nullptr, publisher);
        }
    }

    ASSERT_NO_THROW(smartFactoryF32->shutdown());
}

TEST_P(GenericSmartPublishSubscribeFactoryTest, getPubSubCharTest)
{
    auto smartFactoryChar = std::make_unique<kpsr::streaming::GenericSmartPublishSubscribeFactory<
        kpsr::streaming::I8AlignedVector>>(eventEmitterFactory,
                                           eventLoopFactory,
                                           streamingConfigurationManager.get(),
                                           useTestFactory);
    ASSERT_NO_THROW(smartFactoryChar->startup());

    for (auto stepName : stepNames) {
        kpsr::Subscriber<kpsr::streaming::I8AlignedVector>
            *subscriber;
        ASSERT_NO_THROW(subscriber = smartFactoryChar->getSubscriber(stepName));

        kpsr::Publisher<kpsr::streaming::I8AlignedVector>
            *publisher;
        ASSERT_NO_THROW(publisher = smartFactoryChar->getPublisher(stepName));

        if (((stepName != "testStep_DataMultiplexer") && (stepName != "testStep_Unsupported")) || (useTestFactory)) {
            ASSERT_NE(nullptr, subscriber);
            ASSERT_NE(nullptr, publisher);
        } else {
            ASSERT_EQ(nullptr, subscriber);
            ASSERT_EQ(nullptr, publisher);
        }
    }

    ASSERT_NO_THROW(smartFactoryChar->shutdown());
}

TEST_P(GenericSmartPublishSubscribeFactoryTest, getPubSubUCharTest)
{
    auto smartFactoryUChar = std::make_unique<kpsr::streaming::GenericSmartPublishSubscribeFactory<
        kpsr::streaming::UI8AlignedVector>>(eventEmitterFactory,
                                            eventLoopFactory,
                                            streamingConfigurationManager.get(),
                                            useTestFactory);
    ASSERT_NO_THROW(smartFactoryUChar->startup());

    for (auto stepName : stepNames) {
        kpsr::Subscriber<kpsr::streaming::UI8AlignedVector>
            *subscriber;
        ASSERT_NO_THROW(subscriber = smartFactoryUChar->getSubscriber(stepName));

        kpsr::Publisher<kpsr::streaming::UI8AlignedVector>
            *publisher;
        ASSERT_NO_THROW(publisher = smartFactoryUChar->getPublisher(stepName));

        if (((stepName != "testStep_DataMultiplexer") && (stepName != "testStep_Unsupported")) || (useTestFactory)) {
            ASSERT_NE(nullptr, subscriber);
            ASSERT_NE(nullptr, publisher);
        } else {
            ASSERT_EQ(nullptr, subscriber);
            ASSERT_EQ(nullptr, publisher);
        }
    }

    ASSERT_NO_THROW(smartFactoryUChar->shutdown());
}