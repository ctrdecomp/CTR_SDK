#pragma once

#include <nn/Result.h>
#include <nn/Handle.h>

namespace nn {
namespace ssl {

    Result Initialize();
    Result Finalize();

namespace 
{
    const char PORT_NAME_CONNECTION[]        = "ssl:C";

    enum IpcPortType
    {
        PORT_CONNECTION,
        NUM_OF_PORTS
    };
} // namespace

} // namespace ssl
} // namespace nn

