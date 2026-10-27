#ifndef HASHTABLE_FRAME_H
#define HASHTABLE_FRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../src/core/HashTable.h"
#include <string>

class HashTableFrame : public wxFrame {
private:
    // Хеш-таблица (ключ int, значение string)
    HashTable<int, std::string>* table;
    
    // Виджеты
    wxTextCtrl* keyInput;
    wxTextCtrl* valueInput;
    wxTextCtrl* resultOutput;
    wxListCtrl* tableList;
    
    // Кнопки
    wxButton* insertBtn;
    wxButton* getBtn;
    wxButton* removeBtn;
    wxButton* containsBtn;
    wxButton* clearBtn;
    
    // Обработчики событий
    void OnInsert(wxCommandEvent& evt);
    void OnGet(wxCommandEvent& evt);
    void OnRemove(wxCommandEvent& evt);
    void OnContains(wxCommandEvent& evt);
    void OnClear(wxCommandEvent& evt);
    void RefreshTableList();
    
public:
    HashTableFrame(const wxString& title);
    ~HashTableFrame();
    
    wxDECLARE_EVENT_TABLE();
};

// Таблица событий
enum {
    ID_Insert = 1,
    ID_Get,
    ID_Remove,
    ID_Contains,
    ID_Clear
};

wxBEGIN_EVENT_TABLE(HashTableFrame, wxFrame)
    EVT_BUTTON(ID_Insert, HashTableFrame::OnInsert)
    EVT_BUTTON(ID_Get, HashTableFrame::OnGet)
    EVT_BUTTON(ID_Remove, HashTableFrame::OnRemove)
    EVT_BUTTON(ID_Contains, HashTableFrame::OnContains)
    EVT_BUTTON(ID_Clear, HashTableFrame::OnClear)
wxEND_EVENT_TABLE()

#endif