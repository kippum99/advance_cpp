#include "regex.h"
#include <iostream>


/* Initialize the regex operator to apply exactly once. */
RegexOperator::RegexOperator() {
    minRepeat = 1;
    maxRepeat = 1;
}


/* Returns the "minimum repeat count" value. */
int RegexOperator::getMinRepeat() const {
    return minRepeat;
}


/* Returns the "maximum repeat count" value. */
int RegexOperator::getMaxRepeat() const {
    return maxRepeat;
}


/* Sets the "minimum repeat count" value. */
void RegexOperator::setMinRepeat(int n) {
    assert(n >= 0);
    minRepeat = n;
}


/* Sets the "maximum repeat count" value. */
void RegexOperator::setMaxRepeat(int n) {
    assert(n >= -1);
    maxRepeat = n;
}


/* Clears the list of matches stored in the regex operator.  Typically done
 * in preparation to try to match the regex to a new string.
 */
void RegexOperator::clearMatches() {
    matches.clear();
}


/* Records a new match of the operator in the list of matches. */
void RegexOperator::pushMatch(const Range &r) {
    matches.push_back(r);
}


/* Reports how many times the regex operator has successfully matched in the
 * string.
 */
int RegexOperator::numMatches() const {
    return (int) matches.size();
}


/* Removes the last match the operator successfully matched against.  Used for
 * backtracking by the regex engine.
 */
Range RegexOperator::popMatch() {
    Range r = matches.back();
    matches.pop_back();
    return r;
}


bool MatchChar::match(const string &s, Range &r) const {
    if (r.start < s.length() && s[r.start] == matchChar) {
        r.end++;
        return true;
    }

    return false;
}

bool MatchAny::match(const string &s, Range &r) const {
    if (r.start < s.length()) {
        r.end++;
        return true;
    }

    return false;
}

bool MatchFromSubset::match(const string &s, Range &r) const {
    if (r.start < s.length() && characters.find(s[r.start]) != -1) {
        r.end++;
        return true;
    }

    return false;
}

bool ExcludeFromSubset::match(const string &s, Range &r) const {
    if (r.start < s.length() && characters.find(s[r.start]) == -1) {
        r.end++;
        return true;
    }

    return false;
}


vector<RegexOperator *> parseRegex(const string &expr) {
    vector<RegexOperator *> operators;

    int i = 0;

    while (i < expr.length()) {
        RegexOperator *op;

        if (expr[i] == '\\') {
            op = new MatchChar{expr[i+1]};
            i += 2;
        }
        else if (expr[i] == '.') {
            op = new MatchAny{};
            i++;
        }
        else if (expr[i] == '[') {
            int close_idx = expr.find(']', i + 1);
            string characters = "";

            int j = i + 1;

            while (j < close_idx) {
                if (expr[j] == '\\') {
                    j++;
                }
                characters += expr[j];
                j++;
            }

            if (characters[0] == '^') {
                op = new ExcludeFromSubset{
                    characters.substr(1, characters.length())};
            }
            else {
                op = new MatchFromSubset{characters};
            }

            i = close_idx + 1;
        }
        else {
            op = new MatchChar{expr[i]};
            i++;
        }

        // Handle repeated and optional matches
        if (expr[i] == '?') {
            op->setMinRepeat(0);
            op->setMaxRepeat(1);
            i++;
        }
        else if (expr[i] == '*') {
            op->setMinRepeat(0);
            op->setMaxRepeat(-1);
            i++;
        }
        else if (expr[i] == '+') {
            op->setMinRepeat(1);
            op->setMaxRepeat(-1);
            i++;
        }

        operators.push_back(op);
    }

    return operators;
}
