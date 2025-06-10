#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

// FlashCard class - holds individual Q&A and stats
class FlashCard {
public:
    string question;
    string answer;
    int difficultyScore;

    FlashCard() : difficultyScore(0) {}
    FlashCard(string q, string a) : question(q), answer(a), difficultyScore(0) {}

    void show() const {
        cout << "Q: " << question << endl;
    }

    void showAnswer() const {
        cout << "A: " << answer << endl;
    }
};

// Deck manager
class FlashCardDeck {
private:
    vector<FlashCard> cards;

public:
    void addCard(const FlashCard& card) {
        cards.push_back(card);
    }

    vector<FlashCard>& getCards() {
        return cards;
    }

    const vector<FlashCard>& getCards() const {
        return cards;
    }

    void listCards() {
        if (cards.empty()) {
            cout << "No cards available." << endl;
            return;
        }
        cout << "Total flashcards: " << cards.size() << endl;
        for (size_t i = 0; i < cards.size(); ++i) {
            cout << i + 1 << ". " << cards[i].question << endl;
        }
    }
};

// File manager class
class FileManager {
public:
    static void save(const FlashCardDeck& deck, const string& filename) {
        ofstream out(filename, ios::binary);
        if (!out) {
            cout << "Error saving file!" << endl;
            return;
        }

        const vector<FlashCard>& cards = deck.getCards();
        size_t size = cards.size();
        out.write((char*)&size, sizeof(size));
        for (const auto& card : cards) {
            size_t qlen = card.question.size();
            size_t alen = card.answer.size();
            out.write((char*)&qlen, sizeof(qlen));
            out.write(card.question.c_str(), qlen);
            out.write((char*)&alen, sizeof(alen));
            out.write(card.answer.c_str(), alen);
            out.write((char*)&card.difficultyScore, sizeof(int));
        }

        out.close();
        cout << "Cards saved successfully!" << endl;
    }

    static void load(FlashCardDeck& deck, const string& filename) {
        ifstream in(filename, ios::binary);
        if (!in) {
            cout << "File not found." << endl;
            return;
        }

        size_t size;
        in.read((char*)&size, sizeof(size));
        for (size_t i = 0; i < size; ++i) {
            size_t qlen, alen;
            in.read((char*)&qlen, sizeof(qlen));
            string q(qlen, ' ');
            in.read(&q[0], qlen);

            in.read((char*)&alen, sizeof(alen));
            string a(alen, ' ');
            in.read(&a[0], alen);

            int diff;
            in.read((char*)&diff, sizeof(int));

            FlashCard card(q, a);
            card.difficultyScore = diff;
            deck.addCard(card);
        }

        in.close();
        cout << "Cards loaded successfully!" << endl;
    }
};

// Review engine with spaced repetition
class SpacedRepetitionEngine {
public:
    static void reviewCards(FlashCardDeck& deck) {
        vector<FlashCard>& cards = deck.getCards();
        if (cards.empty()) {
            cout << "No cards to review!" << endl;
            return;
        }

        srand(time(nullptr));
        random_shuffle(cards.begin(), cards.end());

        sort(cards.begin(), cards.end(), [](FlashCard& a, FlashCard& b) {
            return a.difficultyScore < b.difficultyScore;
        });

        for (size_t i = 0; i < cards.size(); ++i) {
            cout << "\nCard " << i + 1 << " of " << cards.size() << endl;
            cout << "(Current Mark: " << cards[i].difficultyScore << ")" << endl;  // 👈 Show mark
            cards[i].show();
            cout << "Press Enter to see the answer...";
            string dummy;
            getline(cin, dummy);
            cards[i].showAnswer();

            cout << "Did you remember this card? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 'n' || choice == 'N') {
                cards[i].difficultyScore = max(cards[i].difficultyScore - 1, -10);
            } else {
                cards[i].difficultyScore = min(cards[i].difficultyScore + 1, 10);
            }
        }
    }
};

// User interaction handler
class FlashCardApp {
private:
    FlashCardDeck deck;
    const string filename = "flashcards.dat";

public:
    void run() {
        int choice;
        do {
            showMenu();
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    addFlashCard();
                    break;
                case 2:
                    deck.listCards();
                    break;
                case 3:
                    FileManager::save(deck, filename);
                    break;
                case 4:
                    FileManager::load(deck, filename);
                    break;
                case 5:
                    SpacedRepetitionEngine::reviewCards(deck);
                    break;
                case 6:
                    deleteFlashCard();
                    break;
                case 7:
                    editFlashCard();
                    break;
                case 8:
                    cout << "Exiting program." << endl;
                    break;
                default:
                    cout << "Invalid option! Try again.\n";
            }
        } while (choice != 8);
    }

    void showMenu() {
        cout << "\n=== Digital Flash Card App ===\n";
        cout << "1. Add New Flashcard\n";
        cout << "2. List All Flashcards\n";
        cout << "3. Save Flashcards to File\n";
        cout << "4. Load Flashcards from File\n";
        cout << "5. Review Flashcards\n";
        cout << "6. Delete Flashcard by Index\n";
        cout << "7. Edit Flashcard\n";
        cout << "8. Exit\n";
        cout << "Choose an option: ";
    }

    void addFlashCard() {
        string q, a;
        cout << "Enter Question: ";
        getline(cin, q);
        cout << "Enter Answer: ";
        getline(cin, a);

        deck.addCard(FlashCard(q, a));
        cout << "Flashcard added!" << endl;
    }

    void deleteFlashCard() {
        deck.listCards();
        if (deck.getCards().empty()) return;

        int index;
        cout << "Enter card number to delete: ";
        cin >> index;
        cin.ignore();

        if (index < 1 || index > deck.getCards().size()) {
            cout << "Invalid index!" << endl;
            return;
        }

        deck.getCards().erase(deck.getCards().begin() + index - 1);
        cout << "Card deleted." << endl;
    }

    void editFlashCard() {
        deck.listCards();
        if (deck.getCards().empty()) return;

        int index;
        cout << "Enter card number to edit: ";
        cin >> index;
        cin.ignore();

        if (index < 1 || index > deck.getCards().size()) {
            cout << "Invalid index!" << endl;
            return;
        }

        FlashCard& card = deck.getCards()[index - 1];
        cout << "Current Question: " << card.question << "\nEnter new question (leave empty to keep): ";
        string q;
        getline(cin, q);
        if (!q.empty()) card.question = q;

        cout << "Current Answer: " << card.answer << "\nEnter new answer (leave empty to keep): ";
        string a;
        getline(cin, a);
        if (!a.empty()) card.answer = a;

        cout << "Card updated." << endl;
    }
};

// Entry point
int main() {
    FlashCardApp app;
    app.run();
    return 0;
}
