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

#include <klepsydra/streaming/event_loop_publish_subscribe_factory_float32.h>
#include <klepsydra/streaming/event_emitter_publish_subscribe_factory_float32.h>
#include <klepsydra/streaming/event_loop_publish_subscribe_factory_char.h>
#include <klepsydra/streaming/event_emitter_publish_subscribe_factory_char.h>

#include <klepsydra/streaming/data_multiplexer_factory_float32_impl.h>
#include <klepsydra/streaming/data_multiplexer_factory_char_impl.h>
#include <klepsydra/streaming/sync_data_multiplexer_factory_float32.h>
#include <klepsydra/streaming/sync_data_multiplexer_factory_char.h>

#include <klepsydra/admin/check_license.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace kpsr {
namespace streaming {

StreamingFactoryProvider::StreamingFactoryProvider(bool testDNN)
    : _container(nullptr)
    , _eventLoopFactoryFloat32(nullptr)
    , _eventLoopFactoryChar(nullptr)
{
    std::vector<std::string> parallisedLayers = {};
    _streamingPolicy = std::make_unique<DefaultStreamingPolicy>(std::thread::hardware_concurrency(), 4, 4, 1, parallisedLayers);
    if (testDNN) {
        setDefaultLogger();
        _eventLoopFactoryFloat32 = std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactoryFloat32>(_container, 10);
        _eventLoopFactoryChar = std::make_shared<kpsr::streaming::EventEmitterPublishSubscribeFactoryChar>(_container, 10);
        _dataMultiplexerFactoryFloat32 = std::make_shared<kpsr::streaming::SyncDataMultiplexerFactoryFloat32>(_container);
        _dataMultiplexerFactoryChar = std::make_shared<kpsr::streaming::SyncDataMultiplexerFactoryChar>(_container);
    } else {
        createFactories();
    }
}

StreamingFactoryProvider::StreamingFactoryProvider(const std::string & envFileName, kpsr::Container * container)
    : _container(container)
    , _eventLoopFactoryFloat32(nullptr)
    , _eventLoopFactoryChar(nullptr)
{
    kpsr::ConfigurationEnvironment environment(envFileName);

    if (!_container) {
        _container = kpsr::admin::AdminContainerFactory::getInstance().getContainerForEnv(&environment);
    }
    initForEnvironment(&environment);
}


StreamingFactoryProvider::StreamingFactoryProvider(kpsr::Environment * environment, kpsr::Container * container)
    : _container(container)
    , _eventLoopFactoryFloat32(nullptr)
    , _eventLoopFactoryChar(nullptr)
{
    if (!_container) {
        _container = kpsr::admin::AdminContainerFactory::getInstance().getContainerForEnv(environment);
    }
    initForEnvironment(environment);
}

void StreamingFactoryProvider::setDefaultLogger(const std::string& logFileName, int logLevel){
    if (spdlog::get("klepsydra")) {
        //noop
    } else {
        if (!logFileName.empty()) {
            auto  kpsrLogger = spdlog::basic_logger_mt("klepsydra", logFileName);
            spdlog::set_default_logger(kpsrLogger);
        } else {
            auto  kpsrLogger = spdlog::stderr_color_mt("klepsydra");
            spdlog::set_default_logger(kpsrLogger);
        }
        spdlog::set_pattern("[%c] [%H:%M:%S %f] [%n] [%l] [%t] %v");
        spdlog::set_level(static_cast<spdlog::level::level_enum>(logLevel));

    }
}

void StreamingFactoryProvider::createFactories() {
    check_license();
    _eventLoopFactoryFloat32 = std::make_shared<kpsr::streaming::EventLoopPublishSubscribeFactoryFloat32>(_container, _streamingPolicy.get());
    _dataMultiplexerFactoryFloat32 = std::make_shared<kpsr::streaming::DataMultiplexerFactoryFloat32Impl>(_container);

    _eventLoopFactoryChar = std::make_shared<kpsr::streaming::EventLoopPublishSubscribeFactoryChar>(_container, _streamingPolicy.get());
    _dataMultiplexerFactoryChar = std::make_shared<kpsr::streaming::DataMultiplexerFactoryCharImpl>(_container);
}

void StreamingFactoryProvider::initForEnvironment(kpsr::Environment * environment)
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
        setDefaultStreaming(environment);
    } else {
        std::string streamingConfigurationFile;
        environment->getPropertyString("streaming_conf_file", streamingConfigurationFile);
        _streamingPolicy = std::make_unique<kpsr::streaming::JsonStreamingPolicy>(streamingConfigurationFile);
    }

    createFactories();

}

void StreamingFactoryProvider::setDefaultStreaming(kpsr::Environment * environment) {
    int poolSize;
    int numberOfCores;
    int nonCriticalThreadPoolSize;
    int numberOfParallelThreads;
    environment->getPropertyInt("pool_size", poolSize);
    spdlog::debug("StreamingFactoryProvider::initForEnvironment. poolSize: {}", poolSize);
    environment->getPropertyInt("number_of_cores", numberOfCores);
    spdlog::debug("StreamingFactoryProvider::initForEnvironment. numberOfCores: {}", numberOfCores);
    environment->getPropertyInt("non_critical_thread_pool_size", nonCriticalThreadPoolSize);
    spdlog::debug("StreamingFactoryProvider::initForEnvironment. nonCriticalThreadPoolSize: {}", nonCriticalThreadPoolSize);
    environment->getPropertyInt("number_of_parallel_threads", numberOfParallelThreads);
    spdlog::debug("StreamingFactoryProvider::initForEnvironment. numberOfParallelThreads: {}", numberOfParallelThreads);
    std::vector<std::string> parallisedLayers = {};
    _streamingPolicy = std::make_unique<DefaultStreamingPolicy>(numberOfCores, poolSize, nonCriticalThreadPoolSize,
                                                                numberOfParallelThreads, parallisedLayers);
}        

StreamingFactoryProvider::~StreamingFactoryProvider() {
    // destroy publishSubscriberFactory first:
    _eventLoopFactoryFloat32.reset();
    _dataMultiplexerFactoryFloat32.reset();

    _eventLoopFactoryChar.reset();
    _dataMultiplexerFactoryChar.reset();
}

std::shared_ptr<kpsr::streaming::PublishSubscribeFactoryFloat32> & StreamingFactoryProvider::getEventLoopFactoryFloat32() {
    return _eventLoopFactoryFloat32;
}

std::shared_ptr<kpsr::streaming::DataMultiplexerFactoryFloat32> & StreamingFactoryProvider::getDataMultiplexerFactoryFloat32() {
    return _dataMultiplexerFactoryFloat32;
}

std::shared_ptr<kpsr::streaming::PublishSubscribeFactoryChar> & StreamingFactoryProvider::getEventLoopFactoryChar() {
    return _eventLoopFactoryChar;
}

std::shared_ptr<kpsr::streaming::DataMultiplexerFactoryChar> & StreamingFactoryProvider::getDataMultiplexerFactoryChar() {
    return _dataMultiplexerFactoryChar;
}

StreamingPolicy * StreamingFactoryProvider::getStreamingPolicy() {
    return _streamingPolicy.get();
}

void StreamingFactoryProvider::start() {
    check_license();
    _eventLoopFactoryFloat32->startup();
    _eventLoopFactoryChar->startup();
}

void StreamingFactoryProvider::stop() {
    _eventLoopFactoryFloat32->shutdown();
    _eventLoopFactoryChar->shutdown();
}

} // namespace streaming
} // namespace kpsr
