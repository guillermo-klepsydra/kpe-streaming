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


#ifndef DATA_MULTIPLEXER_FACTORY_PROVIDER_H
#define DATA_MULTIPLEXER_FACTORY_PROVIDER_H

#include <map>

#include <klepsydra/high_performance/data_multiplexer_middleware_provider.h>
#include <klepsydra/streaming/streaming_types.h>

namespace kpsr {
namespace streaming {

    template <class T, int BUFFER_SIZE>
    using DataMultiplexerProvider = kpsr::high_performance::DataMultiplexerMiddlewareProvider<T, BUFFER_SIZE>;

    template <class T, int BUFFER_SIZE>
    using DataMultiplexerProviderPtr = std::shared_ptr<DataMultiplexerProvider<T, BUFFER_SIZE>>;

    class DataMultiplexerFactoryProvider {
    public:
        DataMultiplexerFactoryProvider() {}
        ~DataMultiplexerFactoryProvider() {}

        template <class T, int BUFFER_SIZE>
        DataMultiplexerProviderPtr<T, BUFFER_SIZE> getDataMultiplexer(const std::string& stepName) {
            auto stepIt = _dataMultiplexers.find(stepName);
            if (stepIt == _dataMultiplexers.end()) {
                return nullptr;
            } else {
                auto internalPtr = stepIt->second;
                auto multiplexerToReturn = std::static_pointer_cast<DataMultiplexerProvider<T, BUFFER_SIZE>>(internalPtr);
                return multiplexerToReturn;
            }
        }

        template <class T, int BUFFER_SIZE>
        DataMultiplexerProviderPtr<T, BUFFER_SIZE> insertMultiplexer(const std::string& stepName,
                                                                     DataMultiplexerProviderPtr<T, BUFFER_SIZE> dataMultiplexer) {
            spdlog::debug("DataMultiplexerFactory::getDataMultiplexer: new instance, stepName : {}", stepName);
            auto insertResult = _dataMultiplexers.insert(std::make_pair(stepName, std::static_pointer_cast<void>(dataMultiplexer)));
            if (!insertResult.second) {
                throw std::runtime_error("Could not save the data multiplexer");
            }
            return dataMultiplexer;
        }

        size_t size() const {
            return _dataMultiplexers.size();
        }
    private:
    std::map<std::string, std::shared_ptr<void> > _dataMultiplexers;

    };
}
}
#endif
