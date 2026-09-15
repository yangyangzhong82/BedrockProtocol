set_project("Protocol")
set_version("0.1.0")

set_languages("c++23")
set_allowedmodes("release", "debug", "releasedbg", "minsizerel")
add_rules("mode.release", "mode.debug", "mode.releasedbg", "mode.minsizerel")

set_policy("build.ccache", false)

-- ============================================================================
-- Options (mirror of the CMake PROTOCOL_ENABLE_* switches)
-- ============================================================================
option("detail_errors")
    set_default(false)
    set_showmenu(true)
    set_description("Enable detailed error information")
option_end()

option("codec")
    set_default(true)
    set_showmenu(true)
    set_description("Enable codec features")
option_end()

option("authentication")
    set_default(true)
    set_showmenu(true)
    set_description("Enable authentication features")
option_end()

option("connection")
    set_default(true)
    set_showmenu(true)
    set_description("Enable connection features")
option_end()

option("exceptions")
    set_default(true)
    set_showmenu(true)
    set_description("Enable C++ exceptions for the Protocol target")
option_end()

option("formatting")
    set_default(false)
    set_showmenu(true)
    set_description("Enable string formatting support for the Protocol target")
option_end()

-- ============================================================================
-- Custom packages for the SculkCatalystMC git dependencies
-- (these are not published to xrepo, so we vendor their build here)
-- ============================================================================

-- boost::pfr 2.2.0 (header-only, standalone usage)
package("sculk-boost-pfr")
    set_kind("library", {headeronly = true})
    add_urls("https://github.com/boostorg/pfr.git")
    add_versions("2.2.0", "2.2.0")
    on_install(function (package)
        os.cp("include", package:installdir())
    end)
package_end()

-- SculkCatalystMC/JSONC (only the header-only C++ API is used)
package("sculk-jsonc")
    set_kind("library", {headeronly = true})
    add_urls("https://github.com/SculkCatalystMC/JSONC.git")
    add_versions("main", "main")
    on_install(function (package)
        os.cp("include", package:installdir())
    end)
package_end()

-- SculkCatalystMC/Reflection (header-only, pulls jsonc + pfr + magic_enum)
package("sculk-reflection")
    set_kind("library", {headeronly = true})
    add_deps("sculk-jsonc", "sculk-boost-pfr", "magic_enum v0.9.7")
    add_urls("https://github.com/SculkCatalystMC/Reflection.git")
    add_versions("main", "main")
    on_install(function (package)
        os.cp("include", package:installdir())
    end)
package_end()

-- SculkCatalystMC/RakNet v1.0.0 (static library built from Source/*.cpp)
package("sculk-raknet")
    add_urls("https://github.com/SculkCatalystMC/RakNet.git")
    add_versions("v1.0.0", "v1.0.0")
    add_defines("RAKNET_SUPPORT_IPV6=1")
    if is_plat("windows") then
        add_syslinks("ws2_32")
    else
        add_syslinks("pthread")
    end
    on_install(function (package)
        local xmake_lua = [[
add_rules("mode.release", "mode.debug")
target("RakNet")
    set_kind("static")
    set_languages("c++23")
    add_files("Source/*.cpp")
    add_headerfiles("Source/(*.h)")
    add_includedirs("Source", {public = true})
    add_defines("RAKNET_SUPPORT_IPV6=1", {public = true})
    if is_plat("windows") then
        add_defines("WIN32", "_RAKNET_LIB")
        add_cxflags("/GS-", "/GR-")
    else
        add_cxflags("-fno-rtti", "-fno-stack-protector")
    end
]]
        io.writefile("xmake.lua", xmake_lua)
        import("package.tools.xmake").install(package)
    end)
package_end()

-- ============================================================================
-- Dependency resolution based on enabled features
-- ============================================================================
local enable_codec = has_config("codec")
local enable_auth = has_config("authentication")
local enable_conn = has_config("connection")

if enable_conn and (not enable_codec or not enable_auth) then
    raise("Invalid configuration: CONNECTION requires CODEC and AUTHENTICATION. "
        .. "Enable --codec=y and --authentication=y, or disable --connection=n.")
end

if enable_codec then
    add_requires("magic_enum v0.9.7")
end

if enable_auth or enable_conn then
    add_requires("openssl3")
    add_requires("sculk-reflection")
end

if enable_auth then
    add_requires("cpp-httplib")
    add_requires("parallel-hashmap")
end

if enable_conn then
    add_requires("concurrentqueue v1.0.4")
    add_requires("parallel-hashmap")
    add_requires("zlib")
    add_requires("snappy")
    add_requires("sculk-raknet")
end

-- ============================================================================
-- Protocol static library
-- ============================================================================
target("Protocol")
    set_kind("static")
    add_includedirs("include", {public = true})

    -- sources per enabled feature
    if enable_codec then
        add_files("src/sculk/protocol/codec/**.cpp")
    end
    if enable_auth then
        add_files("src/sculk/protocol/auth/**.cpp")
    end
    if enable_conn then
        add_files("src/sculk/protocol/connection/**.cpp")
    end

    -- public packages (types leak into installed headers)
    if enable_codec then
        add_packages("magic_enum", {public = true})
    end
    if enable_auth or enable_conn then
        add_packages("parallel-hashmap", {public = true})
    end
    if enable_conn then
        add_packages("concurrentqueue", {public = true})
        add_packages("sculk-raknet", {public = true})
    end

    -- private packages (implementation-only)
    if enable_auth or enable_conn then
        add_packages("openssl3", "sculk-reflection")
    end
    if enable_auth then
        add_packages("cpp-httplib")
        add_defines("CPPHTTPLIB_OPENSSL_SUPPORT", "CPPHTTPLIB_NO_EXCEPTIONS")
    end
    if enable_conn then
        add_packages("zlib", "snappy")
    end

    -- feature defines
    if enable_conn then
        add_defines("RAKNET_SUPPORT_IPV6=1", {public = true})
        add_defines("SCULK_PROTOCOL_ENABLE_CONNECTION", {public = true})
    end
    if has_config("detail_errors") then
        add_defines("SCULK_PROTOCOL_ENABLE_DETAIL_ERRORS", {public = true})
    end
    if enable_codec and has_config("formatting") then
        add_defines("SCULK_PROTOCOL_ENABLE_FORMATTING", {public = true})
    end

    -- detail errors force exceptions on
    local want_exceptions = has_config("exceptions") or has_config("detail_errors")

    -- compiler flags
    if is_plat("windows") then
        add_cxflags("/W4", "/utf-8", "/GR-", "/Zc:preprocessor", "/permissive-")
        add_defines("UNICODE", "NOMINMAX")
        if not want_exceptions then
            set_exceptions("no-cxx")
            add_defines("_HAS_EXCEPTIONS=0")
        end
    else
        add_cxflags("-Wall", "-Wextra", "-Wpedantic", "-fno-rtti")
        if not want_exceptions then
            set_exceptions("no-cxx")
        end
    end

    -- install headers for enabled features (mirror of CMake install dirs)
    add_headerfiles("include/sculk/protocol/Version.hpp")
    add_headerfiles("include/(sculk/protocol/utility/**)")
    if enable_codec then
        add_headerfiles("include/(sculk/protocol/codec/**)")
    end
    if enable_auth then
        add_headerfiles("include/(sculk/protocol/auth/**)")
    end
    if enable_conn then
        add_headerfiles("include/(sculk/protocol/connection/**)")
    end
target_end()
