#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

// Обработчик события создания нового файла
System::Void LibraryApp::MainForm::createFileMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        currentFileName = saveFileDialog->FileName;

        if (fileManager->CreateFile(currentFileName))
        {
            UpdateStatusLabel("Создан новый файл: " + currentFileName);
            booksListView->Items->Clear();
            ClearFormFields();

            // Установим заголовок формы с именем файла
            this->Text = "Система управления библиотекой - " + System::IO::Path::GetFileName(currentFileName);
        }
        else
        {
            MessageBox::Show("Не удалось создать файл!", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
}

// Обработчик события открытия файла
System::Void LibraryApp::MainForm::openFileMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
    {
        currentFileName = openFileDialog->FileName;

        try
        {
            auto books = fileManager->ReadFromFile(currentFileName);
            LoadBooksToListView(books);

            UpdateStatusLabel("Открыт файл: " + currentFileName);
            ClearFormFields();

            // Установим заголовок формы с именем файла
            this->Text = "Система управления библиотекой - " + System::IO::Path::GetFileName(currentFileName);
        }
        catch (...)
        {
            MessageBox::Show("Ошибка при чтении файла!", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
}

// Обработчик события сохранения файла
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

    // Собираем данные из ListView
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
        UpdateStatusLabel("Файл сохранен: " + currentFileName);
    }
    else
    {
        MessageBox::Show("Не удалось сохранить файл!", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

// Обработчик события выхода из программы
System::Void LibraryApp::MainForm::exitMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
    this->Close();
}

// Обработчик нажатия кнопки добавления книги
System::Void LibraryApp::MainForm::addButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (currentFileName == "")
    {
        MessageBox::Show("Сначала создайте или откройте файл!", "Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    if (!ValidateForm())
    {
        MessageBox::Show("Пожалуйста, заполните все поля!", "Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    String^ id = idTextBox->Text;
    String^ title = titleTextBox->Text;
    String^ author = authorTextBox->Text;
    String^ year = yearTextBox->Text;
    String^ genre = genreTextBox->Text;
    String^ available = availableComboBox->SelectedItem->ToString();

    // Проверим, есть ли уже книга с таким ID
    for (int i = 0; i < booksListView->Items->Count; i++)
    {
        if (booksListView->Items[i]->Text == id)
        {
            MessageBox::Show("Книга с таким ID уже существует!", "Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
    }

    if (fileManager->AddBook(currentFileName, id, title, author, year, genre, available))
    {
        // Обновим список книг
        auto books = fileManager->ReadFromFile(currentFileName);
        LoadBooksToListView(books);

        UpdateStatusLabel("Книга добавлена: " + title);
        ClearFormFields();
    }
    else
    {
        MessageBox::Show("Не удалось добавить книгу!", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

// Обработчик нажатия кнопки удаления книги
System::Void LibraryApp::MainForm::removeButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (currentFileName == "")
    {
        MessageBox::Show("Сначала создайте или откройте файл!", "Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    String^ id = idTextBox->Text;

    if (id == "")
    {
        MessageBox::Show("Пожалуйста, введите ID книги для удаления!", "Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    if (MessageBox::Show("Вы уверены, что хотите удалить книгу с ID " + id + "?", "Подтверждение",
        MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes)
    {
        if (fileManager->RemoveBook(currentFileName, id))
        {
            // Обновим список книг
            auto books = fileManager->ReadFromFile(currentFileName);
            LoadBooksToListView(books);

            UpdateStatusLabel("Книга удалена: ID " + id);
            ClearFormFields();
        }
        else
        {
            MessageBox::Show("Не удалось удалить книгу!", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
}

// Обработчик нажатия кнопки очистки формы
System::Void LibraryApp::MainForm::clearButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    ClearFormFields();
}

// Обработчик нажатия кнопки поиска
System::Void LibraryApp::MainForm::searchButton_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (currentFileName == "")
    {
        MessageBox::Show("Сначала создайте или откройте файл!", "Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
    }

    String^ searchTerm = searchTextBox->Text;

    // Если текст совпадает с подсказкой или пустой, загружаем все книги
    if (searchTerm == "Введите текст для поиска..." || searchTerm == "")
    {
        // Загружаем все книги
        auto books = fileManager->ReadFromFile(currentFileName);
        LoadBooksToListView(books);
        UpdateStatusLabel("Загружены все книги");
    }
    else
    {
        // Выполняем поиск
        auto foundBooks = fileManager->SearchBooks(currentFileName, searchTerm);
        LoadBooksToListView(foundBooks);
        UpdateStatusLabel("Найдено книг: " + foundBooks->Count);
    }
}

// Обработчик изменения выбора в ListView
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

// Обработчик закрытия формы
System::Void LibraryApp::MainForm::MainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
{
    if (MessageBox::Show("Вы уверены, что хотите выйти из программы?", "Подтверждение",
        MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::No)
    {
        e->Cancel = true;
    }
}

// Метод для загрузки книг в ListView
void LibraryApp::MainForm::LoadBooksToListView(System::Collections::Generic::List<String^>^ books)
{
    booksListView->Items->Clear();

    for each (String ^ bookLine in books)
    {
        cli::array<String^>^ parts = bookLine->Split(';');

        // Проверяем, что у нас правильное количество частей
        if (parts->Length >= 6)
        {
            ListViewItem^ item = gcnew ListViewItem(parts[0]); // ID
            item->SubItems->Add(parts[1]); // Название
            item->SubItems->Add(parts[2]); // Автор
            item->SubItems->Add(parts[3]); // Год
            item->SubItems->Add(parts[4]); // Жанр
            item->SubItems->Add(parts[5]); // Доступность

            booksListView->Items->Add(item);
        }
    }
}

// Метод для очистки полей формы
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

// Метод для валидации формы
bool LibraryApp::MainForm::ValidateForm()
{
    return idTextBox->Text != "" &&
        titleTextBox->Text != "" &&
        authorTextBox->Text != "" &&
        yearTextBox->Text != "" &&
        genreTextBox->Text != "" &&
        availableComboBox->SelectedIndex >= 0;
}

// Метод для обновления статуса
void LibraryApp::MainForm::UpdateStatusLabel(String^ message)
{
    statusLabel->Text = message;
}

// Точка входа в приложение
[STAThread]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Создаем и запускаем форму
    Application::Run(gcnew LibraryApp::MainForm());
    return 0;
}