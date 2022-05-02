cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

option(WITH_GTEST "Whether to build Gtest test" ON)

project(CustomAllocator VERSION ${PROJECT_VESRION})

add_executable(CustomAllocator_cli
                main.cpp
                custom_allocator.h
                custom_forward_list.h
                )

set_target_properties(CustomAllocator_cli
                     PROPERTIES
                     CXX_STANDARD 14
                     CXX_STANDARD_REQUIRED ON)

if(WITH_GTEST)
    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(tests tests.cpp)

    set_target_properties(tests PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(tests
        gtest_main
    )
endif()

if (MSVC)
    target_compile_options(CustomAllocator_cli PRIVATE
        /W4
    )
    if(WITH_GTEST)
        target_compile_options(tests PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(CustomAllocator_cli PRIVATE
        -Wall -pedantic -Werror
    )
    if(WITH_GTEST)
        target_compile_options(tests PRIVATE
            -Wall -pedantic -Werror
        )
    endif()
endif()

install(TARGETS CustomAllocator_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_GTEST)
    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(tests)
endif()


