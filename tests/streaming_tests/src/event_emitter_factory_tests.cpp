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
#include <klepsydra/mem_core/mem_env.h>
#include <klepsydra/streaming/event_emitter_factory.h>
#include <klepsydra/streaming/stream_internal_types.h>

TEST(EventEmitterFactory, ConstructorTest)
{
    std::shared_ptr<kpsr::streaming::EventEmitterFactory> eventEmitterFactoryInstance = nullptr;
    ASSERT_NO_THROW(
        eventEmitterFactoryInstance = std::make_shared<kpsr::streaming::EventEmitterFactory>(nullptr,
                                                                                             0));
    ASSERT_NE(eventEmitterFactoryInstance.get(), nullptr);
}

TEST(EventEmitterFactory, EmptyFactoryGetTest)
{
    kpsr::streaming::EventEmitterFactory eventEmitterFactory(nullptr, 10);

    std::string testName = "dummy";
    int vectorSize = 10;

    auto initializerFunctionEmitter =
        [vectorSize](kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> &data) {
            data.data->resize(vectorSize);
        };

    ASSERT_NO_THROW(eventEmitterFactory.getEventEmitter<kpsr::streaming::DataBatchWithId<
                        kpsr::streaming::F32AlignedVector>>(testName, initializerFunctionEmitter));
}

TEST(EventEmitterFactory, NonEmptyFactoryGetTest)
{
    kpsr::streaming::EventEmitterFactory eventEmitterFactory(nullptr, 10);

    std::string testName = "dummy";
    int vectorSize = 10;

    auto initializerFunctionEmitter =
        [vectorSize](kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> &data) {
            data.data->resize(vectorSize);
        };

    std::shared_ptr<kpsr::EventEmitterMiddlewareProvider<
        kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>>
        emitterInserted;
    ASSERT_NO_THROW(
        emitterInserted = eventEmitterFactory.getEventEmitter<kpsr::streaming::DataBatchWithId<
                              kpsr::streaming::F32AlignedVector>>(testName,
                                                                  initializerFunctionEmitter));

    std::shared_ptr<kpsr::EventEmitterMiddlewareProvider<
        kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>>
        emitterToCheck;
    ASSERT_NO_THROW(
        emitterToCheck = eventEmitterFactory.getEventEmitter<kpsr::streaming::DataBatchWithId<
                             kpsr::streaming::F32AlignedVector>>(testName, nullptr));
    ASSERT_EQ(emitterToCheck, emitterInserted);
}