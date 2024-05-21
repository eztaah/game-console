#include <raylib.hpp>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void Update();

    bool IsMusicplaying() const;

    void PlayMusic();
    void StopMusic();

private:
    Music _backgroundMusic;
};