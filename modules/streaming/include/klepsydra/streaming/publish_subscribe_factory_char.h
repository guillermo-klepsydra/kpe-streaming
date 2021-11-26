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

#ifndef PUBLISH_SUBSCRIBE_FACTORY_CHAR_H
#define PUBLISH_SUBSCRIBE_FACTORY_CHAR_H

#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/core/service.h>
#include <klepsydra/streaming/stream_internal_types.h>
#include <klepsydra/streaming/streaming_types.h>

namespace kpsr {
namespace streaming {

class PublishSubscribeFactoryChar: virtual public kpsr::Service
{
public:
    
    virtual kpsr::Publisher<DataBatchWithId<std::vector<char>>> * getPublisherChar(const std::string & stepName, const size_t vectorSize) = 0;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<char>>> * getSubscriberChar(const std::string & stepName, const size_t vectorSize = 0) = 0;

    virtual kpsr::Publisher<DataBatchWithId<std::vector<unsigned char>>> * getPublisherUChar(const std::string & stepName, const size_t vectorSize) = 0;
    virtual kpsr::Subscriber<DataBatchWithId<std::vector<unsigned char>>> * getSubscriberUChar(const std::string & stepName, const size_t vectorSize = 0) = 0;
protected:
    virtual void execute() override {};
};

} //streaming
} //kpsr

#endif // PUBLISH_SUBSCRIBE_FACTORY_CHAR_H
