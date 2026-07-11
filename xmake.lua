set_project("Protocol")
set_version("0.1.0")

set_languages("c++23")
set_allowedmodes("release", "debug", "releasedbg", "minsizerel")
add_rules("mode.release", "mode.debug", "mode.releasedbg", "mode.minsizerel")

set_policy("build.ccache", false)

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

-- SculkCatalystMC/JSONC v1.4.0 (only the header-only C++ API is used)
package("sculk-jsonc")
    set_kind("library", {headeronly = true})
    add_urls("https://github.com/SculkCatalystMC/JSONC.git")
    add_versions("v1.4.0", "v1.4.0")
    on_install(function (package)
        os.cp("include", package:installdir())
    end)
package_end()

-- SculkCatalystMC/Reflection v1.4.0 (header-only, pulls jsonc + pfr + magic_enum)
package("sculk-reflection")
    set_kind("library", {headeronly = true})
    add_deps("sculk-jsonc", "sculk-boost-pfr", "magic_enum v0.9.7")
    add_urls("https://github.com/SculkCatalystMC/Reflection.git")
    add_versions("v1.4.0", "v1.4.0")
    on_install(function (package)
        os.cp("include", package:installdir())
    end)
package_end()

-- ============================================================================
-- Dependencies
-- ============================================================================
add_requires("openssl3")
add_requires("magic_enum v0.9.7")
add_requires("sculk-reflection")

-- ============================================================================
-- Protocol static library
-- ============================================================================
target("Protocol")
    set_kind("static")
    add_includedirs("include", {public = true})

    add_files("src/**.cpp")

    -- magic_enum types appear in public headers
    add_packages("magic_enum", {public = true})
    -- implementation-only dependencies
    add_packages("sculk-reflection", "openssl3")

    -- SCULK_PROTOCOL_DEBUG only in debug config
    if is_mode("debug") then
        add_defines("SCULK_PROTOCOL_DEBUG")
    end

    if is_plat("windows") then
        add_cxflags("/W4", "/utf-8")
        add_defines("UNICODE", "NOMINMAX")
        -- reflection v1.4.0's string_utils.hpp uses std::from_chars / std::errc
        -- without including <charconv> / <system_error>; MSVC does not pull them
        -- in transitively, so force-include them here.
        add_cxflags("/FIcharconv", "/FIsystem_error")
    else
        add_cxflags("-Wall", "-Wextra", "-Wpedantic")
        add_cxflags("-include", "charconv", "-include", "system_error", {force = true})
    end

    add_headerfiles("include/(**)")
target_end()
