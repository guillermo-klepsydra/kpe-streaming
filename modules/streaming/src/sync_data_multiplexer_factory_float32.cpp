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

#include <klepsydra/streaming/sync_data_multiplexer_factory_float32.h>

kpsr::streaming::SyncDataMultiplexerFactoryFloat32::SyncDataMultiplexerFactoryFloat32(kpsr::Container * container)
    : kpsr::Service(nullptr, "syncDataMuxFactoryFloat32")
    , _testFactory(container, 0)
{}

kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> * kpsr::streaming::SyncDataMultiplexerFactoryFloat32::getPublisherF32Aligned(const std::string & stepName, const size_t vectorSize) {
    return _testFactory.getPublisherF32Aligned(stepName, vectorSize);
}

kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> * kpsr::streaming::SyncDataMultiplexerFactoryFloat32::getSubscriberF32Aligned(const std::string & stepName, const size_t vectorSize) {
    return _testFactory.getSubscriberF32Aligned(stepName, vectorSize);
}

kpsr::Publisher<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::F32AlignedVector>>> * kpsr::streaming::SyncDataMultiplexerFactoryFloat32::getPublisherMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) {
    return _testFactory.getPublisherMultiF32Aligned(stepName, vectorSize, multiVectorSize);
}
kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<kpsr::streaming::F32AlignedVector>>> * kpsr::streaming::SyncDataMultiplexerFactoryFloat32::getSubscriberMultiF32Aligned(const std::string & stepName, const size_t vectorSize, const size_t multiVectorSize) {
    return _testFactory.getSubscriberMultiF32Aligned(stepName, vectorSize, multiVectorSize);
}

kpsr::Publisher<kpsr::streaming::DataBatchWithId <std::vector<float>>> * kpsr::streaming::SyncDataMultiplexerFactoryFloat32::getPublisherF32(const std::string & stepName, const size_t vectorSize) {
    return _testFactory.getPublisherF32(stepName, vectorSize);
}
kpsr::Subscriber<kpsr::streaming::DataBatchWithId<std::vector<float>>> * kpsr::streaming::SyncDataMultiplexerFactoryFloat32::getSubscriberF32(const std::string & stepName, const size_t vectorSize) {
    return _testFactory.getSubscriberF32(stepName, vectorSize);
}
