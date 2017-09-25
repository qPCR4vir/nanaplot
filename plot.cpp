#include <nana/gui.hpp>
#include "plot.h"
namespace nana
{
namespace plot
{

plot::plot( window parent )
    : myParent( parent )
{
    // arrange for the plot to be updated when needed
    myDrawing = new drawing( parent );
    myDrawing->draw([this](paint::graphics& graph)
    {
        // calculate scaling factors
        // so plot will fit
        CalcScale(
            graph.width(),
            graph.height() );

        // draw all the traces
        for( auto& t : myTrace )
            t.update( graph );
    });
}

void plot::CalcScale( int w, int h )
{
    int maxCount = 0;
    myTrace[0].bounds( myMinY, myMaxY );
    for( auto& t : myTrace )
    {
        if( t.size() > maxCount )
            maxCount = t.size();
        int min, max;
        t.bounds( min, max );
        if( min < myMinY )
            myMinY = min;
        if( max > myMaxY )
            myMaxY = max;
    }

    myXinc = w / maxCount;
    myScale = h / ( myMaxY - myMinY );
    myYOffset = h + myScale * myMinY;
}

void trace::bounds( int& min, int& max )
{
    min = myY[0];
    max = min;
    for( auto y : myY )
    {
        if( y < min )
            min = y;
        if( y > max )
            max = y;
    }
}

void trace::update( paint::graphics& graph )
{
    bool first = true;
    int x = 0;
    int xinc = myPlot->xinc();
    double s = myPlot->Scale();
    int yOffset = myPlot->YOffset();
    int prev;

    // loop over data points
    for( auto y : myY )
    {
        if( first )
        {
            first = false;
            prev = y;
            continue;
        }
        // draw line from previous to this data point
        graph.line(
            point(x, yOffset - prev * s),
            point(x+xinc, yOffset - y * s),
            myColor);

        x += xinc;
        prev = y;
    }
}

}
}