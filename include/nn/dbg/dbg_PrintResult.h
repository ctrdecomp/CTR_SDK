#pragma once

#include <nn/Assert.h>

namespace nn{
namespace dbg{
namespace detail{
void PrintResult(Result result);
void TPrintResult(Result result);
}
}
}

#if !defined(NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK) || !defined(NN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK)
#define NN_DBG_DECLARE_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(module) \
                namespace nn { namespace module { namespace detail { \
                    void GetResultDescriptionStringImplKeeper(); \
                }}}
#define NN_DBG_DECLARE_ADDITIONAL_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(module, option) \
                namespace nn { namespace module { namespace detail { \
                    void GetResult##option##DescriptionStringImplKeeper(); \
                }}}
#define NN_DBG_USE_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(module) \
                (::nn::module::detail::GetResultDescriptionStringImplKeeper())
#define NN_DBG_USE_ADDITIONAL_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(module, option) \
                (::nn::module::detail::GetResult##option##DescriptionStringImplKeeper())
#define NN_DBG_PRINT_RESULT(exp)    ::nn::dbg::detail::PrintResult(exp)
#define NN_DBG_PRINT_TRESULT(exp)    ::nn::dbg::detail::TPrintResult(exp)
#define NN_DBG_CHECK_RESULT(exp)    NN_PANIC_IF_FAILED(exp)
#else
#define NN_DBG_DECLARE_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(module)
#define NN_DBG_DECLARE_ADDITIONAL_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(module, option)
#define NN_DBG_USE_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(module) (void)0
#define NN_DBG_USE_ADDITIONAL_GET_RESULT_DESCRIPTION_STRING_IMPL_KEEPER(module, option) (void)0
#define NN_DBG_PRINT_RESULT(exp)    ((void)(exp))
#define NN_DBG_PRINT_TRESULT(exp)   ((void)(exp))
#define NN_DBG_CHECK_RESULT(exp)    ((void)(exp))
#endif
