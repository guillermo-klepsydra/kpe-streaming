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

#include <klepsydra/streaming/smart_publish_subscribe_factory.h>

#include <klepsydra/mem_core/mem_env.h>
#include <klepsydra/streaming/default_thread_distribution_policy.h>
#include <klepsydra/streaming/default_thread_distribution_policy_factory_impl.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>

#include "config.h"
#include "gtest/gtest.h"
#include <spdlog/spdlog.h>

using TestTuple = std::tuple<kpsr::Container *, bool>;

class SmartPublishSubscribeFactoryTest : public ::testing::TestWithParam<TestTuple>
{
protected:
    SmartPublishSubscribeFactoryTest()
        : container(nullptr)
        , poolSize(2)
        , numberOfCores(std::thread::hardware_concurrency())
        , numberOfEventLoops(std::thread::hardware_concurrency() * 1)
        , nonCriticalThreadPoolSize(1)
        , numberOfParallelThreads(1)
        , parallisedStreams()
        , defaultThreadDistributionPolicy(
              std::make_shared<kpsr::streaming::DefaultThreadDistributionPolicy>(numberOfCores,
                                                                                 numberOfEventLoops))
        , streamingConfigurationManager(
              std::make_unique<kpsr::streaming::StreamingConfigurationManager>(
                  poolSize,
                  numberOfCores,
                  numberOfEventLoops,
                  nonCriticalThreadPoolSize,
                  numberOfParallelThreads,
                  parallisedStreams,
                  defaultThreadDistributionPolicy))
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

    const std::string stepName = "testStep";
    const size_t vectorSize = 1;
    const size_t multiVectorSize = 1;

    kpsr::Container *container;
    bool useTestFactory;
    int poolSize;
    size_t numberOfCores;
    size_t numberOfEventLoops;
    size_t nonCriticalThreadPoolSize;
    int numberOfParallelThreads;
    std::vector<std::string> parallisedStreams;
    std::shared_ptr<kpsr::streaming::DefaultThreadDistributionPolicy> defaultThreadDistributionPolicy;
    std::unique_ptr<kpsr::streaming::StreamingConfigurationManager> streamingConfigurationManager;
    std::shared_ptr<kpsr::streaming::EventEmitterFactory> eventEmitterFactory;
    std::shared_ptr<kpsr::streaming::EventLoopFactory> eventLoopFactory;
};

kpsr::mem::MemEnv smartPubSubFactoryTestEnvironment;
kpsr::Container smartPubSubFactoryTestContainer(&smartPubSubFactoryTestEnvironment,
                                                "SmartPubSubFactoryTestEnvironment");

INSTANTIATE_TEST_SUITE_P(SmartPublishSubscribeFactoryTests,
                         SmartPublishSubscribeFactoryTest,
                         ::testing::Combine(::testing::Values(nullptr,
                                                              &smartPubSubFactoryTestContainer),
                                            ::testing::Values(true, false)));

TEST_P(SmartPublishSubscribeFactoryTest, ConstructorTest)
{
    ASSERT_NO_THROW(kpsr::streaming::SmartPublishSubscribeFactory<kpsr::streaming::F32AlignedVector>
                        smartFactoryFloat32(eventEmitterFactory,
                                            eventLoopFactory,
                                            container,
                                            streamingConfigurationManager.get(),
                                            useTestFactory));

    ASSERT_NO_THROW(kpsr::streaming::SmartPublishSubscribeFactory<kpsr::streaming::I8AlignedVector>
                        smartFactoryChar(eventEmitterFactory,
                                         eventLoopFactory,
                                         container,
                                         streamingConfigurationManager.get(),
                                         useTestFactory));

    ASSERT_NO_THROW(kpsr::streaming::SmartPublishSubscribeFactory<kpsr::streaming::UI8AlignedVector>
                        smartFactoryChar(eventEmitterFactory,
                                         eventLoopFactory,
                                         container,
                                         streamingConfigurationManager.get(),
                                         useTestFactory));
}

