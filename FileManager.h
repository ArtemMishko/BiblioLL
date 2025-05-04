#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <msclr/marshal_cppstd.h>

using namespace System;

namespace LibrarySystem {

    // ��������� ��� �������� ���������� � �����
    struct Book {
        std::string id;
        std::string title;
        std::string author;
        std::string year;
        std::string genre;
        std::string available;

        // ����������� �� ���������
        Book() : id(""), title(""), author(""), year(""), genre(""), available("��") {}

        // ����������� � �����������
        Book(std::string id, std::string title, std::string author, std::string year, std::string genre, std::string available)
            : id(id), title(title), author(author), year(year), genre(genre), available(available) {
        }
    };

    public ref class FileManager
    {
    public:
        // �����������
        FileManager();

        // ������ ��� ������ � �������
        bool CreateFile(String^ fileName);
        bool SaveToFile(String^ fileName, System::Collections::Generic::List<String^>^ data);
        System::Collections::Generic::List<String^>^ ReadFromFile(String^ fileName);

        // ������ ��� ������ � �������
        bool AddBook(String^ fileName, String^ id, String^ title, String^ author, String^ year, String^ genre, String^ available);
        bool RemoveBook(String^ fileName, String^ id);
        System::Collections::Generic::List<String^>^ SearchBooks(String^ fileName, String^ searchTerm);

        // ��������������� ������
        String^ ConvertBookToLine(Book book);
        Book ConvertLineToBook(std::string line);
        String^ StdStringToSystemString(std::string str);
        std::string SystemStringToStdString(String^ str);
    };
}