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

#ifndef DATA_MULTIPLEXER_FACTORY_H
#define DATA_MULTIPLEXER_FACTORY_H

#include <klepsydra/streaming/stream_internal_types.h>
#include <klepsydra/streaming/streaming_types.h>
#include <klepsydra/streaming/publish_subscribe_factory.h>

namespace kpsr {
namespace streaming {

class DataMultiplexerFactory : public PublishSubscribeFactory 
{
public:
    virtual void start(){};

    virtual void stop(){};

    virtual ~DataMultiplexerFactory() {}
};
}
}

#endif // DATA_MULTIPLEXER_FACTORY_H
