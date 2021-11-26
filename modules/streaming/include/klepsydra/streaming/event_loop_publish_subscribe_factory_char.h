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

#ifndef PUBLISH_SUBCRIBE_EVENTLOOP_FACTORY_CHAR_H
#define PUBLISH_SUBCRIBE_EVENTLOOP_FACTORY_CHAR_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>
#include <klepsydra/streaming/streaming_policy.h>

#include <klepsydra/streaming/event_loop_publish_subscribe_factory.h>
#include <klepsydra/streaming/publish_subscribe_factory_char.h>

namespace kpsr {
namespace streaming {

const int CHAR_EVENT_LOOP_SIZE = 32;

using FactoryEventLoopType = kpsr::high_performance::EventLoopMiddlewareProvider<CHAR_EVENT_LOOP_SIZE>;
using EventLoopPtr = std::shared_ptr<FactoryEventLoopType>;

class EventLoopPublishSubscribeFactoryChar : public PublishSubscribeFactoryChar, EventLoopPublishSubscribeFactory
{
public:
    EventLoopPublishSubscribeFactoryChar(kpsr::Container * container,
                                         StreamingPolicy * streamingPolicy);

    virtual ~EventLoopPublishSubscribeFactoryChar();

    virtual kpsr::Publisher<DataBatchWithId<std::vector<char>>> * getPublisherChar(const std::string & stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<char>>> * getSubscriberChar(const std::string & stepName, const size_t vectorSize = 0) override;

    virtual kpsr::Publisher<DataBatchWithId<std::vector<unsigned char>>> * getPublisherUChar(const std::string & stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<unsigned char>>> * getSubscriberUChar(const std::string & stepName, const size_t vectorSize = 0) override;
};
}
}
#endif // PUBLISH_SUBCRIBE_EVENTLOOP_FACTORY_CHAR_H
