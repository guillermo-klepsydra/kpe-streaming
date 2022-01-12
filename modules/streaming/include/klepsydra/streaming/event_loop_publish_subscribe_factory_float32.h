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

#ifndef PUBLISH_SUBCRIBE_EVENTLOOP_FACTORY_FLOAT32_H
#define PUBLISH_SUBCRIBE_EVENTLOOP_FACTORY_FLOAT32_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>
#include <klepsydra/streaming/streaming_policy.h>

#include <klepsydra/streaming/event_loop_publish_subscribe_factory.h>
#include <klepsydra/streaming/publish_subscribe_factory_float32.h>

namespace kpsr {
namespace streaming {

class EventLoopPublishSubscribeFactoryFloat32 : public PublishSubscribeFactoryFloat32
{
public:
    EventLoopPublishSubscribeFactoryFloat32(
        std::shared_ptr<EventLoopPublishSubscribeFactory> &eventLoopPublishSubscribeFactory);

    virtual ~EventLoopPublishSubscribeFactoryFloat32();

    virtual kpsr::Publisher<DataBatchWithId<F32AlignedVector>> *getPublisherF32Aligned(
        const std::string &stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<F32AlignedVector>> *getSubscriberF32Aligned(
        const std::string &stepName, const size_t vectorSize) override;

    virtual kpsr::Publisher<DataBatchWithId<std::vector<F32AlignedVector>>>
        *getPublisherMultiF32Aligned(const std::string &stepName,
                                     const size_t vectorSize,
                                     const size_t multiVectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<F32AlignedVector>>>
        *getSubscriberMultiF32Aligned(const std::string &stepName,
                                      const size_t vectorSize,
                                      const size_t multiVectorSize) override;

    virtual kpsr::Publisher<DataBatchWithId<std::vector<float>>> *getPublisherF32(
        const std::string &stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<float>>> *getSubscriberF32(
        const std::string &stepName, const size_t vectorSize) override;

    void start();
    void stop();

private:
    std::shared_ptr<EventLoopPublishSubscribeFactory> _eventLoopPublishSubscribeFactory;
};
} // namespace streaming
} // namespace kpsr

#endif // PUBLISH_SUBCRIBE_EVENTLOOP_FACTORY_FLOAT32_H
