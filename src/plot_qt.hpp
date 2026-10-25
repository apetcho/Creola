#pragma once

#include<QPainter>
#include<QRectF>

#include "plot.hpp"

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -

void paintFigure(const Figure& fig, QPainter& painter, const QRectF& target);
void ensureQtAppliction(void);

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------