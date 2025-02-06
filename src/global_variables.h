#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <QRegularExpression>
#include <QDir>
#include <QFile>
#include <filesystem>

Q_GLOBAL_STATIC(QRegularExpression, re)
Q_GLOBAL_STATIC(QDir, passwordDir, std::filesystem::path(QDir::homePath().toStdString())/"pmstorage")
Q_GLOBAL_STATIC(QFile, hashFile, passwordDir->filesystemAbsolutePath()/"key")

class Singleton{

private:
    static Singleton* instancePtr;

    Singleton();

public:
    std::vector<unsigned char>keyHash;

    Singleton(const Singleton&obj) = delete;

    static Singleton* getInstance();

    void getKeyHash();
    void setKeyHash();

};

#endif // GLOBAL_VARIABLES_H
