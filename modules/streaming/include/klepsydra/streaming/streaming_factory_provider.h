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

const int TEST_POOL_SIZE = 1;
const size_t TEST_NON_CRITICAL_THREAD_POOL_SIZE = 1;
const size_t TEST_NUMBER_OF_PARALLEL_THREADS = 1;

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

    std::shared_ptr<PublishSubscribeFactory<F32AlignedVector>> &getSmartFactoryFloat32();
    std::shared_ptr<PublishSubscribeFactory<I8AlignedVector>> &getSmartFactoryChar();
    std::shared_ptr<PublishSubscribeFactory<UI8AlignedVector>> &getSmartFactoryUChar();

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

    void createTestFactories();

    std::unique_ptr<StreamingConfigurationManager> _streamingConfigurationManager;
    kpsr::Container *_container;

    std::vector<std::string> _procIntensiveStreams;

    std::shared_ptr<PublishSubscribeFactory<F32AlignedVector>> _smartFactoryFloat32;
    std::shared_ptr<PublishSubscribeFactory<I8AlignedVector>> _smartFactoryChar;
    std::shared_ptr<PublishSubscribeFactory<UI8AlignedVector>> _smartFactoryUChar;
};
} // namespace streaming
} // namespace kpsr

#endif
