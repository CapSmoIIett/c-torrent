#include "sha1.h"

/*
    1. входную строку в ascii
    2. ascii в бинарное значение
    3. дополнить до 8 бит в длинну
    4. добавить в конце 1
    5. заполнять массив нулями до момента
       пока его длинна % 512 != 448
    6. взять длинну массива из 3 шага и перевести его в бинарное состояние
    7. заполнять нулями до 64 символов
    8. добавить новое сообщение в конец сообщения из шага 5
    9. разбить сообщения на чанки по 512 символов
    10. разбить каждый чанк на подмассив из 16 32-битных слов
    11. циклом пройти через каждый чанк и расширить каждый чанк до 80 слов 
    12. проинициализировать некоторые переменнные
    13. пройти циклом через каждый чанк ...?
    14. каждое из 5 чисел перевести в hexdecimal
*/

static const size_t BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
static const size_t BLOCK_BYTES = BLOCK_INTS * 4;

const uint32_t SHA1::constants[5] =
{
    0x67452301,
    0xEFCDAB89,
    0x98BADCFE,
    0x10325476,
    0xC3D2E1F0
};

std::string SHA1::algo(std::string)
{
    std::string buffer;

    // Инициализируйте переменные:
    uint32_t digest[5];
    digest[0] = constants[0];
    digest[1] = constants[1];
    digest[2] = constants[2];
    digest[3] = constants[3];
    digest[4] = constants[4];

    
    while (true)
    {
        
    }

    return "";
}
