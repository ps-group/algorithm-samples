#pragma once
#include "stdafx.h"

class CAppWindow : public sf::RenderWindow
{
public:
    CAppWindow();

    void EnterLoop();

private:
    bool DoAlgorithm(const std::string &inputPath, const std::string &outputPath);
    void OnGraphAlgorithmStep(const std::string &dotCode);

    std::vector<std::string> m_frameNames;
};
