#include "graphwidget.h"
#include <algorithm>
#include <cmath>

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent),
    m_context(nullptr), m_xScale(1.0), m_yScale(1.0), m_minYear(0),
    m_paddedMinValue(0), m_paddedMaxValue(0)
{
    setMinimumSize(WIDTH, HEIGHT);
}

void GraphWidget::setContext(Context* context) {
    m_context = context;
    update(); //перерисовываем при изменении контекста
}

void GraphWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); //сглаживание

    // Заливаем фон
    painter.fillRect(rect(), BACKGROUND_COLOR);

    //отрисовка
    if (m_context && m_context->stats && m_context->stats->valuesCount > 0) {
        calculateScalingParameters(); //доп параметры для отрисовки (перед тем как нарисовать - пересчитываем)
        drawAxes(painter);
        drawGraph(painter);
        drawStats(painter);
        drawLabels(painter);
    }
}

//ниже все доп функции (по хорошему наверно через фасад делать)

void GraphWidget::findMinMaxYears() {
    if (!m_context || !m_context->stats || m_context->stats->valuesCount == 0) return;

    // Находим минимальный и максимальный год
    m_minYear = m_context->stats->years[0];
    m_maxYear = m_context->stats->years[0];
    for (size_t i = 1; i < m_context->stats->valuesCount; ++i) {
        if (m_context->stats->years[i] < m_minYear) m_minYear = m_context->stats->years[i];
        if (m_context->stats->years[i] > m_maxYear) m_maxYear = m_context->stats->years[i];
    }
}

void GraphWidget::calculatePadding() {
    if (!m_context || !m_context->calcValues) return;

    // Добавляем отступы
    double padding = (m_context->calcValues->max - m_context->calcValues->min) * PADDING_FACTOR;
    m_paddedMinValue = m_context->calcValues->min - padding;
    m_paddedMaxValue = m_context->calcValues->max + padding;
}

void GraphWidget::calculateScales() {
    // Вычисляем масштабные коэффициенты
    m_xScale = (width() - 2 * AXIS_MARGIN) / static_cast<double>(m_maxYear - m_minYear);
    m_yScale = (height() - 2 * AXIS_MARGIN) / (m_paddedMaxValue - m_paddedMinValue);
}

void GraphWidget::calculateScalingParameters() {
    if (!m_context || !m_context->stats || m_context->stats->valuesCount == 0 || !m_context->calcValues) return;

    findMinMaxYears();
    calculatePadding();
    calculateScales();
}

void GraphWidget::drawGraphLines(QPainter &painter) {
    if (!m_context || !m_context->stats || m_context->stats->valuesCount < 2)
        return;

    // Рисуем линии между точками
    for (size_t i = 0; i < m_context->stats->valuesCount - 1; ++i) {
        int x1 = AXIS_MARGIN + static_cast<int>((m_context->stats->years[i] - m_minYear) * m_xScale);
        int y1 = height() - AXIS_MARGIN - static_cast<int>((m_context->stats->values[i] - m_paddedMinValue) * m_yScale);
        int x2 = AXIS_MARGIN + static_cast<int>((m_context->stats->years[i+1] - m_minYear) * m_xScale);
        int y2 = height() - AXIS_MARGIN - static_cast<int>((m_context->stats->values[i+1] - m_paddedMinValue) * m_yScale);

        painter.drawLine(x1, y1, x2, y2);
        painter.drawEllipse(QPoint(x1, y1), POINT_RADIUS, POINT_RADIUS);
    }
}

void GraphWidget::drawLastPoint(QPainter &painter) {
    if (!m_context || !m_context->stats || m_context->stats->valuesCount < 2)
        return;

    // Рисуем последнюю точку
    int lastIdx = m_context->stats->valuesCount - 1;
    int x = AXIS_MARGIN + static_cast<int>((m_context->stats->years[lastIdx] - m_minYear) * m_xScale);
    int y = height() - AXIS_MARGIN - static_cast<int>((m_context->stats->values[lastIdx] - m_paddedMinValue) * m_yScale);
    painter.drawEllipse(QPoint(x, y), POINT_RADIUS, POINT_RADIUS);
}

