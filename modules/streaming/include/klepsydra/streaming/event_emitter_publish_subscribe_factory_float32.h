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

#ifndef EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_FLOAT32_H
#define EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_FLOAT32_H

#include <klepsydra/streaming/publish_subscribe_factory_float32.h>
#include <klepsydra/streaming/event_emitter_publish_subscribe_factory.h>

namespace kpsr {
namespace streaming {

using EventEmitterF32A = kpsr::EventEmitterMiddlewareProvider<DataBatchWithId<F32AlignedVector>>;
using EventEmitterMF32A = kpsr::EventEmitterMiddlewareProvider<DataBatchWithId<std::vector<F32AlignedVector>>>;
using EventEmitterF32 = kpsr::EventEmitterMiddlewareProvider<DataBatchWithId<std::vector<float>>>;

class EventEmitterPublishSubscribeFactoryFloat32 : public PublishSubscribeFactoryFloat32
{
public:
    EventEmitterPublishSubscribeFactoryFloat32(std::shared_ptr<EventEmitterPublishSubscribeFactory> & eventEmitterPublishSubscribeFactory);


    virtual ~EventEmitterPublishSubscribeFactoryFloat32();

    virtual kpsr::Publisher<DataBatchWithId<F32AlignedVector>> * getPublisherF32Aligned(const std::string & stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<F32AlignedVector>> * getSubscriberF32Aligned(const std::string & stepName, const size_t vectorSize) override;

    virtual kpsr::Publisher<DataBatchWithId<std::vector<F32AlignedVector>>> * getPublisherMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<F32AlignedVector>>> * getSubscriberMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) override;

    virtual kpsr::Publisher<DataBatchWithId <std::vector<float>>> * getPublisherF32(const std::string & stepName, const size_t vectorSize) override;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<float>>> * getSubscriberF32(const std::string & stepName, const size_t vectorSize) override;

    void start();
    void stop();

private:
    std::shared_ptr<EventEmitterF32A> getEventEmitterF32A(const std::string & stepName, const size_t vectorSize);
    std::shared_ptr<EventEmitterMF32A> getEventEmitterMF32A(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize);
    std::shared_ptr<EventEmitterF32> getEventEmitterF32(const std::string & stepName, const size_t vectorSize);

    std::shared_ptr<EventEmitterPublishSubscribeFactory> _eventEmitterPublishSubscribeFactory;

};
}
}

#endif // EVENT_EMITTER_PUBLISH_SUBSCRIBE_FACTORY_FLOAT32_H