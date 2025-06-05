#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QPainter>
#include "context.h"

#define WIDTH               400
#define HEIGHT              300
// Константы для отрисовки
#define AXIS_MARGIN         100   // Отступ от краёв для осей
#define GRAPH_LINE_WIDTH    2    // Толщина линии графика
#define POINT_RADIUS        3    // Радиус точек на графике
#define STATS_LINE_WIDTH    1    // Толщина линий статистики
#define TEXT_OFFSET         5    // Отступ текста от линий
#define AXIS_LABEL_OFFSET   30   // Отступ подписей осей
#define PADDING_FACTOR      0.1  // Дополнительный отступ (10% от диапазона)

// Константы для отрисовки осей
#define AXIS_LINE_WIDTH     2
#define ARROW_SIZE         10
#define TICK_SIZE          8

// Цвета
#define BACKGROUND_COLOR    Qt::white
#define AXIS_COLOR          Qt::black
#define GRAPH_COLOR         Qt::blue
#define MIN_COLOR           Qt::red
#define MAX_COLOR           Qt::green
#define MEDIAN_COLOR        Qt::magenta

class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraphWidget(QWidget *parent = nullptr);
    void setContext(Context* context); //для установки контекста

protected:
    void paintEvent(QPaintEvent *event) override;

private:

    void calculateScalingParameters();
    void drawAxes(QPainter &painter);
    void drawGraph(QPainter &painter);
    void drawStats(QPainter &painter);
    void drawLabels(QPainter &painter);

    void findMinMaxYears();
    void calculateScales();
    void calculatePadding();

    void drawGraphLines(QPainter &painter);
    void drawLastPoint(QPainter &painter);

    void drawMinLine(QPainter &painter);
    void drawMaxLine(QPainter &painter);
    void drawMedianLine(QPainter &painter);

    void drawLabelX(QPainter &painter);
    void drawLabelY(QPainter &painter);

    void drawAxeX(QPainter &painter);
    void drawAxeY(QPainter &painter);
    void drawStepsX(QPainter &painter);
    void drawStepsY(QPainter &painter);
    void drawArrowX(QPainter &painter);
    void drawArrowY(QPainter &painter);

    Context* m_context; //работаем с самим контекстом

    // Параметры масштабирования
    double m_xScale;
    double m_yScale;
    int m_minYear;
    int m_maxYear;

    double m_paddedMinValue;
    double m_paddedMaxValue;
};


#endif // GRAPHWIDGET_H
