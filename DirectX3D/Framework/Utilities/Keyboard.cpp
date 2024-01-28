#include "Framework.h"

void Keyboard::Update()
{
    memcpy(oldState, curState, sizeof(oldState));

    //0, 1 -> 키보드 입력 X
    //128, 129 -> 키보드 입력 O
    GetKeyboardState(curState);

    for(UINT i = 0 ; i < KEY_MAX ; i++)
    {
        BYTE key = curState[i] & 0x80;
        
        curState[i] = key ? 1 : 0;


        BYTE old = oldState[i];
        BYTE cur = curState[i];

        if (old == 0 && cur == 1)
            mapState[i] = DOWN;
        else if (old == 1 && cur == 0)
            mapState[i] = UP;
        else if (old == 1 && cur == 1)
            mapState[i] = PRESS;
        else
            mapState[i] = NONE;
    }

    if (KEY_DOWN(VK_LBUTTON))
        keyStorage.push_front(make_pair(LMB, 0));
    if (KEY_DOWN(VK_RBUTTON))
        keyStorage.push_front(make_pair(RMB, 0));

    if ((KEY_DOWN(VK_LBUTTON) && KEY_PRESS(VK_RBUTTON)) ||
        (KEY_DOWN(VK_RBUTTON) && KEY_PRESS(VK_LBUTTON)) ||
        (KEY_DOWN(VK_RBUTTON) && KEY_DOWN(VK_LBUTTON)) ||
        (KEY_DOWN(VK_RBUTTON) && KEY_UP(VK_LBUTTON)) ||
        (KEY_UP(VK_RBUTTON) && KEY_DOWN(VK_LBUTTON)) )
        keyStorage.push_front(make_pair(LMBRMB, 0));

    if (KEY_DOWN(VK_CONTROL))
        keyStorage.push_front(make_pair(CTRL, 0));

    if ((KEY_DOWN(VK_CONTROL) && KEY_PRESS(VK_RBUTTON)) ||
        (KEY_DOWN(VK_RBUTTON) && KEY_PRESS(VK_CONTROL)))
        keyStorage.push_front(make_pair(CTRLRMB, 0));

    if (KEY_DOWN(VK_SPACE))
        keyStorage.push_front(make_pair(SPACE, 0));

    if ((KEY_DOWN(VK_SPACE) && KEY_PRESS(VK_CONTROL)) ||
        (KEY_DOWN(VK_CONTROL) && KEY_PRESS(VK_SPACE)))
        keyStorage.push_front(make_pair(CTRLSPACE, 0));

    if (KEY_DOWN(VK_SHIFT))
        keyStorage.push_front(make_pair(SHIFT, 0));

    if (KEY_DOWN('1'))
        keyStorage.push_front(make_pair(BUG1, 0));

    if (KEY_DOWN('2'))
        keyStorage.push_front(make_pair(BUG2, 0));

    for (auto& p : keyStorage)
    {
        p.second += DELTA;

        if (p.second >= keyStoreTime)
        {
                keyStorage.pop_back();
        }
    }

}

bool Keyboard::IsInputBufferFront(UINT key)
{
    if (keyStorage.empty())
        return false;

    else if ( keyStorage.front().first == key)
        return true;
    else 
        return false;

}
