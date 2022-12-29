#include <iostream>
#include <string>
#include <tuple>
#include <iomanip>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>

using Event = std::tuple<int, int, std::string>;

namespace std
{
    istream &operator>>(std::istream &is, Event &event)
    {
        is >> std::get<0>(event) >> std::get<1>(event);
        std::getline(is, std::get<2>(event));
        return is;
    };

    ostream &operator<<(std::ostream &os, Event const &event)
    {
        os << std::setfill('0')
           << std::setw(2) << std::get<0>(event)
           << " to "
           << std::setw(2) << std::get<1>(event)
           << ": "
           << std::get<2>(event);
        return os;
    }
}

/*
  Expected output:

07 to 08:  Eat breakfast
08 to 10:  Go to morning lecture
10 to 12:  Study
12 to 13:  Eat lunch
13 to 15:  Afternoon lecture
15 to 17:  Lab work
17 to 18:  Go Home
18 to 19:  Eat dinner
18 to 22:  Relax
22 to 07:  Sleep

 */

int main()
{
    // Events are sorted with respect to start hour. If two start
    // hours are equal then they are sorted by end hour in second
    // hand. If both start and end hour are equal then they are sorted
    // by description.

    // 1. Read all events from "first.txt" into a vector of Event
    //    called first_schedule.
    std::ifstream fs{"first.txt"};
    std::vector<Event> first_schedule(std::istream_iterator<Event>{fs}, std::istream_iterator<Event>{});

    // 2. Sort first_schedule.
    std::sort(begin(first_schedule), end(first_schedule));

    // 3. Read all events from "second.txt" into a vector of Event
    //    called second_schedule.
    std::ifstream fs_2{"second.txt"};
    std::vector<Event> second_schedule(std::istream_iterator<Event>{fs_2}, std::istream_iterator<Event>{});

    // 4. Sort second_schedule.
    std::sort(std::begin(second_schedule), std::end(second_schedule));

    // 5. Create a new vector of Event called combined_schedule.
    std::vector<Event> combined_schedule;

    // 6. Copy first_schedule and second_schedule into
    //    combined_schedule in such a way that the events are sorted.
    std::merge(std::begin(first_schedule), std::end(first_schedule),
               std::begin(second_schedule), std::end(second_schedule),
               std::back_inserter(combined_schedule));

    // 7. Remove any duplicates from combined_schedule.
    auto last = std::unique(std::begin(combined_schedule), std::end(combined_schedule));
    combined_schedule.erase(last, std::end(combined_schedule));

    // 8. Print each event in combined_schedule, one line per event.
    std::copy(std::begin(combined_schedule), std::end(combined_schedule), std::ostream_iterator<Event>(std::cout, "\n"));
}