include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)      
# --- Fetch BrassUtils from your repo ---
FetchContent_Declare(
    brassutils
    GIT_REPOSITORY https://github.com/CopperGearSoftwareLLC/BrassUtils.git  # Replace with actual URL
    GIT_TAG main  # Or a specific tag/branch
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(brassutils)