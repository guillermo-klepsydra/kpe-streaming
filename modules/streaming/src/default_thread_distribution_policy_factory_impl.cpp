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
#include <klepsydra/streaming/default_thread_distribution_policy.h>
#include <klepsydra/streaming/default_thread_distribution_policy_factory_impl.h>
#include <spdlog/spdlog.h>

namespace kpsr {
namespace streaming {

std::shared_ptr<ThreadDistributionPolicy>
DefaultThreadDistributionPolicyFactoryImpl::createThreadDistributionPolicy()
{
    size_t numberOfCores = std::thread::hardware_concurrency();
    size_t numberOfEventLoops = numberOfCores * CORE_RATIO;
    return std::make_shared<DefaultThreadDistributionPolicy>(numberOfCores, numberOfEventLoops);
}

std::shared_ptr<ThreadDistributionPolicy>
DefaultThreadDistributionPolicyFactoryImpl::createThreadDistributionPolicy(
    kpsr::Environment *environment)
{
    int numberOfCores;
    environment->getPropertyInt("number_of_cores", numberOfCores);
    spdlog::debug("DefaultThreadDistributionPolicyFactoryImpl. numberOfCores: {}", numberOfCores);
    size_t numberOfEventLoops = numberOfCores * CORE_RATIO;
    return std::make_shared<DefaultThreadDistributionPolicy>(numberOfCores, numberOfEventLoops);
}

} // namespace streaming
} // namespace kpsr