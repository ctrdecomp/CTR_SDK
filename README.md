# CTR_SDK

This is a decompilation of the CtrSDK, the standard SDK for the nintendo 3Ds.

The objective is to recreate the SDK for 3DS as accurately as possible.

Note that some names (especially for inlined, templated functions) are just plain guesses.

## Folder Structure

* **/LIBRARY_ROOT/CTR_SDK/**

*    |____ **addins** - Additional libraries used by the *CtrSDK*.

*    |____ **include/GLES2** - GL Headers used in the *CtrSDK*.

*    |____ **include/nn** - nn Headers used for the *CtrSDK*.

*    |____ **profiler/sources** - 3DS CPU Profiler used in debug builds.

*    |____ **sources/libraries** - Module source code.

*    |____ **template** - Project template.

## Addins

* **IS** - Intelligent Systems HostIO PC <---> CTR communications
* **KMC** - KMC HostIO Helper used for CTR.

## Libraries

* **applet** - Application (Initialization, sleep, finalization etc.)
* **camera** - Camera
* **cec** - Streetpass (Stresspass messages, manager, etc.)
* **cfg** - device Config
* **crt0** - C++ Runtime object
* **codec** - Code Decryption (IR Helper, cfg helper)
* **crypto** - Hash handler (SHA block, general hash reading, etc.)
* **CTR** - Program ID handler (Misc mainly)
* **cx** - Context (LZ11 / ZLIB File handlers)
* **dbg** - Debug (Printing, halting, device panicing)
* **dbm** - File Server IO
* **dev** - Developer (HostIO read/write)
* **drivers** - GX Drivers
* **dlp** - Download Play
* **dsp** - DSP audio
* **err** - Error
* **erreula** - Error EULA (for in game EULAs)
* **jpeg** - Image decrypter (`JPEG` / `JPG` image decrypter)
* **fnd** - Foundation
* **font** - Font (Identical to the Nw4c engines **font** class)
* **friends** - Friends (account friends manager)
* **fs** - File Server I/O
* **fslow** - Device file I/O backend
* **gr** - Geometry
* **gx** - Graphics
* **gxlow** - Graphics backend manager for GL
* **hardware** - Register access (ARMv11)
* **hid** - Human Interactable Device
* **hidlow** - Human Interactable Device backend
* **hio** - HostIO (Communication with PCs)
* **http** - Hypertext Transfer Protocol for CTR (WiFi / Network)
* **init** - crt0 Initializers
* **ir** - IR sensor
* **math** - Maths utilities (vector, matrix, etc.)
* **mic** - Microphone
* **ndm** - Network Daemon Manager (WiFi manager)
* **nstd** - NintendoStandard (std print, Memory moving / copying)
* **os** - Operating System (Initializing, OS things)
* **pl** - Pedometer helper
* **ptm** - Playtime manager
* **pxi** - PXI manager (contains the common exh.bin port names)
* **ro** - Relocation objects (Regisration lists,)
* **snd** - Sound
* **srv** - Service (manages the `port service` exheader system)
* **ssl** - Secure Sockets Layer
* **svc** - SVC (IPC Communicator)
* **ubl** - Black list library
* **ulcd** - ULCD Left/Right stereo manager
* **util** - Utilities (Since this uses C++03 these contain some pass arrounds)

### Version specific source

Different features of the CtrSDK can be implemented/left out in conjunction to which game the library is being used for:

Set `NN_VERSION` to:
- `NN_VERSION_CUSTOM`     (0): Starters for making a new configuration. 
- `NN_VERSION_MILLI4C`    (1): Mario & Luigi Dream Team
- `NN_VERSION_REDPEPPER`  (2): Super Mario 3D Land
- `NN_VERSION_CTRDASH`    (3): Mario Kart 7
- `NN_VERSION_GARDEN`     (4): Animal Crossing New Leaf: Welcome Amiibo!
- `NN_VERSION_STICKSTR`   (5): Paper Mario: Sticker Star

Presets and features for more games can be added if desired.

## Building

Building this project requires:

- ARM C++ Complier (ARMCC) Version 4.0/4.1/5.0 [which can be found here.](https://github.com/RE-Pepper/data/releases/tag/dasdasdsa)

### Configuration

CTR_SDK can be configured with several compile-time defines:

* `NN_BUILD_DEBUG`: Enables assertions. (Note: Debug builds use ARM flags `-O0` `-Otime`)
* `NN_BUILD_DEVELOPMENT`: Enables assertions but builds optimized. (Note: Development builds use ARM flags `-O3` `-Otime`)
* `NN_BUILD_RELEASE`: Disables assertions. (Note: Release builds use ARM flags `-O3` `-Otime`)
* `NN_BUILD_ENABLE_HOSTIO`: Allows the use of the `hostio` library.
* `NN_BUILD_ENABLE_RO`: Enables quirks with the `RO` service. Such as: no vfe and RTTI.
* `NN_PLATFORM_HAS_MMU`: If the device uses `nn::srv::Initialize` in its `nninitSystem` function enable this.
* `NN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK`: Disables Printing for SDK.
* `NN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK`: Disables Warning Printing for SDK.
* `NN_MATH_BUILD_FAST`: Uses `nn::math::C` functions compared to ASM.
* `NN_MATH_BUILD_FAST_C`: Allows use of `nn::math::FAST_C` functions to the standard 

## Contributing

### Non-inlined functions
When **implementing non-inlined functions**, please compare the assembly output against the original function and make it match the original code. At this scale, that is pretty much the only reliable way to ensure accuracy and functional equivalency.

However, given the large number of functions, certain kinds of small differences can be ignored when a function would otherwise be equivalent:

* Regalloc differences.

* Instruction reorderings when it is obvious the function is still semantically equivalent (e.g. two add/mov instructions that operate on entirely different registers being reordered)

When ignoring minor differences, add a `// NOT_MATCHING: explanation` comment and explain what does not match.

### Header utilities or inlined functions
For **header-only utilities** (like container classes), use pilot/debug builds, assertion messages and common sense to try to undo function inlining. For example, if you see the same assertion appear in many functions and the file name is a header file, or if you see identical snippets of code in many different places, chances are that you are dealing with an inlined function. In that case, you should refactor the inlined code into its own function.

Also note that introducing inlined functions is sometimes necessary to get the desired codegen.

If a function is inlined, you should try as hard as possible to make it match perfectly. For inlined functions, it is better to use weird code or small hacks to force a match as differences would otherwise appear in every single function that inlines the non-matching code, which drastically complicates matching other functions. If a hack is used, wrap it inside a `#ifdef MATCHING_HACK_CTR` (see above for a list of defines).

### Tentative PR Contributing rules
The `ctrdecomp` organization follows a set of standards to maintain consistency and quality across our projects. To help contributors meet these standards, our team has established the following guidelines:

* **All code must be submitted through the GitHub Pull Request process.**

* **Code must not be obtained from illegal or unauthorized material.** If such material is detected, the contribution **will not** be accepted.

* **Use of AI must be disclosed.** Contributors must disclose when and where they use AI.

* **All code must be reviewed by a human before submission.** Contributors are responible for reviewing to match styling, errors, etc.

* **Decompiled code should be reasonably representative of how the original source code may have been written. Avoid excessive or unnecessary pointer arithmetic when the underlying data is clearly identifiable as a struct or class.** In general, a raw Ghidra decompilation that merely compiles is not sufficient for PR acceptance; the code should be properly cleaned up, structured, and made readable.

* **Most functions should have a corresponding Doxygen documentation comment above its top-most declaration.** Most one-line functions are exempt here, but generally over 2-3 lines is a decent rule of thumb.

* **All code must be C++03-compliant.**