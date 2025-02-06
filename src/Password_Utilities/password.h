#ifndef PASSWORD_H
#define PASSWORD_H

#include <QString>
#include <string_view>

namespace Pass
{
QString weak_password(int len=8, bool num=true, bool specialchar=false);
QString strong_password(int len=12, bool num=true, bool specialchar=true);
QString very_strong_password(int len=16);
}

#endif // PASSWORD_H
