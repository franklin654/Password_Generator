#include "password.h"
#include <QStringList>
#include <qdebug.h>
#include <random>
#include <QRandomGenerator>

std::string specialChars{"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"};
std::string_view LowerCaseAlphabet{"abcdefghijklmnopqrtsuvwxyz"};
std::string_view UpperCaseAlphabet{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
std::string_view numbers{"0123456789"};

QString Pass::weak_password(int len, bool num, bool specialchar) {
    if (len < 8)
        len = 10;
    std::vector<char>passwordarray;
    std::random_device rd;
    std::mt19937 pnrg(rd());
    std::uniform_int_distribution<>genchar(0, 25);
    std::uniform_int_distribution<>gennumber(0, 9);
    QRandomGenerator* differentLengths = QRandomGenerator::global();

    int num_len = differentLengths->bounded(1, 2+len-8+1);
    len -= num_len;

    if (specialchar)
    {
        passwordarray.push_back(specialChars[differentLengths->generate()%specialChars.length()]);
        len--;
    }

    for(int i=0;i<num_len;i++) {
        passwordarray.push_back(numbers[gennumber(pnrg)]);
    }
    for (int i=0;i<len;i++) {
        int index = genchar(pnrg);
        if (differentLengths->bounded(2))
            passwordarray.push_back(LowerCaseAlphabet[index]);
        else
            passwordarray.push_back(UpperCaseAlphabet[index]);
    }
    std::shuffle(passwordarray.begin(), passwordarray.end(), pnrg);
    std::string password(passwordarray.begin(), passwordarray.end());

    return QString(password.c_str());
}

QString Pass::strong_password(int len, bool num, bool specialchar) {
    if (len < 12)
        len = 14;
    std::vector<char>passwordarray;
    std::random_device rd;
    std::mt19937 pnrg(rd());
    std::uniform_int_distribution<>genchar(0, 25);
    std::uniform_int_distribution<>gennumber(0, 9);
    QRandomGenerator* differentLengths = QRandomGenerator::global();
    if(specialchar) {
        std::uniform_int_distribution<>genspecialchar(0, specialChars.length()-1);
        int special_len = differentLengths->bounded(1, 4);
        int num_len = 6-special_len;
        len -= (num_len+special_len);

        for (int i=0;i<special_len;i++)
            passwordarray.push_back(specialChars[genspecialchar(pnrg)]);

        for (int i=0;i<num_len;i++)
            passwordarray.push_back(numbers[gennumber(pnrg)]);

        for (int i=0;i<len;i++) {
            int index = genchar(pnrg);
            if (differentLengths->bounded(2))
                passwordarray.push_back(LowerCaseAlphabet[index]);
            else
                passwordarray.push_back(UpperCaseAlphabet[index]);
        }
    }
    else {
        int num_len = differentLengths->bounded(3, 7);
        len -= num_len;
        for (int i=0;i<num_len;i++)
            passwordarray.push_back(numbers[gennumber(pnrg)]);

        for (int i=0;i<len;i++) {
            int index = genchar(pnrg);
            if (differentLengths->bounded(2))
                passwordarray.push_back(LowerCaseAlphabet[index]);
            else
                passwordarray.push_back(UpperCaseAlphabet[index]);
        }
    }
    std::shuffle(passwordarray.begin(), passwordarray.end(), pnrg);
    std::string password(passwordarray.begin(), passwordarray.end());

    return QString(password.c_str());
}

QString Pass::very_strong_password(int len) {
    if (len < 16)
        len = 20;
    std::vector<char>passwordarray;
    std::random_device rd;
    std::mt19937 pnrg(rd());
    std::uniform_int_distribution<>genchar(0, 25);
    std::uniform_int_distribution<>gennumber(0, 9);
    QRandomGenerator* differentLengths = QRandomGenerator::global();
    std::uniform_int_distribution<>genspecialchar(0, specialChars.length()-1);
    int special_len = differentLengths->bounded(2, 5);
    int num_len = differentLengths->bounded(2, 5);
    len -= (num_len+special_len);
    for (int i=0;i<special_len;i++)
        passwordarray.push_back(specialChars[genspecialchar(pnrg)]);

    for (int i=0;i<num_len;i++)
        passwordarray.push_back(numbers[gennumber(pnrg)]);

    for (int i=0;i<len;i++) {
        int index = genchar(pnrg);
        if (differentLengths->bounded(2))
            passwordarray.push_back(LowerCaseAlphabet[index]);
        else
            passwordarray.push_back(UpperCaseAlphabet[index]);
    }

    std::shuffle(passwordarray.begin(), passwordarray.end(), pnrg);
    std::string password(passwordarray.begin(), passwordarray.end());

    return QString(password.c_str());
}
