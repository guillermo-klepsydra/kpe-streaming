/*****************************************************************************
*                           Klepsydra AI Modules
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

#include "gtest/gtest.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

int main(int argc, char **argv)
{
    kpsr::Threadpool::getCriticalThreadPool(16);
    kpsr::Threadpool::getNonCriticalThreadPool(16);

    spdlog::set_pattern("[%c] [%H:%M:%S %f] [%n] [%l] [%t] %v");
    spdlog::set_level(spdlog::level::info); // Set global log level to info
    auto kpsrLogger = spdlog::stdout_color_mt("kpsr");
    spdlog::set_default_logger(kpsrLogger);


    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}

