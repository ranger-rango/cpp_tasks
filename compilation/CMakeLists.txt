cmake_minimum_required(VERSION 3.10)
project(HelloLib)

add_library(hello SHARED hello.cpp)

add_executable(main main.cpp)
target_link_libraries(main hello)

# add_executable(myapp main.cpp)

# Find your MinGW DLLs (adjust path as needed)
# set(MINGW_DLL_DIR "/usr/x86_64-w64-mingw32/bin")

# # List the needed DLLs
# set(MINGW_DLLS
#     "${MINGW_DLL_DIR}/libwinpthread-1.dll"
#     "${MINGW_DLL_DIR}/libgcc_s_seh-1.dll"
#     "${MINGW_DLL_DIR}/libstdc++-6.dll"
# )

# # Copy them after build
# add_custom_command(TARGET myapp POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         ${MINGW_DLLS}
#         $<TARGET_FILE_DIR:myapp>
# )
