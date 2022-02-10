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

#include <klepsydra/core/configuration_environment.h>
#include <klepsydra/factory/admin_container_factory.h>

#include <klepsydra/streaming/streaming_factory_provider.h>

#include <klepsydra/streaming/event_emitter_publish_subscribe_factory_char.h>
#include <klepsydra/streaming/event_emitter_publish_subscribe_factory_float32.h>
#include <klepsydra/streaming/event_loop_publish_subscribe_factory.h>
#include <klepsydra/streaming/event_loop_publish_subscribe_factory_char.h>
#include <klepsydra/streaming/event_loop_publish_subscribe_factory_float32.h>

#include <klepsydra/admin/check_license.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace kpsr {
namespace streaming {

StreamingFactoryProvider::StreamingFactoryProvider(
    ThreadDistributionPolicyFactory *threadDistributionPolicyFactory,
    bool testDNN,
    bool useChar,
    bool useFloat)
    : _container(nullptr)
    , _eventLoopFactoryFloat32(nullptr)
    , _eventLoopFactoryChar(nullptr)
{
    size_t numberOfCores = std::thread::hardware_concurrency();
    size_t numberOfEventLoops = numberOfCores * 1;
    auto threadDistributionPolicy = threadDistributionPolicyFactory->createThreadDistributionPolicy();
    std::vector<std::string> parallisedLayers = {};
    _streamingConfigurationManager = std::make_unique<StreamingConfigurationManager>(
        4, numberOfCores, numberOfEventLoops, 4, 1, parallisedLayers, threadDistributionPolicy);
    if (testDNN) {
        auto eventEmitterPublishSubscribeFactory =
            std::make_shared<EventEmitterPublishSubscribeFactory>(_container, 10);
        setDefaultLogger();
        if (useChar) {
            _eventLoopFactoryChar =
                std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactoryChar>(
                    eventEmitterPublishSubscribeFactory);
            _dataMultiplexerFactoryChar =
                std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactoryChar>(
                    eventEmitterPublishSubscribeFactory);
        }
        if (useFloat) {
            _eventLoopFactoryFloat32 =
                std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactoryFloat32>(
                    eventEmitterPublishSubscribeFactory);
            _dataMultiplexerFactoryFloat32 =
                std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactoryFloat32>(
                    eventEmitterPublishSubscribeFactory);
        }
    } else {
        createFactories(useChar, useFloat);
    }
}

StreamingFactoryProvider::StreamingFactoryProvider(
    ThreadDistributionPolicyFactory *threadDistributionPolicyFactory,
    const std::string &envFileName,
    kpsr::Container *container)
    : _container(container)
    , _eventLoopFactoryFloat32(nullptr)
    , _eventLoopFactoryChar(nullptr)
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
    , _eventLoopFactoryFloat32(nullptr)
    , _eventLoopFactoryChar(nullptr)
{
    if (!_container) {
        _container = kpsr::admin::AdminContainerFactory::getInstance().getContainerForEnv(
            environment);
    }
    initForEnvironment(threadDistributionPolicyFactory, environment);
}

void StreamingFactoryProvider::setDefaultLogger(const std::string &logFileName, int logLevel)
{
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

void StreamingFactoryProvider::createFactories(bool useChar, bool useFloat)
{
    check_license();
    auto eventLoopPublishSubscribeFactory =
        std::make_shared<EventLoopPublishSubscribeFactory>(_container,
                                                           _streamingConfigurationManager.get());
    if (useChar) {
        _eventLoopFactoryChar =
            std::make_shared<kpsr::streaming::EventLoopPublishSubscribeFactoryChar>(
                eventLoopPublishSubscribeFactory);
        _dataMultiplexerFactoryChar = std::make_shared<kpsr::streaming::DataMultiplexerFactoryChar>(
            _container, _streamingConfigurationManager.get());
    }
    if (useFloat) {
        _eventLoopFactoryFloat32 =
            std::make_shared<kpsr::streaming::EventLoopPublishSubscribeFactoryFloat32>(
                eventLoopPublishSubscribeFactory);
        _dataMultiplexerFactoryFloat32 = std::make_shared<
            kpsr::streaming::DataMultiplexerFactoryFloat32>(_container,
                                                            _streamingConfigurationManager.get());
    }
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

    bool useChar = false;
    environment->getPropertyBool("use_char_data", useChar);
    bool useFloat = true;
    environment->getPropertyBool("use_float_data", useFloat);
    createFactories(useChar, useFloat);
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
}

StreamingFactoryProvider::~StreamingFactoryProvider() {}

std::shared_ptr<kpsr::streaming::PublishSubscribeFactoryFloat32>
    &StreamingFactoryProvider::getEventLoopFactoryFloat32()
{
    return _eventLoopFactoryFloat32;
}

std::shared_ptr<kpsr::streaming::PublishSubscribeFactoryFloat32>
    &StreamingFactoryProvider::getDataMultiplexerFactoryFloat32()
{
    return _dataMultiplexerFactoryFloat32;
}

std::shared_ptr<kpsr::streaming::PublishSubscribeFactoryChar>
    &StreamingFactoryProvider::getEventLoopFactoryChar()
{
    return _eventLoopFactoryChar;
}

std::shared_ptr<kpsr::streaming::PublishSubscribeFactoryChar>
    &StreamingFactoryProvider::getDataMultiplexerFactoryChar()
{
    return _dataMultiplexerFactoryChar;
}

StreamingConfigurationManager *StreamingFactoryProvider::getStreamingConfigurationManager()
{
    return _streamingConfigurationManager.get();
}

void StreamingFactoryProvider::start()
{
    check_license();
    if (_eventLoopFactoryFloat32) {
        _eventLoopFactoryFloat32->startup();
    }
    if (_eventLoopFactoryChar) {
        _eventLoopFactoryChar->startup();
    }
}

void StreamingFactoryProvider::stop()
{
    if (_eventLoopFactoryFloat32) {
        _eventLoopFactoryFloat32->shutdown();
    }
    if (_eventLoopFactoryChar) {
        _eventLoopFactoryChar->shutdown();
    }
}

} // namespace streaming
} // namespace kpsr