#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

// SDL3_mixer 3.x objects
static MIX_Mixer *mixer = NULL;

// Audio Assets (Loaded raw audio buffers)
static MIX_Audio *music_audio = NULL;
static MIX_Audio *ogg_audio = NULL;
static MIX_Audio *wav_audio = NULL;

// Tracks (Playback channels)
static MIX_Track *music_track = NULL;
static MIX_Track *ogg_track = NULL;
static MIX_Track *wav_track = NULL;

// Rectangles for sound triggers (Logical Coordinates)
static SDL_FRect button_ogg = { 140.0f, 1200.0f, 350.0f, 150.0f };
static SDL_FRect button_wav = { 590.0f, 1200.0f, 350.0f, 150.0f };

// Logical reference resolution
#define LOGICAL_WIDTH 1080.0f
#define LOGICAL_HEIGHT 1920.0f

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "60"))
    {
        SDL_Log("Failed to set a frame rate: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Initialize Video and Audio Subsystems
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Initialize SDL3_mixer
    if (!MIX_Init())
    {
        SDL_Log("Couldn't initialize SDL_mixer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create the primary mixer device
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer)
    {
        SDL_Log("Mixer creation failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("SDL3 Audio Example", 0, 0,
        SDL_WINDOW_FULLSCREEN | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, (int)LOGICAL_WIDTH,
        (int)LOGICAL_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // --- Load Audio Files ---
    music_audio = MIX_LoadAudio(mixer, "audio/song18.mp3", true);
    if (!music_audio) {
        SDL_Log("Failed to load music: %s", SDL_GetError());
    } else {
        music_track = MIX_CreateTrack(mixer);
        if (music_track) {
            MIX_SetTrackAudio(music_track, music_audio);
            // -1 sets infinite looping for background music
            MIX_SetTrackLoops(music_track, -1);
            MIX_PlayTrack(music_track, 0);
        }
    }

    ogg_audio = MIX_LoadAudio(mixer, "audio/qubodup-crash.ogg", true);
    if (!ogg_audio) {
        SDL_Log("Failed to load OGG sound: %s", SDL_GetError());
    } else {
        ogg_track = MIX_CreateTrack(mixer);
        if (ogg_track) {
            MIX_SetTrackAudio(ogg_track, ogg_audio);
        }
    }

    wav_audio = MIX_LoadAudio(mixer, "audio/spell3.wav", true);
    if (!wav_audio) {
        SDL_Log("Failed to load WAV sound: %s", SDL_GetError());
    } else {
        wav_track = MIX_CreateTrack(mixer);
        if (wav_track) {
            MIX_SetTrackAudio(wav_track, wav_audio);
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        SDL_FPoint point;
        // Map raw window coordinates to presentation logical coordinates
        SDL_RenderCoordinatesFromWindow(renderer, event->button.x, event->button.y, &point.x, &point.y);

        if (SDL_PointInRectFloat(&point, &button_ogg))
        {
            if (ogg_track) {
                MIX_PlayTrack(ogg_track, 0);
            }
        }
        else if (SDL_PointInRectFloat(&point, &button_wav))
        {
            if (wav_track) {
                MIX_PlayTrack(wav_track, 0);
            }
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 38, 43, 51, 255);
    SDL_RenderClear(renderer);

    // Draw OGG sound button (Red)
    SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
    SDL_RenderFillRect(renderer, &button_ogg);

    // Draw WAV sound button (Blue)
    SDL_SetRenderDrawColor(renderer, 50, 120, 220, 255);
    SDL_RenderFillRect(renderer, &button_wav);

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    MIX_Quit();
}
