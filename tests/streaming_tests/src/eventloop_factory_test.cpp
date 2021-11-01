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
#include <klepsydra/streaming/event_loop_publish_subscribe_factory_float32.h>

#include <numeric>

#include "gtest/gtest.h"

TEST(EventLoopPublishSubscribeFactoryTest, SimpleTest) {

    const int num_listeners = 2;
    int data_sent_ctr = 0;
    std::vector<int> data_received_ctr(num_listeners, 0);

    kpsr::streaming::DataMultiplexerFactoryFloat32Impl dataMultiplexerInstance(nullptr);

    kpsr::Publisher<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> * dataMultiplexerPublisher = dataMultiplexerInstance.getPublisherF32Aligned("dataMultiplexer", 3);
    kpsr::Subscriber<kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector>> * dataMultiplexerSubscriber = dataMultiplexerInstance.getSubscriberF32Aligned("dataMultiplexer", 3);
    
    std::vector<std::string> parallisedStreams = {};
    std::unique_ptr<kpsr::streaming::DefaultStreamingPolicy> defaultStreamingPolicy = std::make_unique<kpsr::streaming::DefaultStreamingPolicy>(std::thread::hardware_concurrency(), 2, 1, 1, parallisedStreams);
    kpsr::streaming::EventLoopPublishSubscribeFactoryFloat32 eventloopInstance(nullptr, defaultStreamingPolicy.get());

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        dataMultiplexerSubscriber->registerListener("dataMultiplexer_" + i, [i, &eventloopInstance](const kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> & event){
                                                                                                    eventloopInstance.getPublisherF32Aligned("eventloop_" + i, 3)->publish(event);                                 
                                                });

        eventloopInstance.getSubscriberF32Aligned("eventloop_" + i, 3)->registerListener("data_received_ctr_" + i, [i, &data_received_ctr](const kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> & event){
                                                                                                                                           data_received_ctr[i]++; 
                                                                                });
    }
    
    eventloopInstance.startup();

    std::thread dataMultipexerPublisherThread([&dataMultiplexerPublisher, &data_sent_ctr]() {
        for (int i = 0; i < 5; i++) {
            std::shared_ptr<kpsr::streaming::F32AlignedVector> inputPtr = std::make_shared<kpsr::streaming::F32AlignedVector>(std::initializer_list<float>{float(i+1), float(i+2), float(i+3)});
            kpsr::streaming::DataBatchWithId<kpsr::streaming::F32AlignedVector> inputDataBatchWithId(0, inputPtr);
            dataMultiplexerPublisher->publish(inputDataBatchWithId);
            data_sent_ctr++;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    dataMultipexerPublisherThread.join();
    eventloopInstance.shutdown();
    
    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        dataMultiplexerSubscriber->removeListener("dataMultiplexer_" + i);
        eventloopInstance.getSubscriberF32Aligned("eventloop_" + i, 3)->removeListener("data_received_ctr_" + i);
    }

    for (size_t i = 0; i < data_received_ctr.size(); i++) {
        spdlog::debug("Data received counter {} value = {}", i, data_received_ctr[i]);
    }
    spdlog::debug("Data sent counter {}", data_sent_ctr);
    EXPECT_EQ(std::accumulate(data_received_ctr.begin(), data_received_ctr.end(), 0), num_listeners*data_sent_ctr);

}