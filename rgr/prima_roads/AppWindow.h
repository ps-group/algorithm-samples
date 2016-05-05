#pragma once
#include "stdafx.h"

class CBoostGraph;

class CAppWindow : public sf::RenderWindow
{
public:
    CAppWindow();
    ~CAppWindow();

    void EnterLoop();

private:
    enum class State
    {
        WaitingInput,
        RunningDemo,
        WaitingOutput,
    };

    void OnGraphAlgorithmStep(const std::string &dotCode);

    void OnClick();
    void OnWaitingInput();
    void OnRunningDemo();
    void OnWaitingOutput();
    void RunAlgorithmDemo();
    bool SwitchNextFrame();

    State m_state = State::WaitingInput;
    sf::Clock m_clock;
    std::deque<std::string> m_pendingFramePaths;
    sf::Font m_font;
    std::unique_ptr<CBoostGraph> m_graph;
    sf::Texture m_activeFrame;
};