void GraphWidget::drawGraph(QPainter &painter) {
    if (!m_context || !m_context->stats || m_context->stats->valuesCount < 2)
        return;

    painter.setPen(QPen(GRAPH_COLOR, GRAPH_LINE_WIDTH)); //цвет и толщина линий

    drawGraphLines(painter);
    drawLastPoint(painter);
}

void GraphWidget::drawMinLine(QPainter &painter) {
    if (!m_context || !m_context->calcValues) return;

    // Линия минимума
    painter.setPen(QPen(MIN_COLOR, STATS_LINE_WIDTH, Qt::DashLine));
    int yMin = height() - AXIS_MARGIN - static_cast<int>((m_context->calcValues->min - m_paddedMinValue) * m_yScale);
    painter.drawLine(AXIS_MARGIN, yMin, width() - AXIS_MARGIN, yMin);
    painter.drawText(width() - AXIS_MARGIN + TEXT_OFFSET, yMin - TEXT_OFFSET,
                     "Min: " + QString::number(m_context->calcValues->min));
}

void GraphWidget::drawMaxLine(QPainter &painter) {
    if (!m_context || !m_context->calcValues) return;

    // Линия максимума
    painter.setPen(QPen(MAX_COLOR, STATS_LINE_WIDTH, Qt::DashLine));
    int yMax = height() - AXIS_MARGIN - static_cast<int>((m_context->calcValues->max - m_paddedMinValue) * m_yScale);
    painter.drawLine(AXIS_MARGIN, yMax, width() - AXIS_MARGIN, yMax);
    painter.drawText(width() - AXIS_MARGIN + TEXT_OFFSET, yMax - TEXT_OFFSET,
                     "Max: " + QString::number(m_context->calcValues->max));
}

void GraphWidget::drawMedianLine(QPainter &painter) {
    if (!m_context || !m_context->calcValues) return;

    // Линия медианы
    painter.setPen(QPen(MEDIAN_COLOR, STATS_LINE_WIDTH, Qt::DashLine));
    int yMedian = height() - AXIS_MARGIN - static_cast<int>((m_context->calcValues->median - m_paddedMinValue) * m_yScale);
    painter.drawLine(AXIS_MARGIN, yMedian, width() - AXIS_MARGIN, yMedian);
    painter.drawText(width() - AXIS_MARGIN + TEXT_OFFSET, yMedian - TEXT_OFFSET,
                     "Median: " + QString::number(m_context->calcValues->median));
}

void GraphWidget::drawStats(QPainter &painter) {
    if (!m_context || !m_context->calcValues)
        return;

    // Линия минимума
    painter.setPen(QPen(MIN_COLOR, STATS_LINE_WIDTH, Qt::DashLine));
    drawMinLine(painter);

    // Линия максимума
    painter.setPen(QPen(MAX_COLOR, STATS_LINE_WIDTH, Qt::DashLine));
    drawMaxLine(painter);

    // Линия медианы
    painter.setPen(QPen(MEDIAN_COLOR, STATS_LINE_WIDTH, Qt::DashLine));
    drawMedianLine(painter);
}

void GraphWidget::drawLabelX(QPainter &painter) {
    // Подпись оси X
    painter.drawText(width() / 2 - AXIS_LABEL_OFFSET, height() - AXIS_MARGIN / 2, "Year");
}

void GraphWidget::drawLabelY(QPainter &painter) {
    // Подпись оси Y (вертикальная)
    painter.save();
    painter.translate(AXIS_MARGIN / 2, height() / 2);
    painter.rotate(-90);
    painter.drawText(0, 0, "Value");
    painter.restore();
}

void GraphWidget::drawLabels(QPainter &painter) {
    painter.setPen(AXIS_COLOR);

    drawLabelX(painter);
    drawLabelY(painter);
}

