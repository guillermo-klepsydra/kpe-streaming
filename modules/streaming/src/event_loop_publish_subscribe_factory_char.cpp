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

#include <klepsydra/admin/check_license.h>
#include <klepsydra/streaming/event_loop_publish_subscribe_factory_char.h>

namespace kpsr {
namespace streaming {

    EventLoopPublishSubscribeFactoryChar::EventLoopPublishSubscribeFactoryChar(kpsr::Container * container,
                                                                       StreamingPolicy * streamingPolicy)
        : kpsr::Service(nullptr, "EventLoopPubSubFactoryService")
        , EventLoopPublishSubscribeFactory(container, streamingPolicy, "EventLoopPubSubFactoryService")
    {
    }

    EventLoopPublishSubscribeFactoryChar::~EventLoopPublishSubscribeFactoryChar()
    {
    }

    kpsr::Publisher<DataBatchWithId<std::vector<char>>> * EventLoopPublishSubscribeFactoryChar::getPublisherChar(
        const std::string & stepName, const size_t vectorSize) {

        spdlog::debug("EventLoopPublishSubscribeFactory::getPublisher: stepName: {}", stepName);
        auto eventLoopName = processStepName(stepName);
        auto eventLoopPtr = getEventLoop(eventLoopName);

        return eventLoopPtr->template getPublisher<DataBatchWithId<std::vector<char>>>(
            eventLoopName,
            _poolSize,
            [vectorSize] (DataBatchWithId<std::vector<char>> & data) {
                data.data->resize(vectorSize);
            },
            nullptr);
    }

    kpsr::Subscriber<DataBatchWithId<std::vector<char>>> * EventLoopPublishSubscribeFactoryChar::getSubscriberChar(const std::string & stepName, const size_t vectorSize) {
        return getSubscriber<DataBatchWithId<std::vector<char>>>(stepName);
    }

    kpsr::Publisher<DataBatchWithId<std::vector<unsigned char>>> * EventLoopPublishSubscribeFactoryChar::getPublisherUChar(
        const std::string & stepName, const size_t vectorSize) {

        spdlog::debug("EventLoopPublishSubscribeFactory::getPublisher: stepName: {}", stepName);
        auto eventLoopName = processStepName(stepName);
        auto eventLoopPtr = getEventLoop(eventLoopName);

        return eventLoopPtr->template getPublisher<DataBatchWithId<std::vector<unsigned char>>>(
            eventLoopName,
            _poolSize,
            [vectorSize] (DataBatchWithId<std::vector<unsigned char>> & data) {
                data.data->resize(vectorSize);
            },
            nullptr);
    }

    kpsr::Subscriber<DataBatchWithId<std::vector<unsigned char>>> * EventLoopPublishSubscribeFactoryChar::getSubscriberUChar(const std::string & stepName, const size_t vectorSize) {
        return getSubscriber<DataBatchWithId<std::vector<unsigned char>>>(stepName);
    }
}
}
