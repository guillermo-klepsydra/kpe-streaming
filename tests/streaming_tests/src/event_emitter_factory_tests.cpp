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

#include <klepsydra/streaming/event_emitter_factory.h>
#include <klepsydra/streaming/stream_internal_types.h>
#include <klepsydra/mem_core/mem_env.h>
#include "gtest/gtest.h"

TEST(EventEmitterFactory, EmptyFactoryTest) {

    kpsr::streaming::EventEmitterFactory eventEmitterFactory;

    ASSERT_EQ(eventEmitterFactory.getEventEmitter<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>("dummy"), nullptr);
}

TEST(EventEmitterFactory, EmptyFactoryInsertTest) {

    kpsr::streaming::EventEmitterFactory eventEmitterFactory;

    std::string testName = "dummy";
    int vectorSize = 10;
    ASSERT_EQ(eventEmitterFactory.getEventEmitter<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>(testName), nullptr);

    auto emitterToInsert = std::make_shared<kpsr::EventEmitterMiddlewareProvider<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>>(
                nullptr,
                testName,
                10,
                [vectorSize] (kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> & data) {
                    data.data->resize(vectorSize);
                },
                nullptr);
    std::shared_ptr<kpsr::EventEmitterMiddlewareProvider<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>> emitterToCheck;
    ASSERT_NO_THROW(emitterToCheck = eventEmitterFactory.insertEmitter(testName, emitterToInsert));
    ASSERT_EQ(emitterToCheck, emitterToInsert);

    ASSERT_ANY_THROW(emitterToCheck = eventEmitterFactory.insertEmitter(testName, emitterToInsert));
}

TEST(EventEmitterFactory, NonEmptyFactoryGetTest) {

    kpsr::streaming::EventEmitterFactory eventEmitterFactory;

    std::string testName = "dummy";
    int vectorSize = 10;

    auto emitterToInsert = std::make_shared<kpsr::EventEmitterMiddlewareProvider<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>>(
                nullptr,
                testName,
                10,
                [vectorSize] (kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> & data) {
                    data.data->resize(vectorSize);
                },
                nullptr);
    ASSERT_NO_THROW(eventEmitterFactory.insertEmitter(testName, emitterToInsert));
    std::shared_ptr<kpsr::EventEmitterMiddlewareProvider<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>> emitterToCheck;
    ASSERT_NO_THROW(emitterToCheck = eventEmitterFactory.getEventEmitter<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>(testName));
    ASSERT_EQ(emitterToCheck, emitterToInsert);
}

TEST(EventEmitterFactory, NonEmptyFactoryInsertTest) {

    kpsr::streaming::EventEmitterFactory eventEmitterFactory;

    std::string testName = "dummy";
    int vectorSize = 10;

    auto emitterToInsert = std::make_shared<kpsr::EventEmitterMiddlewareProvider<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>>(
                nullptr,
                testName,
                10,
                [vectorSize] (kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> & data) {
                    data.data->resize(vectorSize);
                },
                nullptr);
    ASSERT_NO_THROW(eventEmitterFactory.insertEmitter(testName, emitterToInsert));

    auto testName2 = testName + "2";
    auto secondEmitterToInsert = std::make_shared<kpsr::EventEmitterMiddlewareProvider<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>>>(
                nullptr,
                testName2,
                10,
                [vectorSize] (kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> & data) {
                    data.data->resize(vectorSize);
                },
                nullptr);
    ASSERT_ANY_THROW(eventEmitterFactory.insertEmitter(testName, secondEmitterToInsert));
    ASSERT_NO_THROW(eventEmitterFactory.insertEmitter(testName2, secondEmitterToInsert));

}
