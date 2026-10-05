# PROJECT HORIZON
#
# HorizonOS CTR

cmake_minimum_required(VERSION 3.24)

project(CTR_SDK C CXX ASM)

if(NN_BUILD_DEBUG)
    add_compile_definitions(NN_BUILD_DEBUG)
elseif(NN_BUILD_DEVELOPMENT)
    add_compile_definitions(NN_BUILD_DEVELOPMENT)
elseif(NN_BUILD_RELEASE)
    add_compile_definitions(NN_BUILD_RELEASE)
endif()

if(NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK)
    add_compile_definitions(NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK)
elseif(NN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK)
    add_compile_definitions(NN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK)
endif()



function(nn_library TARGET_NAME MODULE_DIR)
    file(GLOB_RECURSE MODULE_SOURCES
        "${MODULE_DIR}/*.cpp"
        "${MODULE_DIR}/*.c"
        "${MODULE_DIR}/*.s"
        "${MODULE_DIR}/*.asm"
        "${MODULE_DIR}/*.inc"
    )

    if(NOT MODULE_SOURCES)
        message(STATUS "Skipping ${TARGET_NAME} (no sources)")
        return()
    endif()

    add_library(${TARGET_NAME} STATIC ${MODULE_SOURCES})

    set_target_properties(${TARGET_NAME} PROPERTIES
        OUTPUT_NAME "${TARGET_NAME}"
        SUFFIX ".fast.a"
    )

    if(NN_BUILD_DEBUG)
        target_compile_options(${TARGET_NAME} PRIVATE -O0)
    endif()

    # Test this.
    if(NN_BUILD_ENABLE_RO)
        target_compile_options(${TARGET_NAME} PRIVATE --no_vfe)
    endif()

    target_compile_options(${TARGET_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:
        --multibyte-chars --signed_chars --cpu=MPCore
        --dollar --littleend --wchar --wchar16 --library_interface=rvct
        --apcs=//interwork --data-reorder --split_sections
        --debug_info=line_inlining_extensions --force_new_nothrow --fpmode=fast --no_exceptions --remarks --vfe --no_allow_fpreg_for_nonfpdata
        --diag_suppress=3017,940,997,177,541,1,1134,550,83,427,2525,117,284,174,111,1441,161,826,177,1549,513,1441,340,228,2530,1297,401,261,381,2516,2192,1596,193,113,1035,2523,2091,1663,1801,1621,2519,1676,63,286,74,271,2945,1301,1945,1847,2813,68,2523
        --cpp --arm -O3 -Otime --gnu
        -DNN_BUILD_RELEASE -DNN_SWITCH_ENABLE_MEMBER_NAME_SHORTCUT
        >
    )

    target_compile_options(${TARGET_NAME} PRIVATE $<$<COMPILE_LANGUAGE:C>:
        --multibyte-chars --signed_chars --cpu=MPCore
        --dollar --littleend --wchar --wchar16
        --apcs=//interwork --data-reorder --split_sections
        --debug_info=line_inlining_extensions --force_new_nothrow --fpmode=fast --no_exceptions --rtti --rtti_data --remarks --no_vfe --no_allow_fpreg_for_nonfpdata
        --diag_suppress=3017,940,997,177,541,1,1134,550,83,427,2525,117,284,174,111,1441,161,826,177,1549,513,1441,340,228,2530,1297,401,261,381,2516,2192,1596,193,113,1035,2523,2091,1663,1801,1621,2519,1676,63,286,74,271,2945,1301,1945,1847,2813,68,2523
        --c99 --arm -O3 -Otime
        -DNN_BUILD_RELEASE -DNN_SWITCH_ENABLE_MEMBER_NAME_SHORTCUT
        >
    )

    target_include_directories(${TARGET_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${MODULE_DIR}
        ${MODULE_DIR}/CTR
        ${MODULE_DIR}/CTR/MPCore
    )

    target_compile_definitions(${TARGET_NAME} PUBLIC
        NON_MATCHING=
    )
endfunction()

nn_library(libnn_applet  ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/applet)
nn_library(libnn_camera  ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/camera)
nn_library(libnn_cec     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/cec)
nn_library(libnn_cfg     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/cfg)
nn_library(crt0          ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/crt0)
nn_library(libnn_crypto  ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/crypto)
nn_library(libnn_cx      ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/cx)
nn_library(libnn_dbg     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/dbg)
nn_library(libnn_dbm     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/dbm)
nn_library(libnn_dev     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/dev)
#nn_library(libnn_dlp     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/dlp)
nn_library(libnn_dsp     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/dsp)
nn_library(libnn_err     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/err)
nn_library(libnn_erreula ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/erreula)
nn_library(libnn_jpeg    ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/jpeg)
nn_library(libnn_fnd     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/fnd)
nn_library(libnn_font    ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/font)
#nn_library(libnn_friends ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/friends)
nn_library(libnn_fs      ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/fs)
nn_library(libnn_fslow   ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/fslow)
nn_library(libnn_gr      ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/gr)
nn_library(libnn_gxlow   ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/gxlow)
nn_library(libnn_hid     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/hid)
nn_library(libnn_hidlow  ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/hidlow)
nn_library(libnn_hio     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/hio, ${CMAKE_CURRENT_SOURCE_DIR}/addins/IS/hio, ${CMAKE_CURRENT_SOURCE_DIR}/addins/KMC/HostIO/HostIO)
#nn_library(libnn_http    ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/http)
nn_library(libnn_init    ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/init)
nn_library(libnn_ir      ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/ir)
nn_library(libnn_math    ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/math)
nn_library(libnn_mic     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/mic)
nn_library(libnn_ndm     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/ndm)
nn_library(libnn_nstd    ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/nstd)
nn_library(libnn_os      ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/os)
nn_library(libnn_pl      ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/pl)
nn_library(libnn_ptm     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/ptm)
nn_library(libnn_rtport  ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/rtport/rvct)
nn_library(libnn_snd     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/snd)
nn_library(libnn_srv     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/srv)
#nn_library(libnn_ssl     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/ssl)
nn_library(libnn_svc     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/svc)
nn_library(libnn_ubl     ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/ugc/ubl)
nn_library(libnn_ulcd    ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/ulcd)
nn_library(libnn_util    ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/util)
nn_library(libnn_gles2      ${CMAKE_CURRENT_SOURCE_DIR}/sources/libraries/gles2)

add_library(CTR_SDK INTERFACE)

target_link_libraries(CTR_SDK INTERFACE
    libnn_applet
    libnn_camera
    libnn_cec
    libnn_cfg
    crt0
    libnn_crypto
    libnn_cx
    libnn_dbg
    libnn_dbm
    libnn_dev
    libnn_dsp
    libnn_err
    libnn_erreula
    libnn_jpeg
    libnn_fnd
    libnn_font
    libnn_fs
    libnn_fslow
    libnn_gr
    libnn_gxlow
    libnn_hid
    libnn_hidlow
    libnn_init
    libnn_ir
    libnn_math
    libnn_mic
    libnn_ndm
    libnn_nstd
    libnn_os
    libnn_pl
    libnn_ptm
    libnn_rtport
    libnn_snd
    libnn_srv
    libnn_svc
    libnn_ubl
    libnn_ulcd
    libnn_util
    libnn_gles2
)