#ifndef _STRING_VIEW_H
#define _STRING_VIEW_H

#include <stdint.h>

// @todo Decide what should happen when length becomes <= 0

#define s_view_invalid_idx ((size_t)-1)

// string_view_t doesn't own the pointer, it just points to memory in given allocated before string
struct string_view_t {
    char *pointer;
    size_t length;
};

string_view_t string_view(char *pointer, size_t length);
string_view_t string_view(string_view_t view, size_t length = s_view_invalid_idx);
string_view_t s_view_advance(string_view_t view, size_t n);
string_view_t s_view_truncate(string_view_t view, size_t n); // @todo Better name
string_view_t s_view_substring(string_view_t view, size_t l_idx, size_t r_idx);
string_view_t s_view_trim(string_view_t view);
size_t        s_view_find_first_not(string_view_t view, char _char);  // @todo Better name
size_t        s_view_find_last_not(string_view_t view, char _char);   // @todo Better name

#ifdef STRING_VIEW_IMPLEMENTATION

#include <assert.h>

inline bool _s_view_is_valid(string_view_t *s_view) { return s_view->pointer && s_view->length >= 0 && s_view->length != s_view_invalid_idx; }

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

string_view_t s_view_advance(string_view_t view, size_t n) {
    assert(_s_view_is_valid(&view) >= 0 && (view.length - n) >= 0);

    string_view_t s_view;
    s_view.pointer = view.pointer + n;
    s_view.length  = view.length - n;
    return s_view;
}

string_view_t s_view_truncate(string_view_t view, size_t n) {
    assert(_s_view_is_valid(&view) && (view.length - n) >= 0);

    string_view_t s_view;
    s_view.pointer = view.pointer;
    s_view.length  = view.length - n;
    return s_view;
}

string_view_t s_view_substring(string_view_t view, size_t l_idx, size_t r_idx) {
    assert(_s_view_is_valid(&view) && l_idx >= 0 && r_idx < view.length && r_idx >= l_idx);

    string_view_t s_view = s_view_advance(view, l_idx);
    return s_view_truncate(s_view, s_view.length - 1 - (r_idx - l_idx)); // @check
}

// @todo For now only spaces ' '
string_view_t s_view_trim(string_view_t view) {
    assert(_s_view_is_valid(&view));

    size_t l_idx = s_view_find_first_not(view, ' ');

    // Whole string is ' ';
    if(l_idx == s_view_invalid_idx) {
        return string_view(view, 0);
    }

    size_t r_idx = s_view_find_last_not(view, ' ');
    assert(r_idx != s_view_invalid_idx);

    return s_view_substring(view, l_idx, r_idx);
}

size_t s_view_find_first_not(string_view_t view, char _char) {
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

size_t s_view_find_last_not(string_view_t view, char _char) {
    assert(_s_view_is_valid(&view));

    if(view.length <= 0) return s_view_invalid_idx;

    // @todo
    for(size_t idx = view.length - 1; idx >= 0; --idx) {
        char _c = *(view.pointer + idx);
        if(_c != _char) {
            return idx;
        }
    }
    return s_view_invalid_idx;
}

#endif

#endif /* _STRING_VIEW_H */