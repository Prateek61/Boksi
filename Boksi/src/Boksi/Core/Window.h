#pragma once
#include "bkpch.h"

#include "Boksi/Events/Event.h"

namespace Boksi
{
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Boksi!!", uint32_t width = 1280, uint32_t height = 720)
            : Title(title), Width(width), Height(height) {}
    };

    // Interface representing a desktop system based Window
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        // virtual void* GetNativeWindow() const = 0;
        static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
    };
}