TEST_P(SmartPublishSubscribeFactoryTest, getPubSubF32Test)
{
    auto smartFactoryF32 = std::make_unique<kpsr::streaming::SmartPublishSubscribeFactory<
        kpsr::streaming::F32AlignedVector>>(eventEmitterFactory,
                                            eventLoopFactory,
                                            container,
                                            streamingConfigurationManager.get(),
                                            useTestFactory);

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> *subscriber;
    ASSERT_NO_THROW(subscriber = smartFactoryF32->getSubscriber(stepName, vectorSize));
    ASSERT_NE(nullptr, subscriber);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> *publisher;
    ASSERT_NO_THROW(publisher = smartFactoryF32->getPublisher(stepName, vectorSize));
    ASSERT_NE(nullptr, publisher);

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::F32AlignedVector>>>
        *subscriberMulti;
    ASSERT_NO_THROW(subscriberMulti = smartFactoryF32->getSubscriberMulti(stepName,
                                                                          vectorSize,
                                                                          multiVectorSize));
    ASSERT_NE(nullptr, subscriberMulti);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::F32AlignedVector>>>
        *publisherMulti;
    ASSERT_NO_THROW(
        publisherMulti = smartFactoryF32->getPublisherMulti(stepName, vectorSize, multiVectorSize));
    ASSERT_NE(nullptr, publisherMulti);
}

TEST_P(SmartPublishSubscribeFactoryTest, getPubSubCharTest)
{
    auto smartFactoryChar = std::make_unique<kpsr::streaming::SmartPublishSubscribeFactory<
        kpsr::streaming::I8AlignedVector>>(eventEmitterFactory,
                                           eventLoopFactory,
                                           container,
                                           streamingConfigurationManager.get(),
                                           useTestFactory);

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::I8AlignedVector>> *subscriber;
    ASSERT_NO_THROW(subscriber = smartFactoryChar->getSubscriber(stepName, vectorSize));
    ASSERT_NE(nullptr, subscriber);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::I8AlignedVector>> *publisher;
    ASSERT_NO_THROW(publisher = smartFactoryChar->getPublisher(stepName, vectorSize));
    ASSERT_NE(nullptr, publisher);

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::I8AlignedVector>>>
        *subscriberMulti;
    ASSERT_NO_THROW(subscriberMulti = smartFactoryChar->getSubscriberMulti(stepName,
                                                                           vectorSize,
                                                                           multiVectorSize));
    ASSERT_NE(nullptr, subscriberMulti);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::I8AlignedVector>>>
        *publisherMulti;
    ASSERT_NO_THROW(
        publisherMulti = smartFactoryChar->getPublisherMulti(stepName, vectorSize, multiVectorSize));
    ASSERT_NE(nullptr, publisherMulti);
}

TEST_P(SmartPublishSubscribeFactoryTest, getPubSubUCharTest)
{
    auto smartFactoryUChar = std::make_unique<kpsr::streaming::SmartPublishSubscribeFactory<
        kpsr::streaming::UI8AlignedVector>>(eventEmitterFactory,
                                            eventLoopFactory,
                                            container,
                                            streamingConfigurationManager.get(),
                                            useTestFactory);

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::UI8AlignedVector>> *subscriber;
    ASSERT_NO_THROW(subscriber = smartFactoryUChar->getSubscriber(stepName, vectorSize));
    ASSERT_NE(nullptr, subscriber);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::UI8AlignedVector>> *publisher;
    ASSERT_NO_THROW(publisher = smartFactoryUChar->getPublisher(stepName, vectorSize));
    ASSERT_NE(nullptr, publisher);

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::UI8AlignedVector>>>
        *subscriberMulti;
    ASSERT_NO_THROW(subscriberMulti = smartFactoryUChar->getSubscriberMulti(stepName,
                                                                            vectorSize,
                                                                            multiVectorSize));
    ASSERT_NE(nullptr, subscriberMulti);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::UI8AlignedVector>>>
        *publisherMulti;
    ASSERT_NO_THROW(publisherMulti = smartFactoryUChar->getPublisherMulti(stepName,
                                                                          vectorSize,
                                                                          multiVectorSize));
    ASSERT_NE(nullptr, publisherMulti);
}