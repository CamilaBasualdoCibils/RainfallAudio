include(FetchContent)

# -----------------------------
# Fetch PortAudio
# -----------------------------
FetchContent_Declare(
    portaudio
    GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
    GIT_TAG master  # or a specific release like v19.8.0
)
set(PA_BUILD_SHARED_LIBS OFF)

FetchContent_MakeAvailable(portaudio)