#pragma once

class TextInputProcessor;
class TextInputObserver {
    public:
    virtual ~TextInputObserver() = default;

    virtual void subscribeTextInput(TextInputProcessor& processor);
    virtual void unsubscribeTextInput(TextInputProcessor& processor);

    virtual bool onTextInput(int unicode) = 0;
};