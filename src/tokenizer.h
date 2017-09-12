#ifndef CMD_TOKENIZER_H
#define CMD_TOKENIZER_H

#include <string>

namespace cmd
{
enum line_token { WANT_MORE, COMPLETE };

class tokenizer
{
public:
    tokenizer();
    // Get next line from buffer (up to size), line buffers to write_to.
    // tok is the token type (check if more data needs read)
    // Returns the address of where to continue looking for next line
    char *get_line(char *buffer, int size, std::string &write_to, line_token &tok);
};
};

#endif
