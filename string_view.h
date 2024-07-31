#ifndef _STRING_VIEW_H
#define _STRING_VIEW_H

#include <stdint.h>
#include <string.h>

// string_view_t doesn't own the pointer, it just points to memory in given allocated before string
struct string_view_t {
    char *pointer = NULL;
    size_t length = 0;
};

inline constexpr size_t        s_view_invalid_idx = ((size_t)-1);
inline constexpr string_view_t s_view_invalid     = string_view_t{ NULL, 0 };
inline constexpr bool operator == (const string_view_t l_view, const string_view_t r_view);

string_view_t string_view(char *pointer, size_t length);
string_view_t string_view(char *c_string);

string_view_t s_view_advance(string_view_t view, size_t n);
string_view_t s_view_truncate(string_view_t view, size_t n);
string_view_t s_view_substring(string_view_t view, size_t l_idx, size_t r_idx);
string_view_t s_view_trim(string_view_t view, char _c = ' ');

size_t        s_view_find_first(string_view_t view, const char *chars, size_t chars_num, size_t offset = 0);
size_t        s_view_find_last (string_view_t view, const char *chars, size_t chars_num, size_t offset = 0);
size_t        s_view_find_first(string_view_t view, char _c, size_t offset = 0);
size_t        s_view_find_last (string_view_t view, char _c, size_t offset = 0);
size_t        s_view_find_first_not(string_view_t view, const char *chars, size_t chars_num, size_t offset = 0);
size_t        s_view_find_last_not (string_view_t view, const char *chars, size_t chars_num, size_t offset = 0);
size_t        s_view_find_first_not(string_view_t view, char _c, size_t offset = 0);
size_t        s_view_find_last_not (string_view_t view, char _c, size_t offset = 0);

size_t        s_view_find(string_view_t view, const char *match);

bool          s_view_begins_with(string_view_t view, const char *match, string_view_t *update_view = NULL);
bool          s_view_ends_with  (string_view_t view, const char *match, string_view_t *update_view = NULL);

bool          s_view_get_line(string_view_t *line, string_view_t view, string_view_t *view_reminder);
string_view_t s_view_get_line(string_view_t *view_reminder);

#if defined(STRING_VIEW_STD_HELPERS)
#include <string>
std::string s_view_to_std_string(string_view_t view);
#endif

#if defined(STRING_VIEW_IMPLEMENTATION)

inline constexpr bool operator == (const string_view_t l_view, const string_view_t r_view) {
    return l_view.length == r_view.length && l_view.pointer == r_view.pointer;
}

#define _s_view_util_min_value(a, b) (((a) > (b)) ? (b) : (a))
#define _s_view_util_max_value(a, b) (((a) < (b)) ? (b) : (a))

#include <assert.h>

string_view_t string_view(char *pointer, size_t length) {
    assert(pointer && length >= 0);
    string_view_t s_view;
    s_view.pointer = pointer;
    s_view.length  = length;
    return s_view;
}

string_view_t string_view(char *c_string) {
    string_view_t s_view = string_view(c_string, strlen(c_string));
    return s_view;
}

string_view_t s_view_advance(string_view_t view, size_t n) {
    assert(n <= view.length); // @error Will advance past the view
    string_view_t s_view;
    s_view.pointer = view.pointer + n;
    s_view.length  = view.length  - n;
    return s_view;
}

string_view_t s_view_truncate(string_view_t view, size_t n) {
    assert(n <= view.length); // @error Will truncate past the beginning
    string_view_t s_view;
    s_view.pointer = view.pointer;
    s_view.length  = view.length - n;
    return s_view;
}

string_view_t s_view_substring(string_view_t view, size_t l_idx, size_t r_idx) {
    assert(r_idx < view.length && l_idx >= 0 && r_idx >= l_idx); // @error Wrong indexes specified
    string_view_t s_view = s_view_advance(view, l_idx);
    return s_view_truncate(s_view, s_view.length - 1 - (r_idx - l_idx));
}

