#pragma once

class Keyboard : public Singleton<Keyboard>
{
public:
    enum Inputs
    {
        NON,
        LMB,
        RMB,
        LMBRMB,
        CTRL,
        CTRLRMB,
        SPACE,
        CTRLSPACE,
        SHIFT,
        BUG1,
        BUG2
        
    };

private:
    friend class Singleton;

    static const UINT KEY_MAX = 255;

    enum KeyState
    {
        NONE, DOWN, UP, PRESS
    };   


    Keyboard() { keyStorage.push_back(make_pair(NON, 0)); }
    ~Keyboard() = default;

public:
    void Update();

    bool Down(UINT key) { return mapState[key] == DOWN; }
    bool Up(UINT key) { return mapState[key] == UP; }
    bool Press(UINT key) { return mapState[key] == PRESS; }

    bool IsInputBufferFront(UINT key) ;
    UINT ReturnFirst() 
    { 
        if (keyStorage.empty())
            return 666;
        else
            return (UINT)keyStorage.front().first;
    }

private:
    const float keyStoreTime = 1.0f;

    BYTE curState[KEY_MAX] = {};
    BYTE oldState[KEY_MAX] = {};
    KeyState mapState[KEY_MAX] = {};

    deque<pair<Inputs, float>> keyStorage;    

};