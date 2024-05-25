#pragma once


class WindowManager {
public:
    WindowManager(int width, int height, const char* title);
    ~WindowManager();

    // === Logic ===
    void ManageWindowResizing(int newWidth, int newHeight);
    void ManageFullScreen();
};
