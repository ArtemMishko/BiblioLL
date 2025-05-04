#pragma once
#include "FileManager.h"

namespace LibraryApp {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace LibrarySystem;

    /// <summary>
    /// ������� ����� ��� ������� ���������� �����������
    /// </summary>
    public ref class MainForm : public System::Windows::Forms::Form
    {
    public:
        MainForm(void)
        {
            InitializeComponent();
            fileManager = gcnew FileManager();
            currentFileName = "";
        }

    protected:
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        // ���������� WinForms
        System::Windows::Forms::MenuStrip^ menuStrip;
        System::Windows::Forms::ToolStripMenuItem^ fileMenu;
        System::Windows::Forms::ToolStripMenuItem^ createFileMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ openFileMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ saveFileMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ exitMenuItem;

        System::Windows::Forms::Panel^ bookPanel;
        System::Windows::Forms::TextBox^ idTextBox;
        System::Windows::Forms::TextBox^ titleTextBox;
        System::Windows::Forms::TextBox^ authorTextBox;
        System::Windows::Forms::TextBox^ yearTextBox;
        System::Windows::Forms::TextBox^ genreTextBox;
        System::Windows::Forms::ComboBox^ availableComboBox;

        System::Windows::Forms::Label^ idLabel;
        System::Windows::Forms::Label^ titleLabel;
        System::Windows::Forms::Label^ authorLabel;
        System::Windows::Forms::Label^ yearLabel;
        System::Windows::Forms::Label^ genreLabel;
        System::Windows::Forms::Label^ availableLabel;

        System::Windows::Forms::Button^ addButton;
        System::Windows::Forms::Button^ removeButton;
        System::Windows::Forms::Button^ clearButton;

        System::Windows::Forms::TextBox^ searchTextBox;
        System::Windows::Forms::Button^ searchButton;

        System::Windows::Forms::ListView^ booksListView;
        System::Windows::Forms::StatusStrip^ statusStrip;
        System::Windows::Forms::ToolStripStatusLabel^ statusLabel;

        System::Windows::Forms::SaveFileDialog^ saveFileDialog;
        System::Windows::Forms::OpenFileDialog^ openFileDialog;

        System::ComponentModel::Container^ components;

        // ������������� �����������
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->menuStrip = (gcnew System::Windows::Forms::MenuStrip());
            this->fileMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->createFileMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->openFileMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveFileMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->exitMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());

            this->bookPanel = (gcnew System::Windows::Forms::Panel());
            this->idTextBox = (gcnew System::Windows::Forms::TextBox());
            this->titleTextBox = (gcnew System::Windows::Forms::TextBox());
            this->authorTextBox = (gcnew System::Windows::Forms::TextBox());
            this->yearTextBox = (gcnew System::Windows::Forms::TextBox());
            this->genreTextBox = (gcnew System::Windows::Forms::TextBox());
            this->availableComboBox = (gcnew System::Windows::Forms::ComboBox());

            this->idLabel = (gcnew System::Windows::Forms::Label());
            this->titleLabel = (gcnew System::Windows::Forms::Label());
            this->authorLabel = (gcnew System::Windows::Forms::Label());
            this->yearLabel = (gcnew System::Windows::Forms::Label());
            this->genreLabel = (gcnew System::Windows::Forms::Label());
            this->availableLabel = (gcnew System::Windows::Forms::Label());

            this->addButton = (gcnew System::Windows::Forms::Button());
            this->removeButton = (gcnew System::Windows::Forms::Button());
            this->clearButton = (gcnew System::Windows::Forms::Button());

            this->searchTextBox = (gcnew System::Windows::Forms::TextBox());
            this->searchButton = (gcnew System::Windows::Forms::Button());

