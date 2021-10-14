/*****************************************************************************
*                           Klepsydra STREAMING Modules
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

#ifndef __STREAMING_FACTORY_PARSER_H__
#define __STREAMING_FACTORY_PARSER_H__

#include <klepsydra/streaming/visibility.h>
#include <klepsydra/core/container.h>
#include <klepsydra/streaming/streaming_policy.h>
#include <klepsydra/streaming/publish_subscribe_factory.h>
#include <klepsydra/streaming/data_multiplexer_factory.h>

namespace kpsr {
namespace streaming {

class StreamingFactoryProvider {
    
public:
    StreamingFactoryProvider(bool testDNN);
    StreamingFactoryProvider(const std::string & envFileName, kpsr::Container * container = nullptr);
    StreamingFactoryProvider(kpsr::Environment * environment, kpsr::Container * container = nullptr);

    virtual ~StreamingFactoryProvider();

    std::shared_ptr<PublishSubscribeFactory> & getPublishSubcriberFactory();
    std::shared_ptr<DataMultiplexerFactory> & getDataMultiplexerFactory();

    StreamingPolicy * getStreamingPolicy();

    void start();
    void stop();

private:

    void setDefaultLogger(const std::string& logFileName = "", int logLevel = 1);
    void initForEnvironment(kpsr::Environment * environment);

    void setDefaultStreaming(kpsr::Environment * environment);

    void createFactories();

    std::unique_ptr<StreamingPolicy> _streamingPolicy;
    kpsr::Container * _container;

    std::vector<std::string> _procIntensiveStreams;

    std::shared_ptr<PublishSubscribeFactory> _publishSubcriberFactory;
    std::shared_ptr<DataMultiplexerFactory> _dataMultiplexerFactory;
};
} // namespace streaming
} // namespace kpsr

#endif
