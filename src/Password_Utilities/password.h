#ifndef PASSWORD_H
#define PASSWORD_H

#include <QString>

namespace Pass
{
/**
 * @brief weak_password generates a simple passwword
 * @param len length of password min 8
 * @param num to include numbers or not
 * @param specialchar to include special characters or not
 * @return returns a Password
 */
QString weak_password(int len=8, bool num=true, bool specialchar=false);

/**
 * @brief strong_password generates a good complicated password
 * @param len length of password min 12
 * @param num whether to include numbers or not
 * @param specialchar whether to include special characters or not
 * @return
 */
QString strong_password(int len=12, bool num=true, bool specialchar=true);

/**
 * @brief very_strong_password generates a very strong pasword
 * @details Generates a very strong password necessarily includes both numbers and special characters
 * @param len length of password minimum 16
 * @return
 */
QString very_strong_password(int len=16);
}

#endif // PASSWORD_H
