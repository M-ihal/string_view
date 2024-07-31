#define STRING_VIEW_IMPLEMENTATION
#define STRING_VIEW_STD_HELPERS
#include "string_view.h"

#include <stdio.h>

int main(int argc, char **argv) {
    printf(" - s_view_find_first_of, s_view_substring\n");
    string_view_t a = string_view(", World!   ");
    string_view_t b = s_view_invalid;
    a = s_view_trim(a);
    size_t comma_idx = s_view_find_first(a, ',');
    if(comma_idx != s_view_invalid_idx) {
        b = comma_idx + 1 < a.length ? s_view_substring(a, comma_idx + 1, a.length - 1) : s_view_invalid;
        a = comma_idx > 0 ? s_view_substring(a, 0, comma_idx - 1) : s_view_invalid;
    }

    printf(" - s_view_get_line\n");
    string_view_t c = string_view("\n\n\n\n\nSailor!\nHello,\n World\n\n\n\n\nSlashNs\n\n\n\n\n\n _\n\n\n\n ");
    // string_view_t c = string_view("\n\n\n\n\n");
    string_view_t c_line;
    while(s_view_get_line(&c_line, c, &c)) {
    }

    string_view_t d = string_view("Blah_blah.aff");
    bool d_1 = s_view_begins_with(d, "Blah", &d);
    bool d_2 = s_view_ends_with(d, ".aff", &d);

    string_view_t e = string_view("####@@@@@@#@!%$#%#$@#$@#%#$%#####@@@######");
    // string_view_t e = string_view("##");
    e = s_view_trim(e, '@');
    e = s_view_trim(e, '#');
    e = s_view_trim(e, '@');

    string_view_t f = string_view("");
    bool f_1 = s_view_begins_with(f, "Blah");
    bool f_2 = s_view_ends_with(f, ".aff");
    size_t f_3 = s_view_find_first(f, "avc", 3);
    size_t f_4 = s_view_find_last(f, ".kks ", 5);
    string_view_t f_5 = s_view_get_line(&f);

    string_view_t g = string_view("BLAJ fdofksfk aLSDLSALDL \n\nfdsfp oskdfskd Test.test gggggg\n\n\n\n 4444\n     \t");
    string_view_t g_token = string_view("Test.test");
    size_t g_1 = s_view_find(g, g_token.pointer);
    if(g_1 != s_view_invalid_idx) {
        string_view_t g_1_1 = s_view_substring(g, 0, g_1);
        string_view_t g_1_2 = s_view_substring(g, g_1 + g_token.length - 1, g.length - 1);
        string_view_t g_1_3 = s_view_substring(g, g_1, g_1 + g_token.length - 1);
        int32_t _ = 44;
    }

    return 0;
}