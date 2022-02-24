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
#include <klepsydra/streaming/event_loop_publish_subscribe_factory.h>

template<typename T>
class EventLoopPublishSubscribeFactoryTest : public testing::Test
{
public:
    EventLoopPublishSubscribeFactoryTest()
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

TYPED_TEST_SUITE(EventLoopPublishSubscribeFactoryTest, TypesToTest);

TYPED_TEST(EventLoopPublishSubscribeFactoryTest, TestConstructor)
{
    using TypeParamFirst = typename TypeParam::first_type;

    ASSERT_NO_THROW(
        kpsr::streaming::EventLoopPublishSubscribeFactory<TypeParamFirst> eventLoopPubSubInstance(
            this->eventLoopFactoryInstance));
}

TYPED_TEST(EventLoopPublishSubscribeFactoryTest, GetPublishersAndSubscribers)
{
    using TypeParamFirst = typename TypeParam::first_type;
    const size_t vectorSize = 3;
    const std::string publisherName = "eventLoopPublisher";
    const std::string subscriberName = "eventLoopSubscriber";
    const std::string registerListenerName = "eventLoopListener";

    kpsr::streaming::EventLoopPublishSubscribeFactory<TypeParamFirst> eventLoopPubSubInstance(
        this->eventLoopFactoryInstance);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<TypeParamFirst>> *publisher = nullptr;
    ASSERT_NO_THROW(publisher = eventLoopPubSubInstance.getPublisher(publisherName, vectorSize));
    ASSERT_NE(publisher, nullptr);

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<TypeParamFirst>> *subscriber = nullptr;
    ASSERT_NO_THROW(subscriber = eventLoopPubSubInstance.getSubscriber(subscriberName, vectorSize));
    ASSERT_NE(subscriber, nullptr);

    ASSERT_NO_THROW(
        subscriber
            ->registerListener(registerListenerName,
                               [](const kpsr::streaming::DataBatchWithId<TypeParamFirst> &event) {
                                   spdlog::debug("Register Listener - single subscriber");
                               }));
    ASSERT_NO_THROW(subscriber->removeListener(registerListenerName));
}

TYPED_TEST(EventLoopPublishSubscribeFactoryTest, GetPublishersAndSubscribersMulti)
{
    using TypeParamFirst = typename TypeParam::first_type;
    const size_t vectorSize = 3;
    const size_t multiVectorSize = 2;
    const std::string publisherMultiName = "eventLoopPublisherMulti";
    const std::string subscriberMultiName = "eventLoopSubscriberMulti";
    const std::string registerListenerName = "eventLoopListener";

    kpsr::streaming::EventLoopPublishSubscribeFactory<TypeParamFirst> eventLoopPubSubInstance(
        this->eventLoopFactoryInstance);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<TypeParamFirst>>> *publisherMulti =
        nullptr;
    ASSERT_NO_THROW(publisherMulti = eventLoopPubSubInstance.getPublisherMulti(publisherMultiName,
                                                                               vectorSize,
                                                                               multiVectorSize));
    ASSERT_NE(publisherMulti, nullptr);

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<TypeParamFirst>>>
        *subscriberMulti = nullptr;
    ASSERT_NO_THROW(subscriberMulti = eventLoopPubSubInstance.getSubscriberMulti(subscriberMultiName,
                                                                                 vectorSize,
                                                                                 multiVectorSize));
    ASSERT_NE(subscriberMulti, nullptr);

    ASSERT_NO_THROW(subscriberMulti->registerListener(
        registerListenerName,
        [](const kpsr::streaming::DataBatchWithId<std::vector<TypeParamFirst>> &event) {
            spdlog::debug("Register Listener - multi subscriber");
        }));
    ASSERT_NO_THROW(subscriberMulti->removeListener(registerListenerName));
}

TYPED_TEST(EventLoopPublishSubscribeFactoryTest, PublisherSubscriberTest)
{
    using TypeParamFirst = typename TypeParam::first_type;
    using TypeParamSecond = typename TypeParam::second_type;

    kpsr::streaming::EventLoopPublishSubscribeFactory<TypeParamFirst> eventLoopPubSubInstance(
        this->eventLoopFactoryInstance);

    const size_t vectorSize = 3;
    const std::string publisherName = "vector-data";
    auto publisher = eventLoopPubSubInstance.getPublisher(publisherName, vectorSize);

    const std::string subscriberName = "vector-data";
    auto subscriber = eventLoopPubSubInstance.getSubscriber(subscriberName, vectorSize);

    const std::string registerListenerName = "vector-data-eventLoopListener";
    const TypeParamFirst expectedData = {TypeParamSecond(1), TypeParamSecond(5), TypeParamSecond(8)};
    bool finished = false;
    subscriber->registerListener(registerListenerName,
                                 [&expectedData, &finished](
                                     const kpsr::streaming::DataBatchWithId<TypeParamFirst> &event) {
                                     spdlog::debug("Register Listener");
                                     EXPECT_EQ(expectedData, (*event.data));
                                     finished = true;
                                 });

    eventLoopPubSubInstance.start();

    auto inputDataPtr = std::make_shared<TypeParamFirst>(expectedData);

    std::thread publisherThread([&publisher, &inputDataPtr]() {
        kpsr::streaming::DataBatchWithId<TypeParamFirst> inputDataBatchWithId(0, inputDataPtr);
        publisher->publish(inputDataBatchWithId);
    });

    while (!finished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    publisherThread.join();
    eventLoopPubSubInstance.stop();

    subscriber->removeListener(registerListenerName);
}