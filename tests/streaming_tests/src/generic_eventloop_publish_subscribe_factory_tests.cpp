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
#include <numeric>

#include "gtest/gtest.h"
#include <klepsydra/streaming/default_thread_distribution_policy.h>
#include <klepsydra/streaming/generic_event_loop_publish_subscribe_factory.h>

template<typename T>
class GenericEventLoopPublishSubscribeFactoryTest : public testing::Test
{
public:
    GenericEventLoopPublishSubscribeFactoryTest()
        : parallisedStreams()
        , poolSize(2)
        , numberOfCores(std::thread::hardware_concurrency())
        , numberOfEventLoops(numberOfCores * 1)
        , nonCriticalThreadPoolSize(1)
        , numberOfParallelThreads(1)
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
        , eventLoopFactoryInstance(
              std::make_shared<kpsr::streaming::EventLoopFactory>(nullptr,
                                                                  streamingConfigurationManager
                                                                      .get()))
    {}

    std::vector<std::string> parallisedStreams;
    int poolSize;
    size_t numberOfCores;
    size_t numberOfEventLoops;
    size_t nonCriticalThreadPoolSize;
    int numberOfParallelThreads;
    std::shared_ptr<kpsr::streaming::DefaultThreadDistributionPolicy> defaultThreadDistributionPolicy;
    std::unique_ptr<kpsr::streaming::StreamingConfigurationManager> streamingConfigurationManager;
    std::shared_ptr<kpsr::streaming::EventLoopFactory> eventLoopFactoryInstance;
};

using TypesToTest = ::testing::Types<std::pair<kpsr::streaming::F32AlignedVector, float>,
                                     std::pair<kpsr::streaming::I8AlignedVector, int8_t>>;

TYPED_TEST_SUITE(GenericEventLoopPublishSubscribeFactoryTest, TypesToTest);

TYPED_TEST(GenericEventLoopPublishSubscribeFactoryTest, TestConstructor)
{
    using TypeParamFirst = typename TypeParam::first_type;

    ASSERT_NO_THROW(
        kpsr::streaming::GenericEventLoopPublishSubscribeFactory<TypeParamFirst> eventLoopPubSubInstance(
            this->eventLoopFactoryInstance));
}

TYPED_TEST(GenericEventLoopPublishSubscribeFactoryTest, GetPublishersAndSubscribers)
{
    using TypeParamFirst = typename TypeParam::first_type;
    const std::string publisherName = "eventLoopPublisher";
    const std::string subscriberName = "eventLoopSubscriber";
    const std::string registerListenerName = "eventLoopListener";

    kpsr::streaming::GenericEventLoopPublishSubscribeFactory<TypeParamFirst> eventLoopPubSubInstance(
        this->eventLoopFactoryInstance);

    kpsr::Publisher<TypeParamFirst> *publisher = nullptr;
    ASSERT_NO_THROW(publisher = eventLoopPubSubInstance.getPublisher(publisherName));
    ASSERT_NE(publisher, nullptr);

    kpsr::Subscriber<TypeParamFirst> *subscriber = nullptr;
    ASSERT_NO_THROW(subscriber = eventLoopPubSubInstance.getSubscriber(subscriberName));
    ASSERT_NE(subscriber, nullptr);

    ASSERT_NO_THROW(
        subscriber
            ->registerListener(registerListenerName,
                               [](const TypeParamFirst &event) {
                                   spdlog::debug("Register Listener - single subscriber");
                               }));
    ASSERT_NO_THROW(subscriber->removeListener(registerListenerName));
}

TYPED_TEST(GenericEventLoopPublishSubscribeFactoryTest, PublisherSubscriberTest)
{
    using TypeParamFirst = typename TypeParam::first_type;
    using TypeParamSecond = typename TypeParam::second_type;

    kpsr::streaming::GenericEventLoopPublishSubscribeFactory<TypeParamFirst> eventLoopPubSubInstance(
        this->eventLoopFactoryInstance);

    const std::string publisherName = "vector-data";
    auto publisher = eventLoopPubSubInstance.getPublisher(publisherName);

    const std::string subscriberName = "vector-data";
    auto subscriber = eventLoopPubSubInstance.getSubscriber(subscriberName);

    const std::string registerListenerName = "vector-data-eventLoopListener";
    const TypeParamFirst expectedData = {TypeParamSecond(1), TypeParamSecond(5), TypeParamSecond(8)};
    bool finished = false;
    subscriber->registerListener(registerListenerName,
                                 [&expectedData, &finished](
                                     const TypeParamFirst &event) {
                                     spdlog::debug("Register Listener");
                                     EXPECT_EQ(expectedData, event);
                                     finished = true;
                                 });

    eventLoopPubSubInstance.start();

    auto inputDataPtr = std::make_shared<TypeParamFirst>(expectedData);

    std::thread publisherThread([&publisher, &inputDataPtr]() {
        publisher->publish(inputDataPtr);
    });

    while (!finished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    publisherThread.join();
    eventLoopPubSubInstance.stop();

    subscriber->removeListener(registerListenerName);
}