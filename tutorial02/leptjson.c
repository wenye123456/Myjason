#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL, strtod() */
#include <math.h>
#include <errno.h>


#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')

typedef struct {
    const char* json;
}lept_context;

static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

static int lept_parse_true(lept_context* c, lept_value* v) {
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_TRUE;
    return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context* c, lept_value* v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = LEPT_FALSE;
    return LEPT_PARSE_OK;
}

static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}
static int lept_parse_literal(lept_context* c, lept_value* v,const char *str) {
    int length = strlen(str);
    for (int i = 0; i < length; i++)
    {
        if(c->json[i]!=str[i])
            return LEPT_PARSE_INVALID_VALUE;
    }
    if (strcmp(str, "null") == 0)
    {
        c->json += 4;
        v->type = LEPT_NULL;
    }
    if (strcmp(str, "true") == 0)
    {
        c->json += 4;
        v->type = LEPT_TRUE;
    }
    if (strcmp(str, "false") == 0)
    {
        c->json += 5;
        v->type = LEPT_FALSE;
    }
    return LEPT_PARSE_OK;
    
}

static int lept_parse_number(lept_context* c, lept_value* v) {
    char* end;
    /* \TODO validate number */
    if (!ISDIGIT(c->json[0]) && c->json[0]!='-')
        return LEPT_PARSE_INVALID_VALUE;
    char* curr= c->json;

    if (*curr == '0' )
    {
        int flag= 0;
        if (*(curr + 1) == '.')
        {
            flag = 1;
        }
        if (*(curr + 1) == '\0')
        {
            flag = 1;
        }
        if (*(curr + 1) == 'e')
        {
            flag = 1;
        }
        if (*(curr + 1) == 'E')
        {
            flag = 1;
        }
        if(flag==0)
          return LEPT_PARSE_ROOT_NOT_SINGULAR;
    }
    curr++;
    while (*curr != '\0')
    {
        switch (*curr)
        {
        case '+':
        case '-':if (!ISDIGIT1TO9(*(curr + 1)))
            return LEPT_PARSE_INVALID_VALUE;
            break;
        case 'e':
        case 'E':
            if (*(curr + 1) == '-' || *(curr + 1) == '+')
                break;
            if (!ISDIGIT1TO9(*(curr + 1)))
                return LEPT_PARSE_INVALID_VALUE;
            break;
        case '.':
            if (!ISDIGIT(*(curr + 1)))
            {
                return LEPT_PARSE_INVALID_VALUE;
            }
            if (!ISDIGIT(*(curr - 1)))
            {
                return LEPT_PARSE_INVALID_VALUE;
            }
            break;

        default:if(!ISDIGIT(*curr))
            return LEPT_PARSE_INVALID_VALUE;
            

        }
        curr++;
    }




    v->n = strtod(c->json, &end);
    if (errno == ERANGE)
    {
        if(v->n==HUGE_VALF|| v->n == -INFINITY || v->n== INFINITY)
          return LEPT_PARSE_NUMBER_TOO_BIG;
    }
    if (c->json == end)
        return LEPT_PARSE_INVALID_VALUE;

    c->json = end;
    v->type = LEPT_NUMBER;
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 't':  return  lept_parse_literal(c, v, "true");
        case 'f':  return lept_parse_literal(c, v,"false");
        case 'n':  return lept_parse_literal(c, v,"null");
        default:   return lept_parse_number(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
    }
}

int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            v->type = LEPT_NULL;
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

double lept_get_number(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->n;
}
