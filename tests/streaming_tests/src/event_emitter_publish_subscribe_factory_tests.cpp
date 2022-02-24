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
#include <klepsydra/streaming/event_emitter_publish_subscribe_factory.h>

template<typename T>
class EventEmitterPublishSubscribeFactoryTest : public testing::Test
{
public:
    EventEmitterPublishSubscribeFactoryTest()
        : eventEmitterFactoryInstance(
              std::make_shared<kpsr::streaming::EventEmitterFactory>(nullptr, 0))
    {}

    std::shared_ptr<kpsr::streaming::EventEmitterFactory> eventEmitterFactoryInstance;
};

using TypesToTest =
    ::testing::Types<kpsr::streaming::F32AlignedVector, kpsr::streaming::I8AlignedVector>;
TYPED_TEST_SUITE(EventEmitterPublishSubscribeFactoryTest, TypesToTest);

TYPED_TEST(EventEmitterPublishSubscribeFactoryTest, TestConstructorFactory)
{
    ASSERT_NO_THROW(
        kpsr::streaming::EventEmitterPublishSubscribeFactory<TypeParam> eventEmitterInstance(
            this->eventEmitterFactoryInstance));
}

TYPED_TEST(EventEmitterPublishSubscribeFactoryTest, GetPublisherSubscriber)
{
    const size_t vectorSize = 3;
    const std::string publisherName = "eventEmitterPublisher";
    const std::string subscriberName = "eventEmitterSubscriber";
    const std::string registerListenerName = "eventEmitterListener";

    auto eventEmitterPublishSubscribeInstance =
        std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactory<TypeParam>>(
            this->eventEmitterFactoryInstance);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<TypeParam>> *publisher = nullptr;
    ASSERT_NO_THROW(
        publisher = eventEmitterPublishSubscribeInstance->getPublisher(publisherName, vectorSize));
    ASSERT_NE(publisher, nullptr);

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<TypeParam>> *subscriber = nullptr;
    ASSERT_NO_THROW(subscriber = eventEmitterPublishSubscribeInstance->getSubscriber(subscriberName,
                                                                                     vectorSize));
    ASSERT_NE(subscriber, nullptr);

    ASSERT_NO_THROW(
        subscriber->registerListener(registerListenerName,
                                     [](const kpsr::streaming::DataBatchWithId<TypeParam> &event) {
                                         spdlog::info("Register Listener");
                                     }));
    ASSERT_NO_THROW(subscriber->removeListener(registerListenerName));
}

TYPED_TEST(EventEmitterPublishSubscribeFactoryTest, GetPublisherSubscriberMulti)
{
    const size_t vectorSize = 3;
    const size_t multiVectorSize = 2;
    const std::string publisherMultiName = "eventEmitterPublisherMulti";
    const std::string subscriberMultiName = "eventEmitterSubscriberMulti";
    const std::string registerListenerName = "eventEmitterListener";

    auto eventEmitterPublishSubscribeInstance =
        std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactory<TypeParam>>(
            this->eventEmitterFactoryInstance);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<TypeParam>>> *publisherMulti =
        nullptr;
    ASSERT_NO_THROW(
        publisherMulti = eventEmitterPublishSubscribeInstance->getPublisherMulti(publisherMultiName,
                                                                                 vectorSize,
                                                                                 multiVectorSize));
    ASSERT_NE(publisherMulti, nullptr);

    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<TypeParam>>> *subscriberMulti =
        nullptr;
    ASSERT_NO_THROW(subscriberMulti = eventEmitterPublishSubscribeInstance
                                          ->getSubscriberMulti(subscriberMultiName,
                                                               vectorSize,
                                                               multiVectorSize));
    ASSERT_NE(subscriberMulti, nullptr);

    ASSERT_NO_THROW(subscriberMulti->registerListener(
        registerListenerName,
        [](const kpsr::streaming::DataBatchWithId<std::vector<TypeParam>> &event) {
            spdlog::info("Register Listener");
        }));
    ASSERT_NO_THROW(subscriberMulti->removeListener(registerListenerName));
}