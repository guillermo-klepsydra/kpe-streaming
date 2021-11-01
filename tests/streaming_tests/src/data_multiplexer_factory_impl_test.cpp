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

#include <klepsydra/streaming/data_multiplexer_factory_float32_impl.h>

#include <numeric>

#include "gtest/gtest.h"

TEST(DataMultiplexerFactoryFloat32ImplTest, SimpleTest) {
    
    const int num_listeners = 2;
    int data_sent_ctr = 0;
    std::vector<int> data_received_ctr(num_listeners, 0);

    kpsr::streaming::DataMultiplexerFactoryFloat32Impl dataMultiplexerInstance(nullptr);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> * dataMultiplexerPublisher = dataMultiplexerInstance.getPublisherF32Aligned("dataMultiplexer", 3);
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> * dataMultiplexerSubscriber = dataMultiplexerInstance.getSubscriberF32Aligned("dataMultiplexer", 3);
    
    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        dataMultiplexerSubscriber->registerListener("data_received_ctr_" + i, [i, &data_received_ctr](const kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> & event){
                                                                                                      data_received_ctr[i]++;                                   
                                                });
    }
    
    std::thread dataMultipexerPublisherThread([&dataMultiplexerPublisher, &data_sent_ctr]() {
        for (size_t i = 0; i < 5; i++) {
            std::shared_ptr<kpsr::streaming::F32AlignedVector> inputPtr = std::make_shared<kpsr::streaming::F32AlignedVector>(std::initializer_list<float>{float(i+1), float(i+2), float(i+3)});
            kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> inputDataBatchWithId(0, inputPtr);
            dataMultiplexerPublisher->publish(inputDataBatchWithId);
            data_sent_ctr++;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    dataMultipexerPublisherThread.join();
    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        dataMultiplexerSubscriber->removeListener("data_received_ctr_" + i);
    }

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        spdlog::debug("Data received counter {} value = {}", i, data_received_ctr[i]);
    }
    spdlog::debug("Data sent counter {}", data_sent_ctr);
    EXPECT_EQ(std::accumulate(data_received_ctr.begin(), data_received_ctr.end(), 0), num_listeners*data_sent_ctr);

}