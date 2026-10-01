cmake_minimum_required(VERSION 3.24)

if(_ARMCC_CMAKE_GLOBAL)
  return()
endif()
set(_ARMCC_CMAKE_GLOBAL TRUE)

include(${CMAKE_SOURCE_DIR}/project/tc.cmake)

if(WIN32)
    set(_OPT_LANG --locale=japanese)
else()
    set(_OPT_LANG --locale=ja_JP.SJIS)
endif()

if(WARNS)
    add_compile_options(--remarks)
    set(_OPT_DIAG --diag_suppress=186,340,401,1256,1297,1568,1764,1786,1788,2523,2819,96,1794,1801,2442,3017,optimizations,6642
              --diag_error=68,88,174,188,223
              --diag_warning=177,193,228,550,826,1301)
else()
    set(_OPT_DIAG --diag_suppress=6642)
endif()


set(_OPT_TYPE --preinclude=${CMAKE_SOURCE_DIR}/library/CTR_SDK/include/nn/types.h) # WHERE EVER THE CTR_SDK IS!!

add_link_options(--cpu=MPCore --fpu=VFPv2 --vfemode=force --diag_suppress=L6314W,L6329W ${_OPT_DIAG})

add_compile_options($<$<COMPILE_LANGUAGE:C>:--c99> $<$<COMPILE_LANGUAGE:CXX>:--cpp>)

add_compile_options(--arm --split_sections -c ${_OPT_DIAG} ${_OPT_TYPE} ${_OPT_LANG})
add_compile_options(--cpu=MPCore --fpmode=fast --apcs=interwork -O3 -Otime --data_reorder)
add_compile_options(--sys_include --signed_chars --dollar --no_vla --multibyte_chars)
add_compile_options(--force_new_nothrow)
add_compile_options(--no_exceptions --cpp --no_rtti --no_vfe --no_rtti_data)

add_compile_definitions(NN_PLATFORM_CTR)
add_compile_definitions(NW_PLATFORM_CTR)
add_compile_definitions(SEAD_PLATFORM_CTR)

if (NOT PROJECT_VERSION)
  message("NO VERSION SPECIFIED, PLEASE DEFINE PROJECT_VERSION")
endif()
add_compile_definitions(PROJECT_VERSION="${PROJECT_VERSION}")