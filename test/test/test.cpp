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