string_view_t s_view_trim(string_view_t view, char _c) {
    size_t l_idx = s_view_find_first_not(view, _c);
    if(l_idx == s_view_invalid_idx) {
        return s_view_invalid; // Whole string is the character or the length is 0, so return invalid
    }

    size_t r_idx = s_view_find_last_not(view, _c);
    assert(r_idx != s_view_invalid_idx); // If l_idx was found, r_idx must be too

    return s_view_substring(view, l_idx, r_idx);
}

inline static bool _s_is_char_any_of(const char _char, const char *chars, const size_t chars_num) {
    for(int32_t idx = 0; idx < chars_num; ++idx) {
        if(_char == chars[idx]) {
            return true;
        }
    }
    return false;
}

size_t s_view_find_first(string_view_t view, const char *chars, size_t chars_num, size_t offset) {
    if(view.length == 0) {
        return s_view_invalid_idx;
    }

    assert(view.pointer);         // @error Length is greater than 0 so pointer should be valid
    assert(offset < view.length); // @error Offset must be betwen 0 and length - 1

    for(size_t idx = offset; idx < view.length; ++idx) {
        if(_s_is_char_any_of(*(view.pointer + idx), chars, chars_num)) {
            return idx;
        }
    }
    return s_view_invalid_idx;
}

size_t s_view_find_last(string_view_t view, const char *chars, size_t chars_num, size_t offset) {
    if(view.length == 0) {
        return s_view_invalid_idx;
    }

    assert(view.pointer);         // @error Length is greater than 0 so pointer should be valid
    assert(offset < view.length); // @error Offset must be betwen 0 and length - 1

    for(size_t idx = view.length - 1 - offset; idx > 0; --idx) {
        if(_s_is_char_any_of(*(view.pointer + idx), chars, chars_num)) {
            return idx;
        }
    }
    return s_view_invalid_idx;
}

size_t s_view_find_first(string_view_t view, char _char, size_t offset) {
    return s_view_find_first(view, &_char, 1, offset);
}

size_t s_view_find_last(string_view_t view, char _char, size_t offset) {
    return s_view_find_last(view, &_char, 1, offset);
}

size_t s_view_find_first_not(string_view_t view, const char *chars, size_t chars_num, size_t offset) {
    if(view.length == 0) {
        return s_view_invalid_idx;
    }

    assert(view.pointer);         // @error Length is greater than 0 so pointer should be valid
    assert(offset < view.length); // @error Offset must be betwen 0 and length - 1

    for(size_t idx = offset; idx < view.length; ++idx) {
        if(!_s_is_char_any_of(*(view.pointer + idx), chars, chars_num)) {
            return idx;
        }
    }
    return s_view_invalid_idx;
}

size_t s_view_find_last_not(string_view_t view, const char *chars, size_t chars_num, size_t offset) {
    if(view.length == 0) {
        return s_view_invalid_idx;
    }

    assert(view.pointer);         // @error Length is greater than 0 so pointer should be valid
    assert(offset < view.length); // @error Offset must be betwen 0 and length - 1

    for(size_t idx = view.length - 1 - offset; idx > 0; --idx) {
        if(!_s_is_char_any_of(*(view.pointer + idx), chars, chars_num)) {
            return idx;
        }
    }
    return s_view_invalid_idx;
}

size_t s_view_find_first_not(string_view_t view, char _c, size_t offset) {
    return s_view_find_first_not(view, &_c, 1, offset);
}

size_t s_view_find_last_not(string_view_t view, char _c, size_t offset) {
    return s_view_find_last_not(view, &_c, 1, offset);
}

size_t s_view_find(string_view_t view, const char *match) {
    const size_t match_length = strlen(match);
    if(view.length < match_length || !match_length) {
        return false;
    }

    // @todo @todo @todo @todo
    const size_t range = view.length - match_length;
    for(size_t idx = 0; idx < range; ++idx) {
        if(s_view_begins_with(view, match)) {
            return idx;
        } else {
            view = s_view_advance(view, 1);
        }
    }
    return s_view_invalid_idx;
}

bool s_view_begins_with(string_view_t view, const char *match, string_view_t *update_view) {
    if(!view.length) {
        return false;
    }

    const size_t match_length = strlen(match);
    if(!match_length || match_length > view.length) {
        return false;
    }

    assert(view.pointer); // @error Length is greater than 0 so pointer should be valid
    for(size_t idx = 0; idx < match_length; ++idx) {
        if(view.pointer[idx] != match[idx]) {
            return false;
        }
    }

    // If begins with 'match' -> maybe push the view
    if(update_view) {
        *update_view = s_view_advance(view, match_length);
    }

    return true;
}


