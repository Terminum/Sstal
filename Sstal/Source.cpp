#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm> // swap
#include <Windows.h> // для корректного вывода русского языка в файл

class Notes
{
private:
	std::string subject;
	std::string note;
public:
	Notes() 
	{
		subject = "Test",
		note = "Test note.";
	}

	std::string getSub()
	{
		return subject;
	}

	std::string getNote()
	{
		return note;
	}

	void setSub(std::string s)
	{
		if (s.size() > 15)
		{
			s.resize(15);
		}
		subject = s;

	}

	void setNote(std::string s)
	{
		note = s;
	}
	friend std::ostream& operator<< (std::ostream& out, const Notes& Note); // перегрузка вывода для своего класса

};

std::ostream& operator<< (std::ostream& out, const Notes& Note) // имеет прямой доступ к subject и note тк friend
{
	std::cout << "Subject : " << Note.subject << std::endl;
	std::cout << "Note : " << Note.note << std::endl;
	return out;
}

void PrMenu() // вывод меню
{
	std::cout << "Menu : <a> - Add Note, <d> - Delete note, <p> - Print all notes, <c> - for clear screen,\n";
	std::cout << "\t<x> Print one note, <r> -Change note, <e> -Exit \n-> ";
}

void BUp(std::vector<Notes> &ANotes) // Сохранение заметки между сеансами приложения
{
	std::ofstream filewrite;
	filewrite.open("Note.txt");

	for (int i = 0; i < ANotes.size(); i++)
	{
		filewrite << ANotes[i].getSub() << std::endl;
		filewrite << ANotes[i].getNote() << std::endl;
	}
	filewrite.close();
	std::cout << "Backup created.\n";
}

int LoadData(std::vector<Notes>& ANotes) // Восстановление заметок для работы в сеансе
{
	Notes note;
	std::string str;
	std::ifstream filewrite;
	filewrite.open("Note.txt");

	if (!filewrite)
	{
		filewrite.close();
		return 0;
	}

	while (!filewrite.eof()) // считываем построчно и через метод класса заносим в экземпляр
	{
		std::getline(filewrite, str);
		note.setSub(str);
		std::getline(filewrite, str);
		note.setNote(str);
		ANotes.push_back(note); // пушим структуру в стек
	}
	ANotes.pop_back(); // удаляем полседнюю записть тк при записи последний endl дает переход на новую строку и происходит считывание пустых значений
	filewrite.close();
	return 1;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	std::vector<Notes> ANotes; // основная структура списка заметок, в одной заметке имеется тема и основное тело заметки

	if (LoadData(ANotes)) //выгрузка из межсессионного бэкапа, при наличии
	{
		std::cout << "Data loaded\n";
	}
	else std::cout << "No data to load\n";

	Notes n;
	if (ANotes.size() == 0) // если структура пуста, происходит добавление объекта, который через конструктор имеет значения 
	{						//"Test" "Test note."
		ANotes.push_back(n);
	}
	
	PrMenu();
	char c; // навигация по меню через символ
	std::string st;
	
	std::cin >> c;
	while (c != 'e') // выход на соотв символ
	{
		if (c == 'a') // lобавление новой заметки
		{
			
			std::cout << "Subject : ";
			std::cin.ignore(32767, '\n'); // для корректного считывания из входного потока
			std::getline(std::cin, st);
			n.setSub(st);
			
			std::cout << "Note : ";
			std::getline(std::cin, st);
			n.setNote(st);
			ANotes.push_back(n);
			system("cls"); // очистка консоли
		}
		if (c == 'p') // вывод списка тем заметок для общего просмотра
		{
			for (int i = 0; i < ANotes.size(); i++)
			{
				std::cout << i + 1 << " -> " << ANotes[i].getSub() << "..." << std::endl;
			}
		}
		if (c == 'd') // удаление заметки
		{
			if (ANotes.size() > 0)
			{
				std::cout << "Count of notes - " << ANotes.size() << std::endl;
				int i;
				std::cout << "Enter number of note to delete: ";
				std::cin >> i;
				if ((i > 0) and (i <= ANotes.size()))
				{
					std::swap(ANotes[i - 1], ANotes[ANotes.size() - 1]);
				}													// поскольку нельзя удалить из вектора произвольную позицию
				else std::cout << "No such note" << std::endl;
				ANotes.pop_back();									// свапаем ее с концом и pop_back приводит к удалению записи
			}
		}
		if (c == 'r') // изменение заметки
		{
			if (ANotes.size() > 0)
			{
				std::cout << "Count of notes - " << ANotes.size() << std::endl;
				int i;
				std::cout << "Enter number of note to change: ";
				std::cin >> i;
				if ((i > 0) and (i <= ANotes.size()))
				{
					std::cout << "Subject : ";
					std::cin.ignore(32767, '\n');
					std::getline(std::cin, st);
					ANotes[i - 1].setSub(st);

					std::cout << "Note : ";
					std::getline(std::cin, st);
					ANotes[i - 1].setNote(st);
				}
				else std::cout << "No such note" << std::endl;
			}
			else std::cout << "No notes";
		}
		if (c == 'c')
		{
			system("cls");
		}
		if (c == 'x') // вывод полоной информации заметки тема и основное тело
		{
			if (ANotes.size() > 0)
			{
				std::cout << "Count of notes - " << ANotes.size() << std::endl;
				int i;
				std::cout << "Enter number of note : ";
				std::cin >> i;
				if ((i > 0) and (i <= ANotes.size()))
				{
					std::cout << ANotes[i - 1] << std::endl;
				}
				else std::cout << "No such note" << std::endl;
			}
			else std::cout << "No notes" << std::endl;
			
		}
		PrMenu();
		std::cin >> c;
	}

	BUp(ANotes); // создание межсеансного бэкапа

	std::cout << "Exit";
}