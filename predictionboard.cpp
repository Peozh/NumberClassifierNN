#include "predictionboard.h"
#include <array>

PredictionBoard::PredictionBoard(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedWidth(300);
    auto* layout = new QVBoxLayout(this);
    for (int idx = 0; idx < 10; ++idx)
    {
        labels[idx] = new QLabel();
        layout->addWidget(labels[idx]);
    }
    this->setLayout(layout);
    nc = new NumberClassifier();
}

PredictionBoard::~PredictionBoard()
{
    delete nc;
}

void PredictionBoard::updatePrediction(float *data)
{
    uint64_t min_x = 27;
    uint64_t min_y = 27;
    uint64_t max_x = 0;
    uint64_t max_y = 0;
    for (uint64_t y = 0; y < 28; ++y)
    {
        for (uint64_t x = 0; x < 28; ++x)
        {
            if (data[28*y + x] > 0.1)
            {
                min_x = std::min(x, min_x);
                max_x = std::max(x, max_x);
                min_y = std::min(y, min_y);
                max_y = std::max(y, max_y);
            }
        }
    }
    std::array<float, 28*28> input = { 0.0f, };
    uint64_t width_y = max_y - min_y + 1;
    uint64_t width_x = max_x - min_x + 1;
    uint64_t offset_y = (28-width_y) / 2;
    uint64_t offset_x = (28-width_x) / 2;
    for (uint64_t y = 0; y < width_y; ++y)
    {
        for (uint64_t x = 0; x < width_x; ++x)
        {
            input[28*(offset_y+y) + (offset_x+x)] = data[28*(min_y+y) + (min_x+x)] * 255;
        }
    }
    auto prediction = nc->predict(input);
    auto softmax = NumberClassifier::softmax(prediction);

    for (int idx = 0; idx < 10; ++idx)
    {
        QString text = QString::number(idx) + " : " + QString::number(softmax[idx]*100, 'g', 3) + " %";
        labels[idx]->setText(text);
    }
}