bool s_view_ends_with(string_view_t view, const char *match, string_view_t *update_view) {
    if(!view.length) {
        return false;
    }

    const size_t match_length = strlen(match);
    if(!match_length || match_length > view.length) {
        return false;
    }

    assert(view.pointer); // @error Length is greater than 0 so pointer should be valid
    const char *match_end = match + match_length - 1;
    const char *view_end  = view.pointer + view.length - 1;

    for(size_t idx = 0; idx < match_length; ++idx) {
        const char c1 = *(match_end - idx);
        const char c2 = *(view_end  - idx);
        if(c1 != c2) {
            return false;
        }
    }

    // If begins with 'match' -> maybe truncate the view
    if(update_view) {
        *update_view = s_view_truncate(view, match_length);
    }

    return true;
}

// On false return the procedure doesn't update line or view_reminder!
bool s_view_get_line(string_view_t *line, string_view_t view, string_view_t *view_reminder) {
    // Push past '\n's
    size_t past_first_new_lines = s_view_find_first_not(view, '\n');

    // Only new lines are left or the view length is 0 -> no line & no reminder
    if(past_first_new_lines == s_view_invalid_idx) {
        return false;
    }

    assert(view.pointer); // @error The view length is not 0 so the pointer needs to be valid, IOC it's a bug
    view = s_view_substring(view, past_first_new_lines, view.length - 1);

    // Find next new line
    size_t first_next_new_line = s_view_find_first(view, '\n');

    // No new line found -> line is whole view & no reminder
    if(first_next_new_line == s_view_invalid_idx) {
        *line = view;
        *view_reminder = s_view_invalid;
        return true;
    }

    // New line found -> split the view
    // @note The view reminder might be only \n characters, the next call will immediately return false then
    *line          = s_view_substring(view, 0, first_next_new_line - 1);
    *view_reminder = s_view_substring(view, first_next_new_line, view.length - 1);
    return true;
}

string_view_t s_view_get_line(string_view_t *view_reminder) {
    string_view_t line_view;
    if(!s_view_get_line(&line_view, *view_reminder, view_reminder)) {
        line_view = s_view_invalid; // s_view_get_line doesn't set line view if returns false, so set it here for this overload
    }
    return line_view;
}

// @todo So should it return true with 0 length found if _c is the last character?
// @todo rewrite 
// @todo Maybe view_reminder should be required so no if checks
#if 0
bool s_view_split_until_first(string_view_t *found, string_view_t *view, char _c, bool or_end_of_view) {
    assert(view && found && _s_view_is_valid(view));

    // @todo
    // while(view->length > 0 && view->pointer[0] == _c) {
    //     *view = s_view_advance(*view, 1);
    // }

    if(view->length == 0) { return false; }


    size_t next_c_idx = s_view_find_first_of(*view, _c);

    // No char found in the string view so maybe return the whole view
    if(next_c_idx == s_view_invalid_idx) {
        if(or_end_of_view) {
            *found = string_view(*view);
            *view = s_view_end(*view);
            return true;
        }

        *found = s_view_end(*view);
        return false;
    }

    // @todo what if view->length - 1 == 0
    if(next_c_idx == 0) {
        *found = { };
        *view = s_view_advance(*view, 1);
        return true;
    }

    *found = s_view_substring(*view, 0, next_c_idx - 1);
    if(next_c_idx == view->length - 1) {
        *view = s_view_end(*view);
    } else {
        *view = s_view_substring(*view, next_c_idx + 1, view->length - 1);
    }
    return true;
}
#endif

bool s_view_split_until_first(string_view_t *found, string_view_t *view, char _c, bool or_end_of_view) {
    return false;
}

#if defined(STRING_VIEW_STD_HELPERS)
std::string s_view_to_std_string(string_view_t view) {
    std::string string;
    string.assign(view.pointer, view.length);
    return string;
}
#endif

#endif /* STRING_VIEW_IMPLEMENTATION */
#endif /* _STRING_VIEW_H */