            this->booksListView = (gcnew System::Windows::Forms::ListView());
            this->statusStrip = (gcnew System::Windows::Forms::StatusStrip());
            this->statusLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());

            this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
            this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());

            this->menuStrip->SuspendLayout();
            this->bookPanel->SuspendLayout();
            this->statusStrip->SuspendLayout();
            this->SuspendLayout();

            // menuStrip
            this->menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^ >(1) { this->fileMenu });
            this->menuStrip->Location = System::Drawing::Point(0, 0);
            this->menuStrip->Name = L"menuStrip";
            this->menuStrip->Size = System::Drawing::Size(800, 24);
            this->menuStrip->TabIndex = 0;
            this->menuStrip->Text = L"menuStrip";

            // fileMenu
            this->fileMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^ >(4) {
                this->createFileMenuItem, this->openFileMenuItem, this->saveFileMenuItem, this->exitMenuItem
            });
            this->fileMenu->Name = L"fileMenu";
            this->fileMenu->Size = System::Drawing::Size(48, 20);
            this->fileMenu->Text = L"����";

            // createFileMenuItem
            this->createFileMenuItem->Name = L"createFileMenuItem";
            this->createFileMenuItem->Size = System::Drawing::Size(180, 22);
            this->createFileMenuItem->Text = L"�������";
            this->createFileMenuItem->Click += gcnew System::EventHandler(this, &MainForm::createFileMenuItem_Click);

            // openFileMenuItem
            this->openFileMenuItem->Name = L"openFileMenuItem";
            this->openFileMenuItem->Size = System::Drawing::Size(180, 22);
            this->openFileMenuItem->Text = L"�������";
            this->openFileMenuItem->Click += gcnew System::EventHandler(this, &MainForm::openFileMenuItem_Click);

            // saveFileMenuItem
            this->saveFileMenuItem->Name = L"saveFileMenuItem";
            this->saveFileMenuItem->Size = System::Drawing::Size(180, 22);
            this->saveFileMenuItem->Text = L"���������";
            this->saveFileMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveFileMenuItem_Click);

            // exitMenuItem
            this->exitMenuItem->Name = L"exitMenuItem";
            this->exitMenuItem->Size = System::Drawing::Size(180, 22);
            this->exitMenuItem->Text = L"�����";
            this->exitMenuItem->Click += gcnew System::EventHandler(this, &MainForm::exitMenuItem_Click);

            // bookPanel
            this->bookPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->bookPanel->Controls->Add(this->idLabel);
            this->bookPanel->Controls->Add(this->idTextBox);
            this->bookPanel->Controls->Add(this->titleLabel);
            this->bookPanel->Controls->Add(this->titleTextBox);
            this->bookPanel->Controls->Add(this->authorLabel);
            this->bookPanel->Controls->Add(this->authorTextBox);
            this->bookPanel->Controls->Add(this->yearLabel);
            this->bookPanel->Controls->Add(this->yearTextBox);
            this->bookPanel->Controls->Add(this->genreLabel);
            this->bookPanel->Controls->Add(this->genreTextBox);
            this->bookPanel->Controls->Add(this->availableLabel);
            this->bookPanel->Controls->Add(this->availableComboBox);
            this->bookPanel->Controls->Add(this->addButton);
            this->bookPanel->Controls->Add(this->removeButton);
            this->bookPanel->Controls->Add(this->clearButton);
            this->bookPanel->Location = System::Drawing::Point(12, 27);
            this->bookPanel->Name = L"bookPanel";
            this->bookPanel->Size = System::Drawing::Size(250, 350);
            this->bookPanel->TabIndex = 1;

            // idLabel
            this->idLabel->AutoSize = true;
            this->idLabel->Location = System::Drawing::Point(10, 15);
            this->idLabel->Name = L"idLabel";
            this->idLabel->Size = System::Drawing::Size(18, 13);
            this->idLabel->TabIndex = 0;
            this->idLabel->Text = L"ID:";

            // idTextBox
            this->idTextBox->Location = System::Drawing::Point(90, 12);
            this->idTextBox->Name = L"idTextBox";
            this->idTextBox->Size = System::Drawing::Size(150, 20);
            this->idTextBox->TabIndex = 1;

            // titleLabel
            this->titleLabel->AutoSize = true;
            this->titleLabel->Location = System::Drawing::Point(10, 45);
            this->titleLabel->Name = L"titleLabel";
            this->titleLabel->Size = System::Drawing::Size(60, 13);
            this->titleLabel->TabIndex = 2;
            this->titleLabel->Text = L"��������:";

            // titleTextBox
            this->titleTextBox->Location = System::Drawing::Point(90, 42);
            this->titleTextBox->Name = L"titleTextBox";
            this->titleTextBox->Size = System::Drawing::Size(150, 20);
            this->titleTextBox->TabIndex = 3;

            // authorLabel
            this->authorLabel->AutoSize = true;
            this->authorLabel->Location = System::Drawing::Point(10, 75);
            this->authorLabel->Name = L"authorLabel";
            this->authorLabel->Size = System::Drawing::Size(40, 13);
            this->authorLabel->TabIndex = 4;
            this->authorLabel->Text = L"�����:";

            // authorTextBox
            this->authorTextBox->Location = System::Drawing::Point(90, 72);
            this->authorTextBox->Name = L"authorTextBox";
            this->authorTextBox->Size = System::Drawing::Size(150, 20);
            this->authorTextBox->TabIndex = 5;

            // yearLabel
            this->yearLabel->AutoSize = true;
            this->yearLabel->Location = System::Drawing::Point(10, 105);
            this->yearLabel->Name = L"yearLabel";
            this->yearLabel->Size = System::Drawing::Size(28, 13);
            this->yearLabel->TabIndex = 6;
            this->yearLabel->Text = L"���:";

            // yearTextBox
            this->yearTextBox->Location = System::Drawing::Point(90, 102);
            this->yearTextBox->Name = L"yearTextBox";
            this->yearTextBox->Size = System::Drawing::Size(150, 20);
            this->yearTextBox->TabIndex = 7;

            // genreLabel
            this->genreLabel->AutoSize = true;
            this->genreLabel->Location = System::Drawing::Point(10, 135);
            this->genreLabel->Name = L"genreLabel";
            this->genreLabel->Size = System::Drawing::Size(39, 13);
            this->genreLabel->TabIndex = 8;
            this->genreLabel->Text = L"����:";

            // genreTextBox
            this->genreTextBox->Location = System::Drawing::Point(90, 132);
            this->genreTextBox->Name = L"genreTextBox";
            this->genreTextBox->Size = System::Drawing::Size(150, 20);
            this->genreTextBox->TabIndex = 9;

            // availableLabel
            this->availableLabel->AutoSize = true;
            this->availableLabel->Location = System::Drawing::Point(10, 165);
            this->availableLabel->Name = L"availableLabel";
            this->availableLabel->Size = System::Drawing::Size(61, 13);
            this->availableLabel->TabIndex = 10;
            this->availableLabel->Text = L"��������:";

            // availableComboBox
            this->availableComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->availableComboBox->FormattingEnabled = true;
            this->availableComboBox->Items->AddRange(gcnew cli::array< System::Object^ >(2) { L"��", L"���" });
            this->availableComboBox->Location = System::Drawing::Point(90, 162);
            this->availableComboBox->Name = L"availableComboBox";
            this->availableComboBox->Size = System::Drawing::Size(150, 21);
            this->availableComboBox->TabIndex = 11;
            this->availableComboBox->SelectedIndex = 0;

            // addButton
            this->addButton->Location = System::Drawing::Point(10, 200);
            this->addButton->Name = L"addButton";
            this->addButton->Size = System::Drawing::Size(230, 30);
            this->addButton->TabIndex = 12;
            this->addButton->Text = L"�������� �����";
            this->addButton->UseVisualStyleBackColor = true;
            this->addButton->Click += gcnew System::EventHandler(this, &MainForm::addButton_Click);

            // removeButton
            this->removeButton->Location = System::Drawing::Point(10, 240);
            this->removeButton->Name = L"removeButton";
            this->removeButton->Size = System::Drawing::Size(230, 30);
            this->removeButton->TabIndex = 13;
            this->removeButton->Text = L"������� ����� �� ID";
            this->removeButton->UseVisualStyleBackColor = true;
            this->removeButton->Click += gcnew System::EventHandler(this, &MainForm::removeButton_Click);

            // clearButton
            this->clearButton->Location = System::Drawing::Point(10, 280);
            this->clearButton->Name = L"clearButton";
            this->clearButton->Size = System::Drawing::Size(230, 30);
            this->clearButton->TabIndex = 14;
            this->clearButton->Text = L"�������� �����";
            this->clearButton->UseVisualStyleBackColor = true;
            this->clearButton->Click += gcnew System::EventHandler(this, &MainForm::clearButton_Click);

            // searchTextBox
            this->searchTextBox->Location = System::Drawing::Point(275, 27);
            this->searchTextBox->Name = L"searchTextBox";
            this->searchTextBox->Size = System::Drawing::Size(400, 20);
            this->searchTextBox->TabIndex = 2;
            this->searchTextBox->Text = L"������� ����� ��� ������...";

            // searchButton
            this->searchButton->Location = System::Drawing::Point(685, 27);
            this->searchButton->Name = L"searchButton";
            this->searchButton->Size = System::Drawing::Size(100, 20);
            this->searchButton->TabIndex = 3;
            this->searchButton->Text = L"�����";
            this->searchButton->UseVisualStyleBackColor = true;
            this->searchButton->Click += gcnew System::EventHandler(this, &MainForm::searchButton_Click);

            // booksListView
            this->booksListView->FullRowSelect = true;
            this->booksListView->GridLines = true;
            this->booksListView->HideSelection = false;
            this->booksListView->Location = System::Drawing::Point(275, 60);
            this->booksListView->MultiSelect = false;
            this->booksListView->Name = L"booksListView";
            this->booksListView->Size = System::Drawing::Size(510, 317);
            this->booksListView->TabIndex = 4;
            this->booksListView->UseCompatibleStateImageBehavior = false;
            this->booksListView->View = System::Windows::Forms::View::Details;
            this->booksListView->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::booksListView_SelectedIndexChanged);

            // ���������� ������� � ListView
            this->booksListView->Columns->Add(L"ID", 40);
            this->booksListView->Columns->Add(L"��������", 150);
            this->booksListView->Columns->Add(L"�����", 120);
            this->booksListView->Columns->Add(L"���", 50);
            this->booksListView->Columns->Add(L"����", 90);
            this->booksListView->Columns->Add(L"��������", 60);

            // statusStrip
            this->statusStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^ >(1) { this->statusLabel });
            this->statusStrip->Location = System::Drawing::Point(0, 389);
            this->statusStrip->Name = L"statusStrip";
            this->statusStrip->Size = System::Drawing::Size(800, 22);
            this->statusStrip->TabIndex = 5;
            this->statusStrip->Text = L"statusStrip";

            // statusLabel
            this->statusLabel->Name = L"statusLabel";
            this->statusLabel->Size = System::Drawing::Size(122, 17);
            this->statusLabel->Text = L"����� � ������";

            // saveFileDialog
            this->saveFileDialog->DefaultExt = L"lib";
            this->saveFileDialog->Filter = L"����� ���������� (*.lib)|*.lib|��� ����� (*.*)|*.*";
            this->saveFileDialog->Title = L"��������� ���� ����������";

            // openFileDialog
            this->openFileDialog->DefaultExt = L"lib";
            this->openFileDialog->Filter = L"����� ���������� (*.lib)|*.lib|��� ����� (*.*)|*.*";
            this->openFileDialog->Title = L"������� ���� ����������";

            // MainForm
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(800, 411);
            this->Controls->Add(this->statusStrip);
            this->Controls->Add(this->booksListView);
            this->Controls->Add(this->searchButton);
            this->Controls->Add(this->searchTextBox);
            this->Controls->Add(this->bookPanel);
            this->Controls->Add(this->menuStrip);
            this->MainMenuStrip = this->menuStrip;
            this->Name = L"MainForm";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"������� ���������� �����������";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);

            this->menuStrip->ResumeLayout(false);
            this->menuStrip->PerformLayout();
            this->bookPanel->ResumeLayout(false);
            this->bookPanel->PerformLayout();
            this->statusStrip->ResumeLayout(false);
            this->statusStrip->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();
        }

    private:
        // ����
        FileManager^ fileManager;
        String^ currentFileName;

        // ����������� ������� ��� ������� ����
        System::Void createFileMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void openFileMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void saveFileMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void exitMenuItem_Click(System::Object^ sender, System::EventArgs^ e);

        // ����������� ������� ��� ������
        System::Void addButton_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void removeButton_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void clearButton_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void searchButton_Click(System::Object^ sender, System::EventArgs^ e);

        // ���������� ��������� ������ � ListView
        System::Void booksListView_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);

        // ���������� �������� �����
        System::Void MainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);

        // ��������������� ������
        void LoadBooksToListView(System::Collections::Generic::List<String^>^ books);
        void ClearFormFields();
        bool ValidateForm();
        void UpdateStatusLabel(String^ message);
    };
}