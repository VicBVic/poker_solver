#pragma once

#include <string>

char human_readable_rank(int rank);
char human_readable_suit(int suit);

std::string verbose_rank(int rank);
std::string verbose_suit(int suit);

int rank_from_char(char c);
int suit_from_char(char c);