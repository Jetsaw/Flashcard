#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
using namespace std;
const int maxnum = 50;
class Card
{
private:
    string question;
    string answer;
public:
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
    string getQuestion()
    {
        return question;
    }
    string getAnswer()
    {
        return answer;
    }
};
class Feedback
{
private:
    int marks;
public:
    Feedback()
    {
        marks = 0;
    }
    Feedback(int m)
    {
        marks = m;
    }
    int getMarks()
    {
        return marks;
    }
};
class CardsManager
{
private:
    Card cards[maxnum];
    Feedback feedbacks[maxnum];
    int num;
    int totalpoint[maxnum] = {0};
public:
    CardsManager()
    {
        num = 0;
    }
    void setNewCard(string ques, string ans)
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
            // Create an array of indexes
            int indexOrder[maxnum];
            for (int i = 0; i < num; ++i)
            {
                indexOrder[i] = i;
            }
            // Sort indexes based on totalpoint values (ascending)
            for (int i = 0; i < num - 1; ++i)
            {
                for (int j = i + 1; j < num; ++j)
                {
                    if (totalpoint[indexOrder[i]] > totalpoint[indexOrder[j]])
                    {
                        swap(indexOrder[i], indexOrder[j]);
                    }
                }
            }
            cout << "\nDisplaying flashcards..." << endl;
            cout << "\nTotal Cards = " << num << endl;
            cout << endl;
            for(int k=0; k<num; ++k)
            {
                int i = indexOrder[k];
                cout << "Question " << i+1 << ": " << cards[i].getQuestion() << endl;
                cout << "Press Enter to see the answer...";
                string dummy;
                getline(cin, dummy);
                cout << "Answer " << i+1 << ": " << cards[i].getAnswer() << endl;
                cout << "\nHow well did you know this? (0: Not well, 1: Normal, 2: Perfectly)" << endl;
                cin >> point;
                while(point < 0 || point >2)
                {
                    cout << "Error. Please try again." << endl;
                    cin >> point;
                    if(point == 0 || point == 1 || point == 2 )
                        break;
                }
                cin.ignore();
                feedbacks[i] = Feedback(point);
                totalpoint[i] = totalpoint[i] + feedbacks[i].getMarks();
                cout << endl;
            }
        }
    }
    void reviewCard()
    {
        if (num == 0)
        {
            cout << "No cards to review." << endl;
            return;
        }
        else
        {
            for(int i=0; i<num; i++)
            {
                    cout << "\nQuestion " << i+1 << ": " << cards[i].getQuestion() << endl;
                    cout << "Current marks: "<< totalpoint[i] << endl;
            }
        }
    }
    void editCard()
    {
        if (num == 0)
        {
            cout << "No cards to edit." << endl;
            return;
        }
        for (int i = 0; i < num; ++i)
        {
            cout << "Question " << i+1 << ": " << cards[i].getQuestion() << endl;
        }
        int index1;
        string edit_q, edit_a;
        cout << "\nEnter card number to edit: ";
        cin >> index1;
        cin.ignore();
        while (index1 < 1 || index1 > num)
        {
            cout << "Invalid index!!" << endl;
            if (index1 >= 1 && index1 <= num)
                break;
        }
        cout << "\nCurrent Question: " << cards[index1-1].getQuestion() << endl;
        cout << "Enter new question (leave it empty to keep): ";
        getline(cin, edit_q);
        if (edit_q.empty())
        {
            edit_q = cards[index1-1].getQuestion();
        }
        cout << "\nCurrent Answer: " << cards[index1-1].getAnswer() << endl;
        cout << "Enter new answer (leave it empty to keep): ";
        getline(cin, edit_a);
        if (edit_a.empty())
        {
            edit_a = cards[index1-1].getAnswer();
        }
        cards[index1-1] = Card(edit_q, edit_a);
        totalpoint[index1-1] = 0;
    }
    void deleteCard()
    {
        if (num == 0)
        {
            cout << "No cards to delete." << endl;
            return;
        }
        for (int i = 0; i < num; ++i)
        {
            cout << "Question " << i+1 << ": " << cards[i].getQuestion() << endl;
        }
        int index2;
        cout << "\nEnter card number to delete: ";
        cin >> index2;
        cin.ignore();
        while (index2 < 1 || index2 > num)
        {
            cout << "Invalid index!!" << endl;
            if (index2 >= 1 && index2 <= num)
                break;
        }
        for (int i = index2-1; i < num; ++i)
        {
            cards[i] = cards[i+1];
            feedbacks[i] = feedbacks[i+1];
            totalpoint[i] = totalpoint[i+1];
        }
        num--;
    }
};
class Process
{
private:
    CardsManager cm;
    string q;
    string a;
public:
    void addNewCard()
    {
        cout << "\nEnter Question: ";
        getline(cin, q);
        cout << "Enter Answer: ";
        getline(cin, a);
        cm.setNewCard(q, a);
    }
    void showCard()
    {
        cm.displayCards();
    }
    void showReview()
    {
        cm.reviewCard();
    }
    void editFlashCard()
    {
        cm.editCard();
        cout << "\nCard updated!" << endl;
    }
    void deleteFlashCard()
    {
        cm.deleteCard();
        cout << "\nCard deleted successfully!" << endl;
    }
};
class Menu
{
    int choice;
    Process p;
    string filename;

public:
    Menu(const string &file = "cards_data.dat") : filename(file) {}
    void saveData()
    {
        ofstream wf(filename, ios::out | ios::binary);
        if (!wf)
        {
            cout << "Cannot open file for writing!" << endl;
            return;
        }

        wf.write((char *)&p, sizeof(Process));
        wf.close();
        cout << "\nSaving all cards data into file... " << endl;
        cout << "Done saving ! " << endl;
    }
    void loadData()
    {

        ifstream rf(filename, ios::in | ios::binary);
        if (!rf)
        {
            cout << "Cannot open file for reading!" << endl;
            return;
        }

        rf.read((char *)&p, sizeof(Process));
        rf.close();
        cout << "\nLoading all cards data from file... " << endl;
        cout << "Done loading ! " << endl;
    }
    void showMenu()
    {
        int choice = 0;
        while(choice != 8)
        {
            cout << "\n===== Flash Card =====" << endl;
            cout << "1. Add new card" << endl;
            cout << "2. Display card" << endl;
            cout << "3. Save cards to file" << endl;
            cout << "4. Load cards from file" << endl;
            cout << "5. Review marks" << endl;
            cout << "6. Edit card" << endl;
            cout << "7. Delete card" << endl;
            cout << "8. Exit" << endl;
            cout << "\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            system("CLS");
            switch(choice)
            {
            case 1:
                p.addNewCard();
                break;
            case 2:
                p.showCard();
                break;
            case 3:
                saveData();
                break;
            case 4:
                loadData();
                break;
            case 5:
                p.showReview();
                break;
            case 6:
                p.editFlashCard();
                break;
            case 7:
                p.deleteFlashCard();
                break;
            case 8:
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
    Menu m1;
    m1.showMenu();
}
