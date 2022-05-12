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
#include <klepsydra/streaming/generic_event_emitter_publish_subscribe_factory.h>

template<typename T>
class GenericEventEmitterPublishSubscribeFactoryTest : public testing::Test
{
public:
    GenericEventEmitterPublishSubscribeFactoryTest()
        : eventEmitterFactoryInstance(
              std::make_shared<kpsr::streaming::EventEmitterFactory>(nullptr, 0))
    {}

    std::shared_ptr<kpsr::streaming::EventEmitterFactory> eventEmitterFactoryInstance;
};

using TypesToTest =
    ::testing::Types<kpsr::streaming::F32AlignedVector, kpsr::streaming::I8AlignedVector>;
TYPED_TEST_SUITE(GenericEventEmitterPublishSubscribeFactoryTest, TypesToTest);

TYPED_TEST(GenericEventEmitterPublishSubscribeFactoryTest, TestConstructorFactory)
{
    ASSERT_NO_THROW(
        kpsr::streaming::GenericEventEmitterPublishSubscribeFactory<TypeParam> eventEmitterInstance(
            this->eventEmitterFactoryInstance));
}

TYPED_TEST(GenericEventEmitterPublishSubscribeFactoryTest, GetPublisherSubscriber)
{
    const std::string publisherName = "eventEmitterPublisher";
    const std::string subscriberName = "eventEmitterSubscriber";
    const std::string registerListenerName = "eventEmitterListener";

    auto eventEmitterPublishSubscribeInstance =
        std::make_shared<kpsr::streaming::GenericEventEmitterPublishSubscribeFactory<TypeParam>>(
            this->eventEmitterFactoryInstance);

    kpsr::Publisher<TypeParam> *publisher = nullptr;
    ASSERT_NO_THROW(
        publisher = eventEmitterPublishSubscribeInstance->getPublisher(publisherName));
    ASSERT_NE(publisher, nullptr);

    kpsr::Subscriber<TypeParam> *subscriber = nullptr;
    ASSERT_NO_THROW(subscriber = eventEmitterPublishSubscribeInstance->getSubscriber(subscriberName));
    ASSERT_NE(subscriber, nullptr);

    ASSERT_NO_THROW(
        subscriber->registerListener(registerListenerName,
                                     [](const TypeParam &event) {
                                         spdlog::info("Register Listener");
                                     }));
    ASSERT_NO_THROW(subscriber->removeListener(registerListenerName));
}