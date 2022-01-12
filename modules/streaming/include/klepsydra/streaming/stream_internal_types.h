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

#ifndef STREAMING_INTERNAL_TYPES_H
#define STREAMING_INTERNAL_TYPES_H

#include <atomic>
#include <memory>

namespace kpsr {
namespace streaming {

using UIDType = unsigned long;

class UUID
{
public:
    UUID()
        : uid(0)
    {}
    UIDType getUid() { return uid++; }

private:
    std::atomic<UIDType> uid;
};

template<class T>
class DataBatchWithId
{
public:
    DataBatchWithId()
        : id(0)
        , data(std::make_shared<T>(0))
    {}

    DataBatchWithId(unsigned long id, T &data)
        : id(id)
        , data(data)
    {}

    DataBatchWithId(unsigned long id, std::shared_ptr<T> &data)
        : id(id)
        , data(data)
    {}

    unsigned long id;
    std::shared_ptr<T> data;
};

} // namespace streaming
} // namespace kpsr

#endif // STREAMING_INTERNAL_TYPES_H
