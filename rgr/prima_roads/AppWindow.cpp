#include "stdafx.h"
#include "AppWindow.h"
#include "Utils.h"
#include "BoostGraph.h"
#include <fstream>

namespace
{
const unsigned WINDOW_WIDTH = 800;
const unsigned WINDOW_HEIGHT = 600;
const unsigned WINDOW_STYLE = sf::Style::Titlebar | sf::Style::Close;
}

CAppWindow::CAppWindow()
    : sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Prima Roads - RGR Sample", WINDOW_STYLE)
{
}

void CAppWindow::EnterLoop()
{
    DoAlgorithm("input.txt", "output.txt");

    size_t nextFrame = 0;
    sf::Texture currentFrame;
    sf::Sprite sprite;

    auto switchFrame = [&]() {
        if (nextFrame < m_frameNames.size())
        {
            currentFrame.loadFromFile(m_frameNames[nextFrame]);
            sprite.setTexture(currentFrame);
            ++nextFrame;
        }
    };

    sf::Clock clock;
    bool justStarted = true;

    while (isOpen())
    {
        sf::Event event;
        while (pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                close();
                return;
            }
        }

        if (justStarted || (clock.getElapsedTime().asMilliseconds() > 1000))
        {
            justStarted = false;
            switchFrame();
            clock.restart();

            clear(sf::Color::Black);
            draw(sprite);
            display();
        }
        else
        {
            sf::sleep(sf::milliseconds(15));
        }
    }
}

bool CAppWindow::DoAlgorithm(const std::string &inputPath, const std::string &outputPath)
{
    CBoostGraph graph;
    std::ifstream input(inputPath);
    std::ofstream output(outputPath);

    if (!input.is_open() && !output.is_open())
    {
        return false;
    }

    graph.SetStepHandler(std::bind(&CAppWindow::OnGraphAlgorithmStep, this, std::placeholders::_1));
    if (!graph.ReadText(input))
    {
        return false;
    }
    graph.RunPrima(output);
    return true;
}

void CAppWindow::OnGraphAlgorithmStep(const std::string &dotCode)
{
    std::string tempPath = "graph.dot";
    std::string nextPath = "frame_" + std::to_string(m_frameNames.size()) + ".png";
    std::ofstream tempDot(tempPath);
    tempDot << dotCode;
    tempDot.close();

    if (CUtils::RunProcess("dot -Tpng -o" + nextPath + " " + tempPath))
    {
        m_frameNames.push_back(nextPath);
    }
}
