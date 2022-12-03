#include "pbPlots.h"
#include "supportLib.h"

int draw_graph(double *X, double *Y, double *Y_real, int n) {
    RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();

    ScatterPlotSeries *series_1 = GetDefaultScatterPlotSeriesSettings();
    series_1->xs = X;
    series_1->xsLength = n;
    series_1->ys = Y;
    series_1->ysLength = n;
    series_1->linearInterpolation = true;
    series_1->lineType = L"solid";
    series_1->lineTypeLength = wcslen(series_1->lineType);
    series_1->lineThickness = 1;
    series_1->color = CreateRGBColor(1, 0, 0);

    ScatterPlotSeries *series_2 = GetDefaultScatterPlotSeriesSettings();
    series_2->xs = X;
    series_2->xsLength = n;
    series_2->ys = Y_real;
    series_2->ysLength = n;
    series_2->linearInterpolation = true;
    series_2->lineType = L"solid";
    series_2->lineTypeLength = wcslen(series_2->lineType);
    series_2->lineThickness = 1;
    series_2->color = CreateRGBColor(0, 0, 1);

    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    settings->width = 1000;
    settings->height = 700;
    settings->autoBoundaries = true;
    settings->autoPadding = true;
    settings->title = L"";
    settings->titleLength = wcslen(settings->title);
    settings->xLabel = L"";
    settings->xLabelLength = wcslen(settings->xLabel);
    settings->yLabel = L"";
    settings->yLabelLength = wcslen(settings->yLabel);
    ScatterPlotSeries *s[] = {series_1, series_2};
    settings->scatterPlotSeries = s;
    settings->scatterPlotSeriesLength = 2;

    _Bool success = DrawScatterPlotFromSettings(imageReference, settings, NULL);

    if (success) {
        size_t length;
        double *png_data = ConvertToPNG(&length, imageReference->image);
        WriteToFile(png_data, length, "solution.png");
        DeleteImage(imageReference->image);
    } else {
        fprintf(stderr, "Error while drawing graph\n");
    }
    return success;
}
