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
#include <fstream>

#include <klepsydra/core/configuration_environment.h>
#include <klepsydra/factory/admin_container_factory.h>

#include <klepsydra/streaming/smart_publish_subscribe_factory.h>
#include <klepsydra/streaming/streaming_factory_provider.h>

#include <klepsydra/admin/check_license.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace kpsr {
namespace streaming {

StreamingFactoryProvider::StreamingFactoryProvider(
    ThreadDistributionPolicyFactory *threadDistributionPolicyFactory, bool testDNN)
    : _container(nullptr)
    , _useTestFactories(testDNN)
{
    auto threadDistributionPolicy = threadDistributionPolicyFactory->createThreadDistributionPolicy();
    size_t numberOfEventLoops = threadDistributionPolicy->eventLoopCoreMap.size();
    size_t numberOfCores = numberOfEventLoops;
    std::vector<std::string> parallisedLayers = {};
    _streamingConfigurationManager =
        std::make_unique<StreamingConfigurationManager>(TEST_POOL_SIZE,
                                                        numberOfCores,
                                                        numberOfEventLoops,
                                                        TEST_NON_CRITICAL_THREAD_POOL_SIZE,
                                                        TEST_NUMBER_OF_PARALLEL_THREADS,
                                                        parallisedLayers,
                                                        threadDistributionPolicy);
    if (_useTestFactories) {
        setDefaultLogger();
    }
    createFactories();
}

StreamingFactoryProvider::StreamingFactoryProvider(
    ThreadDistributionPolicyFactory *threadDistributionPolicyFactory,
    const std::string &envFileName,
    kpsr::Container *container)
    : _container(container)
{
    kpsr::ConfigurationEnvironment environment(envFileName);

    if (!_container) {
        _container = kpsr::admin::AdminContainerFactory::getInstance().getContainerForEnv(
            &environment);
    }
    initForEnvironment(threadDistributionPolicyFactory, &environment);
}

StreamingFactoryProvider::StreamingFactoryProvider(
    ThreadDistributionPolicyFactory *threadDistributionPolicyFactory,
    kpsr::Environment *environment,
    kpsr::Container *container)
    : _container(container)
{
    if (!_container) {
        _container = kpsr::admin::AdminContainerFactory::getInstance().getContainerForEnv(
            environment);
    }
    initForEnvironment(threadDistributionPolicyFactory, environment);
}

void StreamingFactoryProvider::setDefaultLogger(const std::string &logFileName, int logLevel)
{
    // TODO: Improve logger handling.
    if (spdlog::get("klepsydra")) {
        //noop
    } else {
        if (!logFileName.empty()) {
            auto kpsrLogger = spdlog::basic_logger_mt("klepsydra", logFileName);
            spdlog::set_default_logger(kpsrLogger);
        } else {
            auto kpsrLogger = spdlog::stderr_color_mt("klepsydra");
            spdlog::set_default_logger(kpsrLogger);
        }
        spdlog::set_pattern("[%c] [%H:%M:%S %f] [%n] [%l] [%t] %v");
        spdlog::set_level(static_cast<spdlog::level::level_enum>(logLevel));
    }
}

void StreamingFactoryProvider::createFactories()
{
    if (!_useTestFactories) {
        check_license();
    }

    int poolSize = _streamingConfigurationManager->getStreamingConfiguration().poolSize;
    auto eventEmitterFactory = std::make_shared<EventEmitterFactory>(_container, poolSize);
    auto eventLoopFactory = std::make_shared<EventLoopFactory>(_container,
                                                               _streamingConfigurationManager.get());

    _smartFactoryChar =
        std::make_shared<kpsr::streaming::SmartPublishSubscribeFactory<I8AlignedVector>>(
            eventEmitterFactory,
            eventLoopFactory,
            _container,
            _streamingConfigurationManager.get(),
            _useTestFactories);

    _smartFactoryUChar =
        std::make_shared<kpsr::streaming::SmartPublishSubscribeFactory<UI8AlignedVector>>(
            eventEmitterFactory,
            eventLoopFactory,
            _container,
            _streamingConfigurationManager.get(),
            _useTestFactories);

    _smartFactoryFloat32 =
        std::make_shared<kpsr::streaming::SmartPublishSubscribeFactory<F32AlignedVector>>(
            eventEmitterFactory,
            eventLoopFactory,
            _container,
            _streamingConfigurationManager.get(),
            _useTestFactories);
}

void StreamingFactoryProvider::initForEnvironment(
    ThreadDistributionPolicyFactory *threadDistributionPolicyFactory, kpsr::Environment *environment)
{
    std::string logFileName("");
    int logLevel(1);
    try {
        environment->getPropertyString("log_filename", logFileName);
        environment->getPropertyInt("log_level", logLevel);
    } catch (...) {
        // Do nothing. set logger with default values.
    }
    setDefaultLogger(logFileName, logLevel);

    bool useDefaultStreamingFactory;
    environment->getPropertyBool("use_default_streaming_factory", useDefaultStreamingFactory);

    if (useDefaultStreamingFactory) {
        setDefaultStreaming(threadDistributionPolicyFactory, environment);
    } else {
        std::string streamingConfigurationFile;
        environment->getPropertyString("streaming_conf_file", streamingConfigurationFile);
        _streamingConfigurationManager = std::make_unique<StreamingConfigurationManager>(
            streamingConfigurationFile);
    }

    environment->getPropertyBool("test_dnn", _useTestFactories);
    createFactories();
}

void StreamingFactoryProvider::setDefaultStreaming(
    ThreadDistributionPolicyFactory *threadDistributionPolicyFactory, kpsr::Environment *environment)
{
    int poolSize;
    int numberOfCores;
    int nonCriticalThreadPoolSize;
    int numberOfParallelThreads;
    environment->getPropertyInt("pool_size", poolSize);
    spdlog::debug("StreamingFactoryProvider::initForEnvironment. poolSize: {}", poolSize);
    environment->getPropertyInt("number_of_cores", numberOfCores);
    spdlog::debug("StreamingFactoryProvider::initForEnvironment. numberOfCores: {}", numberOfCores);
    environment->getPropertyInt("non_critical_thread_pool_size", nonCriticalThreadPoolSize);
    spdlog::debug("StreamingFactoryProvider::initForEnvironment. nonCriticalThreadPoolSize: {}",
                  nonCriticalThreadPoolSize);
    environment->getPropertyInt("number_of_parallel_threads", numberOfParallelThreads);
    spdlog::debug("StreamingFactoryProvider::initForEnvironment. numberOfParallelThreads: {}",
                  numberOfParallelThreads);

    size_t numberOfEventLoops = numberOfCores * 1;
    auto threadDistributionPolicy = threadDistributionPolicyFactory->createThreadDistributionPolicy(
        environment);
    std::vector<std::string> parallisedLayers = {};
    _streamingConfigurationManager =
        std::make_unique<StreamingConfigurationManager>(poolSize,
                                                        numberOfCores,
                                                        numberOfEventLoops,
                                                        nonCriticalThreadPoolSize,
                                                        numberOfParallelThreads,
                                                        parallisedLayers,
                                                        threadDistributionPolicy);
    bool exportStreamingConfigToJson(false);
    environment->getPropertyBool("export_streaming_configuration", exportStreamingConfigToJson);
    if (exportStreamingConfigToJson) {
        std::string streamingConfigJson = _streamingConfigurationManager->exportJsonString();
        std::string streamingConfigurationFile("");
        environment->getPropertyString("streaming_conf_file", streamingConfigurationFile);
        if (streamingConfigurationFile.empty()) {
            spdlog::debug("kpsr::streaming::EventLoopPFactory::start. autogenerated json file:");
            spdlog::debug(streamingConfigJson);
        } else {
            std::ofstream outputFileStream(streamingConfigurationFile, std::ios::binary);
            outputFileStream << streamingConfigJson;
        }
    }
}

StreamingFactoryProvider::~StreamingFactoryProvider() {}

std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<F32AlignedVector>>
    &StreamingFactoryProvider::getSmartFactoryFloat32()
{
    return _smartFactoryFloat32;
}

std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<I8AlignedVector>>
    &StreamingFactoryProvider::getSmartFactoryChar()
{
    return _smartFactoryChar;
}

std::shared_ptr<kpsr::streaming::PublishSubscribeFactory<UI8AlignedVector>>
    &StreamingFactoryProvider::getSmartFactoryUChar()
{
    return _smartFactoryUChar;
}

StreamingConfigurationManager *StreamingFactoryProvider::getStreamingConfigurationManager()
{
    return _streamingConfigurationManager.get();
}

void StreamingFactoryProvider::start()
{
    _smartFactoryChar->startup();
    _smartFactoryUChar->startup();
    _smartFactoryFloat32->startup();
}

void StreamingFactoryProvider::stop()
{
    _smartFactoryChar->shutdown();
    _smartFactoryUChar->shutdown();
    _smartFactoryFloat32->shutdown();
}

} // namespace streaming
} // namespace kpsr
