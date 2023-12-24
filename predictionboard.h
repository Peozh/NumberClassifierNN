#ifndef PREDICTIONBOARD_H
#define PREDICTIONBOARD_H

#include <QVBoxLayout>
#include <QLabel>
#include "numberClassifier.hpp"

class PredictionBoard : public QWidget
{
    Q_OBJECT
    QLabel* labels[10];
    NumberClassifier* nc;
public:
    explicit PredictionBoard(QWidget *parent = nullptr);
    ~PredictionBoard();
signals:
public slots:
    void updatePrediction(float* data);
};

#endif // PREDICTIONBOARD_H
