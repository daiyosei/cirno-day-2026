cmake_minimum_required(VERSION 3.18)
project(cirno_day LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cirno_day
    main.cpp
)

target_include_directories(cirno_day PRIVATE "${CMAKE_SOURCE_DIR}")

add_subdirectory(lib)
# add_subdirectory(screen)
add_subdirectory(game)

set(PROCESSED_ASSETS_DIR "${CMAKE_BINARY_DIR}/assets")
file(GLOB_RECURSE ASSET_FILES RELATIVE "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_SOURCE_DIR}/assets/*")

find_program(MAGICK_TOOL NAMES magick)
find_program(CONVERT_TOOL NAMES convert)
find_program(FFMPEG_TOOL NAMES ffmpeg)
find_program(MINIFY_TOOL NAMES minify)

set(ASSET_OUTPUTS)
foreach(ASSET_REL ${ASSET_FILES})
  set(ASSET_SRC "${CMAKE_SOURCE_DIR}/assets/${ASSET_REL}")
  set(ASSET_DST "${PROCESSED_ASSETS_DIR}/${ASSET_REL}")
  get_filename_component(ASSET_DST_DIR "${ASSET_DST}" DIRECTORY)
  string(TOLOWER "${ASSET_REL}" ASSET_REL_LOWER)

  if(ASSET_REL_LOWER MATCHES "\\.(png|jpg|jpeg)$")
    if(MAGICK_TOOL)
      add_custom_command(
        OUTPUT "${ASSET_DST}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSET_DST_DIR}"
        COMMAND "${MAGICK_TOOL}" convert "${ASSET_SRC}" -strip -quality 80 "${ASSET_DST}"
        DEPENDS "${ASSET_SRC}"
        COMMENT "Compressing image ${ASSET_REL}"
        VERBATIM
      )
    elseif(CONVERT_TOOL)
      add_custom_command(
        OUTPUT "${ASSET_DST}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSET_DST_DIR}"
        COMMAND "${CONVERT_TOOL}" "${ASSET_SRC}" -strip -quality 80 "${ASSET_DST}"
        DEPENDS "${ASSET_SRC}"
        COMMENT "Compressing image ${ASSET_REL}"
        VERBATIM
      )
    else()
      add_custom_command(
        OUTPUT "${ASSET_DST}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSET_DST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ASSET_SRC}" "${ASSET_DST}"
        DEPENDS "${ASSET_SRC}"
        COMMENT "Image tool not found, copying ${ASSET_REL}"
        VERBATIM
      )
    endif()
#[[
  elseif(ASSET_REL_LOWER MATCHES "\\.wav$")
    if(FFMPEG_TOOL)
      if(ASSET_REL_LOWER STREQUAL "audio/sfx/radio_static.wav")
        # Raylib streaming is sensitive to this file with aggressive ADPCM compression.
        # Keep a safer/light transform for stability on both native + web.
        add_custom_command(
          OUTPUT "${ASSET_DST}"
          COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSET_DST_DIR}"
          COMMAND "${FFMPEG_TOOL}" -y -loglevel error -i "${ASSET_SRC}" -ac 1 -ar 22050 -c:a pcm_s16le "${ASSET_DST}"
          DEPENDS "${ASSET_SRC}"
          COMMENT "Converting radio static with safe WAV settings ${ASSET_REL}"
          VERBATIM
        )
      else()
        add_custom_command(
          OUTPUT "${ASSET_DST}"
          COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSET_DST_DIR}"
          COMMAND "${FFMPEG_TOOL}" -y -loglevel error -i "${ASSET_SRC}" -ac 1 -ar 22050 -c:a adpcm_ima_wav "${ASSET_DST}"
          DEPENDS "${ASSET_SRC}"
          COMMENT "Compressing audio ${ASSET_REL}"
          VERBATIM
        )
      endif()

    else()
      add_custom_command(
        OUTPUT "${ASSET_DST}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSET_DST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ASSET_SRC}" "${ASSET_DST}"
        DEPENDS "${ASSET_SRC}"
        COMMENT "ffmpeg not found, copying ${ASSET_REL}"
        VERBATIM
      )
    endif()
]]

  else()
    add_custom_command(
      OUTPUT "${ASSET_DST}"
      COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSET_DST_DIR}"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ASSET_SRC}" "${ASSET_DST}"
      DEPENDS "${ASSET_SRC}"
      COMMENT "Copying asset ${ASSET_REL}"
      VERBATIM
    )
  endif()

  list(APPEND ASSET_OUTPUTS "${ASSET_DST}")
endforeach()

add_custom_target(prepare_assets ALL DEPENDS ${ASSET_OUTPUTS})
add_dependencies(cirno_day prepare_assets)

find_library(RAYLIB NAMES raylib HINTS "/usr/local/lib")

