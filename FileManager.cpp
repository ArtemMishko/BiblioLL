#include "FileManager.h"
#include <sstream>
#include <algorithm>

namespace LibrarySystem {

    // �����������
    FileManager::FileManager() {}

    // �������� ������ �����
    bool FileManager::CreateFile(String^ fileName) {
        try {
            std::string stdFileName = SystemStringToStdString(fileName);
            std::ofstream file(stdFileName);

            if (file.is_open()) {
                // ��������� ��������� ��� ����� ����������
                file << "ID;��������;�����;���;����;��������\n";
                file.close();
                return true;
            }
            return false;
        }
        catch (...) {
            return false;
        }
    }

    // ���������� ������ � ����
    bool FileManager::SaveToFile(String^ fileName, System::Collections::Generic::List<String^>^ data) {
        try {
            std::string stdFileName = SystemStringToStdString(fileName);
            std::ofstream file(stdFileName);

            if (file.is_open()) {
                // ��������� ��������� ��� ����� ����������
                file << "ID;��������;�����;���;����;��������\n";

                // ���������� ��� ������ ������
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

    // ������ ������ �� �����
    System::Collections::Generic::List<String^>^ FileManager::ReadFromFile(String^ fileName) {
        System::Collections::Generic::List<String^>^ result = gcnew System::Collections::Generic::List<String^>();

        try {
            std::string stdFileName = SystemStringToStdString(fileName);
            std::ifstream file(stdFileName);

            if (file.is_open()) {
                std::string line;
                bool isFirstLine = true;

                while (std::getline(file, line)) {
                    // ���������� ��������� (������ ������)
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
            // � ������ ������ ���������� ������ ������
        }

        return result;
    }

    // ���������� ����� ����� � ����
    bool FileManager::AddBook(String^ fileName, String^ id, String^ title, String^ author, String^ year, String^ genre, String^ available) {
        try {
            // �������� ������� ������
            auto currentData = ReadFromFile(fileName);

            // ������� ����� �����
            Book newBook(
                SystemStringToStdString(id),
                SystemStringToStdString(title),
                SystemStringToStdString(author),
                SystemStringToStdString(year),
                SystemStringToStdString(genre),
                SystemStringToStdString(available)
            );

            // ��������� ����� ����� � ������
            currentData->Add(ConvertBookToLine(newBook));

            // ��������� ����������� ������ ������� � ����
            return SaveToFile(fileName, currentData);
        }
        catch (...) {
            return false;
        }
    }

    // �������� ����� �� ����� �� ID
    bool FileManager::RemoveBook(String^ fileName, String^ id) {
        try {
            // �������� ������� ������
            auto currentData = ReadFromFile(fileName);
            std::string stdId = SystemStringToStdString(id);

            // ������� ����� ������ ��� ��������� �����
            System::Collections::Generic::List<String^>^ newData = gcnew System::Collections::Generic::List<String^>();

            for each (String ^ bookLine in currentData) {
                Book book = ConvertLineToBook(SystemStringToStdString(bookLine));
                if (book.id != stdId) {
                    newData->Add(bookLine);
                }
            }

            // ��������� ����������� ������ ������� � ����
            return SaveToFile(fileName, newData);
        }
        catch (...) {
            return false;
        }
    }

    // ����� ���� �� ���������� ������� (���� �� ���� �����)
    System::Collections::Generic::List<String^>^ FileManager::SearchBooks(String^ fileName, String^ searchTerm) {
        System::Collections::Generic::List<String^>^ result = gcnew System::Collections::Generic::List<String^>();
        std::string stdSearchTerm = SystemStringToStdString(searchTerm);

        // ����������� ��������� ������ � ������ ������� ��� �������������������� ������
        std::transform(stdSearchTerm.begin(), stdSearchTerm.end(), stdSearchTerm.begin(), ::tolower);

        try {
            // �������� ��� �����
            auto allBooks = ReadFromFile(fileName);

            // ���� ����������
            for each (String ^ bookLine in allBooks) {
                std::string stdLine = SystemStringToStdString(bookLine);

                // ����������� ������ � ������ ������� ��� �������������������� ������
                std::string lowerCaseLine = stdLine;
                std::transform(lowerCaseLine.begin(), lowerCaseLine.end(), lowerCaseLine.begin(), ::tolower);

                // ���� ������� ����������, ��������� � ����������
                if (lowerCaseLine.find(stdSearchTerm) != std::string::npos) {
                    result->Add(StdStringToSystemString(stdLine));
                }
            }
        }
        catch (...) {
            // � ������ ������ ���������� ������ ������
        }

        return result;
    }

    // ����������� ������� Book � ������ ��� �����
    String^ FileManager::ConvertBookToLine(Book book) {
        std::stringstream ss;
        ss << book.id << ";" << book.title << ";" << book.author << ";"
            << book.year << ";" << book.genre << ";" << book.available;
        return StdStringToSystemString(ss.str());
    }

    // ����������� ������ �� ����� � ������ Book
    Book FileManager::ConvertLineToBook(std::string line) {
        Book book;
        std::stringstream ss(line);
        std::string token;

        // ID
        if (std::getline(ss, token, ';')) book.id = token;

        // ��������
        if (std::getline(ss, token, ';')) book.title = token;

        // �����
        if (std::getline(ss, token, ';')) book.author = token;

        // ���
        if (std::getline(ss, token, ';')) book.year = token;

        // ����
        if (std::getline(ss, token, ';')) book.genre = token;

        // �����������
        if (std::getline(ss, token, ';')) book.available = token;

        return book;
    }

    // ����������� std::string � System::String^
    String^ FileManager::StdStringToSystemString(std::string str) {
        return gcnew String(str.c_str());
    }

    // ����������� System::String^ � std::string
    std::string FileManager::SystemStringToStdString(String^ str) {
        return msclr::interop::marshal_as<std::string>(str);
    }
}