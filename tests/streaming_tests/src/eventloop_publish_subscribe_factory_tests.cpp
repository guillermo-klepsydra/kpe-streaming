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
#include <klepsydra/streaming/data_multiplexer_factory_float32.h>
#include <klepsydra/streaming/event_loop_publish_subscribe_factory_char.h>
#include <klepsydra/streaming/event_loop_publish_subscribe_factory_float32.h>

TEST(EventLoopPublishSubscribeFactoryTest, TestConstructorFactoryFloat32)
{
    std::vector<std::string> parallisedStreams = {};
    std::unique_ptr<kpsr::streaming::DefaultStreamingPolicy> defaultStreamingPolicy =
        std::make_unique<kpsr::streaming::DefaultStreamingPolicy>(std::thread::hardware_concurrency(),
                                                                  2,
                                                                  1,
                                                                  1,
                                                                  parallisedStreams);
    std::shared_ptr<kpsr::streaming::EventLoopPublishSubscribeFactory>
        eventLoopPubSubFactoryInstance = nullptr;
    ASSERT_NO_THROW(
        eventLoopPubSubFactoryInstance = std::make_shared<
            kpsr::streaming::EventLoopPublishSubscribeFactory>(nullptr,
                                                               defaultStreamingPolicy.get()));
    ASSERT_NE(eventLoopPubSubFactoryInstance.get(), nullptr);

    ASSERT_NO_THROW(kpsr::streaming::EventLoopPublishSubscribeFactoryFloat32 eventLoopInstance(
        eventLoopPubSubFactoryInstance));
}

TEST(EventLoopPublishSubscribeFactoryTest, FactoryFloat32Test)
{
    std::vector<std::string> parallisedStreams = {};
    std::unique_ptr<kpsr::streaming::DefaultStreamingPolicy> defaultStreamingPolicy =
        std::make_unique<kpsr::streaming::DefaultStreamingPolicy>(std::thread::hardware_concurrency(),
                                                                  2,
                                                                  1,
                                                                  1,
                                                                  parallisedStreams);
    auto eventLoopPubSubFactoryInstance = std::make_shared<
        kpsr::streaming::EventLoopPublishSubscribeFactory>(nullptr, defaultStreamingPolicy.get());

    kpsr::streaming::EventLoopPublishSubscribeFactoryFloat32 eventLoopInstance(
        eventLoopPubSubFactoryInstance);

    const size_t vectorSize = 3;
    const std::string publisherName = "eventLoopPublisher";
    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *publisher = nullptr;
    ASSERT_NO_THROW(publisher = eventLoopInstance.getPublisherF32Aligned(publisherName, vectorSize));
    ASSERT_NE(publisher, nullptr);

    const std::string subscriberName = "eventLoopSubscriber";
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *subscriber = nullptr;
    ASSERT_NO_THROW(
        subscriber = eventLoopInstance.getSubscriberF32Aligned(subscriberName, vectorSize));
    ASSERT_NE(subscriber, nullptr);

    const std::string registerListenerName = "eventLoopListener";
    ASSERT_NO_THROW(subscriber->registerListener(
        registerListenerName,
        [](const kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> &event) {
            spdlog::debug("Register Listener Float32");
        }));
    ASSERT_NO_THROW(subscriber->removeListener(registerListenerName));
}

TEST(EventLoopPublishSubscribeFactoryTest, TestConstructorFactoryChar)
{
    std::vector<std::string> parallisedStreams = {};
    std::unique_ptr<kpsr::streaming::DefaultStreamingPolicy> defaultStreamingPolicy =
        std::make_unique<kpsr::streaming::DefaultStreamingPolicy>(std::thread::hardware_concurrency(),
                                                                  2,
                                                                  1,
                                                                  1,
                                                                  parallisedStreams);
    std::shared_ptr<kpsr::streaming::EventLoopPublishSubscribeFactory>
        eventLoopPubSubFactoryInstance = nullptr;
    ASSERT_NO_THROW(
        eventLoopPubSubFactoryInstance = std::make_shared<
            kpsr::streaming::EventLoopPublishSubscribeFactory>(nullptr,
                                                               defaultStreamingPolicy.get()));
    ASSERT_NE(eventLoopPubSubFactoryInstance.get(), nullptr);

    ASSERT_NO_THROW(kpsr::streaming::EventLoopPublishSubscribeFactoryChar eventLoopInstance(
        eventLoopPubSubFactoryInstance));
}

