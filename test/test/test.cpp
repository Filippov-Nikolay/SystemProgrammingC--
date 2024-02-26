/*
#include <iostream>
#include <ctime> // Use ctime instead of time.h

using namespace std;

int main() {
    srand(time(NULL));
    const int numberButtons = 9;
    int move = 0;
    bool f = true;
    bool fAll = true;
    bool arrayMove[numberButtons] = { true, true, true,
                                true, true, true,
                                true, true, true };
    do {
        move = rand() % 9;

        if (arrayMove[move]) {
            arrayMove[move] = false;
        }

        fAll = false;
        for (int i = 0; i < numberButtons; i++) {
            if (arrayMove[i]) {
                fAll = true;
                break;
            }
        }

    } while (fAll);


    for (int i = 0; i < numberButtons; i++) {
        cout << arrayMove[i] << " ";
    }

    std::cout << std::endl;

    std::cout << "Final f: " << f << std::endl;
    std::cout << "Final fAll: " << fAll << std::endl;
    std::cout << "Final move: " << move << std::endl;

    return 0;
}
*/

#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream in("Database.txt");

    /*const int max_size = 255;
    const int number_fields = 5;

    int counter = 0;
    int index = 0;

    char buff[max_size];

    char** temp = new char* [number_fields];

    for (int i = 0; i < number_fields; i++) {
        temp[i] = new char[max_size];
    }*/

    /*while (in.get(buff[index])) {
        if (buff[index] == ']') {
            buff[index] = '\0';

            for (int j = 0; j < strlen(buff); j++) {
                if (buff[j] != '[' && buff[j] != ']' && buff[j] != ' ' && buff[j] != '|') {
                    cout << buff[j];
                    temp[counter][j] = buff[j];

                    cout << "^" << temp[counter][j] << " ";
                }

                if (buff[j] == '|') {
                    temp[counter][j] = '\0';
                    counter++;
                }
            }

            counter = 0;
            index = 0;
        }
        else {
            index++;
        }
    }*/

    /* for (int i = 0; i < number_fields; i++) {
        delete[] temp[i];
    }

    delete[] temp;*/

    /*for (int i = 0; i < max_size; i++) {
    in >> buff[i];
    if (buff[i] == ']') {
        for (int j = 0; j < _tcslen(buff); j++) {
            if (buff[j] != '|' || buff[j] != ']' || buff[j] != ' ')
                temp[counter][max_size] += buff[j];

            if (buff[j] == '|')
                counter++;
        }
    }
}*/

    string temp[5];
    string tempChar;
    int index = 0;
    int counter = 0;

    const int max_size = 255;
    char buff[max_size];

    do {
        in >> buff[index];

        if (buff[index] == ']') {
            buff[index + 1] = '\0';

            for (int j = 0; j < strlen(buff); j++) {
                if (buff[j] != ' ' && buff[j] != '[' && buff[j] != '|' && buff[j] != ']') {
                    tempChar += buff[j];
                }
                else if (buff[j] == '|') {
                    temp[counter++] = tempChar;
                    tempChar = "";
                }
            }

            tempChar = "";
            counter = 0;
            index = 0;
        }
        else {
            index++;
        }
    } while (in);

    for (int i = 0; i < 5; i++) {
        cout << temp[i] << " ";
    }

    return 0;
}