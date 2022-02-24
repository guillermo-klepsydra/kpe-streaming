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

#include <klepsydra/core/container.h>
#include <klepsydra/streaming/publish_subscribe_factory.h>
#include <klepsydra/streaming/streaming_configuration_manager.h>
#include <klepsydra/streaming/thread_distribution_policy_factory.h>
#include <klepsydra/streaming/visibility.h>

namespace kpsr {
namespace streaming {

class StreamingFactoryProvider
{
public:
    StreamingFactoryProvider(ThreadDistributionPolicyFactory *threadDistributionPolicyFactory,
                             bool testDNN);
    StreamingFactoryProvider(ThreadDistributionPolicyFactory *threadDistributionPolicyFactory,
                             const std::string &envFileName,
                             kpsr::Container *container = nullptr);
    StreamingFactoryProvider(ThreadDistributionPolicyFactory *threadDistributionPolicyFactory,
                             kpsr::Environment *environment,
                             kpsr::Container *container = nullptr);

    virtual ~StreamingFactoryProvider();

    std::shared_ptr<PublishSubscribeFactory<F32AlignedVector>> &getEventLoopFactoryFloat32();
    std::shared_ptr<PublishSubscribeFactory<I8AlignedVector>> &getEventLoopFactoryChar();
    std::shared_ptr<PublishSubscribeFactory<UI8AlignedVector>> &getEventLoopFactoryUChar();
    std::shared_ptr<PublishSubscribeFactory<F32AlignedVector>> &getDataMultiplexerFactoryFloat32();
    std::shared_ptr<PublishSubscribeFactory<I8AlignedVector>> &getDataMultiplexerFactoryChar();
    std::shared_ptr<PublishSubscribeFactory<UI8AlignedVector>> &getDataMultiplexerFactoryUChar();

    StreamingConfigurationManager *getStreamingConfigurationManager();

    void start();
    void stop();

private:
    void setDefaultLogger(const std::string &logFileName = "", int logLevel = 1);
    void initForEnvironment(ThreadDistributionPolicyFactory *threadDistributionPolicyFactory,
                            kpsr::Environment *environment);

    void setDefaultStreaming(ThreadDistributionPolicyFactory *threadDistributionPolicyFactory,
                             kpsr::Environment *environment);

    void createFactories();

    std::unique_ptr<StreamingConfigurationManager> _streamingConfigurationManager;
    kpsr::Container *_container;

    std::vector<std::string> _procIntensiveStreams;

    std::shared_ptr<PublishSubscribeFactory<F32AlignedVector>> _eventLoopFactoryFloat32;
    std::shared_ptr<PublishSubscribeFactory<I8AlignedVector>> _eventLoopFactoryChar;
    std::shared_ptr<PublishSubscribeFactory<UI8AlignedVector>> _eventLoopFactoryUChar;
    std::shared_ptr<PublishSubscribeFactory<F32AlignedVector>> _dataMultiplexerFactoryFloat32;
    std::shared_ptr<PublishSubscribeFactory<I8AlignedVector>> _dataMultiplexerFactoryChar;
    std::shared_ptr<PublishSubscribeFactory<UI8AlignedVector>> _dataMultiplexerFactoryUChar;
};
} // namespace streaming
} // namespace kpsr

#endif
