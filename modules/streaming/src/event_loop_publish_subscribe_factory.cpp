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

#include <klepsydra/streaming/event_loop_publish_subscribe_factory.h>


namespace kpsr {
namespace streaming {

    EventLoopPublishSubscribeFactory::EventLoopPublishSubscribeFactory(kpsr::Container * container,
                                                                       StreamingPolicy * streamingPolicy)
        : _container(container)
        , _streamingPolicy(streamingPolicy)
        , _poolSize(_streamingPolicy->getStreamingConfiguration().poolSize)
        , _eventLoops(_streamingPolicy->getStreamingConfiguration().numberOfEventLoops)
        {
            check_license();
            kpsr::Threadpool::getCriticalThreadPool(_streamingPolicy->getStreamingConfiguration().numberOfEventLoops + 2);
            kpsr::Threadpool::getNonCriticalThreadPool(_streamingPolicy->getStreamingConfiguration().nonCriticalThreadPoolSize);

            for(size_t i = 0; i < _eventLoops.size(); i++) {
                spdlog::debug("EventLoopPublishSubscribeFactory::EventLoopPublishSubscribeFactory. New eventloop with name: {}", i);
                std::vector<int> cpuCores = _streamingPolicy->getStreamingConfiguration().eventLoopCoreMap[i];
                _eventLoops[i] = std::make_shared<FactoryEventLoopType>(
                    _container,
                    std::to_string((i)),
                    kpsr::high_performance::EVENT_LOOP_START_TIMEOUT_MICROSEC,
                    cpuCores);
            }
        }

    EventLoopPublishSubscribeFactory::~EventLoopPublishSubscribeFactory() {
        _eventLoops.clear();
    }

    std::string EventLoopPublishSubscribeFactory::processStepName(const std::string stepName) const {
        std::string eventLoopName = stepName;
        findAndReplaceAll(eventLoopName, "/", "");
        return eventLoopName;
    }

    EventLoopPtr EventLoopPublishSubscribeFactory::getEventLoop(const std::string & eventLoopName) {
        return _eventLoops[_streamingPolicy->addStepToEventLoop(eventLoopName)];
    }

    kpsr::Publisher<DataBatchWithId<F32AlignedVector>> * EventLoopPublishSubscribeFactory::getPublisherF32Aligned(
        const std::string & stepName, const size_t vectorSize) {

        spdlog::debug("EventLoopPublishSubscribeFactory::getPublisher: stepName: {}", stepName);
        auto eventLoopName = processStepName(stepName);
        auto eventLoopPtr = getEventLoop(eventLoopName);

        return eventLoopPtr->template getPublisher<DataBatchWithId<F32AlignedVector>>(
            eventLoopName,
            _poolSize,
            [vectorSize] (DataBatchWithId<F32AlignedVector> & data) {
                data.data->resize(vectorSize);
            },
            nullptr);
    }

    kpsr::Publisher<DataBatchWithId<std::vector<float>>> * EventLoopPublishSubscribeFactory::getPublisherF32(
        const std::string & stepName, const size_t vectorSize) {

        spdlog::debug("EventLoopPublishSubscribeFactory::getPublisher: stepName: {}", stepName);
        auto eventLoopName = processStepName(stepName);
        auto eventLoopPtr = getEventLoop(eventLoopName);

        return eventLoopPtr->template getPublisher<DataBatchWithId<std::vector<float>>>(
            eventLoopName,
            _poolSize,
            [vectorSize] (DataBatchWithId<std::vector<float>> & data) {
                data.data->resize(vectorSize);
            },
            nullptr);
    }

    kpsr::Publisher<DataBatchWithId<std::vector<F32AlignedVector>>> * EventLoopPublishSubscribeFactory::getPublisherMultiF32Aligned(
            const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) {

            spdlog::debug("EventLoopPublishSubscribeFactory::getPublisher: stepName: {}", stepName);
            auto eventLoopName = processStepName(stepName);
            auto eventLoopPtr = getEventLoop(eventLoopName);

            return eventLoopPtr->template getPublisher<DataBatchWithId<std::vector<F32AlignedVector>>>(
                eventLoopName,
                _poolSize,
                [multiVectorSize, vectorSize] (DataBatchWithId<std::vector<F32AlignedVector>> & data) {
                    data.data->resize(multiVectorSize);
                    for (auto& d: *data.data) {
                        d.resize(vectorSize);
                    }
                },
                nullptr);
    }

    kpsr::Subscriber<DataBatchWithId<F32AlignedVector>> * EventLoopPublishSubscribeFactory::getSubscriberF32Aligned(const std::string & stepName, const size_t vectorSize) {
        return getSubscriber<DataBatchWithId<F32AlignedVector>>(stepName);
    }

    kpsr::Subscriber<DataBatchWithId<std::vector<F32AlignedVector>>> * EventLoopPublishSubscribeFactory::getSubscriberMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) {
        return getSubscriber<DataBatchWithId<std::vector<F32AlignedVector>>>(stepName);
    }

    kpsr::Subscriber<DataBatchWithId<std::vector<float>>> * EventLoopPublishSubscribeFactory::getSubscriberF32(const std::string & stepName, const size_t vectorSize) {
        return getSubscriber<DataBatchWithId<std::vector<float>>>(stepName);
    }

    void EventLoopPublishSubscribeFactory::start() {
        if (spdlog::default_logger()->level() == spdlog::level::debug) {
            YAML::Node node;
            node[STREAMING_CONF_YAML_NODE] = _streamingPolicy->getStreamingConfiguration();
            std::stringstream yamlSS;
            yamlSS << node;
            spdlog::debug("kpsr::streaming::EventLoopPublishSubscribeFactory::start. autogenerated yaml file:");
            spdlog::debug("{}", yamlSS.str());
        }

        for(kpsr::streaming::EventLoopPtr const& eventloopPtr : _eventLoops) {
            eventloopPtr->start();
        }
        spdlog::debug("kpsr::streaming::EventLoopPublishSubscribeFactory::start. Started!");
    }

    void EventLoopPublishSubscribeFactory::stop() {
        for(EventLoopPtr const& eventloopPtr : _eventLoops) {
            eventloopPtr->stop();
        }
    }

    const std::vector<EventLoopPtr> EventLoopPublishSubscribeFactory::getEventLoops() {
        return _eventLoops;
    }

    const StreamingPolicy * EventLoopPublishSubscribeFactory::getStreamingPolicy() {
        return _streamingPolicy;
    }
}
}