TEST(EventLoopPublishSubscribeFactoryTest, FactoryCharTest)
{
    std::vector<std::string> parallisedStreams = {};
    std::unique_ptr<kpsr::streaming::DefaultStreamingPolicy> defaultStreamingPolicy =
        std::make_unique<kpsr::streaming::DefaultStreamingPolicy>(std::thread::hardware_concurrency(),
                                                                  2,
                                                                  1,
                                                                  1,
                                                                  parallisedStreams);
    auto eventLoopPubSubFactoryInstance = std::make_shared<
        kpsr::streaming::EventLoopPublishSubscribeFactory>(nullptr, defaultStreamingPolicy.get());

    kpsr::streaming::EventLoopPublishSubscribeFactoryChar eventLoopInstance(
        eventLoopPubSubFactoryInstance);

    const size_t vectorSize = 3;
    const std::string publisherName = "eventLoopPublisher";
    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<char>>> *publisher = nullptr;
    ASSERT_NO_THROW(publisher = eventLoopInstance.getPublisherChar(publisherName, vectorSize));
    ASSERT_NE(publisher, nullptr);

    const std::string subscriberName = "eventLoopSubscriber";
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<char>>> *subscriber = nullptr;
    ASSERT_NO_THROW(subscriber = eventLoopInstance.getSubscriberChar(subscriberName, vectorSize));
    ASSERT_NE(subscriber, nullptr);

    const std::string registerListenerName = "eventLoopListener";
    ASSERT_NO_THROW(
        subscriber->registerListener(registerListenerName,
                                     [](const kpsr::streaming::DataBatchWithId<std::vector<char>>
                                            &event) { spdlog::debug("Register Listener Char"); }));
    ASSERT_NO_THROW(subscriber->removeListener(registerListenerName));
}

TEST(EventLoopPublishSubscribeFactoryTest, PublisherSubscriberTest)
{
    std::vector<std::string> parallisedStreams = {};
    std::unique_ptr<kpsr::streaming::DefaultStreamingPolicy> defaultStreamingPolicy =
        std::make_unique<kpsr::streaming::DefaultStreamingPolicy>(std::thread::hardware_concurrency(),
                                                                  2,
                                                                  1,
                                                                  1,
                                                                  parallisedStreams);
    auto eventLoopPublishSubscribeFactory = std::make_shared<
        kpsr::streaming::EventLoopPublishSubscribeFactory>(nullptr, defaultStreamingPolicy.get());
    kpsr::streaming::EventLoopPublishSubscribeFactoryFloat32 eventLoopInstance(
        eventLoopPublishSubscribeFactory);

    const size_t vectorSize = 3;
    const std::string publisherName = "vector-data";
    auto publisher = eventLoopInstance.getPublisherF32Aligned(publisherName, vectorSize);

    const std::string subscriberName = "vector-data";
    auto subscriber = eventLoopInstance.getSubscriberF32Aligned(subscriberName, vectorSize);

    const std::string registerListenerName = "vector-data-eventLoopListener";
    const kpsr::streaming::F32AlignedVector expectedData = {1, 5, 8};
    bool finished = false;
    subscriber->registerListener(
        registerListenerName,
        [&expectedData, &finished](
            const kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> &event) {
            spdlog::debug("Register Listener Float32");
            for (size_t i = 0; i < event.data->size(); i++) {
                EXPECT_EQ(expectedData.at(i), event.data->data()[i]);
            }
            finished = true;
        });

    eventLoopInstance.start();

    auto inputDataPtr = std::make_shared<kpsr::streaming::F32AlignedVector>(
        std::initializer_list<float>{float(1), float(5), float(8)});
    std::thread publisherThread([&publisher, &inputDataPtr]() {
        kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>
            inputDataBatchWithId(0, inputDataPtr);
        publisher->publish(inputDataBatchWithId);
    });

    while (!finished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    publisherThread.join();
    eventLoopInstance.stop();

    subscriber->removeListener(registerListenerName);
}