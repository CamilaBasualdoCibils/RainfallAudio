include(FetchContent)

# -----------------------------
# Fetch PortAudio
# -----------------------------
FetchContent_Declare(
    portaudio
    GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
    GIT_TAG master  # or a specific release like v19.8.0
)
set(PORTAUDIO_BUILD_STATIC ON)
set(PORTAUDIO_BUILD_SHARED OFF)
set(PORTAUDIO_BUILD_EXAMPLES OFF)
set(PORTAUDIO_BUILD_TESTS OFF)
FetchContent_MakeAvailable(portaudio)