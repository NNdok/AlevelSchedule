#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

struct OffPeriod {
    int day;
    int month;
    vector<pair<int, int> > hours; // Pairs of start and end hours for off periods
};

bool isOff(int currentDay, int currentMonth, int hour, const vector<OffPeriod>& offPeriods) {
    for (const auto& op : offPeriods) {
        if (op.day == currentDay && op.month == currentMonth) {
            for (const auto& range : op.hours) {
                if (hour >= range.first && hour < range.second) {
                    return true;
                }
            }
        }
    }
    return false;
}

int main() {
    int m[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int M[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int math, phy, bio, chem, eng, geo, thai;
    int day, startday, month;
    int x, y, z, startHour, endHour;

    vector<OffPeriod> offPeriods;
    vector<pair<string, int*> > subjects;
    subjects.push_back(make_pair("Math", &math));
    subjects.push_back(make_pair("Physics", &phy));
    subjects.push_back(make_pair("Biology", &bio));
    subjects.push_back(make_pair("Chemistry", &chem));
    subjects.push_back(make_pair("English", &eng));
    subjects.push_back(make_pair("Geography", &geo));
    subjects.push_back(make_pair("Thai", &thai));

    cout << "Input remaining hours for Math, Physics, Biology, Chemistry, English, Geography, and Thai:" << endl;
    cin >> math >> phy >> bio >> chem >> eng >> geo >> thai;

    cout << "Input starting day & number of days:" << endl;
    cin >> startday >> month >> day;

    cout << "Input number of off periods:" << endl;
    cin >> z;

    if (z != 0) {
        for (int i = 0; i < z; i++) {
            OffPeriod op;
            cout << "Input day, month, and number of off-hour ranges: ";
            cin >> op.day >> op.month;
            int numRanges;
            cin >> numRanges;

            for (int j = 0; j < numRanges; j++) {
                cout << "Input start and end hours (24-hour format): ";
                cin >> startHour >> endHour;
                op.hours.push_back(make_pair(startHour, endHour));
            }
            offPeriods.push_back(op);
        }
    }

    // Round-robin index for subjects
    int subjectIndex = 0;

    while (day > 0) {
        for (int j = startday; j <= m[month - 1] && day > 0; j++) {
            cout << j << "/" << M[month - 1] << endl;

            // Schedule subjects for each time slot
            vector<pair<string, pair<int, int> > > slots;
            slots.push_back(make_pair("08:00-10:00", make_pair(8, 10)));
            slots.push_back(make_pair("10:00-12:00", make_pair(10, 12)));
            slots.push_back(make_pair("14:00-17:00", make_pair(14, 17)));
            slots.push_back(make_pair("20:00-22:00", make_pair(20, 22)));

            for (const auto& slot : slots) {
                // Check if the current slot overlaps with an off period
                bool slotIsOff = false;
                for (int hour = slot.second.first; hour < slot.second.second; hour++) {
                    if (isOff(j, month, hour, offPeriods)) {
                        slotIsOff = true;
                        break;
                    }
                }

                if (slotIsOff) {
                    cout << slot.first << ": Not Available" << endl;
                }

                // Schedule a subject in a round-robin manner
                bool subjectScheduled = false;
                int initialIndex = subjectIndex; // To prevent infinite looping if no subjects are available

                do {
                    auto& subject = subjects[subjectIndex];
                    if (*(subject.second) > 0) { // Check if the subject has remaining hours
                        cout << slot.first << ": " << subject.first << endl;
                        int l=slot.second.second-slot.second.first;
                        (*(subject.second))-=l; // Decrement remaining hours for the subject
                        subjectScheduled = true;
                        break;
                    }
                    // Move to the next subject
                    subjectIndex = (subjectIndex + 1) % subjects.size();
                } while (subjectIndex != initialIndex);

                if (!subjectScheduled) {
                    cout << slot.first << ": Free" << endl; // No subject to schedule
                }

                // Move to the next subject in the round-robin order
                subjectIndex = (subjectIndex + 1) % subjects.size();
            }

            cout << endl;
            day--; // Decrement remaining days
        }

        // Move to the next month
        startday = 1; // Reset day to 1 for the next month
        month++;
        if (month > 12) month = 1; // Wrap around to January if month exceeds 12
    }

    cout << "Remaining hours for subjects:" << endl;
    for (const auto& subject : subjects) {
        cout << subject.first << ": " << *(subject.second) << endl;
    }

    return 0;
}
