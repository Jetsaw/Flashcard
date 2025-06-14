#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;
const int maxnum = 50;
class Card
{
public:
    string question;
    string answer;
    Card()
    {
        question = " ";
        answer = " ";
    }
    Card(string q, string a)
    {
        question = q;
        answer = a;
    }
};
class Feedback
{
private:
public:
    int marks;
    Feedback()
    {
        marks = 0;
    }
    Feedback(int m)
    {
        marks = m;
    }
};
class CardsManager
{
public:
    Card cards[maxnum];
    Feedback feedbacks[maxnum];
    int num;
    CardsManager()
    {
        num = 0;
    }
    void getNewCard(string ques, string ans)
    {
        if(num<maxnum)
        {
            cards[num] = Card(ques, ans);
            num++;
            cout << "\nCard is added successfully!" << endl;
        }
        else
        {
            cout << "Cannot add more cards. Maximum capacity reached." << endl;
        }
    }
    void displayCards()
    {
        int point = 0;
        if (num == 0)
        {
            cout << "No cards to display." << endl;
            return;
        }
        else
        {
            cout << "\nTotal Cards = " << num << endl;
            cout << endl;
            for(int i=0; i<num; i++)
            {
                cout << "Question " << i+1 << ": " << cards[i].question << endl;
                cout << "Answer " << i+1 << ": " << cards[i].answer << endl;
                cout << "\nHow well did you know this? (0: Not well, 1: Normal, 2: Perfectly)" << endl;
                cin >> point;
                while(point < 0 || point >2)
                {
                    cout << "Error. Please try again." << endl;
                    cin >> point;
                    if(point == 0 || point == 1 || point == 2 )
                        break;
                }
                feedbacks[i] = Feedback(point);
                cout << endl;
            }
        }
    }
};
class Process
{
private:
    CardsManager cm;
    string q;
    string a;
public:
    void addQuestion()
    {
        cout << "Enter Question: ";
        cin >> q;
    }
    void addAnswer()
    {
        cout << "Enter Answer: ";
        cin >> a;
    }
    void addCard()
    {
        addQuestion();
        addAnswer();
        cm.getNewCard(q, a);
    }
    void showCard()
    {
        cout << "Displaying flashcards..." << endl;
        cm.displayCards();
    }
};
class Menu
{
    int choice;
    Process p;
public:
    Menu()
    {
        choice = 0;
    }

    void showMenu()
    {
        while(choice !=3)
        {
            cout << "\n===== Flash Card =====" << endl;
            cout << "1. Add new card" << endl;
            cout << "2. Display card" << endl;
            cout << "3. Exit" << endl;
            cout << "\nEnter choice: ";
            cin >> choice;
            system("CLS");
            switch(choice)
            {
            case 1:
                p.addCard();
                break;
            case 2:
                p.showCard();
                break;
            case 3:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
            }
        }
    }
};

int main()
{
    cout << "Hello\n";
    Menu m1;
    m1.showMenu();
}