if (EMSCRIPTEN)
  set(BUILD_EXAMPLES OFF CACHE BOOL "Disable raylib examples" FORCE)
  set(BUILD_GAMES OFF CACHE BOOL "Disable raylib games" FORCE)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build raylib statically for web" FORCE)
  set(BUILD_TESTING OFF CACHE BOOL "Disable raylib tests" FORCE)
  set(PLATFORM_DESKTOP OFF CACHE BOOL "Disable desktop raylib platform" FORCE)
  set(PLATFORM Web CACHE STRING "Platform to build for." FORCE)

  add_subdirectory("${CMAKE_SOURCE_DIR}/temp/raylib_source" "${CMAKE_BINARY_DIR}/raylib" EXCLUDE_FROM_ALL)
  target_compile_options(raylib PRIVATE
      "$<$<CONFIG:Release>:-Oz>"
      "$<$<CONFIG:Release>:-flto>"
    )

  # set_property(TARGET raylib PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

  set_source_files_properties(
      "${CMAKE_SOURCE_DIR}/temp/raylib_source/src/raudio.c"
      TARGET_DIRECTORY raylib
      PROPERTIES COMPILE_OPTIONS "-fno-lto;-fno-strict-aliasing"
    )

  set(WEB_SHELL_ASSET_SRC "${CMAKE_SOURCE_DIR}/web/static/yinyang.svg")
  set(WEB_SHELL_ASSET_DST "${CMAKE_BINARY_DIR}/static/yinyang.svg")
  if (NOT EXISTS "${WEB_SHELL_ASSET_SRC}")
    message(FATAL_ERROR "Web shell asset not found: ${WEB_SHELL_ASSET_SRC}")
  endif()

  target_link_libraries(cirno_day PRIVATE raylib)
  add_custom_command(
    OUTPUT "${WEB_SHELL_ASSET_DST}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${WEB_SHELL_ASSET_SRC}" "${WEB_SHELL_ASSET_DST}"
    DEPENDS "${WEB_SHELL_ASSET_SRC}"
    COMMENT "Copying web shell asset yinyang.svg"
    VERBATIM
  )
  add_custom_target(prepare_web_shell_assets ALL DEPENDS "${WEB_SHELL_ASSET_DST}")
  add_dependencies(cirno_day prepare_web_shell_assets)

  set_target_properties(cirno_day PROPERTIES SUFFIX ".html")
  set_property(TARGET cirno_day APPEND PROPERTY LINK_DEPENDS "${CMAKE_SOURCE_DIR}/web/shell.html")
  target_link_options(cirno_day PRIVATE
    "-sUSE_GLFW=3"
    "--shell-file" "${CMAKE_SOURCE_DIR}/web/shell.html"
    "--preload-file" "${PROCESSED_ASSETS_DIR}@/assets"
  )
  target_compile_options(cirno_day PRIVATE
    "$<$<CONFIG:Release>:-Oz>"
    "$<$<CONFIG:Release>:-flto>"
    "$<$<CONFIG:Release>:-fno-exceptions>"
    "$<$<CONFIG:Release>:-fno-rtti>"
)

  target_link_options(cirno_day PRIVATE
    "$<$<CONFIG:Release>:-Oz>"
    "$<$<CONFIG:Release>:-flto>"
    "$<$<CONFIG:Release>:-ffast-math>"
    "$<$<CONFIG:Release>:-sASSERTIONS=0>"
    "$<$<CONFIG:Release>:-sSAFE_HEAP=0>"
    "$<$<CONFIG:Release>:-sENVIRONMENT=web>"
    "$<$<CONFIG:Release>:--closure=1>"

    # remove webgl strings
    "$<$<CONFIG:Release>:-sGL_TRACK_ERRORS=0>"
    "$<$<CONFIG:Release>:-sDISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1>"

    # limit memory usage to 192mb
    "$<$<CONFIG:Release>:-sALLOW_MEMORY_GROWTH=0>"
    "$<$<CONFIG:Release>:-sINITIAL_MEMORY=201326592>"

    "$<$<CONFIG:Release>:-sEXPORTED_RUNTIME_METHODS=ccall,cwrap,setCanvasSize>"

    "$<$<CONFIG:Release>:-sEXPORTED_FUNCTIONS=_main>"
)
  if (MINIFY_TOOL)
    add_custom_command(
      TARGET cirno_day
      POST_BUILD
      COMMAND "${MINIFY_TOOL}" -q -i "${CMAKE_CURRENT_BINARY_DIR}/cirno_day.html"
      COMMENT "Minifying Emscripten outputs"
      VERBATIM
    )
  endif()
elseif(RAYLIB)
  target_link_libraries(cirno_day PRIVATE raylib)
else()
  find_package(raylib REQUIRED)
  target_link_libraries(cirno_day PRIVATE raylib)
endif()

if (UNIX AND NOT APPLE AND NOT EMSCRIPTEN)
  target_link_libraries(cirno_day PRIVATE m pthread dl rt X11)
endif()
