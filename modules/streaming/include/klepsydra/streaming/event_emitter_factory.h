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
#ifndef EVENT_EMITTER_FACTORY_H
#define EVENT_EMITTER_FACTORY_H

#include <klepsydra/core/container.h>
#include <map>
#include <string>

#include <klepsydra/core/event_emitter_middleware_provider.h>
#include <klepsydra/streaming/streaming_types.h>

namespace kpsr {
namespace streaming {

class EventEmitterFactory
{
public:
    EventEmitterFactory(kpsr::Container *container, int poolSize)
        : _container(container)
        , _poolSize(poolSize)
    {}

    virtual ~EventEmitterFactory() { _container = nullptr; }

    template<class T>
    std::shared_ptr<kpsr::EventEmitterMiddlewareProvider<T>> getEventEmitter(
        const std::string &stepName, const std::function<void(T &)> initializerFunction)
    {
        auto stepIt = _eventemitterMap.find(stepName);
        if (stepIt == _eventemitterMap.end()) {
            return insertEmitter(stepName,
                                 std::make_shared<kpsr::EventEmitterMiddlewareProvider<T>>(
                                     _container, stepName, _poolSize, initializerFunction, nullptr));
        } else {
            auto internalPtr = stepIt->second;
            auto emitterToReturn = std::static_pointer_cast<kpsr::EventEmitterMiddlewareProvider<T>>(
                internalPtr);
            return emitterToReturn;
        }
    }

protected:
    kpsr::Container *_container;
    int _poolSize;

private:
    std::map<std::string, std::shared_ptr<void>> _eventemitterMap;

    template<class T>
    std::shared_ptr<kpsr::EventEmitterMiddlewareProvider<T>> insertEmitter(
        const std::string &stepName,
        std::shared_ptr<kpsr::EventEmitterMiddlewareProvider<T>> emitter)
    {
        spdlog::debug("EventEmitterFactory::getEventEmitter: new instance, stepName: {}", stepName);
        auto insertResult = _eventemitterMap.insert(
            std::make_pair(stepName, std::static_pointer_cast<void>(emitter)));
        if (!insertResult.second) {
            throw std::runtime_error("Could not save the event emitter");
        }
        return emitter;
    }
};
} // namespace streaming
} // namespace kpsr

#endif // EVENT_EMITTER_FACTORY_H