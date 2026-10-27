#ifndef PERFORMANCE_GRAPH_H
#define PERFORMANCE_GRAPH_H

#include <wx/wx.h>
#include "../src/containers/DynamicArray.h"

class PerformanceGraph : public wxFrame {
private:
    DynamicArray<int> sizes;
    DynamicArray<double> times;
    wxString title;

    void OnPaint(wxPaintEvent& evt);

public:
    PerformanceGraph(const wxString& frameTitle,
                     const DynamicArray<int>& s,
                     const DynamicArray<double>& t)
        : wxFrame(nullptr, wxID_ANY, frameTitle, wxDefaultPosition, wxSize(800, 600)),
          sizes(s), times(t), title(frameTitle) {
        
        wxPanel* panel = new wxPanel(this, wxID_ANY);
        panel->SetBackgroundColour(*wxWHITE);
        panel->Bind(wxEVT_PAINT, &PerformanceGraph::OnPaint, this);
        
        Centre();
    }
};

void PerformanceGraph::OnPaint(wxPaintEvent& evt) {
    wxPaintDC dc(this);
    
    int width = GetClientSize().GetWidth();
    int height = GetClientSize().GetHeight();
    int padding = 60;

    dc.SetTextForeground(*wxBLACK);
    dc.DrawText(title, padding, 10);

    dc.SetPen(wxPen(*wxBLACK, 2));
    dc.DrawLine(padding, height - padding, width - padding, height - padding);
    dc.DrawLine(padding, height - padding, padding, padding);

    dc.DrawText("Количество элементов (N)", width - 200, height - padding + 20);
    dc.DrawText("Время (мс)", 5, padding - 20);

    int count = sizes.GetSize();
    if (count == 0) return;

    double maxTime = times.Get(0);
    for (int i = 1; i < count; ++i) {
        double t = times.Get(i);
        if (t > maxTime) maxTime = t;
    }
    int maxSize = sizes.Get(count - 1);

    if (maxTime == 0) maxTime = 1;

    dc.SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    for (int i = 0; i < count; ++i) {
        int sizeVal = sizes.Get(i);
        int x = padding + (static_cast<double>(sizeVal) / maxSize) * (width - 2 * padding);
        wxString label = wxString::Format("%d", sizeVal);
        dc.DrawText(label, x - 15, height - padding + 5);
    }

    dc.SetPen(wxPen(*wxBLUE, 2));
    for (int i = 0; i < count; ++i) {
        int sizeVal = sizes.Get(i);
        double timeVal = times.Get(i);
        
        int x = padding + (static_cast<double>(sizeVal) / maxSize) * (width - 2 * padding);
        int y = (height - padding) - (timeVal / maxTime) * (height - 2 * padding);
        
        dc.SetBrush(*wxBLUE_BRUSH);
        dc.DrawCircle(x, y, 5);
        
        if (i > 0) {
            int prevSizeVal = sizes.Get(i - 1);
            double prevTimeVal = times.Get(i - 1);
            int prevX = padding + (static_cast<double>(prevSizeVal) / maxSize) * (width - 2 * padding);
            int prevY = (height - padding) - (prevTimeVal / maxTime) * (height - 2 * padding);
            dc.DrawLine(prevX, prevY, x, y);
        }
    }

    dc.SetPen(wxPen(*wxBLUE, 2));
    dc.DrawLine(width - 150, 40, width - 120, 40);
    dc.DrawText("Время вставки", width - 115, 35);
}

#endif