void GraphWidget::drawAxeX(QPainter &painter) {
    // Рисуем горизонтальную ось X
    painter.drawLine(AXIS_MARGIN, height() - AXIS_MARGIN,
                     width() - AXIS_MARGIN, height() - AXIS_MARGIN);
    drawArrowX(painter);
    drawStepsX(painter);
}

void GraphWidget::drawAxeY(QPainter &painter) {
    // Рисуем вертикальную ось Y
    painter.drawLine(AXIS_MARGIN, height() - AXIS_MARGIN,
                     AXIS_MARGIN, AXIS_MARGIN);
    drawArrowY(painter);
    drawStepsY(painter);
}

void GraphWidget::drawStepsX(QPainter &painter) {
    // Рисуем деления и подписи на оси X (годы)
    if (m_context && m_context->stats)
    {
        // Определяем шаг для делений (чтобы не было слишком много подписей)
        int yearStep = qMax(1, (m_maxYear - m_minYear) / 5);

        for (int year = m_minYear; year <= m_maxYear; year += yearStep)
        {
            int xPos = AXIS_MARGIN + static_cast<int>((year - m_minYear) * m_xScale);

            // Деление на оси
            painter.drawLine(xPos, height() - AXIS_MARGIN - TICK_SIZE/2,
                             xPos, height() - AXIS_MARGIN + TICK_SIZE/2);

            // Подпись года
            painter.drawText(xPos - 20, height() - AXIS_MARGIN + TICK_SIZE + 15,
                             QString::number(year));
        }
    }
}

void GraphWidget::drawStepsY(QPainter &painter) {
    // Рисуем деления и подписи на оси Y (значения)
    if (m_context && m_context->calcValues)
    {
        // Вычисляем шаг для значений
        double valueRange = m_paddedMaxValue - m_paddedMinValue;
        double valueStep = valueRange / 5.0;

        double value = m_paddedMinValue;
        while (value <= m_paddedMaxValue) {
            int yPos = height() - AXIS_MARGIN - static_cast<int>((value - m_paddedMinValue) * m_yScale);

            // Деление на оси
            painter.drawLine(AXIS_MARGIN - TICK_SIZE/2, yPos,
                             AXIS_MARGIN + TICK_SIZE/2, yPos);

            // Подпись значения (с форматированием)
            painter.drawText(AXIS_MARGIN - TICK_SIZE - 30, yPos + 5,
                             QString::number(value, 'f', 2));
            value += valueStep;
        }
    }
}

void GraphWidget::drawArrowX(QPainter &painter) {
    // Стрелка оси X
    QPointF xArrow[3] = {
        QPointF(width() - AXIS_MARGIN, height() - AXIS_MARGIN),
        QPointF(width() - AXIS_MARGIN - ARROW_SIZE, height() - AXIS_MARGIN - ARROW_SIZE/2),
        QPointF(width() - AXIS_MARGIN - ARROW_SIZE, height() - AXIS_MARGIN + ARROW_SIZE/2)
    };
    painter.drawPolygon(xArrow, 3);
}
void GraphWidget::drawArrowY(QPainter &painter) {
    // Стрелка оси Y
    QPointF yArrow[3] = {
        QPointF(AXIS_MARGIN, AXIS_MARGIN),
        QPointF(AXIS_MARGIN - ARROW_SIZE/2, AXIS_MARGIN + ARROW_SIZE),
        QPointF(AXIS_MARGIN + ARROW_SIZE/2, AXIS_MARGIN + ARROW_SIZE)
    };
    painter.drawPolygon(yArrow, 3);
}

void GraphWidget::drawAxes(QPainter &painter)
{
    // Устанавливаем цвет и толщину осей
    painter.setPen(QPen(AXIS_COLOR, AXIS_LINE_WIDTH));

    drawAxeX(painter);
    drawAxeY(painter);
}
