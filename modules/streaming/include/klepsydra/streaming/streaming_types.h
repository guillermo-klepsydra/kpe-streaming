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

#ifndef STREAMING_TYPES_H
#define STREAMING_TYPES_H

#include <string>
#include <vector>
#include <klepsydra/streaming/aligned_allocator.h>

namespace kpsr {
namespace streaming {

using F32AlignedVector = std::vector<float, AlignedAllocator<float, 16>>;
using I32AlignedVector = std::vector<int32_t, AlignedAllocator<int32_t, 16>>;
using UI16AlignedVector = std::vector<uint16_t, AlignedAllocator<uint16_t, 16>>;
using I8AlignedVector = std::vector<int8_t, AlignedAllocator<int8_t, 16>>;
using UI8AlignedVector = std::vector<uint8_t, AlignedAllocator<uint8_t, 16>>;

}
}
#endif
