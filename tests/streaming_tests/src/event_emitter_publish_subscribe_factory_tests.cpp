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

#include "gtest/gtest.h"
#include <klepsydra/streaming/event_emitter_publish_subscribe_factory_char.h>
#include <klepsydra/streaming/event_emitter_publish_subscribe_factory_float32.h>

TEST(EventEmitterPublishSubscribeFactoryTest, TestConstructorFactoryFloat32)
{
    std::shared_ptr<kpsr::streaming::EventEmitterPublishSubscribeFactory>
        eventEmitterPubSubFactoryInstance = nullptr;
    ASSERT_NO_THROW(
        eventEmitterPubSubFactoryInstance =
            std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactory>(nullptr, 0));
    ASSERT_NE(eventEmitterPubSubFactoryInstance.get(), nullptr);

    ASSERT_NO_THROW(kpsr::streaming::EventEmitterPublishSubscribeFactoryFloat32 eventEmitterInstance(
        eventEmitterPubSubFactoryInstance));
}

TEST(EventEmitterPublishSubscribeFactoryTest, FactoryFloat32Test)
{
    auto eventEmitterPubSubFactoryInstance =
        std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactory>(nullptr, 0);

    kpsr::streaming::EventEmitterPublishSubscribeFactoryFloat32 eventEmitterInstance(
        eventEmitterPubSubFactoryInstance);

    const size_t vectorSize = 3;
    const std::string publisherName = "eventEmitterPublisher";
    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *publisher = nullptr;
    ASSERT_NO_THROW(
        publisher = eventEmitterInstance.getPublisherF32Aligned(publisherName, vectorSize));
    ASSERT_NE(publisher, nullptr);

    const std::string subscriberName = "eventEmitterSubscriber";
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>
        *subscriber = nullptr;
    ASSERT_NO_THROW(
        subscriber = eventEmitterInstance.getSubscriberF32Aligned(subscriberName, vectorSize));
    ASSERT_NE(subscriber, nullptr);

    const std::string registerListenerName = "eventEmitterListener";
    ASSERT_NO_THROW(subscriber->registerListener(
        registerListenerName,
        [](const kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> &event) {
            spdlog::info("Register Listener Float32");
        }));
    ASSERT_NO_THROW(subscriber->removeListener(registerListenerName));
}

TEST(EventEmitterPublishSubscribeFactoryTest, TestConstructorFactoryChar)
{
    std::shared_ptr<kpsr::streaming::EventEmitterPublishSubscribeFactory>
        eventEmitterPubSubFactoryInstance = nullptr;
    ASSERT_NO_THROW(
        eventEmitterPubSubFactoryInstance =
            std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactory>(nullptr, 0));
    ASSERT_NE(eventEmitterPubSubFactoryInstance.get(), nullptr);

    ASSERT_NO_THROW(kpsr::streaming::EventEmitterPublishSubscribeFactoryChar eventEmitterInstance(
        eventEmitterPubSubFactoryInstance));
}

TEST(EventEmitterPublishSubscribeFactoryTest, FactoryCharTest)
{
    auto eventEmitterPubSubFactoryInstance =
        std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactory>(nullptr, 0);

    kpsr::streaming::EventEmitterPublishSubscribeFactoryChar eventEmitterInstance(
        eventEmitterPubSubFactoryInstance);

    const size_t vectorSize = 3;
    const std::string publisherName = "eventEmitterPublisher";
    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<char>>> *publisher = nullptr;
    ASSERT_NO_THROW(publisher = eventEmitterInstance.getPublisherChar(publisherName, vectorSize));
    ASSERT_NE(publisher, nullptr);

    const std::string subscriberName = "eventEmitterSubscriber";
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<char>>> *subscriber = nullptr;
    ASSERT_NO_THROW(subscriber = eventEmitterInstance.getSubscriberChar(subscriberName, vectorSize));
    ASSERT_NE(subscriber, nullptr);

    const std::string registerListenerName = "eventEmitterListener";
    ASSERT_NO_THROW(
        subscriber->registerListener(registerListenerName,
                                     [](const kpsr::streaming::DataBatchWithId<std::vector<char>>
                                            &event) { spdlog::info("Register Listener Char"); }));
    ASSERT_NO_THROW(subscriber->removeListener(registerListenerName));
}