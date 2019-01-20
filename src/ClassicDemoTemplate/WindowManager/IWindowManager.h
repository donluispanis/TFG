#pragma once

struct Pixel;

class IWindowManager
{
  public:
    virtual void CreateWindow(const char *name, const int width, const int height, const bool fullscreen) = 0;
    virtual void DestroyWindow() = 0;
    virtual ~IWindowManager() = 0;

    virtual void UpdateWindow() = 0;
    virtual void DrawToScreen() = 0;

    virtual void SetFramerateToShow(bool fps) = 0;

    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;
    virtual Pixel *GetScreenPixels() = 0;
    virtual double GetDeltaTime() = 0;
    virtual bool IsWindowOpen() = 0;

    virtual bool IsKeyDown(int key) = 0;
};