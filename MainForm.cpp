#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

// ���������� ������� �������� ������ �����
System::Void LibraryApp::MainForm::createFileMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        currentFileName = saveFileDialog->FileName;

        if (fileManager->CreateFile(currentFileName))
        {
            UpdateStatusLabel("������ ����� ����: " + currentFileName);
            booksListView->Items->Clear();
            ClearFormFields();

            // ��������� ��������� ����� � ������ �����
            this->Text = "������� ���������� ����������� - " + System::IO::Path::GetFileName(currentFileName);
        }
        else
        {
            MessageBox::Show("�� ������� ������� ����!", "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
}

// ���������� ������� �������� �����
System::Void LibraryApp::MainForm::openFileMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        currentFileName = openFileDialog->FileName;

        try
        {
            auto books = fileManager->ReadFromFile(currentFileName);
            LoadBooksToListView(books);

            UpdateStatusLabel("������ ����: " + currentFileName);
            ClearFormFields();

            // ��������� ��������� ����� � ������ �����
            this->Text = "������� ���������� ����������� - " + System::IO::Path::GetFileName(currentFileName);
        }
        catch (...)
        {
            MessageBox::Show("������ ��� ������ �����!", "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
}

// ���������� ������� ���������� �����
System::Void LibraryApp::MainForm::saveFileMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (currentFileName == "")
    {
        if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            currentFileName = saveFileDialog->FileName;
        }
        else
        {
            return;
        }
    }

    // �������� ������ �� ListView
    System::Collections::Generic::List<String^>^ data = gcnew System::Collections::Generic::List<String^>();

    for (int i = 0; i < booksListView->Items->Count; i++)
    {
        ListViewItem^ item = booksListView->Items[i];
        String^ line = "";

        for (int j = 0; j < item->SubItems->Count; j++)
        {
            line += item->SubItems[j]->Text;
            if (j < item->SubItems->Count - 1)
                line += ";";
        }

        data->Add(line);
    }

    if (fileManager->SaveToFile(currentFileName, data))
    {
        UpdateStatusLabel("���� ��������: " + currentFileName);
    }
    else
    {
        MessageBox::Show("�� ������� ��������� ����!", "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

// ���������� ������� ������ �� ���������
System::Void LibraryApp::MainForm::exitMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
    this->Close();
}

// ���������� ������� ������ ���������� �����
System::Void LibraryApp::MainForm::addButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (currentFileName == "")
    {
        MessageBox::Show("������� �������� ��� �������� ����!", "��������������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    if (!ValidateForm())
    {
        MessageBox::Show("����������, ��������� ��� ����!", "��������������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    String^ id = idTextBox->Text;
    String^ title = titleTextBox->Text;
    String^ author = authorTextBox->Text;
    String^ year = yearTextBox->Text;
    String^ genre = genreTextBox->Text;
    String^ available = availableComboBox->SelectedItem->ToString();

    // ��������, ���� �� ��� ����� � ����� ID
    for (int i = 0; i < booksListView->Items->Count; i++)
    {
        if (booksListView->Items[i]->Text == id)
        {
            MessageBox::Show("����� � ����� ID ��� ����������!", "��������������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
    }

    if (fileManager->AddBook(currentFileName, id, title, author, year, genre, available))
    {
        // ������� ������ ����
        auto books = fileManager->ReadFromFile(currentFileName);
        LoadBooksToListView(books);

        UpdateStatusLabel("����� ���������: " + title);
        ClearFormFields();
    }
    else
    {
        MessageBox::Show("�� ������� �������� �����!", "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

// ���������� ������� ������ �������� �����
System::Void LibraryApp::MainForm::removeButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (currentFileName == "")
    {
        MessageBox::Show("������� �������� ��� �������� ����!", "��������������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    String^ id = idTextBox->Text;

    if (id == "")
    {
        MessageBox::Show("����������, ������� ID ����� ��� ��������!", "��������������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    if (MessageBox::Show("�� �������, ��� ������ ������� ����� � ID " + id + "?", "�������������",
        MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes)
    {
        if (fileManager->RemoveBook(currentFileName, id))
        {
            // ������� ������ ����
            auto books = fileManager->ReadFromFile(currentFileName);
            LoadBooksToListView(books);

            UpdateStatusLabel("����� �������: ID " + id);
            ClearFormFields();
        }
        else
        {
            MessageBox::Show("�� ������� ������� �����!", "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
}

// ���������� ������� ������ ������� �����
System::Void LibraryApp::MainForm::clearButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    ClearFormFields();
}

// ���������� ������� ������ ������
System::Void LibraryApp::MainForm::searchButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (currentFileName == "")
    {
        MessageBox::Show("������� �������� ��� �������� ����!", "��������������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    String^ searchTerm = searchTextBox->Text;

    // ���� ����� ��������� � ���������� ��� ������, ��������� ��� �����
    if (searchTerm == "������� ����� ��� ������..." || searchTerm == "")
    {
        // ��������� ��� �����
        auto books = fileManager->ReadFromFile(currentFileName);
        LoadBooksToListView(books);
        UpdateStatusLabel("��������� ��� �����");
    }
    else
    {
        // ��������� �����
        auto foundBooks = fileManager->SearchBooks(currentFileName, searchTerm);
        LoadBooksToListView(foundBooks);
        UpdateStatusLabel("������� ����: " + foundBooks->Count);
    }
}

// ���������� ��������� ������ � ListView
System::Void LibraryApp::MainForm::booksListView_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
    if (booksListView->SelectedItems->Count > 0)
    {
        ListViewItem^ selectedItem = booksListView->SelectedItems[0];

        idTextBox->Text = selectedItem->SubItems[0]->Text;
        titleTextBox->Text = selectedItem->SubItems[1]->Text;
        authorTextBox->Text = selectedItem->SubItems[2]->Text;
        yearTextBox->Text = selectedItem->SubItems[3]->Text;
        genreTextBox->Text = selectedItem->SubItems[4]->Text;
        availableComboBox->SelectedItem = selectedItem->SubItems[5]->Text;
    }
}

// ���������� �������� �����
System::Void LibraryApp::MainForm::MainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
{
    if (MessageBox::Show("�� �������, ��� ������ ����� �� ���������?", "�������������",
        MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::No)
    {
        e->Cancel = true;
    }
}

// ����� ��� �������� ���� � ListView
void LibraryApp::MainForm::LoadBooksToListView(System::Collections::Generic::List<String^>^ books)
{
    booksListView->Items->Clear();

    for each (String ^ bookLine in books)
    {
        cli::array<String^>^ parts = bookLine->Split(';');

        // ���������, ��� � ��� ���������� ���������� ������
        if (parts->Length >= 6)
        {
            ListViewItem^ item = gcnew ListViewItem(parts[0]); // ID
            item->SubItems->Add(parts[1]); // ��������
            item->SubItems->Add(parts[2]); // �����
            item->SubItems->Add(parts[3]); // ���
            item->SubItems->Add(parts[4]); // ����
            item->SubItems->Add(parts[5]); // �����������

            booksListView->Items->Add(item);
        }
    }
}

// ����� ��� ������� ����� �����
void LibraryApp::MainForm::ClearFormFields()
{
    idTextBox->Text = "";
    titleTextBox->Text = "";
    authorTextBox->Text = "";
    yearTextBox->Text = "";
    genreTextBox->Text = "";
    availableComboBox->SelectedIndex = 0;
    idTextBox->Focus();
}

// ����� ��� ��������� �����
bool LibraryApp::MainForm::ValidateForm()
{
    return idTextBox->Text != "" &&
        titleTextBox->Text != "" &&
        authorTextBox->Text != "" &&
        yearTextBox->Text != "" &&
        genreTextBox->Text != "" &&
        availableComboBox->SelectedIndex >= 0;
}

// ����� ��� ���������� �������
void LibraryApp::MainForm::UpdateStatusLabel(String^ message)
{
    statusLabel->Text = message;
}

// ����� ����� � ����������
[STAThread]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // ������� � ��������� �����
    Application::Run(gcnew LibraryApp::MainForm());
    return 0;
}