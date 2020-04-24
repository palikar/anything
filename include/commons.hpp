#pragma once


#define AY_COMPILER_VERSION __VERSION__

#if defined(_WIN32)
#define AY_WIN
#define AY_HAS_DECLSPEC
#if defined(__llvm__)
#define AY_COMPILER_NAME "clang(windows)"
#elif defined(__GNUC__)
#define AY_COMPILER_NAME "gcc(mingw)"
#else
#define AY_COMPILER_NAME "msvc"
#endif
#else
#define AY_POSIX
#if defined(__llvm__)
#define AY_COMPILER_NAME "clang"
#elif defined(__GNUC__)
#define AY_COMPILER_NAME "gcc"
#else
#define AY_COMPILER_NAME "unknown"
#endif
#endif


#ifdef _WIN32
inline constexpr auto AY_OS_NAME = "windows-32";
#elif _WIN64
inline constexpr auto AY_OS_NAME   = "windows-64";
#elif __APPLE__ || __MACH__
inline constexpr auto AY_OS_NAME   = "mac-osx";
#elif __linux__
inline constexpr auto AY_OS_NAME   = "linux";
#elif __FreeBSD__
inline constexpr auto AY_OS_NAME   = "freebsd";
#elif __unix || __unix__
inline constexpr auto AY_OS_NAME   = "unix";
#else
inline constexpr auto AY_OS_NAME = "unknow";
#endif


#ifdef __i386__
inline constexpr auto AY_ARCH_NAME = "i386";
#elif __x86_64__
inline constexpr auto AY_ARCH_NAME = "x86_64";
#elif __arm__
inline constexpr auto AY_ARCH_NAME = "arm";
#elif __powerpc64__
inline constexpr auto AY_ARCH_NAME = "power64pc";
#elif __aarch64__
inline constexpr auto AY_ARCH_NAME = "aarch64";
#else
inline constexpr auto AY_ARCH_NAME = "unknow";
#endif


#ifndef NDEBUG
#define AY_DEBUG_BUILD true
#else
#define AY_DEBUG_BUILD false
#endif


namespace ay
{

constexpr static const int version_major = AY_VERSION_MAJOR;
constexpr static const int version_minor = AY_VERSION_MINOR;
constexpr static const int version_patch = AY_VERSION_PATCH;

constexpr static const char *compiler_version = AY_COMPILER_VERSION;
constexpr static const char *compiler_name    = AY_COMPILER_NAME;
constexpr static const bool debug_build       = AY_DEBUG_BUILD;

struct BuildInfo
{

    [[nodiscard]] constexpr static int version_major() noexcept
    {
        return ay::version_major;
    }

    [[nodiscard]] constexpr static int version_minor() noexcept
    {
        return ay::version_minor;
    }

    [[nodiscard]] constexpr static int version_patch() noexcept
    {
        return ay::version_patch;
    }

    [[nodiscard]] constexpr static bool debug_build() noexcept
    {
        return ay::debug_build;
    }

    [[nodiscard]] static std::string version()
    {
        return std::to_string(version_major()) + '.' + std::to_string(version_minor())
               + '.' + std::to_string(version_patch());
    }

    [[nodiscard]] static std::string build()
    {
        return std::to_string(version_major()) + '.' + std::to_string(version_minor())
               + '.' + std::to_string(version_patch()) + ':'
               + (ay::debug_build ? " Debug" : " Release");
    }
};

}  // namespace ay
