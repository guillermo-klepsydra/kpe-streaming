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

#ifndef PUBLISH_SUBSCRIBE_FACTORY_H
#define PUBLISH_SUBSCRIBE_FACTORY_H

#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/streaming/stream_internal_types.h>
#include <klepsydra/streaming/streaming_types.h>

namespace kpsr {
namespace streaming {

class PublishSubscribeFactory
{
public:
    
    virtual kpsr::Publisher<DataBatchWithId<F32AlignedVector>> * getPublisherF32Aligned(const std::string & stepName, const size_t vectorSize) = 0;
    virtual kpsr::Subscriber<DataBatchWithId<F32AlignedVector>> * getSubscriberF32Aligned(const std::string & stepName, const size_t vectorSize = 0) = 0;

    virtual kpsr::Publisher<DataBatchWithId<std::vector<F32AlignedVector>>> * getPublisherMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) = 0;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<F32AlignedVector>>> * getSubscriberMultiF32Aligned(const std::string & stepName, const size_t vectorSize = 0, const size_t multiVectorSize = 1) = 0;

    virtual kpsr::Publisher<DataBatchWithId <std::vector<float>>> * getPublisherF32(const std::string & stepName, const size_t vectorSize) = 0;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<float>>> * getSubscriberF32(const std::string & stepName, const size_t vectorSize = 0) = 0;

    virtual void start() = 0;

    virtual void stop() = 0;
};

} //streaming
} //kpsr

#endif // PUBLISH_SUBSCRIBE_FACTORY_H
