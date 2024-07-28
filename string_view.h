#ifndef _STRING_VIEW_H
#define _STRING_VIEW_H

#include <stdint.h>

// @todo Decide if length should be signed int
// @todo Check all the places where length gets subtracted to make sure there isnt wrapping to size_t max !!!


#define s_view_invalid_idx ((size_t)-1)

// string_view_t doesn't own the pointer, it just points to memory in given allocated before string
struct string_view_t {
    char *pointer = NULL;
    size_t length = 0;
};

string_view_t string_view(char *pointer, size_t length);
string_view_t string_view(string_view_t view, size_t length = s_view_invalid_idx);
string_view_t string_view(char *c_string); // Must be null terminated
string_view_t s_view_end(string_view_t view);
string_view_t s_view_advance(string_view_t view, size_t n);
string_view_t s_view_truncate(string_view_t view, size_t n); // @todo Better name
string_view_t s_view_substring(string_view_t view, size_t l_idx, size_t r_idx);
string_view_t s_view_trim(string_view_t view);
size_t        s_view_find_first_of(string_view_t view, char _char, size_t offset = 0 /*do for others*/);  // @todo Better name
size_t        s_view_find_last_of(string_view_t view, char _char);   // @todo Better name
size_t        s_view_find_first_not_of(string_view_t view, char _char);  // @todo Better name
size_t        s_view_find_last_not_of(string_view_t view, char _char);   // @todo Better name
bool          s_view_begins_with(string_view_t view, const char *match);
bool          s_view_ends_with(string_view_t view, const char *match);
bool          s_view_get_line(string_view_t *line, string_view_t view, string_view_t *view_reminder);

// @todo Dont like string dep
#if defined(STRING_VIEW_STD_STRING_SUPPORT)
#include <string>
std::string   s_view_to_std_string(string_view_t view);
#endif

#ifdef STRING_VIEW_IMPLEMENTATION

#define _s_view_util_min_value(a, b) (((a) > (b)) ? (b) : (a))
#define _s_view_util_max_value(a, b) (((a) < (b)) ? (b) : (a))

#include <assert.h>
inline bool _s_view_is_valid(string_view_t *s_view) { return s_view->pointer && s_view->length >= 0 && s_view->length != s_view_invalid_idx; }

#if defined(STRING_VIEW_STD_STRING_SUPPORT)
std::string s_view_to_std_string(string_view_t view) {
    std::string string;
    string.assign(view.pointer, view.length);
    return string;
}
#endif

string_view_t string_view(char *pointer, size_t length) {
    assert(pointer && length >= 0);

    string_view_t s_view;
    s_view.pointer = pointer;
    s_view.length = length;
    return s_view;
}

string_view_t string_view(string_view_t view, size_t length) {
    assert(_s_view_is_valid(&view) && (length == s_view_invalid_idx || (length != s_view_invalid_idx && length >= 0 && length <= view.length)));

    string_view_t s_view;
    s_view.pointer = view.pointer;
    s_view.length  = length == s_view_invalid_idx ? view.length : length;
    return s_view;
}

string_view_t string_view(char *c_string) {
    assert(c_string);

    string_view_t s_view = string_view(c_string, strlen(c_string));
    assert(_s_view_is_valid(&s_view));
    return s_view;
}

// Sets to one byte past the last character
string_view_t s_view_end(string_view_t view) {
    assert(_s_view_is_valid(&view));
    string_view_t s_view = string_view(view.pointer + view.length, 0);
    assert(s_view.pointer == view.pointer + view.length && s_view.length == 0);
    return s_view;
}

string_view_t s_view_advance(string_view_t view, size_t n) {
    assert(_s_view_is_valid(&view));

    // If n > view.length -> advance to the end of the string and not further
    n = _s_view_util_min_value(n, view.length);
    assert(n <= view.length);

    string_view_t s_view;
    s_view.pointer = view.pointer + n;
    s_view.length  = view.length - n;
    return s_view;
}

string_view_t s_view_truncate(string_view_t view, size_t n) {
    assert(_s_view_is_valid(&view));

    // If n > view.length -> advance to the beginning of the string and not further
    n = _s_view_util_min_value(n, view.length);
    assert(n <= view.length);

    string_view_t s_view;
    s_view.pointer = view.pointer;
    s_view.length  = view.length - n;
    return s_view;
}

string_view_t s_view_substring(string_view_t view, size_t l_idx, size_t r_idx) {
    assert(_s_view_is_valid(&view) && l_idx >= 0 && r_idx < view.length && r_idx >= l_idx);

    string_view_t s_view = s_view_advance(view, l_idx);
    return s_view_truncate(s_view, s_view.length - 1 - (r_idx - l_idx)); // @check @error MAKE SURE IT DOES NOT WRAP TO SIZE_T MAX
}

