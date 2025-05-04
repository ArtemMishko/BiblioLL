#include "FileManager.h"
#include <sstream>
#include <algorithm>

namespace LibrarySystem {

    // Конструктор
    FileManager::FileManager() {}

    // Создание нового файла
    bool FileManager::CreateFile(String^ fileName) {
        try {
            std::string stdFileName = SystemStringToStdString(fileName);
            std::ofstream file(stdFileName);

            if (file.is_open()) {
                // Добавляем заголовок для файла библиотеки
                file << "ID;Название;Автор;Год;Жанр;Доступна\n";
                file.close();
                return true;
            }
            return false;
        }
        catch (...) {
            return false;
        }
    }

    // Сохранение данных в файл
    bool FileManager::SaveToFile(String^ fileName, System::Collections::Generic::List<String^>^ data) {
        try {
            std::string stdFileName = SystemStringToStdString(fileName);
            std::ofstream file(stdFileName);

            if (file.is_open()) {
                // Добавляем заголовок для файла библиотеки
                file << "ID;Название;Автор;Год;Жанр;Доступна\n";

                // Записываем все строки данных
                for each (String ^ line in data) {
                    std::string stdLine = SystemStringToStdString(line);
                    file << stdLine << "\n";
                }

                file.close();
                return true;
            }
            return false;
        }
        catch (...) {
            return false;
        }
    }

    // Чтение данных из файла
    System::Collections::Generic::List<String^>^ FileManager::ReadFromFile(String^ fileName) {
        System::Collections::Generic::List<String^>^ result = gcnew System::Collections::Generic::List<String^>();

        try {
            std::string stdFileName = SystemStringToStdString(fileName);
            std::ifstream file(stdFileName);

            if (file.is_open()) {
                std::string line;
                bool isFirstLine = true;

                while (std::getline(file, line)) {
                    // Пропускаем заголовок (первую строку)
                    if (isFirstLine) {
                        isFirstLine = false;
                        continue;
                    }

                    if (!line.empty()) {
                        result->Add(StdStringToSystemString(line));
                    }
                }

                file.close();
            }
        }
        catch (...) {
            // В случае ошибки возвращаем пустой список
        }

        return result;
    }

    // Добавление новой книги в файл
    bool FileManager::AddBook(String^ fileName, String^ id, String^ title, String^ author, String^ year, String^ genre, String^ available) {
        try {
            // Получаем текущие данные
            auto currentData = ReadFromFile(fileName);

            // Создаем новую книгу
            Book newBook(
                SystemStringToStdString(id),
                SystemStringToStdString(title),
                SystemStringToStdString(author),
                SystemStringToStdString(year),
                SystemStringToStdString(genre),
                SystemStringToStdString(available)
            );

            // Добавляем новую книгу в список
            currentData->Add(ConvertBookToLine(newBook));

            // Сохраняем обновленный список обратно в файл
            return SaveToFile(fileName, currentData);
        }
        catch (...) {
            return false;
        }
    }

    // Удаление книги из файла по ID
    bool FileManager::RemoveBook(String^ fileName, String^ id) {
        try {
            // Получаем текущие данные
            auto currentData = ReadFromFile(fileName);
            std::string stdId = SystemStringToStdString(id);

            // Создаем новый список без удаляемой книги
            System::Collections::Generic::List<String^>^ newData = gcnew System::Collections::Generic::List<String^>();

            for each (String ^ bookLine in currentData) {
                Book book = ConvertLineToBook(SystemStringToStdString(bookLine));
                if (book.id != stdId) {
                    newData->Add(bookLine);
                }
            }

            // Сохраняем обновленный список обратно в файл
            return SaveToFile(fileName, newData);
        }
        catch (...) {
            return false;
        }
    }

    // Поиск книг по поисковому запросу (ищет во всех полях)
    System::Collections::Generic::List<String^>^ FileManager::SearchBooks(String^ fileName, String^ searchTerm) {
        System::Collections::Generic::List<String^>^ result = gcnew System::Collections::Generic::List<String^>();
        std::string stdSearchTerm = SystemStringToStdString(searchTerm);

        // Преобразуем поисковый запрос в нижний регистр для регистронезависимого поиска
        std::transform(stdSearchTerm.begin(), stdSearchTerm.end(), stdSearchTerm.begin(), ::tolower);

        try {
            // Получаем все книги
            auto allBooks = ReadFromFile(fileName);

            // Ищем совпадения
            for each (String ^ bookLine in allBooks) {
                std::string stdLine = SystemStringToStdString(bookLine);

                // Преобразуем строку в нижний регистр для регистронезависимого поиска
                std::string lowerCaseLine = stdLine;
                std::transform(lowerCaseLine.begin(), lowerCaseLine.end(), lowerCaseLine.begin(), ::tolower);

                // Если найдено совпадение, добавляем в результаты
                if (lowerCaseLine.find(stdSearchTerm) != std::string::npos) {
                    result->Add(StdStringToSystemString(stdLine));
                }
            }
        }
        catch (...) {
            // В случае ошибки возвращаем пустой список
        }

        return result;
    }

    // Конвертация объекта Book в строку для файла
    String^ FileManager::ConvertBookToLine(Book book) {
        std::stringstream ss;
        ss << book.id << ";" << book.title << ";" << book.author << ";"
            << book.year << ";" << book.genre << ";" << book.available;
        return StdStringToSystemString(ss.str());
    }

    // Конвертация строки из файла в объект Book
    Book FileManager::ConvertLineToBook(std::string line) {
        Book book;
        std::stringstream ss(line);
        std::string token;

        // ID
        if (std::getline(ss, token, ';')) book.id = token;

        // Название
        if (std::getline(ss, token, ';')) book.title = token;

        // Автор
        if (std::getline(ss, token, ';')) book.author = token;

        // Год
        if (std::getline(ss, token, ';')) book.year = token;

        // Жанр
        if (std::getline(ss, token, ';')) book.genre = token;

        // Доступность
        if (std::getline(ss, token, ';')) book.available = token;

        return book;
    }

    // Конвертация std::string в System::String^
    String^ FileManager::StdStringToSystemString(std::string str) {
        return gcnew String(str.c_str());
    }

    // Конвертация System::String^ в std::string
    std::string FileManager::SystemStringToStdString(String^ str) {
        return msclr::interop::marshal_as<std::string>(str);
    }
}