#include "HashTableFrame.h"
#include <wx/wx.h>
#include <sstream>

// ==================== Реализация HashTableFrame ====================

HashTableFrame::HashTableFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    
    table = new HashTable<int, std::string>(16);
    
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    wxStaticText* titleLabel = new wxStaticText(mainPanel, wxID_ANY, "Hash Table Manager");
    titleLabel->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    mainSizer->Add(titleLabel, 0, wxALIGN_CENTER | wxALL, 10);
    
    wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
    
    inputSizer->Add(new wxStaticText(mainPanel, wxID_ANY, "Key:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    keyInput = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
    inputSizer->Add(keyInput, 0, wxRIGHT, 10);
    
    inputSizer->Add(new wxStaticText(mainPanel, wxID_ANY, "Value:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    valueInput = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    inputSizer->Add(valueInput, 0, wxRIGHT, 10);
    
    mainSizer->Add(inputSizer, 0, wxALL | wxEXPAND, 10);
    
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    
    insertBtn = new wxButton(mainPanel, ID_Insert, "Insert");
    getBtn = new wxButton(mainPanel, ID_Get, "Get");
    removeBtn = new wxButton(mainPanel, ID_Remove, "Remove");
    containsBtn = new wxButton(mainPanel, ID_Contains, "Contains");
    clearBtn = new wxButton(mainPanel, ID_Clear, "Clear All");
    
    buttonSizer->Add(insertBtn, 0, wxRIGHT, 5);
    buttonSizer->Add(getBtn, 0, wxRIGHT, 5);
    buttonSizer->Add(removeBtn, 0, wxRIGHT, 5);
    buttonSizer->Add(containsBtn, 0, wxRIGHT, 5);
    buttonSizer->Add(clearBtn, 0);
    
    mainSizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);
    
    resultOutput = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 100), 
                                   wxTE_MULTILINE | wxTE_READONLY);
    mainSizer->Add(resultOutput, 0, wxALL | wxEXPAND, 10);
    
    wxStaticText* listLabel = new wxStaticText(mainPanel, wxID_ANY, "Table Contents:");
    mainSizer->Add(listLabel, 0, wxLEFT | wxTOP, 10);
    
    tableList = new wxListCtrl(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(-1, 200), 
                                wxLC_REPORT | wxLC_SINGLE_SEL);
    tableList->InsertColumn(0, "Key", wxLIST_FORMAT_LEFT, 100);
    tableList->InsertColumn(1, "Value", wxLIST_FORMAT_LEFT, 300);
    
    mainSizer->Add(tableList, 1, wxALL | wxEXPAND, 10);
    
    mainPanel->SetSizer(mainSizer);
    Centre();
}

HashTableFrame::~HashTableFrame() {
    delete table;
}

void HashTableFrame::OnInsert(wxCommandEvent& evt) {
    (void)evt;
    wxString keyStr = keyInput->GetValue();
    wxString valueStr = valueInput->GetValue();
    
    if (keyStr.IsEmpty() || valueStr.IsEmpty()) {
        wxMessageBox("Please enter both key and value", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    long key;
    if (!keyStr.ToLong(&key)) {
        wxMessageBox("Key must be an integer", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    try {
        table->insert(static_cast<int>(key), std::string(valueStr.mb_str()));
        resultOutput->AppendText("Inserted: " + keyStr + " -> " + valueStr + "\n");
        keyInput->Clear();
        valueInput->Clear();
        RefreshTableList();
    } catch (const std::exception& e) {
        resultOutput->AppendText("Error: " + wxString(e.what()) + "\n");
    }
}

void HashTableFrame::OnGet(wxCommandEvent& evt) {
    (void)evt;
    wxString keyStr = keyInput->GetValue();
    
    if (keyStr.IsEmpty()) {
        wxMessageBox("Please enter a key", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    long key;
    if (!keyStr.ToLong(&key)) {
        wxMessageBox("Key must be an integer", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    try {
        std::string value = table->get(static_cast<int>(key));
        resultOutput->AppendText("Get(" + keyStr + ") = " + wxString(value) + "\n");
    } catch (const std::out_of_range&) {
        resultOutput->AppendText("Key " + keyStr + " not found\n");
    }
}

void HashTableFrame::OnRemove(wxCommandEvent& evt) {
    (void)evt;
    wxString keyStr = keyInput->GetValue();
    
    if (keyStr.IsEmpty()) {
        wxMessageBox("Please enter a key", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    long key;
    if (!keyStr.ToLong(&key)) {
        wxMessageBox("Key must be an integer", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (table->remove(static_cast<int>(key))) {
        resultOutput->AppendText("Removed key: " + keyStr + "\n");
        keyInput->Clear();
        RefreshTableList();
    } else {
        resultOutput->AppendText("Key " + keyStr + " not found\n");
    }
}

void HashTableFrame::OnContains(wxCommandEvent& evt) {
    (void)evt;
    wxString keyStr = keyInput->GetValue();
    
    if (keyStr.IsEmpty()) {
        wxMessageBox("Please enter a key", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    long key;
    if (!keyStr.ToLong(&key)) {
        wxMessageBox("Key must be an integer", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (table->contains(static_cast<int>(key))) {
        resultOutput->AppendText("Key " + keyStr + " exists\n");
    } else {
        resultOutput->AppendText("Key " + keyStr + " does not exist\n");
    }
}

void HashTableFrame::OnClear(wxCommandEvent& evt) {
    (void)evt;
    table->clear();
    resultOutput->AppendText("Table cleared\n");
    RefreshTableList();
}

void HashTableFrame::RefreshTableList() {
    tableList->DeleteAllItems();
    
    // Перебираем все возможные ключи от 0 до 10000
    // Это неэффективно, но для демонстрации подойдёт
    for (int i = 0; i < 10000; ++i) {
        if (table->contains(i)) {
            std::string value;
            if (table->try_get(i, value)) {
                long idx = tableList->InsertItem(tableList->GetItemCount(), wxString::Format("%d", i));
                tableList->SetItem(idx, 1, wxString(value));
            }
        }
    }
}

// ==================== wxApp ====================

class HashTableApp : public wxApp {
public:
    virtual bool OnInit() {
        HashTableFrame* frame = new HashTableFrame("Hash Table GUI");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(HashTableApp);