// @todo For now only spaces ' '
string_view_t s_view_trim(string_view_t view) {
    assert(_s_view_is_valid(&view));

    size_t l_idx = s_view_find_first_not_of(view, ' ');

    // Whole string is ' ' or the length is 0;
    if(l_idx == s_view_invalid_idx) {
        return s_view_end(view);
    }

    size_t r_idx = s_view_find_last_not_of(view, ' ');
    assert(r_idx != s_view_invalid_idx);

    return s_view_substring(view, l_idx, r_idx);
}

size_t s_view_find_first_of(string_view_t view, char _char, size_t offset) {
    assert(_s_view_is_valid(&view));

    if(view.length <= 0) return s_view_invalid_idx;

    // @todo
    for(size_t idx = offset; idx < view.length; ++idx) {
        char _c = *(view.pointer + idx);
        if(_c == _char) {
            return idx;
        }
    }
    return s_view_invalid_idx;
}

size_t s_view_find_last_of(string_view_t view, char _char) {
    assert(_s_view_is_valid(&view));

    if(view.length <= 0) return s_view_invalid_idx;

    for(size_t idx = view.length - 1; idx >= 0; --idx) {
        char _c = *(view.pointer + idx);
        if(_c == _char) {
            return idx;
        }
    }
    return s_view_invalid_idx;
}

size_t s_view_find_first_not_of(string_view_t view, char _char) {
    assert(_s_view_is_valid(&view));

    if(view.length <= 0) return s_view_invalid_idx;

    // @todo
    for(size_t idx = 0; idx < view.length; ++idx) {
        char _c = *(view.pointer + idx);
        if(_c != _char) {
            return idx;
        }
    }
    return s_view_invalid_idx;
}

size_t s_view_find_last_not_of(string_view_t view, char _char) {
    assert(_s_view_is_valid(&view));

    if(view.length == 0) return s_view_invalid_idx;

    // @todo
    for(size_t idx = view.length - 1; idx >= 0; --idx) {
        char _c = *(view.pointer + idx);
        if(_c != _char) {
            return idx;
        }
    }
    return s_view_invalid_idx;
}

bool s_view_begins_with(string_view_t view, const char *match) {
    assert(_s_view_is_valid(&view) && match);

    const size_t match_length = strlen(match);
    if(match_length > view.length) {
        return false;
    }
    
    for(size_t idx = 0; idx < match_length; ++idx) {
        if(*(view.pointer + idx) != *(match + idx)) {
            return false;
        }
    }
    return true;
}

// @note if match is "" then 0 loops happen and the return is true
bool s_view_ends_with(string_view_t view, const char *match) {
    assert(_s_view_is_valid(&view) && match);

    const size_t match_length = strlen(match);
    if(match_length > view.length) {
        return false;
    }

    const char *match_end = match + match_length - 1;
    const char *view_end = view.pointer + view.length - 1;

    for(size_t idx = 0; idx < match_length; ++idx) {
        if(*(view_end - idx) != *(match_end - idx)) {
            return false;
        }
    }
    return true;
}

// Returns string view of the first found line with characters and pushes view_reminder to the next line character or end of string view
//  At the last call sets view_reminder to string_view end with 0 length
//   Example use: s_view_get_line(&line_view, file_view, &file_view)
bool s_view_get_line(string_view_t *line, string_view_t view, string_view_t *view_reminder) {
    assert(_s_view_is_valid(&view) && line && view_reminder);

    // View length is 0 -> no line & no reminder
    if(view.length == 0) {
        *line = s_view_end(view);
        *view_reminder = s_view_end(view);
        return false;
    }

    // Push past '\n's
    size_t past_first_new_lines = s_view_find_first_not_of(view, '\n');

    // Only new lines are left -> no line & no reminder
    if(past_first_new_lines == s_view_invalid_idx) {
        *line = s_view_end(view);
        *view_reminder = s_view_end(view);
        return false;
    }

    view = s_view_substring(view, past_first_new_lines, view.length - 1);

    size_t first_next_new_line = s_view_find_first_of(view, '\n');

    // No new line found -> line is whole view & no reminder
    if(first_next_new_line == s_view_invalid_idx) {
        *line = view;
        *view_reminder = s_view_end(view);
        return true;
    }

    // New line found -> line until new line character & reminder(not necessarely with data(could be only newline characters))
    *line = s_view_substring(view, 0, first_next_new_line - 1);
    *view_reminder = s_view_substring(view, first_next_new_line, view.length - 1);
    return true;
}

#endif /* STRING_VIEW_IMPLEMENTATION */

#endif /* _STRING_VIEW_H */