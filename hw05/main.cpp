#include "supermarket.cpp"

int main(void) {
    CSupermarket s;
    s.store("bread", CDate(2016, 4, 30), 100)
        .store("butter", CDate(2016, 5, 10), 10)
        .store("beer", CDate(2016, 8, 10), 50)
        .store("bread", CDate(2016, 4, 25), 100)
        .store("okey", CDate(2016, 7, 18), 5);

    std::list<std::pair<std::string, int> > l0 = s.expired(CDate(2018, 4, 30));
    assert(l0.size() == 4);
    assert((l0 == std::list<std::pair<std::string, int> >{{"bread", 200}, {"beer", 50}, {"butter", 10}, {"okey", 5}}));

    std::list<std::pair<std::string, int> > l1{{"bread", 2}, {"Coke", 5}, {"butter", 20}};
    s.sell(l1);
    assert(l1.size() == 2);
    assert((l1 == std::list<std::pair<std::string, int> >{{"Coke", 5}, {"butter", 10}}));

    std::list<std::pair<std::string, int> > l2 = s.expired(CDate(2016, 4, 30));
    assert(l2.size() == 1);
    assert((l2 == std::list<std::pair<std::string, int> >{{"bread", 98}}));

    std::list<std::pair<std::string, int> > l3 = s.expired(CDate(2016, 5, 20));
    assert(l3.size() == 1);
    assert((l3 == std::list<std::pair<std::string, int> >{{"bread", 198}}));

    std::list<std::pair<std::string, int> > l4{{"bread", 105}};
    s.sell(l4);
    assert(l4.size() == 0);
    assert((l4 == std::list<std::pair<std::string, int> >{}));

    std::list<std::pair<std::string, int> > l5 = s.expired(CDate(2017, 1, 1));
    assert(l5.size() == 3);
    assert((l5 == std::list<std::pair<std::string, int> >{{"bread", 93}, {"beer", 50}, {"okey", 5}}));

    s.store("Coke", CDate(2016, 12, 31), 10);

    std::list<std::pair<std::string, int> > l6{{"Cake", 1}, {"Coke", 1}, {"cake", 1}, {"coke", 1}, {"cuke", 1}, {"Cokes", 1}};
    s.sell(l6);
    assert(l6.size() == 3);
    assert((l6 == std::list<std::pair<std::string, int> >{{"cake", 1}, {"cuke", 1}, {"Cokes", 1}}));

    std::list<std::pair<std::string, int> > l7 = s.expired(CDate(2017, 1, 1));
    assert(l7.size() == 4);
    assert((l7 == std::list<std::pair<std::string, int> >{{"bread", 93}, {"beer", 50}, {"Coke", 7}, {"okey", 5}}));

    s.store("cake", CDate(2016, 11, 1), 5);

    std::list<std::pair<std::string, int> > l8{{"Cake", 1}, {"Coke", 1}, {"cake", 1}, {"coke", 1}, {"cuke", 1}};
    s.sell(l8);
    assert(l8.size() == 2);
    assert((l8 == std::list<std::pair<std::string, int> >{{"Cake", 1}, {"coke", 1}}));

    std::list<std::pair<std::string, int> > l9 = s.expired(CDate(2017, 1, 1));
    assert(l9.size() == 5);
    assert((l9 == std::list<std::pair<std::string, int> >{{"bread", 93}, {"beer", 50}, {"Coke", 6}, {"okey", 5}, {"cake", 3}}));

    std::list<std::pair<std::string, int> > l10{{"cake", 15}, {"Cake", 2}};
    s.sell(l10);
    assert(l10.size() == 2);
    assert((l10 == std::list<std::pair<std::string, int> >{{"cake", 12}, {"Cake", 2}}));

    std::list<std::pair<std::string, int> > l11 = s.expired(CDate(2017, 1, 1));
    assert(l11.size() == 4);
    assert((l11 == std::list<std::pair<std::string, int> >{{"bread", 93}, {"beer", 50}, {"Coke", 6}, {"okey", 5}}));

    std::list<std::pair<std::string, int> > l12{{"Cake", 4}};
    s.sell(l12);
    assert(l12.size() == 0);
    assert((l12 == std::list<std::pair<std::string, int> >{}));

    std::list<std::pair<std::string, int> > l13 = s.expired(CDate(2017, 1, 1));
    assert(l13.size() == 4);
    assert((l13 == std::list<std::pair<std::string, int> >{{"bread", 93}, {"beer", 50}, {"okey", 5}, {"Coke", 2}}));

    std::list<std::pair<std::string, int> > l14{{"Beer", 20}, {"Coke", 1}, {"bear", 25}, {"beer", 10}};
    s.sell(l14);
    assert(l14.size() == 1);
    assert((l14 == std::list<std::pair<std::string, int> >{{"beer", 5}}));

    s.store("ccccb", CDate(2019, 3, 11), 100)
        .store("ccccd", CDate(2019, 6, 9), 100)
        .store("dcccc", CDate(2019, 2, 14), 100);

    std::list<std::pair<std::string, int> > l15{{"ccccc", 10}};
    s.sell(l15);
    assert(l15.size() == 1);
    assert((l15 == std::list<std::pair<std::string, int> >{{"ccccc", 10}}));

    std::cout << "ok :)" << std::endl;
    return EXIT_SUCCESS;
}
